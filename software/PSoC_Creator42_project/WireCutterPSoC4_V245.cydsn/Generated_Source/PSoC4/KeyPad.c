//****************************************************************
//
//	KeyPad Matrix KeyPad Component
//	Version 1.0 	August 2012
//	Author:			Bob Marlowe
//	J. Meier Software - Entwicklung
//	Steinfelder Str. 11
//	D-27404 Bruemmerhof
//	(C) 2012 J.Meier Software - Entwicklung
//
//	Given as an Example to the Cypress PSoC community
//	Use as-it-is, use at own risk
//*****************************************************************
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
#define KeyPad_KEYBUFFSIZE 16						// Number of Keypresses to buffer
#define KEYCOLMASK ((1 << 4)-1)							// Mask-bits Column
#define KEYROWMASK ((1 << 4)-1)							// Mask-bits Row
#define LONG_PRESS_DELAY_DEFAULT (25)
//********************* Prototypes **********************************************/
CY_ISR_PROTO(KeyPad_Debounce);								// Interrrupt handler 

//************************ Data Area ********************************************/
/********** Circular Key Buffer ***************/
uint8	KeyPad_WriteIndex = 0;							
uint8	KeyPad_ReadIndex = 0;
char	KeyPad_Buffer[KeyPad_KEYBUFFSIZE];
/********************************************/
/********** Circular Key Buffer ***************/
uint8	KeyPad_BufLongWriteIndex = 0;							
uint8	KeyPad_BufLongReadIndex = 0;
char	KeyPad_BufLong[KeyPad_KEYBUFFSIZE];
/********************************************/


volatile uint8 KeyPad_NewKey;
volatile uint8_t KeyPad_longPressLatched = 0;

char * KeyPad_TransTable = NULL;								// Address of supplied translation table
/***************** Example of a translation-table *******************************
char TranslateTable[MAXKEYVALUE] = {
										'1','2','3','4',
										'5','6','7','8',
										'9','*','0','#'
									 };
*/

/************************* Code Area ********************************************/
void KeyPad_Start(char * TranslationTable)					// API to start the component
{
	KeyPad_KbdScan_Int_StartEx(KeyPad_Debounce);
	KeyPad_KbdScan_Int_SetPriority(7u);
	KeyPad_TransTable = TranslationTable;;
	
}
/********************************************************************************/

void KeyPad_Sleep(void)
{
	// Shutdown the Interrupt
	KeyPad_KbdScan_Int_Disable();
	// Shutdown the ports
	KeyPad_Rows_SetDriveMode(KeyPad_Rows_DM_DIG_HIZ);
	KeyPad_Columns_SetDriveMode(KeyPad_Columns_DM_DIG_HIZ);
}
/********************************************************************************/

void KeyPad_Wakeup(void)
{
	// Wakeup ports
	KeyPad_Rows_SetDriveMode(KeyPad_Rows_DM_RES_DWN);
	KeyPad_Columns_SetDriveMode(KeyPad_Columns_DM_STRONG);
	// Wakeup Interrupt
	KeyPad_KbdScan_Int_Enable();
}
/********************************************************************************/

char KeyPad_TranslateKey(char Key)
{
	if(KeyPad_TransTable == NULL) return Key;
	else return KeyPad_TransTable[(uint8)Key-1];
}
/********************************************************************************/

void KeyPad_PutCharLong(char Ch)									// Can be introduced as API later to simulate keystrokes
{
uint8 TrChar;
	if((TrChar=KeyPad_TranslateKey(Ch))== INVALIDCHAR) return;
	KeyPad_BufLong[KeyPad_BufLongWriteIndex++] = TrChar;
	if (KeyPad_BufLongWriteIndex >= KeyPad_KEYBUFFSIZE) KeyPad_BufLongWriteIndex = 0;
	// Signal Keypress
#ifdef KeyPad_Fire_int	
	KeyPad_KbdControl_Write(0x02);
	KeyPad_KbdControl_Write(0x00);
#endif
}
/********************************************************************************/
void KeyPad_PutChar(char Ch)									// Can be introduced as API later to simulate keystrokes
{
uint8 TrChar;
	if((TrChar=KeyPad_TranslateKey(Ch))== INVALIDCHAR) return;
	KeyPad_Buffer[KeyPad_WriteIndex++] = TrChar;
	if (KeyPad_WriteIndex >= KeyPad_KEYBUFFSIZE) KeyPad_WriteIndex = 0;
	// Signal Keypress
#ifdef KeyPad_Fire_int	
	KeyPad_KbdControl_Write(0x01);
	KeyPad_KbdControl_Write(0x00);
#endif
}

/********************************************************************************/

char KeyPad_CharReady(void)									// API to check for keys in buffer
{
	return (KeyPad_WriteIndex != KeyPad_ReadIndex);
}
/********************************************************************************/
char KeyPad_CharLongReady(void)									// API to check for keys in buffer
{
	return (KeyPad_BufLongWriteIndex != KeyPad_BufLongReadIndex);
}

/********************************************************************************/

char KeyPad_GetChar(void)										// API to rerieve a char from buffer or wait until key pressed (blocking)
{
char Result;
	while (!KeyPad_CharReady());								// Wait until buffer contains a char
	Result = KeyPad_Buffer[KeyPad_ReadIndex++];
	if (KeyPad_ReadIndex >= KeyPad_KEYBUFFSIZE) KeyPad_ReadIndex = 0; // Adjust index 
	return Result;
}
/********************************************************************************/

char KeyPad_GetCharLong(void)										// API to rerieve a char from buffer or wait until key pressed (blocking)
{
char Result;
	while (!KeyPad_CharLongReady());								// Wait until buffer contains a char
	Result = KeyPad_BufLong[KeyPad_BufLongReadIndex++];
	if (KeyPad_BufLongReadIndex >= KeyPad_KEYBUFFSIZE) KeyPad_BufLongReadIndex = 0; // Adjust index 
	return Result;
}

/********************************************************************************/

uint8 KeyPad_LowestBitPosition(uint8 Val)						// Check for a bit on, return the position in Val
{
uint8 ii;
	for (ii=0; ii < 8;ii++,Val >>= 1)
	{
		if (Val & 0x01) return ii;
	}
	return 0xff;
}

/********************************************************************************/

uint8 KeyPad_Readswitches(void) CYREENTRANT;
uint8 KeyPad_ReadSwitches(void) 
{
    volatile uint8	Row;
    volatile uint8	Column;
	//*** Get Row ***//
	KeyPad_Rows_Write(KEYROWMASK);								// Rows write Ones
	KeyPad_Columns_Write(~KEYCOLMASK);							// Cols write Zeroes
	if ((Row=KeyPad_Rows_Read()) == KEYROWMASK) return 0;			// No key pressed if all rows still ones
	//*** Get Column ***//
	KeyPad_Columns_Write(KEYCOLMASK);								// Columns write Ones
	KeyPad_Rows_Write(~KEYROWMASK);								// Rows write Zeroes
	if ((Column = KeyPad_Columns_Read()) == KEYCOLMASK) return 0;	// Error or bounce

	return (KeyPad_LowestBitPosition(~Row) | (KeyPad_LowestBitPosition(~Column) << ColShift))+1; 
}
/********************************************************************************/

uint8 KeyPad_getNewKeyScan(void)
{
    return     KeyPad_NewKey;
}

//***************************************************//
//* Service routine called by KbdScan_Int interrupt	*//
//***************************************************//
CY_ISR(KeyPad_Debounce)
{
    static uint8	Debounced_Keys = 0; 							// Last debounced state of the switches
    static uint8 	State[5]	= {0};	// 	Array that maintains bounce status
    static uint8	Index = 0;					//	Index into State
    static uint8	Key_Pressed_Delay	= 0;	//	Delay for continous pressed Key before first keystroke is generated
    static uint8	Key_Generate_Delay	= 0;	//	Delay for generating next Keystroke
    static uint16_t Key_longPressDelay = 0;
    uint8 ii;
    uint8 NewKey;

    KeyPad_NewKey = KeyPad_ReadSwitches();
    State[Index++] = KeyPad_NewKey;
	//State[Index++] = KeyPad_ReadSwitches();
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
		KeyPad_PutChar(NewKey);
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
            KeyPad_longPressLatched = 0;
        }      
        else
        {
            if (KeyPad_longPressLatched == 0) KeyPad_PutCharLong(NewKey);
            KeyPad_longPressLatched = 1;
          
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
				KeyPad_PutChar(NewKey);
			}
		}
		Debounced_Keys = NewKey;	
	}
}
/********************************************************************************/

/* [] END OF FILE */
