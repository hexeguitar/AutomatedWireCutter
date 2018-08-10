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

#ifndef BUZZER_H
#define BUZZER_H
#include <cytypes.h>

    // ### Job Complete signal ###
    #define ALARM_JOBCMPL_COUNT     3
    #define ALARM_JOBCMPL_SOUND     0x55
    #define ALARM_JOBCMPL_PATT      0x0033ul
    
    // ### Error alarm signal ###
    #define ALARM_ERROR_COUNT       5
    #define ALARM_ERROR_SOUND       0xFF
    #define ALARM_ERROR_PATT        0x0055ul

    // ### Preset save success sound ###
    #define ALARM_PRESET_SAVE_COUNT 1
    #define ALARM_PRESET_SAVE_SOUND 0x03
    #define ALARM_PRESET_SAVE_PATT  0x5555
    
    // ### Exceeding min or max input value
    #define ALARM_VALUE_COUNT       1
    #define ALARM_VALUE_SOUND       0xA5
    #define ALARM_VALUE_PATT        0x00FFul
    
    
    typedef enum
    {
        BUZZER_IDLE,
        BUZZER_BUSY
    } buzzerState_t;
    

void Buzzer_init(void);
void Buzzer_handler(void);
void Buzzer_playSound(uint8_t sound, uint16_t pattern, uint32_t count);  
void Buzzer_beepKey(void);    
#endif
/* [] END OF FILE */
