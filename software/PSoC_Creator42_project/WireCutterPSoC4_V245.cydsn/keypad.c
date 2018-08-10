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
#include "keypad.h"
#include "buzzer.h"

volatile uint8_t keyPressFlag = 0, keyPressLongFlag = 0;

char TranslateTable[MAXKEYVALUE] = {
										'1','2','3','A',
										'4','5','6','B',
										'7','8','9','C',
                                        '*','0','#','D'
									 };


CY_ISR_PROTO(KeyPressed_ISR);
CY_ISR_PROTO(KeyPressedLong_ISR);

// #############################################################################
CY_ISR (KeyPressed_ISR)
{
    keyPressFlag = 1;
    Buzzer_beepKey();
}

CY_ISR (KeyPressedLong_ISR)
{
    keyPressLongFlag = 1;
}


void keypad_init(void)
{
    isr_KeyPressed_ClearPending();
    isr_KeyPressed_StartEx(KeyPressed_ISR);
    isr_KeyPressedLong_ClearPending();
    isr_KeyPressedLong_StartEx(KeyPressedLong_ISR);
    MatrixKeypad_Start(TranslateTable);
}

void keypad_handler(void)
{
    
}

/* [] END OF FILE */
