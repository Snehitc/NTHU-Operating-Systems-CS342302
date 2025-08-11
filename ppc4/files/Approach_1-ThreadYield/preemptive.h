/*
 * file: preemptive.h
 */

#ifndef __PREEMPTIVE_H__
#define __PREEMPTIVE_H__

#define MAXTHREADS 4 /* not including the scheduler */
/* the scheduler does not take up a thread of its own */

// Thread-related type definitions
typedef char ThreadID;
typedef void (*FunctionPtr)(void);

// Semaphore type definition
typedef char Semaphore;   // Semaphore is a signed char to handle negative values

ThreadID ThreadCreate(FunctionPtr);
void ThreadYield(void);
void ThreadExit(void);

// Semaphore macros
#define CNAME(s) _ ## s   // Converts s (in C) _s (in assembly)

#define SemaphoreCreate(s, n) s = n   // Initialize semaphore to value n

#define SemaphoreSignal(s) \
	{					   \
	__asm 				   \
        INC CNAME(s)       \
    __endasm; 			   \
	}


#define SemaphoreWait(s) SemaphoreWaitBody(s, __COUNTER__)

#define LABEL(label) label ## $

#define SemaphoreWaitBody(s, label)     \
    {									\
		__asm 							\
		LABEL(label): 							\
				MOV A, CNAME(s)         \
				JZ LABEL(label)                \
				JB ACC.7, LABEL(label)         \
				DEC CNAME(s)            \
		__endasm; 						\
	}
	

#endif // __PREEMPTIVE_H__