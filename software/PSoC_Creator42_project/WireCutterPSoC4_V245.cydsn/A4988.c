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
#include "A4988.h"


const uint8_t A4988_ustepTable[5] = {0, 1, 2, 3, 7};


/*****************************************************************************\
 * Function:    A4988_setUsteps
 * Input:       ustep setting
 * Returns:     none
 * Description: 
 *     Sets the M1-M3 microstep control signals.
\*****************************************************************************/
uint8_t A4988_setUsteps(uint8_t usteps)
{
    uint8_t idx = 0;
    uint8_t usteps_final;
    
    if (usteps == 0) usteps = 1;
    if (usteps > A4988_USTEP_MAX) usteps = A4988_USTEP_MAX;
    usteps_final = usteps;
    
    usteps--;               //0-15       
    
    while (usteps > 0)
    {
        usteps >>= 1;
        idx++;
    }
    
    Stepper_CtrlReg_Write(A4988_ustepTable[idx]);
    
    return usteps_final;
}


/* [] END OF FILE */
