
// The console command interface is generally used only by console.c, 
// if you want to add a command, go to consoleCommands.c

#ifndef CONSOLE_COMMANDS_H
#define CONSOLE_COMMANDS_H

#include <stdint.h>
#include "console.h"
  
#define CONSOLE_COMMAND_MAX_COMMAND_LENGTH  10		    // command only
#define CONSOLE_COMMAND_MAX_LENGTH          15			// whole command with argument
#define CONSOLE_COMMAND_MAX_HELP_LENGTH     32			// if this is zero, there will be no  help (XXXOPT: RAM reduction)

#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
	#define HELP(x)  (x)
#else
	#define HELP(x)	  0
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH

typedef eCommandResult_T(*ConsoleCommand_T)(const char buffer[]);

/* -------------------------------------------------------------- 
 * This parameteres tell what the console response type is:
 * 1. COMMAND_RET_TYPE_MSG -    classic message type response
 * 2. COMMAND_RET_TYPE_DATA -   pure byte data stripped out of txt formatting
 *                              chars.
*/
typedef enum
{
    COMMAND_RET_TYPE_MSG,
    COMMAND_RET_TYPE_DATA
    
}eCommandRetType_T;


typedef struct sConsoleCommandStruct
{
    char* name;
    uint8_t nameLength;
    eCommandRetType_T returnType;
    ConsoleCommand_T execute;
    #if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
	char help[CONSOLE_COMMAND_MAX_HELP_LENGTH];
    #else
	uint8_t help;
    #endif // CONSOLE_COMMAND_MAX_HELP_LENGTH 
} sConsoleCommandTable_T;



#define CONSOLE_COMMAND_TABLE_END {NULL,0,COMMAND_RET_TYPE_MSG,NULL, HELP("")}

const sConsoleCommandTable_T* ConsoleCommandsGetTable(void);

eCommandResult_T ConsoleCommandHelp(const char buffer[]);


#endif // CONSOLE_COMMANDS_H

