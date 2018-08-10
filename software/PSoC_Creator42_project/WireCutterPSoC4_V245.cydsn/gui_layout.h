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
#ifndef GUI_LAYOUT_H
#define GUI_LAYOUT_H
#include <cytypes.h>
 
    #define TITLE_LAYOUT_X  7
    #define TITLE_LAYOUT_Y  0
    
    #define JOB_LAYOUT_PCS_ELAPSED_X    0
    #define JOB_LAYOUT_PCS_ELAPSED_Y    35
    
    #define JOB_LAYOUT_COL0 0
    #define JOB_LAYOUT_COL1 64
    
    #define JOB_LAYOUT_ROW_DELTA 14
    #define JOB_LAYOUT_ROW0 18
    #define JOB_LAYOUT_ROW1 (JOB_LAYOUT_ROW0 + JOB_LAYOUT_ROW_DELTA-2)
    #define JOB_LAYOUT_ROW2 (JOB_LAYOUT_ROW1 + JOB_LAYOUT_ROW_DELTA+25)
    #define JOB_LAYOUT_ROW3 (JOB_LAYOUT_ROW2 + JOB_LAYOUT_ROW_DELTA)
    #define JOB_LAYOUT_ROW4 (JOB_LAYOUT_ROW3 + JOB_LAYOUT_ROW_DELTA)
    #define JOB_LAYOUT_ROW5 (JOB_LAYOUT_ROW4 + JOB_LAYOUT_ROW_DELTA)
    
    
#endif

/* [] END OF FILE */
