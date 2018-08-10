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
#include <stdlib.h>
#include "utils.h"
#include "debug.h"

#define MAXLONG 2147483647



int32_t mapRange(int32_t val, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    // TEST: in_min must be lower than in_max => flip the ranges
    // must be done before out of range test
    if (in_min > in_max) return mapRange(val, in_max, in_min, out_max, out_min);

    // TEST: if input value out of range it is mapped to border values. By choice.
    if (val <= in_min) return out_min;
    if (val >= in_max) return out_max;

    // TEST: special range cases
    if (out_min == out_max) return out_min;
    if (in_min == in_max) return out_min/2 + out_max/2;    // out_min or out_max? better

    // test if there will be an overflow with well known (unbalanced) formula
    if ((   (MAXLONG / abs(out_max - out_min)) < (val - in_min) )  // multiplication overflow test
        || ((MAXLONG - in_max) < -in_min ))                        // division overflow test
    {
        // if overflow would occur that means the ranges are too big
        // To solve this we divide both the input & output range in two
        // alternative is to throw an error.
        // Serial.print(" >> "); // just to see the dividing
        int32_t mid = in_min/2 + in_max/2;
        int32_t Tmid = out_min/2 + out_max/2;  
        if (val > mid)
        {
            // map with upper half of original range
            return mapRange(val, mid, in_max, Tmid, out_max);
        }
        // map with lower half of original range
        return mapRange(val, in_min, mid, out_min, Tmid);  
    }

    // finally we have a range that can be calculated
    // unbalanced
    // return out_min + ((out_max - out_min) * (val - in_min)) / (in_max - in_min);

    // or balanced
    // return BalancedMap(val, in_min, in_max, out_min, out_max);
    uint32_t niv = in_max - in_min + 1;          // number input valuer
    uint32_t nov = abs(out_max - out_min) + 1;   // number output values
    uint32_t pos = val - in_min + 1;             // position of val

    uint32_t newpos = ((pos * nov) + niv-1) / niv;  // new position with rounding
    if (out_min < out_max) return out_min + newpos -1;
    return out_min - newpos + 1;
}


/*****************************************************************************\
 * Function:    ms_to_time
 * Input:       input time in ms, pointer to min and sec variables
 * Returns:     none
 * Description: 
 *     Calculates time in hours/min/sec from a give ms input
\*****************************************************************************/
void ms_to_time(uint32_t input_ms, uint32_t *out_min, uint32_t *out_s)
{
    uint32_t t32;
    *out_min = input_ms/(1000ul*60ul);               //minutes
    t32 = input_ms % (1000ul*60ul);
    *out_s = (t32 + 500ul)/1000ul;                   //seconds  
}

/*****************************************************************************\
 * Function:    printBin
 * Input:       input value, number of places
 * Returns:     none
 * Description: 
 *     prints out a binary representation of the input variable
\*****************************************************************************/
void printBin(uint32_t v, uint8_t num_places)
{
    uint32_t mask=0, n;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001UL;
    }
    v = v & mask;  // truncate v to specified number of places

    while(num_places)
    {

        if (v & (0x0001 << (num_places-1))) UART_UartPutChar('0');
        else                                UART_UartPutChar('1');
        --num_places;
        if(((num_places%4) == 0) && (num_places != 0))
        {
            UART_UartPutChar('_');
        }
    }
    
}

/*****************************************************************************\
 * Function:    putChar
 * Input:       character and an extra pointer :)
 * Returns:     none
 * Description: 
 *     function required for the tinyprintf library
\*****************************************************************************/
void putChar(void* p, char c)
{
    if (&p == &p){};    //ingone unused variable
    UART_UartPutChar(c);
}

/*****************************************************************************\
 * Function:    crc16
 * Input:       pointer to a buffer, length
 * Returns:     CRC-16-CCITT (poly 0x1021) value
 * Description: 
 *     calculates the CRC-16-CCITT (poly 0x1021) value
\*****************************************************************************/
uint16_t crc16(const uint8_t* data_p, uint8_t length)
{
    unsigned char x;
    unsigned short crc = 0xFFFF;

    while (length--)
    {
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
    }
    return crc;
}

/* [] END OF FILE */
