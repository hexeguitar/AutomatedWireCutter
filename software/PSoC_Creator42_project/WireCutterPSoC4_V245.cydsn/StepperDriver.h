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

#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H
#include <cytypes.h>
#include "TMC2130.h"
    
// #############################################################################
/*  Define how much microsteps the stepper motor should use.
    Available options for a TMC2130 driver:
    - 256 (native)
    - 128
    - 64
    - 32
    - 16
    - 8
    - 4
    - 2
    - 1 (full step)
    
    Available options for A4988 driver:
    - 16
    - 8
    - 4
    - 2
    - 1    
*/
#define MICROSTEPS  64ULL          //define how much microsteps

// #############################################################################   
/*  
    Stepper motor driver: TMC2130 or A4988    
*/
    
#define USE_TMC2130

    
#ifndef USE_TMC2130
    #define USE_A4988
#endif
#ifdef USE_A4988                //A4988 driver allows only up to 16 microsteps
    #if MICROSTEPS > 16
        #undef MICROSTEPS
        #define MICROSTEPS 16
    #endif
#endif


#define WIRE_CAL_LENGTH_MM          100ULL      //length of the wire used to calibrate the steps/mm value

#define STEPPER_USE_RAMP
#define STEPPER_SLOW_SPEED          300ul
#define STEPPER_CRUISE_SPEED        70ul //70
#define STEPPER_RAMP_TIME           500ul
#define STEPPER_SPEED_DELTA         (((STEPPER_SLOW_SPEED-STEPPER_CRUISE_SPEED)/STEPPER_RAMP_TIME)|0x01)

typedef enum
{
    STEPPER_ROT_BWD = 0,
    STEPPER_ROT_FWD = 1
}Stepper_dir_t;

typedef enum
{
    STPDRV_IDLE,
    STPDRV_RUNNING,
    STPDRV_ERROR
   
}StepperStatus_t;

typedef enum 
{
    STPDRV_TMC2130,
    STPDRV_A4988
}Stepper_drvType_t;

void Stepper_init(void);
void Stepper_reset(void);
void Stepper_runSteps(uint32_t steps);
uint8 Stepper_runMM(uint32_t length, uint32_t stepsPerMm, Stepper_dir_t dir);
void Stepper_abortRun(void);
void Stepper_setPause(uint8_t value);

uint8_t Stepper_calStepsPerMm(uint32_t length_um); // TODO!!!

StepperStatus_t Stepper_getStatus(void);
void printStepperStatus(void);
uint8_t Stepper_checkErrors(void);
bool Stepper_printErrors(void);
uint32_t Stepper_getErrorStatus(void);

uint8_t Stepper_clearErrors(void);
uint32_t Stepper_getLoad(void);

Stepper_drvType_t Stepper_getDriverType(void);
uint8_t Stepper_getUsteps(void);
    
#endif
    

/* [] END OF FILE */