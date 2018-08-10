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
/*	De-Bouncing of a Keypad, Buffering, Key-Repeating							*/
/*																				*/
/*	Principle of operation:														*/
/*	In an interrupt-routine triggered by a timer the ros and columns			*/
/*	are red to see a keypress.													*/
/*	When a given (adjustable) number of keypresses are seen it is recorded 		*/
/*	in a circular buffer. To increase the flexibility of the component a		*/
/*	translation with a user-supplied lookup-tabe can be done before the 		*/
/*	keypress is recorded.														*/
/*																				*/
/*	For longer keypresses a key-repetition mechanism is implemented with		*/
/*	an adjustable timing.														*/
/*																				*/
/********************************************************************************/
//

//************************ Libraries ********************************************/
#include <project.h>


//************************ Definitions / Macros**********************************/
#define INITIAL_DELAY (0 - 0) 					// Wait-Time Before Key first Repeated
/********** Key Buffer **********************/
#define ColShift UpLog2(4)								// Number of bits needed to express the column
#define MatrixKeypad_KEYBUFFSIZE 4						// Number of Keypresses to buffer
#define KEYCOLMASK ((1 << 4)-1)							// Mask-bits Column
#define KEYROWMASK ((1 << 4)-1)							// Mask-bits Row
#define LONG_PRESS_DELAY_DEFAULT (25)
//********************* Prototypes **********************************************/
CY_ISR_PROTO(MatrixKeypad_Debounce);								// Interrrupt handler 

//************************ Data Area ********************************************/
/********** Circular Key Buffer ***************/
uint8	MatrixKeypad_WriteIndex = 0;							
uint8	MatrixKeypad_ReadIndex = 0;
char	MatrixKeypad_Buffer[MatrixKeypad_KEYBUFFSIZE];
/********************************************/
/********** Circular Key Buffer ***************/
uint8	MatrixKeypad_BufLongWriteIndex = 0;							
uint8	MatrixKeypad_BufLongReadIndex = 0;
char	MatrixKeypad_BufLong[MatrixKeypad_KEYBUFFSIZE];
/********************************************/


volatile uint8 MatrixKeypad_NewKey;
volatile uint8_t MatrixKeypad_longPressLatched = 0;

char * MatrixKeypad_TransTable = NULL;								// Address of supplied translation table
/***************** Example of a translation-table *******************************
char TranslateTable[MAXKEYVALUE] = {
										'1','2','3','4',
										'5','6','7','8',
										'9','*','0','#'
									 };
*/

/************************* Code Area ********************************************/
void MatrixKeypad_Start(char * TranslationTable)					// API to start the component
{
	MatrixKeypad_KbdScan_Int_StartEx(MatrixKeypad_Debounce);
	MatrixKeypad_KbdScan_Int_SetPriority(7u);
	MatrixKeypad_TransTable = TranslationTable;;
	
}
/********************************************************************************/

void MatrixKeypad_Sleep(void)
{
	// Shutdown the Interrupt
	MatrixKeypad_KbdScan_Int_Disable();
	// Shutdown the ports
	MatrixKeypad_Rows_SetDriveMode(MatrixKeypad_Rows_DM_DIG_HIZ);
	MatrixKeypad_Columns_SetDriveMode(MatrixKeypad_Columns_DM_DIG_HIZ);
}
/********************************************************************************/

void MatrixKeypad_Wakeup(void)
{
	// Wakeup ports
	MatrixKeypad_Rows_SetDriveMode(MatrixKeypad_Rows_DM_RES_DWN);
	MatrixKeypad_Columns_SetDriveMode(MatrixKeypad_Columns_DM_STRONG);
	// Wakeup Interrupt
	MatrixKeypad_KbdScan_Int_Enable();
}
/********************************************************************************/

char MatrixKeypad_TranslateKey(char Key)
{
	if(MatrixKeypad_TransTable == NULL) return Key;
	else return MatrixKeypad_TransTable[(uint8)Key-1];
}
/********************************************************************************/

void MatrixKeypad_PutCharLong(char Ch)									// Can be introduced as API later to simulate keystrokes
{
uint8 TrChar;
	if((TrChar=MatrixKeypad_TranslateKey(Ch))== INVALIDCHAR) return;
	MatrixKeypad_BufLong[MatrixKeypad_BufLongWriteIndex++] = TrChar;
	if (MatrixKeypad_BufLongWriteIndex >= MatrixKeypad_KEYBUFFSIZE) MatrixKeypad_BufLongWriteIndex = 0;
	// Signal Keypress
#ifdef MatrixKeypad_Fire_int	
	MatrixKeypad_KbdControl_Write(0x02);
	MatrixKeypad_KbdControl_Write(0x00);
#endif
}
/********************************************************************************/
void MatrixKeypad_PutChar(char Ch)									// Can be introduced as API later to simulate keystrokes
{
uint8 TrChar;
	if((TrChar=MatrixKeypad_TranslateKey(Ch))== INVALIDCHAR) return;
	MatrixKeypad_Buffer[MatrixKeypad_WriteIndex++] = TrChar;
	if (MatrixKeypad_WriteIndex >= MatrixKeypad_KEYBUFFSIZE) MatrixKeypad_WriteIndex = 0;
	// Signal Keypress
#ifdef MatrixKeypad_Fire_int	
	MatrixKeypad_KbdControl_Write(0x01);
	MatrixKeypad_KbdControl_Write(0x00);
#endif
}

/********************************************************************************/

char MatrixKeypad_CharReady(void)									// API to check for keys in buffer
{
	return (MatrixKeypad_WriteIndex != MatrixKeypad_ReadIndex);
}
/********************************************************************************/
// reset the short press char buffer
void MatrixKeypad_clearCharBuffer(void)
{
    MatrixKeypad_WriteIndex = MatrixKeypad_ReadIndex = 0;
}
/********************************************************************************/
char MatrixKeypad_CharLongReady(void)									// API to check for keys in buffer
{
	return (MatrixKeypad_BufLongWriteIndex != MatrixKeypad_BufLongReadIndex);
}
/********************************************************************************/
// reset the short press char buffer
void MatrixKeypad_clearCharBufferLong(void)
{
    MatrixKeypad_BufLongWriteIndex = MatrixKeypad_BufLongReadIndex = 0;
}
/********************************************************************************/


char MatrixKeypad_GetChar(void)										// API to rerieve a char from buffer or wait until key pressed (blocking)
{
char Result;
	while (!MatrixKeypad_CharReady());								// Wait until buffer contains a char
	Result = MatrixKeypad_Buffer[MatrixKeypad_ReadIndex++];
	if (MatrixKeypad_ReadIndex >= MatrixKeypad_KEYBUFFSIZE) MatrixKeypad_ReadIndex = 0; // Adjust index 
	return Result;
}
/********************************************************************************/



char MatrixKeypad_GetCharLong(void)										// API to rerieve a char from buffer or wait until key pressed (blocking)
{
char Result;
	while (!MatrixKeypad_CharLongReady());								// Wait until buffer contains a char
	Result = MatrixKeypad_BufLong[MatrixKeypad_BufLongReadIndex++];
	if (MatrixKeypad_BufLongReadIndex >= MatrixKeypad_KEYBUFFSIZE) MatrixKeypad_BufLongReadIndex = 0; // Adjust index 
	return Result;
}

/********************************************************************************/

uint8 MatrixKeypad_LowestBitPosition(uint8 Val)						// Check for a bit on, return the position in Val
{
uint8 ii;
	for (ii=0; ii < 8;ii++,Val >>= 1)
	{
		if (Val & 0x01) return ii;
	}
	return 0xff;
}

/********************************************************************************/

uint8 MatrixKeypad_Readswitches(void) CYREENTRANT;
uint8 MatrixKeypad_ReadSwitches(void) 
{
    volatile uint8	Row;
    volatile uint8	Column;
	//*** Get Row ***//
	MatrixKeypad_Rows_Write(KEYROWMASK);								// Rows write Ones
	MatrixKeypad_Columns_Write(~KEYCOLMASK);							// Cols write Zeroes
	if ((Row=MatrixKeypad_Rows_Read()) == KEYROWMASK) return 0;			// No key pressed if all rows still ones
	//*** Get Column ***//
	MatrixKeypad_Columns_Write(KEYCOLMASK);								// Columns write Ones
	MatrixKeypad_Rows_Write(~KEYROWMASK);								// Rows write Zeroes
	if ((Column = MatrixKeypad_Columns_Read()) == KEYCOLMASK) return 0;	// Error or bounce

	return (MatrixKeypad_LowestBitPosition(~Row) | (MatrixKeypad_LowestBitPosition(~Column) << ColShift))+1; 
}
/********************************************************************************/

uint8 MatrixKeypad_getNewKeyScan(void)
{
    return     MatrixKeypad_NewKey;
}

//***************************************************//
//* Service routine called by KbdScan_Int interrupt	*//
//***************************************************//
CY_ISR(MatrixKeypad_Debounce)
{
    static uint8	Debounced_Keys = 0; 							// Last debounced state of the switches
    static uint8 	State[5]	= {0};	// 	Array that maintains bounce status
    static uint8	Index = 0;					//	Index into State
    static uint8	Key_Pressed_Delay	= 0;	//	Delay for continous pressed Key before first keystroke is generated
    static uint8	Key_Generate_Delay	= 0;	//	Delay for generating next Keystroke
    static uint16_t Key_longPressDelay = 0;
    uint8 ii;
    uint8 NewKey;

    MatrixKeypad_NewKey = MatrixKeypad_ReadSwitches();
    State[Index++] = MatrixKeypad_NewKey;
	//State[Index++] = MatrixKeypad_ReadSwitches();
	if(Index >= 5)Index = 0;

	for(ii = 0, NewKey = 0xff; ii < 5; NewKey &= State[ii++]);

    /***** NewKey is new, Debounced_Keys is old *****/
    /*	new old 									*/
    /*	0	0	 	nothing happened	!new & !old	*/
    /*	0	1		key released		!new & old	*/
    /*	1	0		key pressed			new & !old	*/
    /*	1	1		continued keypress	new & old	*/
    /************************************************/
	if ((uint8)((~NewKey) & (~Debounced_Keys))== 0xffu)	// Nothing has happened
	{
		Debounced_Keys = NewKey;
		return;
	}
	
	if ((uint8)(~NewKey & Debounced_Keys)!= 0x00u)		// Any key released
	{
		Debounced_Keys = NewKey;
		Key_Pressed_Delay = 0;
        Key_longPressDelay = 0;
		return;
	}
	
	if ((uint8)(NewKey & ~Debounced_Keys)!= 0x00u)		// Key pressed seen first time
	{
		MatrixKeypad_PutChar(NewKey);
		Key_Pressed_Delay = 0;
        Key_longPressDelay = 0;
		Debounced_Keys = NewKey;
		return;
	}
	if ((uint8)(NewKey & Debounced_Keys)!= 0x00u)		// Key is continously pressed
	{
        //check for long press
        if (Key_longPressDelay < LONG_PRESS_DELAY_DEFAULT)
        {
            Key_longPressDelay++;
            MatrixKeypad_longPressLatched = 0;
        }      
        else
        {
            if (MatrixKeypad_longPressLatched == 0) MatrixKeypad_PutCharLong(NewKey);
            MatrixKeypad_longPressLatched = 1;
          
        }
        
        
		if (INITIAL_DELAY == 0) return;			// Key repetition not wanted
		if (Key_Pressed_Delay < INITIAL_DELAY)
		{
			Key_Pressed_Delay++;
			Key_Generate_Delay = 0;
		}
		else
		{
			Key_Generate_Delay++;
			if (Key_Generate_Delay >= 0)
			{
				Key_Generate_Delay = 0;
				MatrixKeypad_PutChar(NewKey);
			}
		}
		Debounced_Keys = NewKey;	
	}
}
/********************************************************************************/

/* [] END OF FILE */
