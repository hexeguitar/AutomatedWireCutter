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

#ifndef ENGINE_H
#define ENGINE_H
     
    #include <cytypes.h>
    #include <stdbool.h>
    #include "StepperDriver.h"
    
    #define WIRE_DETECTOR_TIMEOUT   (50ul)
    
    #define MAX_WIRE_LENGTH_MM  (16000l)
    #define MAX_WIRE_COUNT      (16000l)
    #define MAX_NUMBEROFCUTS    (3)
    
    #define TASK_TIMEOUT_x1ms  50000      //5 sec 
  
    extern volatile uint16_t ADC0_value;
    
    
    typedef enum
    {
        ENGINE_IDLE                 = 0,
        ENGINE_START                = 1,
        ENGINE_JOB_PULLING          = 2,
        ENGINE_JOB_CUTTING          = 3,
        ENGINE_JOB_PIECE_COMPLETE   = 4,
        ENGINE_JOB_PAUSE            = 5,
        ENGINE_JOB_COMPLETE         = 6,
        ENGINE_JOB_ABORTED          = 7,
        ENGINE_ENTER_LENGTH         = 8,
        ENGINE_ENTER_COUNT          = 9,
        ENGINE_ERROR                = 10,
        ENGINE_DISPLAY_HELP         = 11,
        ENGINE_UNKNOWN              = 12,
        ENGINE_DIAGNOSTICS          = 13
        
    }engineState_t;

    typedef enum
    {
        ENGINE_OK,
        ENGINE_CMD_ERR,
        ENGINE_ERR_DIAG0,
        ENGINE_ERR_DIAG1        
        
    }engineErrors_t;
        
    typedef struct
    {
        uint16_t wireLength_mm;         //desired wire length
        uint16_t wireCount;             
        uint16_t wireCount_elapsed;
        uint32_t timeToEnd_ms;
        uint32_t timeForOnePcs;
        uint32_t jobTime_elapsed_ms;   
        uint32_t jobLength_elapsed_mm;
    }job_t;

    typedef enum
    {
        EDIT_CMD_RESET,
        EDIT_CMD_ADD_DIGIT,
        EDIT_CMD_RM_DIGIT

    } editCommands_t;


    void engineInit(void);
    engineErrors_t engine_loadLastJob(void);
    engineErrors_t engine_loadNewJob(uint32_t length, uint32_t count);
    engineErrors_t engine_startCurrentJob(void);
    engineErrors_t engine_pauseCurrentJob(void);
    engineErrors_t engine_resumeCurrentJob(void);
    engineErrors_t engine_abortCurrentJob(void);
    engineErrors_t engine_resetJob(void);
    
    engineErrors_t engine_performCut(void);
    engineErrors_t engine_rewindMM(uint32_t length_mm);
    engineErrors_t engine_runMM(uint32_t length_mm, Stepper_dir_t dir);
    engineErrors_t engine_runSteps(uint32_t steps);

    engineState_t engine_getStatus(void);
    uint32_t engine_get1pcsTime_ms(void);
    uint32_t engine_getElapsedLength_mm(void);
    uint32_t engine_getRequiredLength_mm(void);
    uint32_t engine_getRemainingTime_ms(void);
    uint32_t engine_getJobTimeTotal(void);
    uint8_t engine_getWirePresence(void);
    uint8_t engine_getOTempWarning(void);
    uint16_t engine_getWireCount(void);
    uint16_t engine_getWireCountElapsed(void);
    uint16_t engine_getWireLength_mm(void);
    uint8_t engine_getFeedOverride(void);
    
    // ### EEPROM read/write functions ###
    bool engine_savePreset(uint8_t presetNo);
    engineErrors_t engine_loadPreset(uint8_t presetNo);
    uint32_t engine_getLengthCalValue(void);
    bool engine_saveLengthCalValue(uint32_t newValue);
    bool engine_setNumberOfCuts(uint8_t value);
    
    uint8_t engine_getNumberOfCuts(void);

    bool job_setWireLength(uint32_t length_mm);
    bool job_setWireCount(uint32_t count);
    
    void doYourJob(void);

    
    void engine_SignalError(uint8_t motorStop);
    void engine_setWireCountMode(void);
    void engine_setWireLengthMode(void);
    void engine_exitEditMode(void);
    void engine_setDiagnosticsMode(void);
    
    void engine_editValue(int16 *src_ptr, int16_t  newValue, int16_t minVal, int16_t maxVal, editCommands_t command);
    uint8_t engine_pressAnyKey(void);
    
    void engine_checkButtons(void);
    
    void engine_FactoryReset(void);
    
    void UART_SetLocalEcho(bool value);
    bool UART_getLocalEcho(void);
    
    
#endif
/* [] END OF FILE */
