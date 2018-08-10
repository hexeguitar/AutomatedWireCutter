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
#include "buzzer.h"

// ### VARIABLES ###
const uint16_t beep_repeatRate = 100ul;
uint8_t beepSound= 0x0A;
uint16_t beepPattern = 0x0000ul;

const uint8_t beepKeypress = 0x01;

buzzerState_t buzzerState;

uint32_t beepCounter = 0;

// ### Function declatations ###


// ### Function definitions ###

/*****************************************************************************\
 * Function:    buzzer_init
 * Input:       none
 * Returns:     none
 * Description: 
 *     Initializes the buzzer
\*****************************************************************************/
void Buzzer_init(void)
{
    BuzzDriver_Start();
}  

/*****************************************************************************\
 * Function:    Buzzer_handler
 * Input:       none
 * Returns:     none
 * Description: 
 *     Buzzer handler invoked in the main loop
\*****************************************************************************/
void Buzzer_handler(void)
{
    static uint8_t patternIdx = 0;
    static uint32_t systickLast = 0;
    uint32_t systickNow;

        if (beepCounter && buzzerState != BUZZER_IDLE)
        {
            systickNow = SysTimers_GetSysTickValue();
            if ((systickNow-systickLast) >= beep_repeatRate)
            {        
                BuzzDriver_WriteRegValue(beepPattern&(1<<patternIdx)?beepSound:0x00);
                systickLast = systickNow;                
                if (++patternIdx ==15)
                {
                    patternIdx = 0;
                    beepCounter--;
                } 
            }   
        }
        else
        {
            buzzerState = BUZZER_IDLE;
        }
}

/*****************************************************************************\
 * Function:    Buzzer_beepKey
 * Input:       none
 * Returns:     none
 * Description: 
 *     Generates a single key press beep
\*****************************************************************************/
void Buzzer_beepKey(void)
{
    BuzzDriver_WriteRegValue(beepKeypress);
}

/*****************************************************************************\
 * Function:    Buzzer_setAlarm
 * Input:       none
 * Returns:     none
 * Description: 
 *     
\*****************************************************************************/
void Buzzer_playSound(uint8_t sound, uint16_t pattern, uint32_t count)
{ 
    beepCounter = count;
    beepSound = sound;
    beepPattern = pattern;
    buzzerState = BUZZER_BUSY;
}

/* [] END OF FILE */
