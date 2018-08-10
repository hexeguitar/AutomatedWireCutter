/*


*/


// ConsoleCommands.c
// This is where you add commands:
//		1. Add a protoype
//			static eCommandResult_T ConsoleCommandVer(const char buffer[]);
//		2. Add the command to mConsoleCommandTable
//		    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
//		3. Implement the function, using ConsoleReceiveParam<Type> to get the parameters from the buffer.


#include <project.h>
#include <string.h>
#include "consoleCommands.h"
#include "console.h"
#include "consoleIo.h"
#include "tinyprintf.h"
#include "gui.h"
#include "engine.h"
#include "adc.h"
#include "utils.h"
#include "EEP_25.h"

#define IGNORE_UNUSED_VARIABLE(x)     if ( &x == &x ) {}

static eCommandResult_T ConsoleCommandComment(const char buffer[]);
static eCommandResult_T ConsoleCommandVer(const char buffer[]);
static eCommandResult_T ConsoleCommandSetWireLength(const char buffer[]);
static eCommandResult_T ConsoleCommandSetWireCount(const char buffer[]);
static eCommandResult_T ConsoleCommandStartJob(const char buffer[]);
static eCommandResult_T ConsoleCommandAbortJob(const char buffer[]);
static eCommandResult_T ConsoleCommandPauseJob(const char buffer[]);
static eCommandResult_T ConsoleCommandResetJob(const char buffer[]);
static eCommandResult_T ConsoleCommandMakeCut(const char buffer[]);
static eCommandResult_T ConsoleCommandRunMM(const char buffer[]);
static eCommandResult_T ConsoleCommandRewMM(const char buffer[]);
static eCommandResult_T ConsoleCommandGetReport(const char buffer[]);
static eCommandResult_T ConsoleCommandSetCut(const char buffer[]);
static eCommandResult_T ConsoleCommandTMCstat(const char buffer[]);
static eCommandResult_T ConsoleCommandFreset(const char buffer[]);



//##############################################################################
static const sConsoleCommandTable_T mConsoleCommandTable[] =
{
    {"help",4,COMMAND_RET_TYPE_MSG, &ConsoleCommandHelp,HELP("Lists the commands")},
    {"ver",3,COMMAND_RET_TYPE_MSG, &ConsoleCommandVer, HELP("Show hardware revision")},
    {"setlen",6,COMMAND_RET_TYPE_MSG, &ConsoleCommandSetWireLength, HELP("Set wire length in mm")},
    {"setcnt",6,COMMAND_RET_TYPE_MSG, &ConsoleCommandSetWireCount, HELP("Set wire count in pcs")},
    {"start",5,COMMAND_RET_TYPE_MSG, &ConsoleCommandStartJob, HELP("Start new job")},
    {"abort",5,COMMAND_RET_TYPE_MSG, &ConsoleCommandAbortJob, HELP("Abort job")},
    {"reset",5,COMMAND_RET_TYPE_MSG, &ConsoleCommandResetJob, HELP("Reset job")},
    {"pause",5,COMMAND_RET_TYPE_MSG, &ConsoleCommandPauseJob, HELP("1=ON, 0=OFF ")},
    {"cut",3,COMMAND_RET_TYPE_MSG, &ConsoleCommandMakeCut, HELP("Make a cut")},
    {"run",3,COMMAND_RET_TYPE_MSG, &ConsoleCommandRunMM, HELP("Pull X mm")},
    {"rew",3,COMMAND_RET_TYPE_MSG, &ConsoleCommandRewMM, HELP("Rewind X mm")},
    {"report",6,COMMAND_RET_TYPE_DATA, &ConsoleCommandGetReport, HELP("Get device report")},
    {"setcut",6,COMMAND_RET_TYPE_MSG, &ConsoleCommandSetCut, HELP("set number of cuts (max 3)")},
    {"freset",6,COMMAND_RET_TYPE_MSG, &ConsoleCommandFreset, HELP("Factory reset")},
    {"TMCsts",6,COMMAND_RET_TYPE_MSG, &ConsoleCommandTMCstat, HELP("print TMC2130 status")},
	CONSOLE_COMMAND_TABLE_END // must be LAST
};

//##############################################################################
static eCommandResult_T ConsoleCommandSetWireLength(const char buffer[])
{
    eCommandResult_T result = COMMAND_ERROR;
    long length;
    
    result =  ConsoleReceiveParamInt16(buffer, 1, &length);
    
    if (job_setWireLength((uint32_t) length) == true)
    {
        printf("L=%dmm",length);
        printf(STR_ENDLINE);
        result = COMMAND_SUCCESS;
        gui_update_flag = GUI_PART_UPDATE;
    }
    else result = COMMAND_PARAMETER_ERROR;
    
    return result;
}

//##############################################################################
static eCommandResult_T ConsoleCommandSetWireCount(const char buffer[])
{
    eCommandResult_T result = COMMAND_ERROR;
    long count;
    
    result =  ConsoleReceiveParamInt16(buffer, 1, &count);
    
    if (job_setWireCount((uint32_t) count) == true)
    {
        printf("Cnt=%dpcs",count);
        printf(STR_ENDLINE);
        result = COMMAND_SUCCESS;
        gui_update_flag = GUI_PART_UPDATE;
    }
    else result = COMMAND_PARAMETER_ERROR;
    
    return result; 
}
//##############################################################################
static eCommandResult_T ConsoleCommandStartJob(const char buffer[])
{
    eCommandResult_T result = COMMAND_ERROR;
    IGNORE_UNUSED_VARIABLE(buffer);
    
    if (engine_startCurrentJob() == ENGINE_OK) result = COMMAND_SUCCESS;
    
    return result;
}
//##############################################################################
static eCommandResult_T ConsoleCommandAbortJob(const char buffer[])
{
    eCommandResult_T result = COMMAND_ERROR;
    IGNORE_UNUSED_VARIABLE(buffer);
    
    if (engine_abortCurrentJob() == ENGINE_OK) result = COMMAND_SUCCESS;
    
    return result;
}
//##############################################################################
static eCommandResult_T ConsoleCommandPauseJob(const char buffer[])
{
    eCommandResult_T result = COMMAND_ERROR;
    long state;
    result = ConsoleReceiveParamInt16(buffer, 1, &state);
    
    if (state)
    {
        if (engine_pauseCurrentJob() == ENGINE_OK) result = COMMAND_SUCCESS;
        else                                result = COMMAND_ERROR;
    }
    else
    {
        if (engine_resumeCurrentJob() == ENGINE_OK) result = COMMAND_SUCCESS;
        else                                 result = COMMAND_ERROR;   
    }

    return result;    
}

//##############################################################################
static eCommandResult_T ConsoleCommandResetJob(const char buffer[])
{
    eCommandResult_T result = COMMAND_ERROR;
    IGNORE_UNUSED_VARIABLE(buffer);
    
    if (engine_resetJob() == ENGINE_OK) result = COMMAND_SUCCESS;
    
    return result;
}

//##############################################################################
static eCommandResult_T ConsoleCommandMakeCut(const char buffer[])
{
    eCommandResult_T result = COMMAND_ERROR;
    IGNORE_UNUSED_VARIABLE(buffer);
    
    if (engine_performCut() == ENGINE_OK) result = COMMAND_SUCCESS;
    
    return result;    
}
//##############################################################################
static eCommandResult_T ConsoleCommandRunMM(const char buffer[])
{
    eCommandResult_T result = COMMAND_ERROR;
    long length;
    
    result =  ConsoleReceiveParamInt16(buffer, 1, &length);
    
    printf("Run %dmm",length);
    printf(STR_ENDLINE);
    
    if (engine_runMM((uint32_t) length, STEPPER_ROT_FWD)==ENGINE_OK && result == COMMAND_SUCCESS)  result = COMMAND_SUCCESS;
    else    result = COMMAND_ERROR;
    
    return result;    
}
//##############################################################################
static eCommandResult_T ConsoleCommandRewMM(const char buffer[])
{
    eCommandResult_T result = COMMAND_ERROR;
    long length;
    
    result =  ConsoleReceiveParamInt16(buffer, 1, &length);
   
    printf("Rewind %dmm",length);
    printf(STR_ENDLINE);
    
    if (engine_runMM((uint32_t) length, STEPPER_ROT_BWD)==ENGINE_OK && result == COMMAND_SUCCESS)  result = COMMAND_SUCCESS;
    else    result = COMMAND_ERROR;
    
    return result;     
}

/*****************************************************************************\
 * Function:    ConsoleCommandGetReport
 * Input:       buffer pointer
 * Returns:     result
 * Description: 
 *      Request a packed full status report.
 *      Report data structure [bytes]:
 *      0: engine status (EngineState)
 *      1: error report
 *      2: wire count lo byte
 *      3: wire count hi byte
 *      4: wire length lo byte
 *      5: wire length hi byte
 *      6: wire count elapsed lo byte
 *      7: wire count elapsed hi byte
 *      8: Time to finish ms byte 0
 *      9: Time to finish ms byte 1
 *     10: Time to finish ms byte 2
 *     11: Time to finish ms byte 3
 *     12: Time used to make 1 unit ms byte 0
 *     13: Time used to make 1 unit ms byte 1
 *     14: Time used to make 1 unit ms byte 2
 *     15: Time used to make 1 unit ms byte 3
 *     16: CRC16 byte lo
 *     17: CRC16 byte hi
\*****************************************************************************/
static eCommandResult_T ConsoleCommandGetReport(const char buffer[])
{
    eCommandResult_T result = COMMAND_SUCCESS;
    IGNORE_UNUSED_VARIABLE(buffer);
    uint8_t databuf[18];
    uint32_t t32;
    
    databuf[0] = (uint8_t) engine_getStatus();
    databuf[1] = 0x00;  // !!! to do !!!
    t32 = engine_getWireCount();
    databuf[2] = (t32 >> 8UL) & 0xFF;  
    databuf[3] = t32 & 0xFF;
    t32 = engine_getWireLength_mm();
    databuf[4] = (t32 >> 8UL) & 0xFF;  
    databuf[5] = t32 & 0xFF;
    t32 = engine_getWireCountElapsed();
    databuf[6] = (t32 >> 8UL) & 0xFF;  
    databuf[7] = t32 & 0xFF;    
    t32 = engine_getRemainingTime_ms();
    databuf[8] = (t32 >> 24UL) & 0xFF;  
    databuf[9] = (t32 >> 16UL) & 0xFF;
    databuf[10] = (t32 >> 8UL) & 0xFF;  
    databuf[11] =  t32  & 0xFF;
    t32 = engine_get1pcsTime_ms();
    databuf[12] = (t32 >> 24UL) & 0xFF;  
    databuf[13] = (t32 >> 16UL) & 0xFF;
    databuf[14] = (t32 >> 8UL) & 0xFF;  
    databuf[15] =  t32  & 0xFF;    
    t32 = crc16(databuf,16);
    databuf[16] = (t32 >> 8UL) & 0xFF;  
    databuf[17] = t32 & 0xFF;
    
    UART_SpiUartPutArray(databuf,sizeof(databuf));    

    return result;  
}
//##############################################################################
static eCommandResult_T ConsoleCommandSetCut(const char buffer[])
{
    eCommandResult_T result = COMMAND_ERROR;

    long cuts;
    
    result =  ConsoleReceiveParamInt16(buffer, 1, &cuts);

    if (engine_setNumberOfCuts(cuts) == true)   
    {
        result = COMMAND_SUCCESS;
        printf("Number of cuts set to %d", cuts);
    }
    else result = COMMAND_PARAMETER_ERROR;
 
    printf(STR_ENDLINE);
    return result;  
}
//##############################################################################
static eCommandResult_T ConsoleCommandTMCstat(const char buffer[])
{
    eCommandResult_T result = COMMAND_SUCCESS;
    IGNORE_UNUSED_VARIABLE(buffer);
    
    if (Stepper_printErrors() == false)
    {
        printf("TMC2130 not detected.");
        printf(STR_ENDLINE);
    }

    return result;  
}
//##############################################################################
static eCommandResult_T ConsoleCommandFreset(const char buffer[])
{
    eCommandResult_T result = COMMAND_SUCCESS;
    IGNORE_UNUSED_VARIABLE(buffer);
    engine_FactoryReset();
 
    printf(STR_ENDLINE);
    return result;  
}

//##############################################################################
static eCommandResult_T ConsoleCommandComment(const char buffer[])
{
	// do nothing
	IGNORE_UNUSED_VARIABLE(buffer);
	return COMMAND_SUCCESS;
}
//##############################################################################
 eCommandResult_T ConsoleCommandHelp(const char buffer[])
{
	uint32_t i;
	uint32_t tableLength;
	eCommandResult_T result = COMMAND_SUCCESS;

    IGNORE_UNUSED_VARIABLE(buffer);

	tableLength = sizeof(mConsoleCommandTable) / sizeof(mConsoleCommandTable[0]);
    
	for ( i = 0u ; i < tableLength - 1u ; i++ )
	{
		printf(mConsoleCommandTable[i].name);

#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
		printf(" : ");
		printf((char*)mConsoleCommandTable[i].help);
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
		printf(STR_ENDLINE);
	}
	return result;
}

//##############################################################################
static eCommandResult_T ConsoleCommandVer(const char buffer[])
{
	eCommandResult_T result = COMMAND_SUCCESS;

    IGNORE_UNUSED_VARIABLE(buffer);
    printf((char *)msg_HWver);
    printf("Stepper driver: %s",Stepper_getDriverType()==STPDRV_A4988 ? "A4988":"TMC2130"); 
    
	return result;
}
//##############################################################################
const sConsoleCommandTable_T* ConsoleCommandsGetTable(void)
{
	return (mConsoleCommandTable);
}


