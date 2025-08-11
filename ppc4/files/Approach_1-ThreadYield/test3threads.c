/*
 * file: testpreempt.c
 */
#include <8051.h>
#include "preemptive.h"

// Bounded buffer and indices
#define BUFFER_SIZE 3

__data __at (0x27) Semaphore mutex;
__data __at (0x28) Semaphore full;
__data __at (0x29) Semaphore empty;

__data __at (0x2A) char head;        // head index (write position)
__data __at (0x2B) char tail;        // tail index (read position)
__data __at (0x2C) char sharedBuffer[BUFFER_SIZE];      // Shared buffer
__data __at (0x2F) char currentChar;	// current character: A - Z
__data __at (0x26) char currentDig;	   // current character: 0 - 9
__data __at (0x25) char producer;      // flag to switch producer

void Producer1(void)
{
	currentChar = 'A';
    while (1)
    {
		SemaphoreWait(empty);
		__critical{
			SemaphoreWait(mutex);
			sharedBuffer[head] = currentChar;
			head = (head==BUFFER_SIZE-1) ? 0 : head+1;
			SemaphoreSignal(mutex); 
        }
		SemaphoreSignal(full);
        currentChar = (currentChar == 'Z') ? 'A' : currentChar + 1;
		ThreadYield();
    }
	
}

void Producer2(void)
{
	currentDig = '0';
    while (1)
    {
		SemaphoreWait(empty);
		__critical{
			SemaphoreWait(mutex);
			sharedBuffer[head] = currentDig;
			head = (head==BUFFER_SIZE-1) ? 0 : head+1;
			SemaphoreSignal(mutex); 
        }
		SemaphoreSignal(full);
        currentDig = (currentDig == '9') ? '0' : currentDig + 1;
		ThreadYield();
    }
	
}


void Consumer(void)
{
	// Configure serial port for polling mode
	TMOD |= 0x20;   // Timer1 mode 2: 8-bit auto-reload
    TH1 = 0xFA;     // (Hex) Baud rate 4800 for 11.0592 MHz or TH1=-6 //(dec)
    SCON = 0x50;    // Mode 1: 8-bit UART, REN enabled
    TR1 = 1;        // Start Timer1

    while (1)
    {
		SemaphoreWait(full);
		__critical{
			SemaphoreWait(mutex);
			SBUF = sharedBuffer[tail];
			while (!TI) ThreadYield();  // Wait for transmission to complete
			TI = 0;       // Clear transmit interrupt flag
			tail = (tail==BUFFER_SIZE-1) ? 0 : tail+1; 
			SemaphoreSignal(mutex);
			
		}
        SemaphoreSignal(empty);
    }
	
}


void main(void)
{
	__critical{
		sharedBuffer[0] = sharedBuffer[1] = sharedBuffer[2] = '\0';
		head = 0;
		tail = 0;
		SemaphoreCreate(mutex, 1);
		SemaphoreCreate(full, 0);
		SemaphoreCreate(empty, BUFFER_SIZE);
		producer = 1;
	}

	ThreadCreate(Producer1);
	ThreadCreate(Producer2);
	//ThreadCreate(Consumer);
	Consumer();
	//ThreadYield();
}


void _sdcc_gsinit_startup(void)
{
    __asm
        LJMP _Bootstrap
    __endasm;
}

void _mcs51_genRAMCLEAR(void) {}
void _mcs51_genXINIT(void) {}
void _mcs51_genXRAMCLEAR(void) {}


void timer0_ISR(void) __interrupt(1) {
	__asm
		ljmp _myTimer0Handler
	__endasm;
}