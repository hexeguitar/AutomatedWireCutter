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
 */
#include <project.h>
#include "ugui.h"
#include "ugui_st7735.h"
#include "gui.h"
#include "keypad.h"
#include "FNbutton.h"
#include "tinyprintf.h"
#include "utils.h"
#include "StepperDriver.h"
#include "ServoDriver.h"
#include "debug.h"
#include "engine.h"
#include "TMC2130.h"
#include "TMC2130_reg.h"
#include "buzzer.h"
#include "console.h"
#include "adc.h"
#include "config.h"



// #############################################################################
// #### Global variables ###

// #############################################################################
// ### Function declarations ###


// #############################################################################
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    UART_Start();
    
    init_printf(NULL,putChar);
    SysTimers_Start();
      
    EEP_CS_Write(1);    
    SPI_Start();
    SPI_SpiSetActiveSlaveSelect(SPI_SPI_SLAVE_SELECT0); //select display
    gui_init();
    keypad_init();
    FNbutton_init();
    Buzzer_init();
    engineInit();
    ConsoleInit();
    
    //Servo_enableCut(0);   //the cut operation can be optionally disabled (ie for testing)
    
    PWM_0_Start();
    
    ADC_init();
    
    //configure watchdog
    CySysWdtWriteMode(CY_SYS_WDT_COUNTER0,CY_SYS_WDT_MODE_RESET);   //counter 0 reset mode
    
    CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0,0xFFFF);                 //
    CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, 1u);
    #ifndef DEBUG_ENABLED
    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);                       //enable counter 0
    #endif
    
    for(;;)
    {
        //clear the watchdog counter
        CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET);
        
        if (SysTimers_GetSysTickValue() > 10000ul)  // startup delay 10s - ESP8266 boot messages 
        {
            ConsoleProcess();
        }
        else
        {
            while (UART_SpiUartGetRxBufferSize() > 0)
            {
                 UART_UartGetChar();                //just read the incoming data to flush out the buffer
            }
        }

        //engine_checkButtons();
        Buzzer_handler();       
        doYourJob();            //main engine handler
    }
}

/* [] END OF FILE */
