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
#include <string.h>
#include <stdlib.h>
#include "gui.h"
#include "utils.h"
#include "tinyprintf.h"
#include "keypad.h"
#include "FNbutton.h"
#include "engine.h"
#include "debug.h"
#include "ugui.h"
#include "ugui_st7735.h"
#include "gui_layout.h"
#include "gui_bargraph.h"
#include "StepperDriver.h"



#define DEFAULT_TXT_COLOR (C_YELLOW)

UG_GUI gui7735;
uint8_t gui_forceUpdate = 1;
gui_update_t gui_update_flag;

char txtbufA[26];

void gui_displayPrompt(void);
void gui_displayStatus(void);
void gui_displayCounter(void);
void gui_displayWireCount(void);
void gui_displayWireLength(void);
void gui_display1pcTime(void);
void gui_displayTimeLeft(void);
void gui_displayFooter(void);
void gui_formatLength(uint32_t input, char* txtbuf);
void gui_displayElapsedLength(void);

/*****************************************************************************\
 * Function:    gui_init
 * Input:       none
 * Returns:     none
 * Description: 
 *     Initializes the GUI
\*****************************************************************************/
void gui_init(void)
{
    /* *** Initialize display and related hardware. * ***/
    ST7735_Init(1);             //initializes GUI struct here too (required for display rotation)
    UG_FillScreen(C_BLACK);
    UG_SetForecolor(DEFAULT_TXT_COLOR);
    UG_SetBackcolor(C_BLACK);
}

/*****************************************************************************\
 * Function:    gui_displayJob
 * Input:       display mode
 * Returns:     none
 * Description: 
 *      Shows the main application gui in two modes:
 *      1. Full redraw of everything
 *      2. Update parameters only
\*****************************************************************************/

void gui_displayJob(gui_update_t gui_updateType)
{
    if (gui_updateType == GUI_NO_UPDATE) return;
    
    if (gui_updateType == GUI_FULL_UPDATE)
    {
        gui_forceUpdate = 1;
        gui_displayTitle();
       // ### V line ###   
        UG_DrawLine(JOB_LAYOUT_COL1+15,JOB_LAYOUT_ROW2+1,JOB_LAYOUT_COL1+15,JOB_LAYOUT_ROW4-1,C_YELLOW);       
        gui_updateWirePresenceIndicator(engine_getWirePresence(),engine_getFeedOverride());  
    }
    
    gui_displayPrompt();
    gui_displayStatus();
    gui_displayCounter();
    gui_displayWireCount();
    gui_displayWireLength();    
    gui_displayElapsedLength();
    gui_displayTimeLeft();
    gui_displayFooter();
    
    if (engine_getOTempWarning())
    {
        UG_FillFrame(UG_GetXDim()-5,0,UG_GetXDim(),5,C_RED);
    }
    else
    {
        UG_FillFrame(UG_GetXDim()-5,0,UG_GetXDim(),5,C_BLACK);
    }
    gui_forceUpdate = 0;
    gui_update_flag = GUI_NO_UPDATE;
}

/*****************************************************************************\
 * Function:    gui_updateWirePresenceIndicator
 * Input:       state
 * Returns:     none
 * Description: 
 *     Displays wire presence icon
\*****************************************************************************/
void gui_updateWirePresenceIndicator(uint8_t state, uint8_t override)
{
    static uint8_t state_old = 0xFF, overrideOld=0xFF;
    if ((state_old == state && overrideOld == override) && gui_forceUpdate==0) return;
    state_old = state;
    overrideOld = override;
    // wire presence indicator
    UG_SetForecolor(C_BLACK);
    if (state)  
    {        
        if (override)   UG_SetBackcolor(C_LAVENDER); //show in purple if override is engaged
        else            UG_SetBackcolor(C_GREEN);
        
        UG_PutString(JOB_LAYOUT_COL1+33,JOB_LAYOUT_ROW5+3, " FEED ");
    }
    else
    {
        UG_SetBackcolor(C_RED);
        UG_PutString(JOB_LAYOUT_COL1+33,JOB_LAYOUT_ROW5+3, "!FEED!");
    }
    
    UG_SetBackcolor(C_BLACK);
    UG_SetForecolor(DEFAULT_TXT_COLOR);
}

/*****************************************************************************\
 * Function:    gui_displayTitle
 * Input:       none
 * Returns:     none
 * Description: 
 *     as the name implies
\*****************************************************************************/
void gui_displayTitle(void)
{
    UG_FillScreen(C_BLACK);
    UG_FontSetHSpace(4);   
    UG_FontSelect(&FONT_5X12_STRIPPED);
    UG_SetBackcolor(C_BLACK);
    UG_SetForecolor(C_CYAN);
    UG_PutString(TITLE_LAYOUT_X, TITLE_LAYOUT_Y, "ROBO-SCHNAPPI");
    UG_FontSetHSpace(0);  
    UG_DrawLine(0,TITLE_LAYOUT_Y+13,UG_GetXDim(),TITLE_LAYOUT_Y+13,C_YELLOW);
    UG_SetForecolor(DEFAULT_TXT_COLOR);
}

/*****************************************************************************\
 * Function:    gui_displayPrompt
 * Input:       none
 * Returns:     none
 * Description: 
 *     as the name implies
\*****************************************************************************/
void gui_displayPrompt(void)
{
    static engineState_t stateOld = ENGINE_UNKNOWN;
    engineState_t stateCurrent = engine_getStatus();
    
    if (stateCurrent != stateOld || gui_forceUpdate)
    {
        if (stateCurrent == ENGINE_ENTER_COUNT)
        {
            UG_SetBackcolor(C_YELLOW);
            UG_SetForecolor(C_BLACK);
            UG_PutString(JOB_LAYOUT_COL0,JOB_LAYOUT_ROW0, " Enter pcs: ");
            UG_SetBackcolor(C_BLACK);
            UG_SetForecolor(DEFAULT_TXT_COLOR);
            UG_PutString(JOB_LAYOUT_COL0+60,JOB_LAYOUT_ROW0, "    ");
        }
        else if (stateCurrent == ENGINE_ENTER_LENGTH)
        {
            UG_SetBackcolor(C_YELLOW);
            UG_SetForecolor(C_BLACK);
            UG_PutString(JOB_LAYOUT_COL0,JOB_LAYOUT_ROW0, " Enter length in mm: ");
            UG_FillFrame(105,JOB_LAYOUT_ROW0,UG_GetXDim(),JOB_LAYOUT_ROW0+JOB_LAYOUT_ROW_DELTA-2,C_BLACK);
            UG_SetForecolor(DEFAULT_TXT_COLOR);
            UG_SetBackcolor(C_BLACK);
        }
        else
        {
            UG_PutString(JOB_LAYOUT_COL0,JOB_LAYOUT_ROW0, "Produced:       ");   
        }
        stateOld = stateCurrent;
    }
    else    return;
}

/*****************************************************************************\
 * Function:    gui_displayStatus
 * Input:       engineStatus
 * Returns:     none
 * Description: 
 *     as the name implies
\*****************************************************************************/
void gui_displayStatus(void)
{
    static engineState_t stateOld = ENGINE_UNKNOWN;
    engineState_t stateCurrent = engine_getStatus();
    
    if (stateCurrent != stateOld || gui_forceUpdate)
    {
        switch (stateCurrent)
        {
            case ENGINE_IDLE:
                    sprintf(txtbufA,"%10s"," READY  ");
                    break;
            case ENGINE_START ... ENGINE_JOB_PIECE_COMPLETE:
                    UG_SetForecolor(C_CYAN);
                    sprintf(txtbufA,"%10s","RUNNING ");
                    break;
            case ENGINE_JOB_PAUSE:
                    UG_SetForecolor(DEFAULT_TXT_COLOR);
                    sprintf(txtbufA,"%10s","PAUSED ");             
                    break;
            case ENGINE_JOB_COMPLETE:
                    UG_SetBackcolor(C_GREEN);
                    UG_SetForecolor(C_BLACK);
                    sprintf(txtbufA,"%10s"," COMPLETE ");
                    break;
            case ENGINE_JOB_ABORTED:
                    UG_SetBackcolor(C_RED);
                    UG_SetForecolor(C_BLACK);
                    sprintf(txtbufA,"%10s","ABORTED");
                    break;    
            case ENGINE_ENTER_COUNT:
                    sprintf(txtbufA,"%10s",""); //clear status
                    break;
            case ENGINE_ENTER_LENGTH:
            case ENGINE_DISPLAY_HELP:
            case ENGINE_UNKNOWN:
            case ENGINE_DIAGNOSTICS:
                    txtbufA[0] = '\0';  //this will stop printing the string
                    break;
            case ENGINE_ERROR:
                    UG_SetBackcolor(C_RED);
                    UG_SetForecolor(C_BLACK);
                    sprintf(txtbufA,"%10s","ERROR! ");
                    break;
        }
        UG_PutString(JOB_LAYOUT_COL1+12,JOB_LAYOUT_ROW0, txtbufA);
        UG_SetBackcolor(C_BLACK);
        UG_SetForecolor(DEFAULT_TXT_COLOR);
        stateOld = stateCurrent;
    }
}

/*****************************************************************************\
 * Function:    gui_displayCounter
 * Input:       engineStatus, new value
 * Returns:     none
 * Description: 
 *     as the name implies
\*****************************************************************************/
void gui_displayCounter(void)
{
    static uint16_t valueOld = 0xFFFF;
    uint16_t valueNew;
    engineState_t stateCurrent = engine_getStatus();
    
    if (stateCurrent == ENGINE_ENTER_COUNT)
    {
        valueNew =  engine_getWireCount();
    }
    else if (stateCurrent == ENGINE_ENTER_LENGTH)
    {
        valueNew = engine_getWireLength_mm();
    }
    else
    {
        valueNew = engine_getWireCountElapsed();
    }
    
    if(valueNew != valueOld || gui_forceUpdate)
    {
        sprintf(txtbufA,"%5d",valueNew);
        UG_FontSelect(&FONT_24X40_NUMBERS_ONLY); 
    /*  
        print large number: produced pcs or edited value
        replace the leading spaces with commas, which are in fact spaces in the FONT_24X40_NUMBERS_ONLY array.
        This was done to save the Flash and not store symbols that will not be used.
    */
        char *str;
        str = txtbufA;
        while(*str == ' ') {*str++ = ',';} 
        UG_PutString(JOB_LAYOUT_COL0+4, JOB_LAYOUT_ROW1 , txtbufA);
        
        UG_FontSelect(&FONT_5X12_STRIPPED);
        
        valueOld = valueNew;
    }    
}

/*****************************************************************************\
 * Function:    gui_displayWireCount
 * Input:       none
 * Returns:     none
 * Description: 
 *     as the name implies
\*****************************************************************************/
void gui_displayWireCount(void)
{
    static uint16_t countOld = 0xFFFF;
    uint16_t count = engine_getWireCount(); 
    
    if(count != countOld || gui_forceUpdate)
    {
        sprintf(txtbufA,"Count: %5dpcs",count);
        UG_PutString(JOB_LAYOUT_COL0,JOB_LAYOUT_ROW2, txtbufA);
        countOld = count;
    }    
}

/*****************************************************************************\
 * Function:    gui_displayWireLength
 * Input:       length new value
 * Returns:     none
 * Description: 
 *     as the name implies
\*****************************************************************************/
void gui_displayWireLength(void)
{
    static uint16_t lengthOld= 0xFFFF;
    uint16_t length = engine_getWireLength_mm();
    if(length != lengthOld || gui_forceUpdate)
    {
        sprintf(txtbufA,"Length: %5dmm",length);
        UG_PutString(JOB_LAYOUT_COL0,JOB_LAYOUT_ROW3, txtbufA);
        lengthOld = length;
    }    
}

/*****************************************************************************\
 * Function:    gui_display1pcTime
 * Input:       none
 * Returns:     none
 * Description: 
 *     as the name implies
\*****************************************************************************/
void gui_display1pcTime(void)
{
    static uint32_t timeOld = 0xFFFFFFFF;
    uint32_t time = engine_get1pcsTime_ms();
    if (time != timeOld || gui_forceUpdate)
    {    
        if (time)                   sprintf(txtbufA,"%5d",time);
        else                        sprintf(txtbufA,"%5s","");
        UG_PutString(JOB_LAYOUT_COL0+101,JOB_LAYOUT_ROW2, txtbufA);
    }
}

/*****************************************************************************\
 * Function:    gui_displayElapsedLength
 * Input:       none
 * Returns:     none
 * Description: 
 *     as the name implies
\*****************************************************************************/
void gui_displayElapsedLength(void)
{
    engineState_t stateCurrent = engine_getStatus();
    static uint32_t lengthOld = 0xFFFFFFFF;
    uint32_t length;
    const char txt_req[] = "Required";
    const char txt_elp[] = "Elapsed ";
    const char *label;
    int16_t x;
    
    if (stateCurrent == ENGINE_IDLE || stateCurrent == ENGINE_ENTER_COUNT || stateCurrent == ENGINE_ENTER_LENGTH)    
    {
        length = engine_getRequiredLength_mm();
        label = txt_req;
    }
    else                                
    {
        length = engine_getElapsedLength_mm();
        label = txt_elp;
    }
    if (length != lengthOld || gui_forceUpdate)
    {    
        gui_formatLength(length, txtbufA);
        
        UG_PutString(JOB_LAYOUT_COL0+86,JOB_LAYOUT_ROW3, (char *) label);
        x = UG_PutString(JOB_LAYOUT_COL0+86,JOB_LAYOUT_ROW2, txtbufA);
        UG_FillFrame(x, JOB_LAYOUT_ROW2, UG_GetXDim(), JOB_LAYOUT_ROW2 + 12, C_BLACK);
        
    }
}

/*****************************************************************************\
 * Function:    gui_formatLength
 * Input:       input value, pointer to a char buffer
 * Returns:     none
 * Description: 
 *     mm/m/km length formatting
\*****************************************************************************/
void gui_formatLength(uint32_t input, char* txtbuf)
{
    if (input == 0)                    sprintf(txtbufA,"%s","");
    else if (input < 1000)             sprintf(txtbufA,"%dmm",input);
    else if (input > 1000000ul)
    {
        uint32_t km = input / 1000000ul;
        uint32_t meters = ((input % 1000000ul) + 500) / 1000;
        sprintf(txtbuf,"%d.%dkm", km, meters);
    }
    else if (input > 100000000ul)   sprintf(txtbufA,"%5s","crazy?");
    else 
    {
        uint32_t meters = input / 1000;
        uint32_t dmeters = ((input % 1000) + 50) / 100;
        sprintf(txtbuf,"%d.%dm",meters, dmeters);
    }
}

/*****************************************************************************\
 * Function:    gui_displayTimeLeft
 * Input:       status, new value
 * Returns:     none
 * Description: 
 *     as the name implies
\*****************************************************************************/
void gui_displayTimeLeft(void)
{
    static engineState_t stateOld = ENGINE_UNKNOWN;
    uint32_t time_min, time_sec;
    engineState_t stateCurrent = engine_getStatus();
    uint32_t timeNew = engine_getRemainingTime_ms();
    
    if (stateCurrent == ENGINE_JOB_COMPLETE || stateCurrent == ENGINE_JOB_ABORTED)
    {   
        if (stateCurrent != stateOld || gui_forceUpdate)  UG_PutString(JOB_LAYOUT_COL0,JOB_LAYOUT_ROW4, "Time elapsed:");
        
        time_sec = engine_getJobTimeTotal() % 60;  
        time_min = engine_getJobTimeTotal() / 60;
        stateOld = stateCurrent;
    }
    else
    {
        if (stateCurrent != stateOld || gui_forceUpdate)  UG_PutString(JOB_LAYOUT_COL0,JOB_LAYOUT_ROW4, "Time left:   ");
        
        ms_to_time(timeNew,&time_min,&time_sec);  
        stateOld = stateCurrent;
    }
    sprintf(txtbufA,"%3dmin %2ds",time_min,time_sec);
    UG_PutString(JOB_LAYOUT_COL0+76,JOB_LAYOUT_ROW4, txtbufA);    
}

/*****************************************************************************\
 * Function:    gui_displayFooter
 * Input:       status
 * Returns:     none
 * Description: 
 *     as the name implies
\*****************************************************************************/
void gui_displayFooter(void)
{
    static engineState_t stateOld = ENGINE_UNKNOWN;
    engineState_t stateCurrent = engine_getStatus();
    
    if (stateCurrent != stateOld || gui_forceUpdate)
    {
        UG_DrawLine(0,JOB_LAYOUT_ROW5-1,UG_GetXDim(),JOB_LAYOUT_ROW5-1,C_YELLOW);
        UG_SetForecolor(C_CYAN);
        switch (stateCurrent)
        {
            case ENGINE_IDLE:
                    //footer                   
                    UG_PutString(JOB_LAYOUT_COL0,JOB_LAYOUT_ROW5+3, "Btn: ");    
                    UG_FontSelect(&FONT_7x12_CUSTOM_SYMBOLS);
                    UG_PutString(JOB_LAYOUT_COL0+22,JOB_LAYOUT_ROW5+3, "10");               //short press symbol  
                    UG_PutString(JOB_LAYOUT_COL0+31,JOB_LAYOUT_ROW5+3, "3540");             //pause/play + space            
                    UG_PutString(JOB_LAYOUT_COL1-5,JOB_LAYOUT_ROW5+3,"20");     //long press symbol  +space   

                    UG_FontSelect(&FONT_5X12_STRIPPED);
                    UG_PutString(JOB_LAYOUT_COL1+5,JOB_LAYOUT_ROW5+3, "Abort");                    
                    break;
            case ENGINE_START:
            case ENGINE_JOB_PULLING:
            case ENGINE_JOB_CUTTING:
            case ENGINE_JOB_PIECE_COMPLETE:
            case ENGINE_JOB_PAUSE:
                    break;
            case ENGINE_JOB_COMPLETE:
            case ENGINE_JOB_ABORTED:
                    UG_PutString(JOB_LAYOUT_COL0,JOB_LAYOUT_ROW5+3, "Press any key");
                    UG_FillFrame(JOB_LAYOUT_COL0+65,JOB_LAYOUT_ROW5+3,JOB_LAYOUT_COL1+32,JOB_LAYOUT_ROW5+3+12,C_BLACK);
                    break;    
            case ENGINE_ENTER_COUNT:
            case ENGINE_ENTER_LENGTH:
                    UG_PutString(JOB_LAYOUT_COL0,JOB_LAYOUT_ROW5+3, "# = Enter * = Del.") ;
                    UG_FillFrame(JOB_LAYOUT_COL0+90,JOB_LAYOUT_ROW5+3,JOB_LAYOUT_COL1+32,JOB_LAYOUT_ROW5+3+12,C_BLACK);
                    break;
            case ENGINE_DISPLAY_HELP:        
            case ENGINE_UNKNOWN:
                    break;
            case ENGINE_ERROR:
                    UG_PutString(JOB_LAYOUT_COL0,JOB_LAYOUT_ROW5+3, "Btn: Error report") ;
                    UG_FillFrame(JOB_LAYOUT_COL0+86,JOB_LAYOUT_ROW5+3,JOB_LAYOUT_COL1+32,JOB_LAYOUT_ROW5+3+12,C_BLACK);
                    break;
            default:
                    break;
        }
        UG_SetForecolor(DEFAULT_TXT_COLOR);
        stateOld = stateCurrent;
    }  
}

void gui_displayHelp(uint8_t page)
{
    uint8_t i;
    
    UG_FillScreen(C_BLACK);
    UG_FontSetHSpace(4);   
    UG_FontSelect(&FONT_5X12_STRIPPED);
    UG_SetBackcolor(C_BLACK);
    UG_SetForecolor(C_CYAN);

    UG_PutString(TITLE_LAYOUT_X, TITLE_LAYOUT_Y,(char *) msg_help[page*GUI_HELP_PAGE_OFFSET]);
    UG_DrawLine(0,TITLE_LAYOUT_Y+13,UG_GetXDim(),TITLE_LAYOUT_Y+13,C_YELLOW);
    UG_FontSetHSpace(0);
    UG_SetForecolor(DEFAULT_TXT_COLOR);
    
    if (page < 2)
    {
        for (i=0;i<9;i++)
        {
            UG_PutString(JOB_LAYOUT_COL0, JOB_LAYOUT_ROW0+12*i, (char *) msg_help[ i+(page*GUI_HELP_PAGE_OFFSET) + 1]) ;
        }
    }
    else if (page == 2)   // STATUS page
    {
        i = UG_PutString(JOB_LAYOUT_COL0, JOB_LAYOUT_ROW0, "Stepper driver: ");
        UG_PutString(i, JOB_LAYOUT_ROW0, Stepper_getDriverType()==STPDRV_A4988 ? "A4988":"TMC2130");
        
        i = UG_PutString(JOB_LAYOUT_COL0, JOB_LAYOUT_ROW0+12, "Microsteps: ");
        sprintf(txtbufA, "%d", Stepper_getUsteps());
        UG_PutString(i, JOB_LAYOUT_ROW0+12, txtbufA);
        
        i = UG_PutString(JOB_LAYOUT_COL0, JOB_LAYOUT_ROW0+24, "Cut count: ");
        sprintf(txtbufA, "%d", engine_getNumberOfCuts());
        UG_PutString(i, JOB_LAYOUT_ROW0+24, txtbufA);
        
        i = UG_PutString(JOB_LAYOUT_COL0, JOB_LAYOUT_ROW0+36, "UART local echo: ");
        UG_PutString(i, JOB_LAYOUT_ROW0+36, UART_getLocalEcho() ? "ON":"OFF");
    }
}

/*****************************************************************************\
 * Function:    gui_displayErrorReport
 * Input:       error report 
 * Returns:     none
 * Description: 
 *     Displays error report
\*****************************************************************************/
void gui_displayErrorReport(uint32_t errorReport)
{
    uint8_t idx = 0;
    UG_FillScreen(C_BLACK);
    gui_displayTitle();
    UG_FontSetHSpace(4);
    UG_PutString(JOB_LAYOUT_COL0,JOB_LAYOUT_ROW0 , "ERROR REPORT");
    UG_FontSetHSpace(0);
    UG_SetForecolor(C_RED);
    if (errorReport & (1<<23)) UG_PutString(JOB_LAYOUT_COL0, JOB_LAYOUT_ROW1 + (idx++)*13, "DRIVER ERROR GSTAT[1]");    //Driver Error
    if (errorReport & (1<<21)) UG_PutString(JOB_LAYOUT_COL0, JOB_LAYOUT_ROW1 + (idx++)*13, "DIAG0 LOW");                //DIAG0
    if (errorReport & (1<<22)) UG_PutString(JOB_LAYOUT_COL0, JOB_LAYOUT_ROW1 + (idx++)*13, "DIAG1 LOW");                //DIAG1
    if (errorReport & (1<<25)) UG_PutString(JOB_LAYOUT_COL0, JOB_LAYOUT_ROW1 + (idx++)*13, "OVERTEMP ERROR");           //overtemperature
    if (errorReport & (1<<24)) UG_PutString(JOB_LAYOUT_COL0, JOB_LAYOUT_ROW1 + (idx++)*13, "MOTOR STALL");               //Motor stall
    
}

/* [] END OF FILE */
