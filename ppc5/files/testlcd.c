#include <8051.h>
#include "preemptive.h"
#include "lcdlib.h"
#include "buttonlib.h"
#include "keylib.h"


// Bounded buffer and indices
#define BUFFER_SIZE 3

__data __at (0x27) Semaphore mutex;
__data __at (0x28) Semaphore full;
__data __at (0x29) Semaphore empty;

__data __at (0x2A) char head;        // head index (write position)
__data __at (0x2B) char tail;        // tail index (read position)
__data __at (0x2C) char sharedBuffer[BUFFER_SIZE];      // Shared buffer
__data __at (0x2F) char currentButton;	   // Button: 0 - 7
__data __at (0x26) char currentKey;	   // Key: 1 - 9 and *, 0, #
__data __at (0x25) char producer;      // flag to switch producer
__data __at (0x24) char sb;      // flag to switch producer

//__data __at (0x3A) unsigned char lcd_ready;  /* @@@ change to a different location if needed. It just needs a bit, no need to be a char. */

void Producer1(void)
{
	currentButton = '/0';
    while (1)
    {
		while(!AnyButtonPressed());
		currentButton = ButtonToChar();
		while(AnyButtonPressed());
		
		SemaphoreWait(empty);
		
		SemaphoreWait(mutex);
		sharedBuffer[head] = currentButton;
		head = (head==BUFFER_SIZE-1) ? 0 : head+1;
		SemaphoreSignal(mutex); 
		
		SemaphoreSignal(full);
    }
}

void Producer2(void)
{
	currentKey = '/0';
    while (1)
    {
		while(!AnyKeyPressed());
		currentKey = KeyToChar();
		while(AnyKeyPressed());
		
		SemaphoreWait(empty);
		
		SemaphoreWait(mutex);
		sharedBuffer[head] = currentKey;
		head = (head==BUFFER_SIZE-1) ? 0 : head+1;
		SemaphoreSignal(mutex); 
		
		SemaphoreSignal(full);
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
		
		SemaphoreWait(mutex);
		sb = sharedBuffer[tail];
		tail = (tail==BUFFER_SIZE-1) ? 0 : tail+1; 
		SemaphoreSignal(mutex);
		
        SemaphoreSignal(empty);
		
		while(!LCD_ready());
		LCD_write_char(sb);
		
    }	
}


void main(void)
{
	sharedBuffer[0] = sharedBuffer[1] = sharedBuffer[2] = '\0';
	head = 0;
	tail = 0;
	//sb = 0;
	SemaphoreCreate(mutex, 1);
	SemaphoreCreate(full, 0);
	SemaphoreCreate(empty, BUFFER_SIZE);
	producer = 1;
	
	Init_Keypad();
	LCD_Init();

	ThreadCreate(Producer1);
	ThreadCreate(Producer2);
	
	Consumer();
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