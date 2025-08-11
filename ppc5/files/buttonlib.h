/*
 * buttonlib.h
 *
 * This is the library for the button bank.
 * It provides two functions:
 * - AnyButtonPressed: to check if any button is pressed.
 * - ButtonToChar: to read the pressed button as ASCII; if multiple buttons
 *   are pressed, then read the highest-priority button.
 */

#ifndef __BUTTONLIB_H__
#define __BUTTONLIB_H__

/* 
 * Returns 1 if any button is pressed, 0 otherwise.
 */
char AnyButtonPressed(void);

/* 
 * Returns the ASCII character of the highest-priority pressed button.
 * If no button is pressed, returns '\0'.
 */
char ButtonToChar(void);

#endif // __BUTTONLIB_H__
