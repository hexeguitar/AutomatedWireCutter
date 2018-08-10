//****************************************************************
//
//	KeyPad Matrix KeyPad Component
//	Version 1.0
//	Author Bob Marlowe
//	J. Meier Software - Entwicklung
//	Steinfelder Str. 11
//	D-27404 Bruemmerhof
//	(C) 2012 J.Meier Software - Entwicklung
//
//	Given as an Example to the Cypress PSoC community
//	Use as-it-is, use at own risk, but use it
//*******************************************************************************
/********************************************************************************/
/*																				*/
/*	De-Bouncing of a Keyboard, Buffering, Key-Repeating							*/
/*																				*/
/********************************************************************************/
//
#ifndef __KeyPad_H__
#define __KeyPad_H__

#include <cytypes.h>

#define UpLog2(x) (x>4?3:x>2?2:x>1?1:0)									// Uprounded log2(x) 1<= x <=8
#define EXP2(x) (x==4?16:x==3?8:x==2?4:x==1?2:1)						// 2 to the power of x 0<= x <=4
#define MAXKEYVALUE (EXP2(UpLog2(4)) * EXP2(UpLog2(4)))
#define INVALIDCHAR 0xff
#define KeyPad_Fire_int
    
//********************** APIs ***************************************************************
//	Start the component. The TranslationTable must be char [Row * Column] or NULL.			*
//	A pressed key results in a number from 0 to (Row * Column)-1 and can be translated to	*
//	any character automatically by using the key number as index into TranslationTable		*
void KeyPad_Start(char * TranslationTable);	//									*
//																							*
//	Check the buffer for a key already entered. Returns != 0 if character available			*
char KeyPad_CharReady(void);					//									*
//																							*
//	Get a character. When there is no character in the buffer the program waits				*
//	blocking until a character is ready.													*
char KeyPad_GetChar(void);					//									*
//  Power down all component																*
void KeyPad_Sleep(void);						//									*
//	Power up after going to sleep															*
void KeyPad_Wakeup(void);						//		

char KeyPad_CharLongReady(void);
char KeyPad_GetCharLong(void);

uint8 KeyPad_getNewKeyScan(void);
//*******************************************************************************************
#endif
//[] END OF FILE
