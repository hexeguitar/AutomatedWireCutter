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
#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

    #include <cytypes.h>
    
    #define SERVO_OFF_ANGLE     (-60)
    #define SERVO_CUT_ANGLE     (60)
    #define SERVO_MID_RELEASE_ANGLE     (-5)

    
    #define SERVO_MIN_ANGLE     (-60)
    #define SERVO_MAX_ANGLE     (60)
    
    typedef enum
    {
        SERVO_IDLE,
        SERVO_START,
        SERVO_CUTTING,
        SERVO_RELEASING
    } servoStatus_t;
    
    servoStatus_t Servo_getStatus(void);
    servoStatus_t Servo_handler(void);
    void Servo_startCut(uint8_t count);
    
    void ServoDriver_init(uint32_t startPos);
    void Servo_SetAngle(int32_t degs);
    void Servo_enableCut(uint8_t state);
    uint8_t Servo_getEnable(void);
    

#endif
/* [] END OF FILE */
