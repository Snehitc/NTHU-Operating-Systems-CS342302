/*
 * file: testpreempt.c
 */
#include <8051.h>
#include "preemptive.h"

__data __at (0x3D) char bufferFull;        // buffer status (0: empty, 1: full)
__data __at (0x3E) char sharedBuffer;      // Shared buffer
__data __at (0x3F) char currentChar;	   // current character: A - Z


void Producer(void)
{
	currentChar = 'A';
    while (1)
    {
		while (bufferFull!=0);
		__critical{
			sharedBuffer = currentChar;
			bufferFull = 1;  
        }
        currentChar = (currentChar == 'Z') ? 'A' : currentChar + 1;
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
		while (bufferFull==0); 
		__critical{
			SBUF = sharedBuffer;
			bufferFull = 0;
		}
        while (!TI);  // Wait for transmission to complete
        TI = 0;       // Clear transmit interrupt flag
		
    }
}


void main(void)
{
	__critical{
		bufferFull = 0;
	}

	ThreadCreate(Producer);
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