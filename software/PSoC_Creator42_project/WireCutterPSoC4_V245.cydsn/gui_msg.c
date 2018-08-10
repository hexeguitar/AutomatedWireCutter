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

const char *msg_help[] =
{
    
    "HELP 1/2",                 // 0 page 1 title
    "# = Enter length [mm]",    // 1 page 1 line0
    "* = Enter count [pcs]",    // 2 page 1 line1
    "While entering a value",   // 3 page 1 line2
    "# = Enter  * = Delete",    // 4 page 1 line3
    "Btn press: start/pause",   // 5 page 1 line4
    "Btn hold: abort",          // 6 page 1 line5
    "A-D: load preset",         // 7 page 1 line6
    "Btn+ A-D: save preset",    // 8 page 1 line7
    "Btn+*: uart echo",         // 9 page 1 line8
    "HELP 2/2",                 // 9 page 2 title
    "Hold down:",               //10 page 2 line0
    "# = Perform cut",          //11 page 2 line1
    "* = Rewind",               //12 page 2 line2
    "0 = Wire detector",        //13 page 2 line3
    "    override",             //14 page 2 line4
    "1-9 = show Help",          //15 page 2 line5
    "",                         //16 page 2 line6
    "(c) 2018 Piotr Zapart",    //16 page 2 line7
    "www.hexeguitar.com",       //17 page 2 line8
    "STATUS"                    //18 page 3 title    
};

const char msg_HWver[] = "ROBO_SCHNAPPI\r\nAutomated Wire Cutter\r\nV2.4\r\n(c) 2018 by Piotr Zapart\r\n";

/* [] END OF FILE */
