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
#include "tinyprintf.h"
#include "ServoDriver.h"
#include "utils.h"
#include "debug.h"

#define SERVO_MIN_PWM     (7373L)
#define SERVO_MAX_PWM     (17203L)
#define SERVO_DELAY_CUT_MS (450ul)
#define SERVO_DELAY_REL_MS (450ul)


servoStatus_t servoStatus;
uint32_t Timer_servoDelay;
uint8_t servoEnable = 1;        //enables/disables cutting
uint8_t cutCount = 1;

CY_ISR_PROTO(ServoOV_ISR);



/*****************************************************************************\
 * Function:    ServoOV_ISR
 * Input:       none
 * Returns:     none
 * Description: 
 *     Servo timer overflow ISR
\*****************************************************************************/
CY_ISR(ServoOV_ISR)
{


    
    
}

/*****************************************************************************\
 * Function:    ServoDriver_init
 * Input:       startPos
 * Returns:     nothing
 * Description: 
 *     Initializes the servo driver
\*****************************************************************************/
void ServoDriver_init(uint32_t startPos)
{
    Servo_PWR_Write(0);     //Power on
    Servo_Start();
    Timer_servoDelay = SysTimers_GetTimer(SERVO_DELAY_CUT_MS);   //
    Servo_SetAngle(startPos);
    servoStatus = SERVO_IDLE;  
    isr_ServoOV_ClearPending();
    //isr_ServoOV_StartEx(ServoOV_ISR);
}


/*****************************************************************************\
 * Function:    Servo_SetAngle
 * Input:       angle in degrees
 * Returns:     none
 * Description: 
 *     rotates the servo to a set angle in allowed range
\*****************************************************************************/
void Servo_SetAngle(int32_t degs)
{
    uint16_t pwm;
    if (degs<SERVO_MIN_ANGLE || degs>SERVO_MAX_ANGLE)   return;
    pwm = mapRange(degs,SERVO_MIN_ANGLE,SERVO_MAX_ANGLE,SERVO_MIN_PWM,SERVO_MAX_PWM);
    Servo_WriteCompare(pwm);
     
}


/*****************************************************************************\
 * Function:    Servo_startCut
 * Input:       none
 * Returns:     none
 * Description: 
 *     starts cutting procedure only if previous one is finished 
\*****************************************************************************/
void Servo_startCut(uint8_t count)

{
    if (count == 0) count = 1;
    cutCount = count;
    if (servoStatus == SERVO_IDLE) servoStatus = SERVO_START;
}

// #############################################################################

/*****************************************************************************\
 * Function:    Servo_handler
 * Input:       none
 * Returns:     actual servo driver status
 * Description: 
 *     Hadles the servo driver state machine
\*****************************************************************************/
servoStatus_t Servo_handler(void)
{   

    switch (servoStatus)
    {
        case SERVO_IDLE:
                        break;
        case SERVO_START:
                        SysTimers_ResetTimer(Timer_servoDelay,SERVO_DELAY_CUT_MS);   //reset timer
                        if (servoEnable) Servo_SetAngle(SERVO_CUT_ANGLE);              //start cutting
                        servoStatus = SERVO_CUTTING;
                        break;
        case SERVO_CUTTING:
                        if (SysTimers_GetTimerStatus(Timer_servoDelay)) //wait for servo
                        {
                            if (cutCount>1)
                            {
                                if (servoEnable) Servo_SetAngle(SERVO_MID_RELEASE_ANGLE);        //release
                                servoStatus = SERVO_RELEASING;
                                SysTimers_ResetTimer(Timer_servoDelay,SERVO_DELAY_REL_MS); //reset timer
                            }
                            else
                            {                                
                                if (servoEnable) Servo_SetAngle(SERVO_OFF_ANGLE);        //release
                                servoStatus = SERVO_RELEASING;
                                SysTimers_ResetTimer(Timer_servoDelay,SERVO_DELAY_REL_MS); //reset timer
                            }
                        }
                        break;
        case SERVO_RELEASING:

                        if (SysTimers_GetTimerStatus(Timer_servoDelay)) //
                        {
                            cutCount--;
                            if (cutCount == 0)
                            {
                                servoStatus = SERVO_IDLE;
                            }
                            else 
                            {
                                servoStatus = SERVO_START;
                            }
                        }
                        break;
    }
    return servoStatus;
}

/*****************************************************************************\
 * Function:    Servo_enableCut
 * Input:       state, 0=OFF, 1 = ON
 * Returns:     none
 * Description: 
 *     Enables/disables the cutter. Useful for testing purposes.
\*****************************************************************************/
void Servo_enableCut(uint8_t state)
{   
    servoEnable = state ? 1:0; 
}


/*****************************************************************************\
 * Function:    Servo_getEnable
 * Input:       none
 * Returns:     servo enable flag state
 * Description: 
 *     Returns the cutter enable flag
\*****************************************************************************/
uint8_t Servo_getEnable(void)
{
    return servoEnable;
}


/* [] END OF FILE */
