/*
 * file: testcoop.c
 */
#include <8051.h>
#include "cooperative.h"

/*
 * [TODO]
 * declare your global variables here, for the shared buffer
 * between the producer and consumer.
 * Hint: you may want to manually designate the location for the
 * variable.  you can use
 *        __data __at (0x30) type var;
 * to declare a variable var of the type
 */
__data __at (0x3D) char bufferFull;        // buffer status (0: empty, 1: full)
__data __at (0x3E) char sharedBuffer;      // Shared buffer
__data __at (0x3F) char currentChar;	   // current character: A - Z

/* [TODO for this function]
 * the producer in this test program generates one characters at a
 * time from 'A' to 'Z' and starts from 'A' again. The shared buffer
 * must be empty in order for the Producer to write.
 */

void Producer(void)
{
	/*
     * [TODO]
     * initialize producer data structure, and then enter
     * an infinite loop (does not return)
     */
	currentChar = 'A';
    while (1)
    {
        /* [TODO]
         * wait for the buffer to be available,
         * and then write the new data into the buffer */
		while (bufferFull!=0) ThreadYield();
		sharedBuffer = currentChar;
        bufferFull = 1;  
        currentChar = (currentChar == 'Z') ? 'A' : currentChar + 1;
		ThreadYield();
    }
}

/* [TODO for this function]
 * the consumer in this test program gets the next item from
 * the queue and consume it and writes it to the serial port.
 * The Consumer also does not return.
 */
void Consumer(void)
{
    /*
     * [TODO]
     * initialize Tx for polling
     */
	// Configure serial port for polling mode
	TMOD = 0x20;   // Timer1 mode 2: 8-bit auto-reload
    TH1 = 0xFA;    // (Hex) Baud rate 4800 for 11.0592 MHz or TH1=-6 //(dec)
    SCON = 0x50;   // Mode 1: 8-bit UART, REN enabled
    TR1 = 1;       // Start Timer1

    while (1)
    {
        /*
         * [TODO]
         * wait for new data from producer
         */
		while (bufferFull==0) ThreadYield(); 
		/*
         * [TODO]
         * write data to serial port Tx,
         * poll for Tx to finish writing (TI),
         * then clear the flag
         */
		SBUF = sharedBuffer;
		bufferFull = 0;
        while (!TI) ThreadYield();  // Wait for transmission to complete
        TI = 0;       // Clear transmit interrupt flag
		
    }
}

/* [TODO for this function]
 * main() is started by the thread bootstrapper as thread-0.
 * It can create more thread(s) as needed:
 * one thread can act as producer and another as consumer.
 */

void main(void)
{
    /*
     * [TODO]
     * initialize globals
     */
	bufferFull = 0;
    /*
     * [TODO]
     * set up Producer and Consumer.
     * Because both are infinite loops, there is no loop
     * in this function and no return.
     */
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
