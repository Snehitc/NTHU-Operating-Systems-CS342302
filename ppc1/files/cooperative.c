#include <8051.h>

#include "cooperative.h"

/*
 * [TODO]
 * declare the static globals here using
 *        __data __at (address) type name; syntax
 * manually allocate the addresses of these variables, for
 * - saved stack pointers (MAXTHREADS)
 * - current thread ID
 * - a bitmap for which thread ID is a valid thread;
 *   maybe also a count, but strictly speaking not necessary
 * - plus any temporaries that you need.
 */
__data __at (0x30) char savedSP[MAXTHREADS];  // Saved stack pointers for threads
__data __at (0x34) ThreadID currentThread;    // Current thread ID
__data __at (0x35) ThreadID newThread;		  // New thread ID
__data __at (0x36) char threadMask;           // Bitmap for valid threads
__data __at (0x37) char tempSP;               // Temporary storage for SP
__data __at (0x38) char a;					  // temporary register
__data __at (0x39) char i;					  // temporary incrementer

/*
 * [TODO]
 * define a macro for saving the context of the current thread by
 * 1) push ACC, B register, Data pointer registers (DPL, DPH), PSW
 * 2) save SP into the saved Stack Pointers array
 *   as indexed by the current thread ID.
 * Note that 1) should be written in assembly,
 *     while 2) can be written in either assembly or C
 */
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

/*
 * [TODO]
 * define a macro for restoring the context of the current thread by
 * essentially doing the reverse of SAVESTATE:
 * 1) assign SP to the saved SP from the saved stack pointer array
 * 2) pop the registers PSW, data pointer registers, B reg, and ACC
 * Again, popping must be done in assembly but restoring SP can be
 * done in either C or assembly.
 */
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

/*
 * we declare main() as an extern so we can reference its symbol
 * when creating a thread for it.
 */

extern void main(void);

/*
 * Bootstrap is jumped to by the startup code to make the thread for
 * main, and restore its context so the thread can run.
 */

void Bootstrap(void)
{
    /*
     * [TODO]
     * initialize data structures for threads (e.g., mask)
     *
     * optional: move the stack pointer to some known location
     * only during bootstrapping. by default, SP is 0x07.
     *
     * [TODO]
     *     create a thread for main; be sure current thread is
     *     set to this thread ID, and restore its context,
     *     so that it starts running main().
     */
	threadMask = 0x00;    // initialise threadMask with 0
    //savedSP[0] = SP;    // Initialize stack pointer for main
    currentThread = ThreadCreate(main); // Create main as a thread
    RESTORESTATE;         // Restore its state to start running main

}

/*
 * ThreadCreate() creates a thread data structure so it is ready
 * to be restored (context switched in).
 * The function pointer itself should take no argument and should
 * return no argument.
 */
ThreadID ThreadCreate(FunctionPtr fp)
{
    /*
     * [TODO]
     * check to see we have not reached the max #threads.
     * if so, return -1, which is not a valid thread ID.
     */
	if (threadMask == 0x0F)  return -1;  // if all threads in use return -1

    /*
     * [TODO]
     *     otherwise, find a thread ID that is not in use,
     *     and grab it. (can check the bit mask for threads),
     *
     * [TODO] below
     * a. update the bit mask
         (and increment thread count, if you use a thread count,
          but it is optional)
       b. calculate the starting stack location for new thread
       c. save the current SP in a temporary
          set SP to the starting location for the new thread
       d. push the return address fp (2-byte parameter to
          ThreadCreate) onto stack so it can be the return
          address to resume the thread. Note that in SDCC
          convention, 2-byte ptr is passed in DPTR.  but
          push instruction can only push it as two separate
          registers, DPL and DPH.
       e. we want to initialize the registers to 0, so we
          assign a register to 0 and push it four times
          for ACC, B, DPL, DPH.  Note: push #0 will not work
          because push takes only direct address as its operand,
          but it does not take an immediate (literal) operand.
       f. finally, we need to push PSW (processor status word)
          register, which consist of bits
           CY AC F0 RS1 RS0 OV UD P
          all bits can be initialized to zero, except <RS1:RS0>
          which selects the register bank.
          Thread 0 uses bank 0, Thread 1 uses bank 1, etc.
          Setting the bits to 00B, 01B, 10B, 11B will select
          the register bank so no need to push/pop registers
          R0-R7.  So, set PSW to
          00000000B for thread 0, 00001000B for thread 1,
          00010000B for thread 2, 00011000B for thread 3.
       g. write the current stack pointer to the saved stack
          pointer array for this newly created thread ID
       h. set SP to the saved SP in step c.
       i. finally, return the newly created thread ID.
     */
	
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
    SAVESTATE;
    do
    {
        /*
         * [TODO]
         * do round-robin policy for now.
         * find the next thread that can run and
         * set the current thread ID to it,
         * so that it can be restored (by the last line of
         * this function).
         * there should be at least one thread, so this loop
         * will always terminate.
         */
		currentThread = (currentThread == MAXTHREADS-1) ? 0 : currentThread+1;
	} while (!(threadMask & (1 << currentThread)));
    RESTORESTATE;
}

/*
 * ThreadExit() is called by the thread's own code to terminate
 * itself.  It will never return; instead, it switches context
 * to another thread.
 */
void ThreadExit(void)
{
    /*
     * clear the bit for the current thread from the
     * bit mask, decrement thread count (if any),
     * and set current thread to another valid ID.
     * Q: What happens if there are no more valid threads?
     */
	threadMask &= ~(1 << currentThread);

    // Find another valid thread to switch to
    do {
        currentThread = (currentThread == MAXTHREADS-1) ? 0 : currentThread+1;
    } while (!(threadMask & (1 << currentThread)));

    RESTORESTATE;
}
