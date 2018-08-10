/*
 * Copyright (c) 2018 by Piotr Zapart / www.hexeguitar.com
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list 
 * of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this 
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 *  
 * Neither the name of the Piotr Zapart or HEXE Guitar Electronics nor the names of its 
 * contributors may be used to endorse or promote products derived from this software 
 * without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 * 
 */
#include <project.h>
#include "engine.h"
#include "ugui.h"
#include "ugui_st7735.h"
#include "gui.h"
#include "EEP_25.h"
#include "keypad.h"
#include "tinyprintf.h"
#include "utils.h"
#include "StepperDriver.h"
#include "ServoDriver.h"
#include "FNbutton.h"
#include "buzzer.h"
#include "debug.h"
#include "console.h"

// #############################################################################
// #### Global variables ###
volatile engineState_t engineState = ENGINE_IDLE;
volatile engineState_t engineStateOld = ENGINE_IDLE;

volatile job_t currentJob;

uint32_t timeForOnePcsTimer, timeForOnePcs_ms;
uint32_t timerJob, time_job_s;
uint32_t timeoutTimer;
uint32_t stepsPerMm;
volatile uint8_t wirePresent = 1;
uint8_t pauseTriggered = 0;
uint8_t feedOverride = 0;
uint32_t Stepper_ErrorReport = 0;
uint8_t numberOfCuts = 2;

extern gui_update_t gui_update_flag;

bool UART_LocalEcho = true;

// #############################################################################
// ### Emulated EEPROM data ###
#define MAX_NUMBER_OF_PRESETS   5
#define EEPROM_PRESETA_OFFSET   0
#define EEPROM_PRESETB_OFFSET   4
#define EEPROM_PRESETC_OFFSET   8
#define EEPROM_PRESETD_OFFSET   12
#define EEPROM_LASTJOB_OFFSET   16

#define EEPROM_LENGTH_OFFSET    0
#define EEPROM_COUNT_OFFSET     2
#define STEPMMCAL_OFFSET        20
#define EEPROM_CFGFLAGS_OFFSET  24

#define CFGFLAGS_FEEDOVERR_BIT      0
#define CFGFLAGS_NUMOFCUTS_BIT      1
#define CFGFLAGS_UARTECHO_BIT       3

#define FAN_TIMEOUT_MS          (10000ul)

enum
{
    PRESET_A = 0,
    PRESET_B = 1,
    PRESET_C = 2,
    PRESET_D = 3,
    PRESET_LAST = 4
};

static const uint8 CYCODE eep_table[]=
{ 
    // ### PRESET 0 ###
    0x00,                // 0 PresetA Wire Length byte hi  
    0x1C,                // 1 PresetA Wire Length byte lo 
    0x00,                // 2 PresetA Wire Count  byte hi
    0xB2,                // 3 PresetA Wire Count  byte lo
    // ### PRESET 1 ###
    0x00,                // 4 PresetB Wire Length byte hi
    0x13,                // 5 PresetB Wire Length byte lo
    0x00,                // 6 PresetB Wire Count  byte hi
    0xC8,                // 7 PresetB Wire Count  byte lo  
    // ### PRESET 2 ###
    0x00,                // 8 PresetC Wire Length byte hi
    0x10,                // 9 PresetC Wire Length byte lo
    0x00,                //10 PresetC Wire Count  byte hi
    0x0A,                //11 PresetC Wire Count  byte lo
    // ### PRESET 3 ###
    0x00,                //12 PresetD Wire Length byte hi
    0x13,                //13 PresetD Wire Length byte lo
    0x00,                //14 PresetD Wire Count  byte hi
    0x05,                //15 PresetD Wire Count  byte lo
    // ### PRESET 4 ### - last job
    0x07,                //16 PresetD Wire Length byte hi
    0xD0,                //17 PresetD Wire Length byte lo
    0x00,                //18 PresetD Wire Count  byte hi
    0x01,                //19 PresetD Wire Count  byte lo
    // ### steps per mm calibration value ###
    0x00,                //20 steps/mm cal value   byte 3
    0x00,                //21 steps/mm cal value   byte 2
    0x06,                //22 steps/mm cal value   byte 1
    0x20,                //23 steps/mm cal value   byte 0
    
    0x05                 //24 config flags
};                


/*****************************************************************************\
 * Config flags
 * |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
 *                                 | UART  |   #of cuts    |feedOR |
 *                                   echo
 *     
\*****************************************************************************/

//const uint32_t factoryLengthCal = 0x0000620;

// #############################################################################
// ### Function declarations ###
void printJob(void);
bool engine_checkWirePresence(void);
bool engine_isWorking(void);

CY_ISR_PROTO(WireNotFound_ISR);

/*****************************************************************************\
 * Function:    WireNotFound_ISR
 * Input:       none
 * Returns:     none
 * Description: 
 *     Wire not found interrupt routine
\*****************************************************************************/
CY_ISR(WireNotFound_ISR)
{
    WireSnsTimeout_WriteCounter(WIRE_DETECTOR_TIMEOUT); //reloaded to avoid rollover
    wirePresent = 0;
}

/*****************************************************************************\
 * Function:    engineInit
 * Input:       none
 * Returns:     none
 * Description: 
 *     Initializes the main engine
\*****************************************************************************/
void engineInit(void)
{
    uint8_t eep_cfgByte;
    
    Stepper_init();
    ServoDriver_init(SERVO_OFF_ANGLE);
    WireSnsTimeout_Start();
    isr_WireNotFound_ClearPending();
    isr_WireNotFound_StartEx(WireNotFound_ISR);
    
    stepsPerMm = engine_getLengthCalValue();                           //load calibrated value from eeprom  
    
    engine_loadLastJob();
    timeForOnePcsTimer = SysTimers_GetTimer(1);                 //timer used to measure the cut time 1ms
    timerJob = SysTimers_GetTimer(1000);                        //timer used to measure job time, resolution 1 second
    timeoutTimer = SysTimers_GetTimer(TASK_TIMEOUT_x1ms);      //generic timeout if something takes too much time
    
    eep_cfgByte = EEP25_readByte(EEPROM_CFGFLAGS_OFFSET);       
    
    if (eep_cfgByte & (1<<CFGFLAGS_FEEDOVERR_BIT)) feedOverride = 1;
    else                                       feedOverride = 0;
    
    numberOfCuts = (eep_cfgByte >> 1) & 0x03;
    
    if (eep_cfgByte & (1<<CFGFLAGS_UARTECHO_BIT))   UART_LocalEcho = true;
    else                                            UART_LocalEcho = false;
    
    
    engineState = ENGINE_IDLE;
}

/*****************************************************************************\
 * Function:    UART_SetLocalEcho
 * Input:       none
 * Returns:     status 
 * Description: 
 *     
\*****************************************************************************/
void UART_SetLocalEcho(bool value)
{
   uint8_t eep_cfgByte= EEP25_readByte(EEPROM_CFGFLAGS_OFFSET);
    
    UART_LocalEcho = value;
    
    printf("\r\nUART echo ");

    if (UART_LocalEcho == true)     
    {
        eep_cfgByte |= (1<<CFGFLAGS_UARTECHO_BIT);
        printf("ON\r\n");
    }
    else
    {
        eep_cfgByte &= ~(1<<CFGFLAGS_UARTECHO_BIT);
        printf("OFF\r\n");
    }
    
    EEP25_writeByte(EEPROM_CFGFLAGS_OFFSET, eep_cfgByte);               // save to eeprom
}

bool UART_getLocalEcho(void) {return UART_LocalEcho;}

/*****************************************************************************\
 * Function:    engine_isWorking
 * Input:       none
 * Returns:     status 
 * Description: 
 *     returns one if the job is in progress
\*****************************************************************************/
bool engine_isWorking(void)
{
    return (    engineState == ENGINE_START                 ||
                engineState == ENGINE_JOB_PULLING           ||
                engineState == ENGINE_JOB_CUTTING           ||
                engineState == ENGINE_JOB_PIECE_COMPLETE    ||
                engineState == ENGINE_JOB_PAUSE);
}

/*****************************************************************************\
 * Function:    engine_getStatus
 * Input:       none
 * Returns:     status 
 * Description: 
 *     
\*****************************************************************************/
engineState_t engine_getStatus(void) { return engineState; }

/*****************************************************************************\
 * Function:    loadDefaultJob
 * Input:       none
 * Returns:     none
 * Description: 
 *     Loads previous, stored in EEPROM job as default at startup
\*****************************************************************************/
engineErrors_t engine_loadLastJob(void)
{
    engineState = ENGINE_IDLE;
    
    //gui_displayJob( GUI_FULL_REDRAW);
    
    engine_loadPreset(PRESET_LAST);
    gui_update_flag = GUI_FULL_UPDATE;
    currentJob.wireCount_elapsed = 0;
    currentJob.jobTime_elapsed_ms = 0;
    currentJob.timeToEnd_ms = 0;
    currentJob.timeForOnePcs = 0;
    currentJob.jobLength_elapsed_mm = 0;
    #ifdef DEBUG_JOB
        printf("-Job added-\r\n");
        printJob();           
    #endif
    time_job_s = 0;
    
    return ENGINE_OK;
}

/*****************************************************************************\
 * Function:    printJob
 * Input:       none
 * Returns:     none
 * Description: 
 *     Prints out details about the current job via UART (debug helper)
\*****************************************************************************/
void printJob(void)
{
    printf("Job Details:\r\n");
    printf("Length=%umm\r\n",currentJob.wireLength_mm);
    printf("Count=%u pieces\r\n",currentJob.wireCount);
    printf("CountElapsed=%u\r\n",currentJob.wireCount_elapsed);
    printf("Steps/mm=%u\r\n",stepsPerMm);
}

/*****************************************************************************\
 * Function:    abortCurrentJob
 * Input:       none
 * Returns:     none
 * Description: 
 *     Aborts the current running job:
 *      - resets the time
 *      - resets the alredy done amount
 *      - disables the StepPulser TxIn to stop the ongoing stepping
 *      - clears the StepPulse Tx fifo to flush any non sent pulses
\*****************************************************************************/
engineErrors_t engine_abortCurrentJob(void)
{
    engineErrors_t result = ENGINE_CMD_ERR;
    
    if (engineState > ENGINE_IDLE && engineState < ENGINE_JOB_COMPLETE)
    {
        engineState = ENGINE_JOB_ABORTED;
        
        currentJob.timeToEnd_ms = 0;
        Stepper_abortRun();
        gui_update_flag = GUI_PART_UPDATE;
        #ifdef DEBUG_JOB
            printf("-Job aborted-\r\n");
            printJob();           
        #endif
        result = ENGINE_OK;
    }
    return result;
}

/*****************************************************************************\
 * Function:    pauseCurrentJob
 * Input:       none
 * Returns:     none
 * Description: 
 *     Pauses the current ongoing job if running
 *     When cutting: wait till the cut is complete and then pause the job
\*****************************************************************************/
engineErrors_t engine_pauseCurrentJob(void)
{
    engineErrors_t result = ENGINE_CMD_ERR;

    if (engineState > ENGINE_IDLE && engineState < ENGINE_JOB_PAUSE)
    {
        time_job_s += SysTimers_GetTimerStatus(timerJob);                          //store elapsed job time
        timeForOnePcs_ms += SysTimers_GetTimerStatus(timeForOnePcsTimer);
        pauseTriggered = 1;
        engineStateOld = engineState;
        if (Servo_handler()==SERVO_IDLE)  engineState = ENGINE_JOB_PAUSE;
        
        Stepper_setPause(1);
        
        #ifdef DEBUG_JOB
            printf("-Job paused-\r\n");
            printJob();           
        #endif
        result = ENGINE_OK;
    } 
    return result;
    
}

/*****************************************************************************\
 * Function:    resumeCurrentJob
 * Input:       none
 * Returns:     none
 * Description: 
 *     Resumes the current job if it has been paused.
\*****************************************************************************/
engineErrors_t engine_resumeCurrentJob(void)
{
    engineErrors_t result = ENGINE_CMD_ERR;
    
    if (engineState == ENGINE_JOB_PAUSE)
    {
        SysTimers_ResetTimer(timerJob,0);   
        SysTimers_ResetTimer(timeForOnePcsTimer,0);
        engineState = engineStateOld;
        pauseTriggered = 0;
        
        Stepper_setPause(0);
        result = ENGINE_OK;
    }
    gui_update_flag = GUI_PART_UPDATE;
    return result;
}

/*****************************************************************************\
 * Function:    startCurrentJob
 * Input:       none
 * Returns:     none
 * Description: 
 *     Initializes and starts a new job
\*****************************************************************************/
engineErrors_t engine_startCurrentJob(void)
{
    engineErrors_t result = ENGINE_CMD_ERR;
    
    #ifdef DEBUG_JOB
    printJob();    
    #endif
    if (engineState == ENGINE_IDLE || engineState == ENGINE_JOB_COMPLETE)
    {        
        engine_resetJob();
        engine_savePreset(PRESET_LAST);        //store last used job settings
        engineState = ENGINE_START;

        SysTimers_ResetTimer(timeoutTimer,0);
        SysTimers_ResetTimer(timeForOnePcsTimer,0);
        SysTimers_ResetTimer(timerJob,0);
        result = ENGINE_OK;  
    }
    gui_update_flag = GUI_PART_UPDATE;
    
    return result;
}

/*****************************************************************************\
 * Function:    loadNewJob
 * Input:       none
 * Returns:     none
 * Description: 
 *     Initializes new job
\*****************************************************************************/
engineErrors_t engine_loadNewJob(uint32_t length, uint32_t count)
{
    engineErrors_t result = ENGINE_CMD_ERR;
    
    #ifdef DEBUG_JOB
    printJob();    
    #endif
    
    if (engineState == ENGINE_IDLE)
    {   
        engine_resetJob();
        currentJob.wireCount = count;
        currentJob.wireLength_mm = length;
        if (engine_savePreset(PRESET_LAST) == CYRET_SUCCESS)        //store last used job settings
            result = ENGINE_OK;
    }
    gui_update_flag = GUI_PART_UPDATE;
    
    return result;
}


/*****************************************************************************\
 * Function:    resetJob
 * Input:       none
 * Returns:     none
 * Description: 
 *     Resets the time measurement and elapsed psc values to 0
\*****************************************************************************/
engineErrors_t engine_resetJob(void)
{
    currentJob.jobTime_elapsed_ms = 0;
    currentJob.jobLength_elapsed_mm = 0;
    currentJob.timeForOnePcs = 0;
    currentJob.timeToEnd_ms = 0;
    currentJob.wireCount_elapsed = 0;
    time_job_s = 0;
    timeForOnePcs_ms = 0;
    engineState = ENGINE_IDLE;
    gui_update_flag = GUI_PART_UPDATE;
    return ENGINE_OK;   
}

/*****************************************************************************\
 * Function:    doYourJob
 * Input:       none
 * Returns:     none
 * Description: 
 *      Main state machine handler
 *      
\*****************************************************************************/
void doYourJob(void)
{
    volatile uint8_t lastKeyPressed;
    uint8_t error;
    static uint8_t repeatBlocker = 0;
    static uint8_t help_page = 0;
    static uint32_t systickLast = 0;
    uint32_t systickNow;    
    static uint32_t fan_timeout = 0;
    
    engine_checkButtons();
    
    // ### check if the wire is present, override state and display info
    engine_checkWirePresence();
    gui_updateWirePresenceIndicator(wirePresent,feedOverride);
    
    // ### check the TMC2130 overtemperature flag, turn on the fan if necessary
    // turn on the fan if using the A4988
    if (engine_getOTempWarning())  
    {
        PWM_0_WriteCompare(100);                                                //full power
        Stepper_ErrorReport = Stepper_getErrorStatus();                         //check if changed   
        fan_timeout = SysTimers_GetSysTickValue();
    }
    else                                                                        //the flag is off, but we still run the fan for a few sec more           
    {
        systickNow = SysTimers_GetSysTickValue();
        if ((systickNow-fan_timeout) >= FAN_TIMEOUT_MS)
        {
            PWM_0_WriteCompare(0);      //fan off 
        }          
    }
    // ### GUI update ###
    gui_displayJob(gui_update_flag);
    
    // ### Main state machine ###
    switch (engineState)
    {
        case ENGINE_IDLE:                       
                        while (MatrixKeypad_CharReady())
                        {
                            lastKeyPressed = MatrixKeypad_GetChar(); 
                            switch (lastKeyPressed)
                            {
                                case '0'... '9': 
                                        break;  
                                case '#': 
                                        engine_setWireLengthMode();
                                        break;
                                case '*':
                                        if (FNbutton_inputStateReg_Read()==0)
                                        {
                                            UART_SetLocalEcho(UART_LocalEcho==true ? false : true);
                                        }
                                        else    engine_setWireCountMode();
                                        break; 
                                case 'A':
                                case 'B':
                                case 'C':
                                case 'D':
                                        if (FNbutton_inputStateReg_Read()==0)    
                                        {
                                                engine_savePreset(lastKeyPressed-'A');
                                        }
                                        else    engine_loadPreset(lastKeyPressed-'A');
                                        break;
                            }    
                        }
                            
                        while (MatrixKeypad_CharLongReady())
                        {
                            lastKeyPressed = MatrixKeypad_GetCharLong(); 
                            switch (lastKeyPressed)
                            {
                                case '0':
                                        feedOverride ^=1;       //toggle the wire presence detector override (useful for testing without wire)
                                        uint8_t eep_cfgByte = EEP25_readByte(EEPROM_CFGFLAGS_OFFSET);       // read the config byte
                                        if (feedOverride) eep_cfgByte |= (1<<CFGFLAGS_FEEDOVERR_BIT);           // update the flag
                                        else              eep_cfgByte &= ~(1<<CFGFLAGS_FEEDOVERR_BIT);
                                        
                                        EEP25_writeByte(EEPROM_CFGFLAGS_OFFSET, eep_cfgByte);               // save to eeprom

                                        break;
                                case '1'... '9': 
                                        engineState = ENGINE_DISPLAY_HELP;//show help screen
                                        gui_displayHelp(0);
                                        break;  
                                case '#':   //it also triggers Edit Wire Length - cancel that
                                        engine_exitEditMode();      
                                        engine_performCut();
                                        break;
                                case '*':   //it lso triggers Edit Wire Count - cancel that
                                        engine_exitEditMode();
                                        engine_rewindMM(50);
                                        break; 
                                case 'A' ... 'D':
                                        break;
                            } 
                        }
                        repeatBlocker = 0;
                        break;
        
        case ENGINE_START:                                                       //starting new piece   
                        
                            MatrixKeypad_clearCharBuffer();                     //clear any button presses
                            MatrixKeypad_clearCharBufferLong();
                        
                            if (wirePresent == 0) engine_pauseCurrentJob();
                            if (currentJob.wireCount && currentJob.wireLength_mm)
                            {   
                                error = Stepper_runMM(currentJob.wireLength_mm,stepsPerMm, STEPPER_ROT_FWD);
                            }
                            else                                                      //wire count or length set to 0? 
                            {
                                currentJob.timeForOnePcs = 0;
                                engineState = ENGINE_JOB_COMPLETE;
                                break;
                            }
                                
                            if (error || SysTimers_GetTimerStatus(timeoutTimer))  //not ready after timeout? something went wrong
                            {
                                engineState = ENGINE_ERROR;
                                #ifdef DEBUG_JOB
                                    printf("Error! Can't pull the wire\r\n");         
                                #endif
                            }                            
                            else                                                         //ok! 
                            {             
                                engineState = ENGINE_JOB_PULLING;
                                #ifdef DEBUG_JOB
                                    printf("Start pulling...\r\n");         
                                #endif                                
                                SysTimers_ResetTimer(timeoutTimer,0);
                            }
                            break; 
        case ENGINE_JOB_PULLING:
                            if (wirePresent == 0) engine_pauseCurrentJob();                   
                            
                            switch(Stepper_getStatus())
                            {
                                case STPDRV_IDLE:           //ok, done pulling, we can start cutting
                                                #ifdef DEBUG_JOB
                                                    printf("Start cutting...\r\n");         
                                                #endif  
                                                Servo_startCut(numberOfCuts);
                                                engineState = ENGINE_JOB_CUTTING;
                                                break;
                                case STPDRV_RUNNING:
                                                break;
                                case STPDRV_ERROR:
                                                #ifdef DEBUG_JOB
                                                    printf("Error! Can't pull the wire\r\n");         
                                                #endif
                                                break;                               
                            }
                            #ifdef USE_TMC2130
                            
                            Stepper_ErrorReport = Stepper_getErrorStatus();
                                
                            if (Stepper_checkErrors())              //check for any DIAG01 errors 
                            {
                                engineState = ENGINE_ERROR;
                            }
                            #endif
                            
                            MatrixKeypad_clearCharBuffer();
                            MatrixKeypad_clearCharBufferLong();

                            repeatBlocker = 0;
                            break;
        case ENGINE_JOB_CUTTING:
                            if (wirePresent == 0) engine_pauseCurrentJob();
                            if (Servo_handler()==SERVO_IDLE)
                            {
                                if (pauseTriggered) 
                                {
                                    pauseTriggered = 0;
                                    engineState = ENGINE_JOB_PAUSE;
                                }
                                else engineState = ENGINE_JOB_PIECE_COMPLETE;
                            }
                            repeatBlocker = 0;
                            MatrixKeypad_clearCharBuffer();
                            MatrixKeypad_clearCharBufferLong();
                            break;
        case ENGINE_JOB_PIECE_COMPLETE:                                         //ready to start making another piece
                            
                            currentJob.timeForOnePcs = timeForOnePcs_ms + SysTimers_GetTimerStatus(timeForOnePcsTimer); // calc the time used to produce one pcs
                            timeForOnePcs_ms = 0;                                                                       // reset timer        
                            currentJob.jobLength_elapsed_mm += currentJob.wireLength_mm;                                // increase total length
                            currentJob.wireCount_elapsed++;                                                             // inc the produced wire count
                            if (currentJob.wireCount_elapsed >= currentJob.wireCount)                                   // are we done?
                            {
                                engineState = ENGINE_JOB_COMPLETE;                                   
                            }
                            else
                            {
                                engineState = ENGINE_START;
                            }
                            // recalculate times
                            currentJob.jobTime_elapsed_ms += currentJob.timeForOnePcs;
                            currentJob.timeToEnd_ms = (currentJob.wireCount-currentJob.wireCount_elapsed)*currentJob.timeForOnePcs;
                            printf("\r\nPiece #%d out of %d produced in %dms",currentJob.wireCount_elapsed, currentJob.wireCount,currentJob.timeForOnePcs);
                            gui_update_flag = GUI_PART_UPDATE;
                            MatrixKeypad_clearCharBuffer();
                            MatrixKeypad_clearCharBufferLong();
                            if (wirePresent == 0) engine_pauseCurrentJob();
                            
                            repeatBlocker = 0;
                            break;
        case ENGINE_JOB_COMPLETE:
                            if (repeatBlocker == 0)
                            {                            
                                time_job_s += SysTimers_GetTimerStatus(timerJob); //update elapsed time job (might has been paused)
                                gui_update_flag = GUI_PART_UPDATE;
                                Buzzer_playSound(ALARM_JOBCMPL_SOUND,ALARM_JOBCMPL_PATT,ALARM_JOBCMPL_COUNT); //beep 
                                printf("\r\nJob complete!\r\n");
                                printf(CONSOLE_PROMPT);
                                repeatBlocker = 1;
                            }
                            while (MatrixKeypad_CharReady())
                            {
                                lastKeyPressed = MatrixKeypad_GetChar();
                                engine_resetJob();
                                switch (lastKeyPressed)
                                {
                                    case '0'... '9':
                                            engineState = ENGINE_IDLE;
                                            break;  
                                    case '#':  
                                            engine_setWireLengthMode();
                                            break;
                                    case '*':   
                                            engine_setWireCountMode();
                                            break; 
                                    case 'A' ... 'D':
                                            engine_loadPreset(lastKeyPressed-'A');
                                            engineState = ENGINE_IDLE;
                                            break;
                                }   
                            }
                            if (FNbutton_getShortPress() || FNbutton_getLongPress())
                            {
                                engine_resetJob();                         
                            }
                            MatrixKeypad_clearCharBufferLong();
                            break;
        case ENGINE_JOB_PAUSE:
                            while (MatrixKeypad_CharLongReady())
                            {
                                lastKeyPressed = MatrixKeypad_GetCharLong();
                                switch (lastKeyPressed)
                                {
                                    case '0':
                                        feedOverride ^=1;       //toggle the wire presence detector override (useful for testing without wire)
                                            break;
                                    
                                    case '1'... '9':
                                            break;  
                                    case '#':  
                                            break;
                                    case '*':   
                                            break; 
                                    case 'A' ... 'D':
                                            break;
                                }
                            }
                            
                            if (repeatBlocker == 0)
                            {
                                repeatBlocker = 1;                                  //should only be printed out once 
                                gui_update_flag = GUI_PART_UPDATE;
                                printf("\r\nJob paused!\r\n");     
                            }
                            MatrixKeypad_clearCharBuffer();
                            break;
        case ENGINE_JOB_ABORTED:
                            if (repeatBlocker == 0) 
                            {
                                repeatBlocker = 1;
                                time_job_s += SysTimers_GetTimerStatus(timerJob); //update elapsed time job (might has been paused)
                                gui_update_flag = GUI_PART_UPDATE;
                                printf("\r\nJob aborted!\r\n");        //should only be printed out once     
                            }
                            if (engine_pressAnyKey())
                            {
                                engine_resetJob();
                            }
                            break;
        case ENGINE_ERROR:
                            
                            if (repeatBlocker==0)
                            {
                                #ifdef DEBUG_JOB
                                    printf("ERROR!!!\r\n");  
                                    printf("Hold down FNbutton to reset...\r\n");
                                    printStepperStatus();
                                #endif
                                gui_update_flag = GUI_PART_UPDATE;
                                Buzzer_playSound(ALARM_ERROR_SOUND,ALARM_ERROR_PATT,ALARM_ERROR_COUNT);
                                Stepper_setPause(1);
                                
                                Stepper_printErrors();
                            }
                            repeatBlocker = 1;
                            if (FNbutton_getShortPress())       //show error report
                            {
                                gui_displayErrorReport(Stepper_getErrorStatus());
                            }
                            
                            if (FNbutton_getLongPress())
                            {

                                engine_abortCurrentJob();
                                Stepper_setPause(0);
                                error = Stepper_clearErrors();
                                if (error)
                                {
                                    #ifdef DEBUG_JOB 
                                        printStepperStatus();
                                        printf("Stepper Driver errors still present\r\n");
                                        printBin(error,8);
                                        
                                    #endif
                                }
                                engine_resetJob();
                                gui_update_flag = GUI_FULL_UPDATE;
                            }

                            break;
        case ENGINE_ENTER_COUNT:
                            while (MatrixKeypad_CharReady())
                            {
                                lastKeyPressed = MatrixKeypad_GetChar(); 
                                switch (lastKeyPressed)
                                {
                                    case '0'... '9': 
                                            //edit num value
                                            engine_editValue((int16_t *)&currentJob.wireCount,(lastKeyPressed-'0'),0l,MAX_WIRE_COUNT,EDIT_CMD_ADD_DIGIT);
                                            gui_update_flag = GUI_PART_UPDATE;
                                            break;  
                                    case '#': 
                                            gui_update_flag = GUI_PART_UPDATE;
                                            engine_editValue((int16_t *)&currentJob.wireCount,0,0,0,EDIT_CMD_RESET);
                                            engine_exitEditMode();
                                            break;
                                    case '*':
                                            engine_editValue((int16_t *)&currentJob.wireCount,0,0l,10l,EDIT_CMD_RM_DIGIT);
                                            engine_loadNewJob(currentJob.wireLength_mm,currentJob.wireCount); //reload job with new settings
                                            gui_update_flag = GUI_PART_UPDATE;
                                            break;
                                    case 'A':
                                    case 'B':
                                    case 'C':
                                    case 'D':
                                            break;
                                }
                            }
                            
                            while (MatrixKeypad_CharLongReady())
                            {
                                lastKeyPressed =MatrixKeypad_GetCharLong(); 
                                switch (lastKeyPressed)
                                {
                                    case '0'... '9': 
                                            break;  
                                    case '#':   //it also triggers Edit Wire Length - cancel that
                                            engine_exitEditMode();
                                            break;
                                    case '*':   //it also triggers Edit Wire Count - cancel that
                                            engine_exitEditMode();
                                            gui_displayJob(GUI_PART_UPDATE);    //force update
                                            engine_rewindMM(50);
                                            break; 
                                    case 'A' ... 'D':
                                            break;
                                } 
                            }
                            break;
        case ENGINE_ENTER_LENGTH:
                            if (keyPressFlag)
                            {
                                while (MatrixKeypad_CharReady())
                                {
                                    lastKeyPressed =MatrixKeypad_GetChar(); 
                                    switch (lastKeyPressed)
                                    {
                                        case '0'... '9': 
                                                //edit num value
                                                engine_editValue((int16_t *)&currentJob.wireLength_mm,(lastKeyPressed-'0'),0l,MAX_WIRE_LENGTH_MM,EDIT_CMD_ADD_DIGIT);
                                                gui_update_flag = GUI_PART_UPDATE;
                                                break;  
                                        case '#':
                                                engine_editValue((int16_t *)&currentJob.wireLength_mm,0,0,0,EDIT_CMD_RESET);
                                                engine_loadNewJob(currentJob.wireLength_mm,currentJob.wireCount); //reload job with new settings
                                                engine_exitEditMode();
                                                break;
                                        case '*':
                                                engine_editValue((int16_t *)&currentJob.wireLength_mm,0,0l,10l,EDIT_CMD_RM_DIGIT);
                                                gui_update_flag = GUI_PART_UPDATE;
                                                break;
                                        case 'A':
                                        case 'B':
                                        case 'C':
                                        case 'D':
                                                break;
                                    }
                                }
                                keyPressFlag = 0;
                            }
                            while (MatrixKeypad_CharLongReady())
                            {
                                lastKeyPressed =MatrixKeypad_GetCharLong(); 
                                switch (lastKeyPressed)
                                {
                                    case '0'... '9': 
                                            break;  
                                    case '#':   //it lso triggers Edit Wire Length - cancel that
                                            engine_exitEditMode();
                                            gui_displayJob(GUI_PART_UPDATE);    //force update
                                            engine_performCut();
                                            break;
                                    case '*':   //it lso triggers Edit Wire Count - cancel that
                                            engine_exitEditMode();
                                            break; 
                                    case 'A' ... 'D':
                                            break;
                                } 
                            }                            
                            break;
        case ENGINE_DISPLAY_HELP:
                            if (MatrixKeypad_CharReady())   
                            {
                                MatrixKeypad_clearCharBuffer();
                                if (++help_page >= GUI_HELP_MAX_PAGES) help_page = 0; 
                                gui_displayHelp(help_page);
                                
                            }
                            if (MatrixKeypad_CharLongReady())
                            {                               
                                MatrixKeypad_clearCharBuffer();
                                MatrixKeypad_clearCharBufferLong();
                                engine_exitEditMode();
                                gui_update_flag = GUI_FULL_UPDATE;                            
                            }
                            
                            break;                            
        case ENGINE_UNKNOWN:        
                            break;
        case ENGINE_DIAGNOSTICS:
                            
        default:            break;
    }


    systickNow = SysTimers_GetSysTickValue();
    if ((systickNow-systickLast) >= 1000)
    {
        //do something at 1s rate
//        printStepperStatus();
//        Stepper_printErrors();        
        
        systickLast = systickNow;
    }  
}

void engine_setDiagnosticsMode(void)
{
    engineState = ENGINE_DIAGNOSTICS;
}

/*****************************************************************************\
 * Function:    job_setWireLength
 * Input:       length_mm
 * Returns:     none
 * Description: 
 *     Set desired wire length in mm
\*****************************************************************************/
bool job_setWireLength(uint32_t length_mm) 
{ 
    bool result = false;
    
    if (length_mm <= MAX_WIRE_LENGTH_MM) 
    {
        currentJob.wireLength_mm = length_mm; 
        result = true;
    }
    
    return result; 
}

/*****************************************************************************\
 * Function:    job_setWireCount
 * Input:       count
 * Returns:     none
 * Description: 
 *     Set desired piece count
\*****************************************************************************/
bool job_setWireCount(uint32_t count) 
{   
    bool result = false;
    if (count <= MAX_WIRE_COUNT) 
    {
        currentJob.wireCount = count;  
        result = true;
    }
    return result; 
}

// #############################################################################
void engine_SignalError(uint8_t motorStop)
{
    engineState = ENGINE_ERROR;
    if (motorStop) StepPulser_DisableTxInt();
}

/*****************************************************************************\
 * Function:    performCut
 * Input:       none
 * Returns:     none
 * Description: 
 *      Performs cut only, blocking helper function for signle actions
 *      Since it is a blocking function, it coud trigger the watchdog.
*       Hence it's cleared in the while loop
\*****************************************************************************/
engineErrors_t engine_performCut(void)
{
    Servo_startCut(numberOfCuts);
    while(Servo_handler()!=SERVO_IDLE)
    {
        CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET); //may trigger the watchdog
    }
    return ENGINE_OK;
}

/*****************************************************************************\
 * Function:    runMM
 * Input:       length in mm
 * Returns:     none
 * Description: 
 *     Pulls requested length of wire, uses currentJob steps/mm value.
 *     Blocking helper function for signle actions
\*****************************************************************************/
engineErrors_t engine_runMM(uint32_t length_mm, Stepper_dir_t dir)
{
    engineErrors_t result = ENGINE_OK;
    
    StepDir_Write(dir);
    Stepper_runMM(length_mm,stepsPerMm, dir);
    while (Stepper_getStatus()!=STPDRV_IDLE)
    {
        //clear the watchdog counter
        CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET);
        //Stepper_checkErrors();
    } 
    return result;
}

/*****************************************************************************\
 * Function:    rewindMM
 * Input:       length in mm
 * Returns:     none
 * Description: 
 *      Pulls requested length of wire in opposite direction, 
 *      uses currentJob steps/mm value.
 *      Blocking helper function for signle actions
\*****************************************************************************/
engineErrors_t engine_rewindMM(uint32_t length_mm)
{
    engineErrors_t result = ENGINE_OK;
    
    Stepper_runMM(length_mm,stepsPerMm, STEPPER_ROT_BWD);
    while (Stepper_getStatus()!=STPDRV_IDLE)
    {
        //clear the watchdog counter
        CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET);
        //Stepper_checkErrors();
    }
    StepDir_Write(STEPPER_ROT_FWD);
    return result;
}

/*****************************************************************************\
 * Function:    runSteps
 * Input:       number of steps
 * Returns:     none
 * Description: 
 *      generate desired number of steps
 *      Blocking helper function for signle actions
\*****************************************************************************/
engineErrors_t engine_runSteps(uint32_t steps)
{
    engineErrors_t result = ENGINE_OK;
    
    Stepper_runSteps(steps);
    while (Stepper_getStatus()!=STPDRV_IDLE)
    {
        //clear the watchdog counter
        CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET);
        //(check for errors)
    } 
    return result;
}

/*****************************************************************************\
 * Function:    getRemainingTime_ms
 * Input:       none
 * Returns:     Time to finish the job in ms
 * Description: 
 *     This is a helper function
\*****************************************************************************/
uint32_t engine_getRemainingTime_ms(void) { return currentJob.timeToEnd_ms; }

/*****************************************************************************\
 * Function:    getJobTimeTotal
 * Input:       none
 * Returns:     finished job time in ms
 * Description: 
 *     Returns the total time it took to finish the current job
\*****************************************************************************/
uint32_t engine_getJobTimeTotal(void) { return time_job_s; }

/*****************************************************************************\
 * Function:    getOTempWarning
 * Input:       none
 * Returns:     overtemperature flag state
 * Description: TMC2130: return the overtemperature pre-warning flag
 *              A4988: return 1, fan always on when working
 *     
\*****************************************************************************/
uint8_t engine_getOTempWarning(void) 
{ 
    if (Stepper_getDriverType() == STPDRV_TMC2130) return (Stepper_ErrorReport & (1<<26)?1:0); 
    else                                         return (engine_isWorking());
}

/*****************************************************************************\
 * Function:    getWirePresence
 * Input:       none
 * Returns:     wire detected flag
 * Description: 
 *     
\*****************************************************************************/
uint8_t engine_getWirePresence(void) { return wirePresent; }

uint32_t engine_get1pcsTime_ms(void) { return currentJob.timeForOnePcs; }

uint16_t engine_getWireCount(void)   { return currentJob.wireCount; }

uint16_t engine_getWireCountElapsed(void) { return currentJob.wireCount_elapsed; }

uint16_t engine_getWireLength_mm(void) { return currentJob.wireLength_mm; }

uint8_t engine_getFeedOverride(void) { return feedOverride; }

uint32_t engine_getElapsedLength_mm(void) { return currentJob.jobLength_elapsed_mm; }

uint32_t engine_getRequiredLength_mm(void) { return (currentJob.wireCount * currentJob.wireLength_mm); }

/*****************************************************************************\
 * Function:    engine_setWireCountMode
 * Input:       none
 * Returns:     none
 * Description: 
 *     Enters the wire count edit mode
 *     
\*****************************************************************************/
void engine_setWireCountMode(void)
{
    engineState = ENGINE_ENTER_COUNT;
    gui_update_flag = GUI_PART_UPDATE;
}

/*****************************************************************************\
 * Function:    engine_setWireLengthMode
 * Input:       none
 * Returns:     none
 * Description: 
 *     Enters the wire length edit mode
 *     
\*****************************************************************************/
void engine_setWireLengthMode(void)
{
    engineState = ENGINE_ENTER_LENGTH;
    gui_update_flag = GUI_PART_UPDATE;
}

/*****************************************************************************\
 * Function:    engine_exitEditMode
 * Input:       none
 * Returns:     none
 * Description: 
 *     Exits the edit modes
 *     
\*****************************************************************************/
void engine_exitEditMode(void)
{
    engine_resetJob();
    engineState = ENGINE_IDLE;
    gui_update_flag = GUI_PART_UPDATE;
}

/*****************************************************************************\
 * Function:    engine_checkWirePresence
 * Input:       none
 * Returns:     wire present flag
 * Description: 
 *     checks the wire detector and resets the flag if required
 *     can be globally overrided with "feedOverride" flag
\*****************************************************************************/
bool engine_checkWirePresence(void)
{
    if (feedOverride == 1) WireSnsTimeout_WriteCounter(0);
    if (((WireSnsTimeout_ReadCounter() < WIRE_DETECTOR_TIMEOUT)) || feedOverride) wirePresent = 1;
    else wirePresent = 0;
    
    return wirePresent;
}

/*****************************************************************************\
 * Function:    engine_editValue
 * Input:       *src, newValue, command
 * Returns:     edited value
 * Description: 
 *     Function used to edit a numerical value using a keypad
 *     
\*****************************************************************************/
void engine_editValue(int16 *src_ptr, int16_t  newValue, int16_t minVal, int16_t maxVal, editCommands_t command)
{
    static uint8_t firstEditFlag = 1;
    int32_t editVal = *src_ptr;                                 // copy value
    
    switch (command)
    {
        case EDIT_CMD_RESET:
                    firstEditFlag = 1;
                    break;
        case EDIT_CMD_ADD_DIGIT:
                    if (firstEditFlag)  editVal = 0;                                     
                    editVal = (editVal * 10)+newValue;
                    if (editVal <= maxVal) *src_ptr = editVal;  //update only if max value not exceeded
                    else                    Buzzer_playSound(ALARM_VALUE_SOUND,ALARM_VALUE_PATT,ALARM_VALUE_COUNT); //beep
                    firstEditFlag = 0;
                    break;
        case EDIT_CMD_RM_DIGIT:
                    if (firstEditFlag)  editVal = 0;
                    
                    editVal = (editVal+5) / 10;
                    if (editVal >= minVal)  *src_ptr = editVal;
                    else                    Buzzer_playSound(ALARM_VALUE_SOUND,ALARM_VALUE_PATT,ALARM_VALUE_COUNT); //beep
                    firstEditFlag = 0;
                    break;
        default:    break;
    }   
    
    
}

/*****************************************************************************\
 * Function:    savePreset
 * Input:       preset number 
 * Returns:     status: success/fail
 * Description: 
 *     Saves job params in external SPI eeprom
\*****************************************************************************/

bool engine_savePreset(uint8_t presetNo)
{
    uint8_t dataOut[4];

    if (presetNo >= MAX_NUMBER_OF_PRESETS) return false;  
    
    dataOut[0] = currentJob.wireLength_mm>>8;
    dataOut[1] = currentJob.wireLength_mm;
    dataOut[2] = currentJob.wireCount>>8;
    dataOut[3] = currentJob.wireCount;
    
    EEP25_writeArray(presetNo*4,dataOut,4);
    
    if (presetNo<MAX_NUMBER_OF_PRESETS-1)
    {
        Buzzer_playSound(ALARM_PRESET_SAVE_SOUND, ALARM_PRESET_SAVE_PATT, ALARM_PRESET_SAVE_COUNT);
    }

    return true;
}    

/*****************************************************************************\
 * Function:    loadPreset
 * Input:       presetNo
 * Returns:     status
 * Description: 
 *     loads stored in EEPROM preset or last used job 
\*****************************************************************************/
engineErrors_t engine_loadPreset(uint8 presetNo)
{
    uint8_t presetData[4];
    EEP25_readArray(presetNo*4, presetData, 4);
    
    currentJob.wireCount = (uint16_t)(presetData[EEPROM_COUNT_OFFSET]<<8) + (uint16_t)(presetData[EEPROM_COUNT_OFFSET+1]);
    currentJob.wireLength_mm = (uint16_t)(presetData[EEPROM_LENGTH_OFFSET]<<8) + (uint16_t)(presetData[EEPROM_LENGTH_OFFSET+1]);
    currentJob.wireCount_elapsed = 0;                         
    currentJob.timeToEnd_ms = 0;
    currentJob.timeForOnePcs = 0;
    currentJob.jobTime_elapsed_ms = 0;
    currentJob.jobLength_elapsed_mm = 0;
    gui_update_flag = GUI_PART_UPDATE;
    
    return ENGINE_OK;
}

/*****************************************************************************\
 * Function:    engine_setNumberOfCuts
 * Input:       new number of cuts
 * Returns:     status
 * Description: 
 *     sets a new number of cuts and stores the value in eeprom 
\*****************************************************************************/
bool engine_setNumberOfCuts(uint8_t value)
{
    uint8_t eep_cfgByte;
    if (value > MAX_NUMBEROFCUTS) return false;
    
    eep_cfgByte = EEP25_readByte(EEPROM_CFGFLAGS_OFFSET);
    eep_cfgByte &= ~(0x06);                                 // clear bits 1 & 2
    eep_cfgByte |= (value & 0x03) << 1;
    EEP25_writeByte(EEPROM_CFGFLAGS_OFFSET, eep_cfgByte);
    numberOfCuts = value;
    return true;
}

uint8_t engine_getNumberOfCuts(void)
{
    return numberOfCuts;
}

/*****************************************************************************\
 * Function:    getLengthCalValue
 * Input:       none
 * Returns:     steps/mm calibration value
 * Description: 
 *      returns the stored in EEPROM steps/mm calibration value
 *      for higher precision, the cal value is shifted left 8bits
 *      out=( (actualCalVal<<8)*Actual_usteps )>>8
 *            |-value stored--|
\*****************************************************************************/
uint32_t engine_getLengthCalValue(void)
{ 
    uint32_t t32; 
    uint8_t dataIn[4];
    
    EEP25_readArray(STEPMMCAL_OFFSET, dataIn, 4);
    
    t32 = (dataIn[0]<<24ul)|(dataIn[1]<<16ul)|(dataIn[2]<<8ul)|(dataIn[3]);
    return ((t32*Stepper_getUsteps())>>8);
}

/*****************************************************************************\
 * Function:    saveLengthCalValue
 * Input:       calibration value
 * Returns:     result
 * Description: 
 *     Stores the steps/mm calibration value in emEEPROM
\*****************************************************************************/
bool saveLengthCalValue(uint32_t newValue)
{
    uint8_t data[4];
    
    if (newValue == engine_getLengthCalValue()) return CYRET_SUCCESS;  //no need to save
    data[0] = newValue >> 24;
    data[1] = newValue >> 16;
    data[2] = newValue >> 8;
    data[3] = newValue;
    
    EEP25_writeArray(STEPMMCAL_OFFSET, data, 4);

    #ifdef DEBUG_JOB
    printf("EEPROM Cal save %s",(result== CYRET_SUCCESS)?"Success":"FAIL!");   
    #endif
    
    Buzzer_playSound(ALARM_PRESET_SAVE_SOUND, ALARM_PRESET_SAVE_PATT, ALARM_PRESET_SAVE_COUNT);
    
    return true;
}

/*****************************************************************************\
 * Function:    engine_pressAnyKey
 * Input:       none
 * Returns:     1 = any key pressed, 0 = no key pressed
 * Description: 
 *     
\*****************************************************************************/
uint8_t engine_pressAnyKey(void)
{
    uint8_t result = 0;
    if (keyPressFlag || keyPressLongFlag || FNbutton_getShortPress())
    {
        keyPressFlag = 0;
        keyPressLongFlag = 0;
        MatrixKeypad_clearCharBuffer();
        MatrixKeypad_clearCharBufferLong();
        result = 1;      
    }
    return result;
}

/*****************************************************************************\
 * Function:    checkButtons
 * Input:       none
 * Returns:     none
 * Description: 
 *     Button press handler
\*****************************************************************************/
void engine_checkButtons(void)
{
    if (FNbutton_getShortPress())
    {
        //FNbutton_clearShortPress();
        #ifdef DEBUG_MAIN
        printf("Button Short Press detected!\r\n"); 
        #endif
        
        switch (engine_getStatus())
        {
            case ENGINE_IDLE:   
                                engine_startCurrentJob();
                                break;
            case ENGINE_START:     
            case ENGINE_JOB_PULLING:            
            case ENGINE_JOB_PIECE_COMPLETE: 
            case ENGINE_JOB_CUTTING:
                                engine_pauseCurrentJob();
                                break;
            case ENGINE_JOB_COMPLETE:                                
                                break;
            case ENGINE_JOB_PAUSE:
                                engine_resumeCurrentJob();
                                break;
            case ENGINE_JOB_ABORTED:
                                engine_loadLastJob();
                                break;
            case ENGINE_ERROR:
                                break;
            case ENGINE_ENTER_COUNT:
                                break;
            case ENGINE_ENTER_LENGTH:
                                break;
            default:            break;
        }    
    }
    if (FNbutton_getLongPress())
    {
        
        #ifdef DEBUG_MAIN
        printf("Button Long Press detected!\r\n");            
        #endif
        
        Stepper_clearErrors();
        
        switch (engine_getStatus())
        {
            case ENGINE_IDLE:   break;
            
            case ENGINE_START:     
            case ENGINE_JOB_PULLING:
            case ENGINE_JOB_CUTTING:
            case ENGINE_JOB_PIECE_COMPLETE:
            case ENGINE_JOB_PAUSE:
                                engine_abortCurrentJob();
                                break;
            case ENGINE_JOB_COMPLETE:

                                break;
            case ENGINE_ERROR:
                                Stepper_clearErrors();
                                break;
            case ENGINE_ENTER_COUNT:
                                break;
            case ENGINE_ENTER_LENGTH:
                                break;
            default:            break;
        }
    }
}

/*****************************************************************************\
 * Function:    engine_FactoryReset
 * Input:       none
 * Returns:     none
 * Description: 
 *     reset settings
\*****************************************************************************/
void engine_FactoryReset(void)
{
    printf("Factory Reset...\r\n");
    EEP25_writeArray(0, (uint8_t *)eep_table, sizeof(eep_table));
    printf("done!\r\n");
    

}

/* [] END OF FILE */
