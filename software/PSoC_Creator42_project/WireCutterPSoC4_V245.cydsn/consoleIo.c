// Console IO is a wrapper between the actual in and output and the console code
// In an embedded system, this might interface to a UART driver.

#include <project.h>
#include "consoleIo.h"
#include "consoleCommands.h"
#include <stdio.h>
#include <stdbool.h>

extern bool UART_LocalEcho;



eConsoleError ConsoleIoInit(void)
{

	return CONSOLE_SUCCESS;
}

/*****************************************************************************\
 * Function:    ConsoleIoReceive
 * Input:       pointer to a char buffer, its length, pointer to readLength variable
 * Returns:     eConsoleError
 * Description: 
 *      / 01.2018 by Piotr Zapart /
 *      function adapted to used hardware and their APIs
 *      Also - the receive function ignores LF chars
 *      Added optional local echo parameter
\*****************************************************************************/

eConsoleError ConsoleIoReceive(uint8_t *buffer, const uint32_t bufferLength, uint32_t *readLength)
{
	uint8_t i = 0;
	char ch = 0;
    
	while ( (UART_SpiUartGetRxBufferSize() > 0) && (i < bufferLength) )
	{
        ch = UART_UartGetChar();
        if (UART_LocalEcho == true)     UART_UartPutChar(ch);           
		if (ch != '\n' )    
        {
            buffer[i] = (uint8_t) ch;
		    i++; 
        }    
    }
	*readLength = i;
	return CONSOLE_SUCCESS;
}

//##############################################################################
eConsoleError ConsoleIoSend(const uint8_t *buffer, const uint32_t bufferLength, uint32_t *sentLength)
{
    eConsoleError result = CONSOLE_SUCCESS;
    UART_UartPutString((char*)buffer);
    *sentLength = bufferLength;

	return result;
}

//##############################################################################
eConsoleError ConsoleIoSendString(const char *buffer)
{
    eConsoleError result = CONSOLE_SUCCESS;
    
    UART_UartPutString((char*)buffer);
	return result;
}

//##############################################################################
