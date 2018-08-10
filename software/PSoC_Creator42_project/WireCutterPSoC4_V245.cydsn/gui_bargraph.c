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
#include "ugui.h"
#include "gui_bargraph.h"
#include "gui_layout.h"
#include "utils.h"




/*****************************************************************************\
 * Function:    gui_drawBargraph
 * Input:       pointer to a bargraph struct, new value
 * Returns:     none
 * Description: 
 *     Draws a new bargraph
\*****************************************************************************/
void gui_drawBargraph(gui_bargraph_t *bargr_ptr, uint16_t value, uint8_t fullRedraw)
{
    uint32_t pos;
    if (bargr_ptr->valueMax >= bargr_ptr->valueMin) pos = mapRange( value, \
                                                                    bargr_ptr->valueMin,    \
                                                                    bargr_ptr->valueMax,    \
                                                                    bargr_ptr->xstart+1,    \
                                                                    bargr_ptr->width-1);
    else                                            pos = mapRange( value, \
                                                                    bargr_ptr->valueMax,    \
                                                                    bargr_ptr->valueMin,    \
                                                                    bargr_ptr->xstart,    \
                                                                    bargr_ptr->width-1);
    if (fullRedraw)
    {
        UG_DrawFrame(   bargr_ptr->xstart,                  \
                        bargr_ptr->ystart,                  \
                        bargr_ptr->xstart+bargr_ptr->width, \
                        bargr_ptr->ystart+bargr_ptr->height,\
                        bargr_ptr->borderColor);
    }
    


    UG_FillFrame(   bargr_ptr->xstart+1,                  \
                    bargr_ptr->ystart+1,                  \
                    pos, \
                    bargr_ptr->ystart+bargr_ptr->height-1,\
                    value ? bargr_ptr->barColor:bargr_ptr->bgrColor);
   if (pos < bargr_ptr->width-1)
    {
        UG_FillFrame(   bargr_ptr->xstart+pos+1,                  \
                        bargr_ptr->ystart+1,                  \
                        bargr_ptr->xstart + bargr_ptr->width-1, \
                        bargr_ptr->ystart+bargr_ptr->height-1,\
                        bargr_ptr->bgrColor);
    }
      
}


/* [] END OF FILE */
