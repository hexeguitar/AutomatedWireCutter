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
#include "FNbutton.h"
#include "tinyprintf.h"
#include "utils.h"
#include "debug.h"


static volatile uint8_t FNbutton_state;

CY_ISR_PROTO(BtnShortPress_ISR);
CY_ISR_PROTO(BtnLongPress_ISR);

void (*FNbutton_shortPress_Callback) (void);
void (*FNbutton_longPress_Callback) (void);

// #############################################################################
CY_ISR (BtnShortPress_ISR)
{
    if ( FNbutton_state & FNBUTTON_LONGPRESS_LATCHED_MASK)      FNbutton_state &=~FNBUTTON_LONGPRESS_LATCHED_MASK;  //clear the long press latch   
    else                                                        FNbutton_state |= FNBUTTON_SHORTPRESS_MASK;         //ignore the press if the button was "long pressed"                                          
    
}
// #############################################################################
CY_ISR (BtnLongPress_ISR)
{
    FNbutton_state |= FNBUTTON_LONGPRESS_MASK;
    FNbutton_state |= FNBUTTON_LONGPRESS_LATCHED_MASK;
    
}

/*****************************************************************************\
 * Function:    FNbutton_init
 * Input:       none
 * Returns:     none
 * Description: 
 *     Initialize the Function button
\*****************************************************************************/
void FNbutton_init(void)
{
    BtnLongPressDetector_Start();
    isr_BtnShortPress_StartEx(BtnShortPress_ISR);    
    isr_BtnLongPress_StartEx(BtnLongPress_ISR);
    CyDelay(300);
    isr_BtnLongPress_ClearPending();
    isr_BtnShortPress_ClearPending();
    FNbutton_state = 0x00;
    
}

/*****************************************************************************\
 * Function:    FNbutton_getShortPress
 * Input:       none
 * Returns:     state
 * Description: 
 *      Returns 1 if there has been a short press detected, otherwise 0.
 *      Short press is "release-active" in order to distinguish it from 
 *      the long press. Otherwise, the short press will be triggered while
 *      executing a long press, too.
\*****************************************************************************/
uint8_t FNbutton_getShortPress(void)
{
    uint8_t out;
    if (FNbutton_state & FNBUTTON_SHORTPRESS_MASK) out = 1;
    else                                           out = 0;
    FNbutton_state &=~FNBUTTON_SHORTPRESS_MASK; //clear the press
    return out;
}

/*****************************************************************************\
 * Function:    FNbutton_clearShortPress
 * Input:       none
 * Returns:     none
 * Description: 
 *     Clears the short press event
\*****************************************************************************/
void FNbutton_clearShortPress(void)
{
    FNbutton_state &=~FNBUTTON_SHORTPRESS_MASK; //clear the press
}

/*****************************************************************************\
 * Function:    FNbutton_getLongPress
 * Input:       none
 * Returns:     state
 * Description: 
 *     Returns 1 if there has been a long press detected.
\*****************************************************************************/
uint8_t FNbutton_getLongPress(void)
{
    uint8_t out;
    if (FNbutton_state & FNBUTTON_LONGPRESS_MASK) out = 1;
    else                                          out = 0;
    FNbutton_state &=~FNBUTTON_LONGPRESS_MASK; //clear the press
    return out;
}

/*****************************************************************************\
 * Function:    FNbutton_clearLongPress
 * Input:       none
 * Returns:     none
 * Description: 
 *     Clears the Long press event
\*****************************************************************************/
void FNbutton_clearLongPress(void)
{
    FNbutton_state &=~FNBUTTON_LONGPRESS_MASK; //clear the press
}

/*****************************************************************************\
 * Function:    FNbutton_regShortPressCallback
 * Input:       callback function
 * Returns:     none
 * Description: 
 *     Registers a callback function for a short press event
\*****************************************************************************/
void FNbutton_regShortPressCallback(void(*callback)(void))
{
    FNbutton_shortPress_Callback = callback;
    
}

/*****************************************************************************\
 * Function:    FNbutton_regLongPressCallback
 * Input:       callback function
 * Returns:     none
 * Description: 
 *     Registers a callback function for a long press event
\*****************************************************************************/
void FNbutton_regLongPressCallback(void(*callback)(void))
{
    FNbutton_longPress_Callback = callback;
    
}

/*****************************************************************************\
 * Function:    FNbutton_checkState
 * Input:       none
 * Returns:     none
 * Description: 
 *     Checks for the short/long press event and fires up callback functions.
\*****************************************************************************/
void FNbutton_checkState(void)
{
    if (FNbutton_getShortPress() && FNbutton_shortPress_Callback)
    {
        FNbutton_shortPress_Callback();
    }
    if (FNbutton_getLongPress() && FNbutton_longPress_Callback)
    {
        FNbutton_longPress_Callback();
    }
}


/* [] END OF FILE */
