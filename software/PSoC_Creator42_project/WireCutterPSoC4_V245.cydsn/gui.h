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

#ifndef GUI_H
#define GUI_H

    #include <cytypes.h>
    #include "engine.h"
    
    #define SYMBOL_SPACE        0
    #define SYMBOL_PRESS_SHORT  1
    #define SYMBOL_PRESS_LONG   2
    #define SYMBOL_PAUSE        3
    #define SYMBOL_PLAY         4
    #define SYMBOL_SLASH        5
    
    #define GUI_HELP_MAX_PAGES      3
    #define GUI_HELP_PAGE_MASK      1
    #define GUI_HELP_PAGE_LINES     7
    #define GUI_HELP_PAGE_OFFSET    10
    
    extern const char *msg_help[];          // help messages
    extern const char msg_HWver[];
    
    typedef enum
    {
        GUI_NO_UPDATE,
        GUI_PART_UPDATE,
        GUI_FULL_UPDATE,
    
    } gui_update_t;
    
    extern gui_update_t gui_update_flag;
    
    void gui_init(void);
    void gui_displayJob(gui_update_t gui_updateType);
    void gui_displayTitle(void);
    void gui_displayErrorReport(uint32_t errorReport);
    void gui_updateWirePresenceIndicator(uint8_t state, uint8_t override);
    void gui_displayHelp(uint8_t page);
    
    

    
#endif
/* [] END OF FILE */
