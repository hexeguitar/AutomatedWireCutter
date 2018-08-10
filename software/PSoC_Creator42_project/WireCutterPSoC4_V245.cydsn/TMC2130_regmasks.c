/*
 * Copyright (c) 2018 by Piotr Zapart / www.hexeguitar.com
 * All rights reserved.
 *
 * Based on TMC2130 Arduino library by Moritz Walter 
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
#include <cytypes.h>

// CHOPCONF MASKS
// mask the bits from the values we want to set
const uint8_t TMC_CHOPCONF_MASKS[] = 
{
    0b1111, // 0 TOFF
    0b111,  // 1
    0b11,   // 2
    0b1,    // 3
    0b111,  // 4 HYSTERESIS_START
    0b11,   // 5
    0b1,    // 6
    0b0001, // 7 HYSTERESIS_LOW
    0b001,  // 8
    0b01,   // 9
    0b1,    // 10
    0b1,    // 11 FAST_DECAY_TIMING
    0b1,    // 12 FAST_DECAY_MODE
    0b1,    // 13 RANDOM_TOFF
    0b1,    // 14 CHOPPER_MODE
    0b11,   // 15 TBL
    0b1,    // 16
    0b1,    // 17 SENSE_CURRENT_SCALING
    0b1,    // 18 HIGH_VELOCITY_STEPS
    0b1,    // 19 HIGH_VELOCITY_CHOPPER
    0b1111, // 20 SYNC_PWM
    0b111,  // 21
    0b11,   // 22
    0b1,    // 23
    0b1111, // 24 MRES
    0b111,  // 25
    0b11,   // 26
    0b1,    // 27
    0b1,    // 28 INTERPOLATE
    0b1,    // 29 DOUBLE_EDGE_PULSES
    0b1,    // 30 SHORT_PROTECTION
    0b1     // 31
};


// COOLCONF MASKS
// mask the bits from the values we want to set
const uint8_t TMC_COOLCONF_MASKS[] = 
{
    0b1111, // 0 TMC_COOLCONF_SEMIN
    0b111, // 1
    0b11, // 2
    0b1, // 3
    0b0, // 4
    0b11, // 5 TMC_COOLCONF_SEUP
    0b1, // 6
    0b0, // 7
    0b1111, // 8 TMC_COOLCONF_SEMAX
    0b111, // 9
    0b11, // 10
    0b1, // 11
    0b0, // 12
    0b11, // 13 TMC_COOLCONF_SEDN
    0b1, // 14
    0b1, // 15 TMC_COOLCONF_SEIMIN
    0b1111111, // 16 TMC_COOLCONF_SGT
    0b111111, // 17
    0b11111, // 18
    0b1111, // 19
    0b111, // 20
    0b11, // 21
    0b1, // 22
    0b0, // 23
    0b1, // 24 TMC_COOLCONF_SFILT
};



// PWMCONF MASKS
// mask the bits from the values we want to set
const uint8_t TMC_PWMCONF_MASKS[] = 
{
    0b11111111, // 0 TMC_PWMCONF_PWM_AMPL
    0b1111111, // 1
    0b111111, // 2
    0b11111, // 3
    0b1111, // 4
    0b111, // 5
    0b11, // 6
    0b1, // 7
    0b11111111, // 8 TMC_PWMCONF_PWM_GRAD
    0b1111111, // 9
    0b111111, // 10
    0b11111, // 11
    0b1111, // 12
    0b111, // 13
    0b11, // 14
    0b1, // 15
    0b11, // 16 TMC_PWMCONF_PWM_FREQ
    0b1, // 17
    0b1, // 18 TMC_PWMCONF_PWM_AUTOSCALE
    0b1, // 19 TMC_PWMCONF_PWM_SYMMETRIC
    0b11, // 20 TMC_PWMCONF_FREEWHEEL
    0b1, // 21
};


/* [] END OF FILE */
