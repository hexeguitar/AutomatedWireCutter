//****************************************************************
//
//	`$INSTANCE_NAME` Matrix KeyPad Component
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
//  - fixed a bug the component was not able to return a press of the fist key (index = 0)
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
#define INITIAL_DELAY (`$InitRepCount` - `$RepCount`) 					// Wait-Time Before Key first Repeated
/********** Key Buffer **********************/
#define ColShift UpLog2(`$NumberOfCols`)								// Number of bits needed to express the column
#define `$INSTANCE_NAME`_KEYBUFFSIZE `$KeyBuffSize`						// Number of Keypresses to buffer
#define KEYCOLMASK ((1 << `$NumberOfCols`)-1)							// Mask-bits Column
#define KEYROWMASK ((1 << `$NumberOfRows`)-1)							// Mask-bits Row
#define LONG_PRESS_DELAY_DEFAULT (`$LongPressDelay`)
//********************* Prototypes **********************************************/
CY_ISR_PROTO(`$INSTANCE_NAME`_Debounce);								// Interrrupt handler 

//************************ Data Area ********************************************/
/********** Circular Key Buffer ***************/
uint8	`$INSTANCE_NAME`_WriteIndex = 0;							
uint8	`$INSTANCE_NAME`_ReadIndex = 0;
char	`$INSTANCE_NAME`_Buffer[`$INSTANCE_NAME`_KEYBUFFSIZE];
/********************************************/
/********** Circular Key Buffer ***************/
uint8	`$INSTANCE_NAME`_BufLongWriteIndex = 0;							
uint8	`$INSTANCE_NAME`_BufLongReadIndex = 0;
char	`$INSTANCE_NAME`_BufLong[`$INSTANCE_NAME`_KEYBUFFSIZE];
/********************************************/


volatile uint8 `$INSTANCE_NAME`_NewKey;
volatile uint8_t `$INSTANCE_NAME`_longPressLatched = 0;

char * `$INSTANCE_NAME`_TransTable = NULL;								// Address of supplied translation table
/***************** Example of a translation-table *******************************
char TranslateTable[MAXKEYVALUE] = {
										'1','2','3','4',
										'5','6','7','8',
										'9','*','0','#'
									 };
*/

/************************* Code Area ********************************************/
void `$INSTANCE_NAME`_Start(char * TranslationTable)					// API to start the component
{
	`$INSTANCE_NAME`_KbdScan_Int_StartEx(`$INSTANCE_NAME`_Debounce);
	`$INSTANCE_NAME`_KbdScan_Int_SetPriority(7u);
	`$INSTANCE_NAME`_TransTable = TranslationTable;;
	
}
/********************************************************************************/

void `$INSTANCE_NAME`_Sleep(void)
{
	// Shutdown the Interrupt
	`$INSTANCE_NAME`_KbdScan_Int_Disable();
	// Shutdown the ports
	`$INSTANCE_NAME`_Rows_SetDriveMode(`$INSTANCE_NAME`_Rows_DM_DIG_HIZ);
	`$INSTANCE_NAME`_Columns_SetDriveMode(`$INSTANCE_NAME`_Columns_DM_DIG_HIZ);
}
/********************************************************************************/

void `$INSTANCE_NAME`_Wakeup(void)
{
	// Wakeup ports
	`$INSTANCE_NAME`_Rows_SetDriveMode(`$INSTANCE_NAME`_Rows_DM_RES_DWN);
	`$INSTANCE_NAME`_Columns_SetDriveMode(`$INSTANCE_NAME`_Columns_DM_STRONG);
	// Wakeup Interrupt
	`$INSTANCE_NAME`_KbdScan_Int_Enable();
}
/********************************************************************************/

char `$INSTANCE_NAME`_TranslateKey(char Key)
{
	if(`$INSTANCE_NAME`_TransTable == NULL) return Key;
	else return `$INSTANCE_NAME`_TransTable[(uint8)Key-1];
}
/********************************************************************************/

void `$INSTANCE_NAME`_PutCharLong(char Ch)									// Can be introduced as API later to simulate keystrokes
{
uint8 TrChar;
	if((TrChar=`$INSTANCE_NAME`_TranslateKey(Ch))== INVALIDCHAR) return;
	`$INSTANCE_NAME`_BufLong[`$INSTANCE_NAME`_BufLongWriteIndex++] = TrChar;
	if (`$INSTANCE_NAME`_BufLongWriteIndex >= `$INSTANCE_NAME`_KEYBUFFSIZE) `$INSTANCE_NAME`_BufLongWriteIndex = 0;
	// Signal Keypress
#ifdef `$INSTANCE_NAME`_Fire_int	
	`$INSTANCE_NAME`_KbdControl_Write(0x02);
	`$INSTANCE_NAME`_KbdControl_Write(0x00);
#endif
}
/********************************************************************************/
void `$INSTANCE_NAME`_PutChar(char Ch)									// Can be introduced as API later to simulate keystrokes
{
uint8 TrChar;
	if((TrChar=`$INSTANCE_NAME`_TranslateKey(Ch))== INVALIDCHAR) return;
	`$INSTANCE_NAME`_Buffer[`$INSTANCE_NAME`_WriteIndex++] = TrChar;
	if (`$INSTANCE_NAME`_WriteIndex >= `$INSTANCE_NAME`_KEYBUFFSIZE) `$INSTANCE_NAME`_WriteIndex = 0;
	// Signal Keypress
#ifdef `$INSTANCE_NAME`_Fire_int	
	`$INSTANCE_NAME`_KbdControl_Write(0x01);
	`$INSTANCE_NAME`_KbdControl_Write(0x00);
#endif
}

/********************************************************************************/

char `$INSTANCE_NAME`_CharReady(void)									// API to check for keys in buffer
{
	return (`$INSTANCE_NAME`_WriteIndex != `$INSTANCE_NAME`_ReadIndex);
}
/********************************************************************************/
// reset the short press char buffer
void `$INSTANCE_NAME`_clearCharBuffer(void)
{
    `$INSTANCE_NAME`_WriteIndex = `$INSTANCE_NAME`_ReadIndex = 0;
}
/********************************************************************************/
char `$INSTANCE_NAME`_CharLongReady(void)									// API to check for keys in buffer
{
	return (`$INSTANCE_NAME`_BufLongWriteIndex != `$INSTANCE_NAME`_BufLongReadIndex);
}
/********************************************************************************/
// reset the short press char buffer
void `$INSTANCE_NAME`_clearCharBufferLong(void)
{
    `$INSTANCE_NAME`_BufLongWriteIndex = `$INSTANCE_NAME`_BufLongReadIndex = 0;
}
/********************************************************************************/


char `$INSTANCE_NAME`_GetChar(void)										// API to rerieve a char from buffer or wait until key pressed (blocking)
{
char Result;
	while (!`$INSTANCE_NAME`_CharReady());								// Wait until buffer contains a char
	Result = `$INSTANCE_NAME`_Buffer[`$INSTANCE_NAME`_ReadIndex++];
	if (`$INSTANCE_NAME`_ReadIndex >= `$INSTANCE_NAME`_KEYBUFFSIZE) `$INSTANCE_NAME`_ReadIndex = 0; // Adjust index 
	return Result;
}
/********************************************************************************/



char `$INSTANCE_NAME`_GetCharLong(void)										// API to rerieve a char from buffer or wait until key pressed (blocking)
{
char Result;
	while (!`$INSTANCE_NAME`_CharLongReady());								// Wait until buffer contains a char
	Result = `$INSTANCE_NAME`_BufLong[`$INSTANCE_NAME`_BufLongReadIndex++];
	if (`$INSTANCE_NAME`_BufLongReadIndex >= `$INSTANCE_NAME`_KEYBUFFSIZE) `$INSTANCE_NAME`_BufLongReadIndex = 0; // Adjust index 
	return Result;
}

/********************************************************************************/

uint8 `$INSTANCE_NAME`_LowestBitPosition(uint8 Val)						// Check for a bit on, return the position in Val
{
uint8 ii;
	for (ii=0; ii < 8;ii++,Val >>= 1)
	{
		if (Val & 0x01) return ii;
	}
	return 0xff;
}

/********************************************************************************/

uint8 `$INSTANCE_NAME`_Readswitches(void) CYREENTRANT;
uint8 `$INSTANCE_NAME`_ReadSwitches(void) 
{
    volatile uint8	Row;
    volatile uint8	Column;
	//*** Get Row ***//
	`$INSTANCE_NAME`_Rows_Write(KEYROWMASK);								// Rows write Ones
	`$INSTANCE_NAME`_Columns_Write(~KEYCOLMASK);							// Cols write Zeroes
	if ((Row=`$INSTANCE_NAME`_Rows_Read()) == KEYROWMASK) return 0;			// No key pressed if all rows still ones
	//*** Get Column ***//
	`$INSTANCE_NAME`_Columns_Write(KEYCOLMASK);								// Columns write Ones
	`$INSTANCE_NAME`_Rows_Write(~KEYROWMASK);								// Rows write Zeroes
	if ((Column = `$INSTANCE_NAME`_Columns_Read()) == KEYCOLMASK) return 0;	// Error or bounce

	return (`$INSTANCE_NAME`_LowestBitPosition(~Row) | (`$INSTANCE_NAME`_LowestBitPosition(~Column) << ColShift))+1; 
}
/********************************************************************************/

uint8 `$INSTANCE_NAME`_getNewKeyScan(void)
{
    return     `$INSTANCE_NAME`_NewKey;
}

//***************************************************//
//* Service routine called by KbdScan_Int interrupt	*//
//***************************************************//
CY_ISR(`$INSTANCE_NAME`_Debounce)
{
    static uint8	Debounced_Keys = 0; 							// Last debounced state of the switches
    static uint8 	State[`$NumberOfReadings`]	= {0};	// 	Array that maintains bounce status
    static uint8	Index = 0;					//	Index into State
    static uint8	Key_Pressed_Delay	= 0;	//	Delay for continous pressed Key before first keystroke is generated
    static uint8	Key_Generate_Delay	= 0;	//	Delay for generating next Keystroke
    static uint16_t Key_longPressDelay = 0;
    uint8 ii;
    uint8 NewKey;

    `$INSTANCE_NAME`_NewKey = `$INSTANCE_NAME`_ReadSwitches();
    State[Index++] = `$INSTANCE_NAME`_NewKey;
	//State[Index++] = `$INSTANCE_NAME`_ReadSwitches();
	if(Index >= `$NumberOfReadings`)Index = 0;

	for(ii = 0, NewKey = 0xff; ii < `$NumberOfReadings`; NewKey &= State[ii++]);

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
		`$INSTANCE_NAME`_PutChar(NewKey);
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
            `$INSTANCE_NAME`_longPressLatched = 0;
        }      
        else
        {
            if (`$INSTANCE_NAME`_longPressLatched == 0) `$INSTANCE_NAME`_PutCharLong(NewKey);
            `$INSTANCE_NAME`_longPressLatched = 1;
          
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
			if (Key_Generate_Delay >= `$RepCount`)
			{
				Key_Generate_Delay = 0;
				`$INSTANCE_NAME`_PutChar(NewKey);
			}
		}
		Debounced_Keys = NewKey;	
	}
}
/********************************************************************************/

/* [] END OF FILE */
