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
#include<project.h>
#include "adc.h"
#include "debug.h"
#include "tinyprintf.h"

volatile int16_t ADC0_value;
int16_t ADC_Offset;

CY_ISR_PROTO(ADC12done_ISR);
/*****************************************************************************\
 * Function:    ADC12done_ISR
 * Input:       none
 * Returns:     none
 * Description: 
 *      ADC12 scan complete interrupt handler
\*****************************************************************************/
CY_ISR (ADC12done_ISR)
{
    ADC0_value = ADC12_GetResult16(0);
}

/*****************************************************************************\
 * Function:    ADC_init
 * Input:       none
 * Returns:     none
 * Description: 
 *      Initializes the ADC input
\*****************************************************************************/
void ADC_init(void)
{
    isr_ADC12done_ClearPending();
    isr_ADC12done_StartEx(ADC12done_ISR);
        
    ADC12_Start();
    ADC12_StartConvert();
    //ADC_calOffset();    //also starts convert
    
}

/*****************************************************************************\
 * Function:    ADC_calOffset
 * Input:       none
 * Returns:     none
 * Description: 
 *      Calibrates the adc input offset
 *      ADC0 pin is a combined analog and bidirectional digital pin
 *      having an open drain output. Writing 0 creates a short to GND
 *      used here to measure the ADC input offset.
 *      !!! IMPORTANT !!!
 *      The analog signal fed into the ADC input MUST run through an RC lowpass
 *      filter (or just a series resistor) to avoid shorting the sources output
 *      directly to ground.
\*****************************************************************************/
void ADC_calOffset(void)
{
    uint8_t repeat = 8;
    int32_t offset = 0;
    
    ADC12_StopConvert();
    //calibrate the input offset:

    ADC12_StartConvert();
    while (repeat--)
    {
        while (ADC12_IsEndConversion(ADC12_WAIT_FOR_RESULT) == 0); //wait till complete
        offset +=ADC12_GetResult16(0);                             //accumulate offset
    }
    offset >>= 3;                                                   //divide by 8
    ADC12_SetOffset(0,offset);
    ADC12_StopConvert();
    ADC12_StartConvert();
    #ifdef DEBUG_ADC
    printf("ADCoffset=%dmV\r\n",offset);
    #endif

}

/*****************************************************************************\
 * Function:    ADC_getOffset_mV
 * Input:       none
 * Returns:     offset value in mV
 * Description: 
 *      Returns the ADC offset in mV
\*****************************************************************************/
int16_t ADC_getOffset_mV(void)
{
    return ADC12_CountsTo_mVolts(0,0); //send 0 counts to get the stored offset value
}

/* [] END OF FILE */
