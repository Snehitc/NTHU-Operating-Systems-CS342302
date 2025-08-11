#include <8051.h>
#include "buttonlib.h"

/*
 * buttonlib.c
 * This library provides functions to interact with the button bank
 * connected to Port P2 on the 8051 MCU.
 * When a button is pressed, the corresponding P2.x pin reads 0;
 * otherwise, it reads 1.
 */

// Returns 1 if any button is pressed, 0 otherwise.
char AnyButtonPressed(void) {
    return (P2 != 0xFF); // If any bit in P2 is 0, a button is pressed
}

// Returns the ASCII character of the highest-priority pressed button.
// Priority: P2.7 ('7') > P2.6 ('6') > ... > P2.0 ('0')
// If no button is pressed, returns '\0'.
char ButtonToChar(void) {
    // Check each button from highest (P2.7) to lowest (P2.0)
    if ((~P2) & 0x80) {        // P2.7
        return '7';
    } else if ((~P2) & 0x40) { // P2.6
        return '6';
    } else if ((~P2) & 0x20) { // P2.5
        return '5';
    } else if ((~P2) & 0x10) { // P2.4
        return '4';
    } else if ((~P2) & 0x08) { // P2.3
        return '3';
    } else if ((~P2) & 0x04) { // P2.2
        return '2';
    } else if ((~P2) & 0x02) { // P2.1
        return '1';
    } else if ((~P2) & 0x01) { // P2.0
        return '0';
    }
    // If no button is pressed, return '\0'
    return '\0';
}
