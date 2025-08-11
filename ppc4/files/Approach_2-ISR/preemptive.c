/*
 * file: preemptive.c
 */
#include <8051.h>
#include "preemptive.h"


__data __at (0x30) char savedSP[MAXTHREADS];  // Saved stack pointers for threads
__data __at (0x34) ThreadID currentThread;    // Current thread ID
__data __at (0x35) ThreadID newThread;		  // New thread ID
__data __at (0x36) char threadMask;           // Bitmap for valid threads
__data __at (0x37) char tempSP;               // Temporary storage for SP
__data __at (0x38) char a;					  // temporary register
__data __at (0x39) char i;					  // temporary incrementer
__data __at (0x25) char producer;


#define SAVESTATE               \
    {                           \
        __asm                   \
        PUSH ACC                \
        PUSH B                  \
        PUSH DPL                \
        PUSH DPH                \
        PUSH PSW                \
        __endasm;               \
		savedSP[currentThread] = SP;    \
    }


#define RESTORESTATE            \
    {                           \
		SP = savedSP[currentThread];   \
        __asm                   \
        POP PSW                 \
        POP DPH                 \
        POP DPL                 \
        POP B                   \
        POP ACC                 \
        __endasm;               \
    }

extern void main(void);


void Bootstrap(void)
{
	TMOD = 0;    // timer 0 mode 0
	IE = 0x82;   // enable timer 0 interrupt; keep consumer polling
				 // EA - ET2 ES ET1 EX1 ET0 EX0
	TR0 = 1;     // set bit TR0 to start running timer 0

	
	threadMask = 0x00;    // initialise threadMask with 0
    //savedSP[0] = SP;    // Initialize stack pointer for main
    currentThread = ThreadCreate(main); // Create main as a thread
    RESTORESTATE;         // Restore its state to start running main

}


ThreadID ThreadCreate(FunctionPtr fp)
{
	if (threadMask == 0x0F)  return -1;  // if all threads in use return -1
	
	__critical{
		// Find an available thread ID
		for (i=0; i<MAXTHREADS; i++) {
			if ((threadMask & (1 << i)) == 0) {
				threadMask |= (1 << i);
				newThread = i;
				break;
			}
		}

		// Save the current SP and set a new stack for the new thread
		tempSP = SP;
		//SP = 0x3F + (i<<4);  // Set new stack starting location
		SP = 0x3F + newThread * 0x10;  // Set new stack starting location
		
		
		
		// Initialize registers to 0 and push them 
		__asm
			PUSH DPL
			PUSH DPH
			MOV A, #0
			MOV B, #0
			MOV DPL, #0
			MOV DPH, #0
			
			PUSH A         ; Push A (0)
			PUSH B         ; Push B (0)
			PUSH DPL         ; Push DPL (0)
			PUSH DPH         ; Push DPH (0)	
		__endasm;
		
		// Set PSW based on thread ID and push it
		a = (newThread << 3);  // Set register bank
		__asm
			PUSH _a
		__endasm;

		// Save the SP for the new thread and restore the original SP
		savedSP[newThread] = SP;
		SP = tempSP;

		// Explicitly reference `fp` to avoid compiler warnings
		if (fp) { /* No operation - explicitly reference fp */ }
	}
    return newThread;  // Return the newly created thread ID

}

/*
 * this is called by a running thread to yield control to another
 * thread.  ThreadYield() saves the context of the current
 * running thread, picks another thread (and set the current thread
 * ID to it), if any, and then restores its state.
 */

void ThreadYield(void)
{
	__critical{
		SAVESTATE;
		do
		{
			currentThread = (currentThread == MAXTHREADS-1) ? 0 : currentThread+1;
		} while (!(threadMask & (1 << currentThread)));
		RESTORESTATE;
	}
}


void ThreadExit(void)
{
	__critical{
		threadMask &= ~(1 << currentThread);

		// Find another valid thread to switch to
		do {
			currentThread = (currentThread == MAXTHREADS-1) ? 0 : currentThread+1;
		} while (!(threadMask & (1 << currentThread)));

		RESTORESTATE;
	}
}

void myTimer0Handler(void) {
	EA = 0;
	SAVESTATE;
	__asm
		mov ACC, r1
		mov B, r2
		mov DPL, r3
		mov DPH, r4
		mov PSW, r5
		mov _tempSP, r6
		mov _a, r7
	__endasm;
	
	do
    {
		if (currentThread==0) {
			if (producer==1) {
				currentThread = 1;        // Spawn Producer 1
			}
			else if (producer==2){
				currentThread = 2;        // Spawn Producer 2
			}
		}
		else {
			currentThread = 0;           // Spawn Consumer
			if (producer==1) {
				producer = 2;            // Switch Producer 2 in next execution
			}
			else if (producer==2){
				producer = 1;            // Switch Producer 1 in next execution
			}
		}
		
		
		//currentThread = (currentThread == MAXTHREADS-1) ? 0 : currentThread+1;
	} while (!(threadMask & (1 << currentThread)));
	
	__asm
		mov r1, ACC
		mov r2, B
		mov r3, DPL
		mov r4, DPH
		mov r5, PSW
		mov r6, _tempSP
		mov r7, _a
	__endasm;
	RESTORESTATE;
	EA = 1;
	
	__asm
		RETI
	__endasm;
}