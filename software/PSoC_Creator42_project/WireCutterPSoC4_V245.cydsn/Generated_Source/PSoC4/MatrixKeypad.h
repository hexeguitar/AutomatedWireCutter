//****************************************************************
//
//	MatrixKeypad Matrix KeyPad Component
//	Version 1.0
//	Author Bob Marlowe
//	J. Meier Software - Entwicklung
//	Steinfelder Str. 11
//	D-27404 Bruemmerhof
//	(C) 2012 J.Meier Software - Entwicklung
//
//	Given as an Example to the Cypress PSoC community
//	Use as-it-is, use at own risk, but use it
//
//  01.2018 (c) Piotr Zapart
//  - fixed a bug hte component was not able to return a press of the fist key (index = 0)
//  - added 2nd buffer + API for long press events
//  - added clear buffer functions 
//*******************************************************************************
/********************************************************************************/
/*																				*/
/*	De-Bouncing of a Keyboard, Buffering, Key-Repeating							*/
/*																				*/
/********************************************************************************/
//
#ifndef __MatrixKeypad_H__
#define __MatrixKeypad_H__

#include <cytypes.h>

#define UpLog2(x) (x>4?3:x>2?2:x>1?1:0)									// Uprounded log2(x) 1<= x <=8
#define EXP2(x) (x==4?16:x==3?8:x==2?4:x==1?2:1)						// 2 to the power of x 0<= x <=4
#define MAXKEYVALUE (EXP2(UpLog2(4)) * EXP2(UpLog2(4)))
#define INVALIDCHAR 0xff
#define MatrixKeypad_Fire_int
    
//********************** APIs ***************************************************************
//	Start the component. The TranslationTable must be char [Row * Column] or NULL.			*
//	A pressed key results in a number from 0 to (Row * Column)-1 and can be translated to	*
//	any character automatically by using the key number as index into TranslationTable		*
void MatrixKeypad_Start(char * TranslationTable);	//									*
//																							*
//	Check the buffer for a key already entered. Returns != 0 if character available			*
char MatrixKeypad_CharReady(void);					//									*
//																							*
//	Get a character. When there is no character in the buffer the program waits				*
//	blocking until a character is ready.													*
char MatrixKeypad_GetChar(void);					//									*
//  Power down all component																*
void MatrixKeypad_Sleep(void);						//									*
//	Power up after going to sleep															*
void MatrixKeypad_Wakeup(void);						//		

char MatrixKeypad_CharLongReady(void);
char MatrixKeypad_GetCharLong(void);

uint8 MatrixKeypad_getNewKeyScan(void);

// reset the short press char buffer
void MatrixKeypad_clearCharBuffer(void);
// reset the long press char buffer
void MatrixKeypad_clearCharBufferLong(void);

//*******************************************************************************************
#endif
//[] END OF FILE
