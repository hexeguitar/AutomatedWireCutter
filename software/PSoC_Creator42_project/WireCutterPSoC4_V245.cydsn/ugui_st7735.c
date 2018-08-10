/*******************************************************************************
* File Name: ugui_st7735.c
*
* Description:
*  This is a driver for the uGui graphical library developed by 
*  Achim Döbler.
*  It is for ST7735 graphic controller found in a lot of low cost graphics 
*  chinese displays an to be used with PSoC microcontrollers from Cypress.
*  At the time of writting this, developing and testing with PSoC 4 micros.
*  Will test it with other PSoC micros as soon as i can.
*
* Note:
*  For more information about uGui...
*  Website: http://www.embeddedlightning.com/ugui/
*  Git: https://github.com/achimdoebler/UGUI
*  Forum: http://www.embeddedlightning.com/forum/
*  PDF Reference manual (excellent): http://www.embeddedlightning.com/download/reference-guide/?wpdmdl=205
*
*  Thanks to Achim Döbler for such a god job.
*
* Log version:
*  1.1 - December 2017      A few upgrades and changes by Piotr Zapart
*  1.0 - April, 2016.       First version.
*
********************************************************************************
* Copyright (c) 2015 Jesús Rodríguez Cacabelos
* This driver follows the same license than the uGui library.
* http:
*******************************************************************************/

#include <project.h>
#include "ugui.h"
#include "ugui_st7735.h"
#include "tinyprintf.h"

#define ST7735_NOP      0x0
#define ST7735_SWRESET  0x01
#define ST7735_RDDID    0x04
#define ST7735_RDDST    0x09

#define ST7735_SLPIN    0x10
#define ST7735_SLPOUT   0x11
#define ST7735_PTLON    0x12
#define ST7735_NORON    0x13

#define ST7735_INVOFF   0x20
#define ST7735_INVON    0x21
#define ST7735_DISPOFF  0x28
#define ST7735_DISPON   0x29
#define ST7735_CASET    0x2A
#define ST7735_RASET    0x2B
#define ST7735_RAMWR    0x2C
#define ST7735_RAMRD    0x2E

#define ST7735_VSCLLDEF	0x33//Vertical Scroll Definition	
#define ST7735_COLMOD   0x3A
#define ST7735_MADCTL   0x36
#define ST7735_VSSTADRS	0x37//Vertical Scrolling Start address

#define ST7735_FRMCTR1  0xB1
#define ST7735_FRMCTR2  0xB2
#define ST7735_FRMCTR3  0xB3
#define ST7735_INVCTR   0xB4
#define ST7735_DISSET5  0xB6

#define ST7735_PWCTR1   0xC0
#define ST7735_PWCTR2   0xC1
#define ST7735_PWCTR3   0xC2
#define ST7735_PWCTR4   0xC3
#define ST7735_PWCTR5   0xC4
#define ST7735_VMCTR1   0xC5

#define ST7735_RDID1    0xDA
#define ST7735_RDID2    0xDB
#define ST7735_RDID3    0xDC
#define ST7735_RDID4    0xDD

#define ST7735_PWCTR6   0xFC

#define ST7735_GMCTRP1  0xE0
#define ST7735_GMCTRN1  0xE1

#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_ML  0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

#define ST7735_RAM_OFFSET   32

uint8_t ST7735_colstart = ST7735_RAM_OFFSET;       //this is an offset in y axis. Some 1.44" LCD require a shift of 32 pixels in y axis 
uint8_t ST7735_rowstart = 0;
uint8_t ST7735_rotation = 0;
uint8_t ST7735_width = DISPLAY_WIDTH;
uint8_t ST7735_height = DISPLAY_HEIGHT;

#define ST7735_DELAY 0x80

// #############################################################################
const uint8_t LCD_initData[] = 
{                                       // Init for 7735R, part 1 (red or green tab)
    21,                                 // 21 commands in list:
    ST7735_SWRESET,   ST7735_DELAY,     //  1: Software reset, 0 args, w/delay
      150,                              //     150 ms delay
    ST7735_SLPOUT ,   ST7735_DELAY,     //  2: Out of sleep mode, 0 args, w/delay
      255,                              //     500 ms delay
    ST7735_FRMCTR1, 3      ,            //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,                 //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3      ,            //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,                 //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6      ,            //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,                 //     Dot inversion mode
      0x01, 0x2C, 0x2D,                 //     Line inversion mode
    ST7735_INVCTR , 1      ,            //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                             //     No inversion
    ST7735_PWCTR1 , 3      ,            //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                             //     -4.6V
      0x84,                             //     AUTO mode
    ST7735_PWCTR2 , 1      ,            //  8: Power control, 1 arg, no delay:
      0xC5,                             //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 , 2      ,            //  9: Power control, 2 args, no delay:
      0x0A,                             //     Opamp current small
      0x00,                             //     Boost frequency
    ST7735_PWCTR4 , 2      ,            // 10: Power control, 2 args, no delay:
      0x8A,                             //     BCLK/2, Opamp current small & Medium low
      0x2A,  
    ST7735_PWCTR5 , 2      ,            // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1 , 1      ,            // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735_INVOFF , 0      ,            // 13: Don't invert display, no args, no delay
    ST7735_MADCTL , 1      ,            // 14: Memory access control (directions), 1 arg:
      0xA8,                             //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD , 1      ,            // 15: set color mode, 1 arg, no delay:
      0x05,
    ST7735_CASET  , 4      ,            // 16: Column addr set, 4 args, no delay:
      0x00, 0x00,                       //     XSTART = 0
      0x00, 0x7F,                       //     XEND = 127
    ST7735_RASET  , 4      ,            // 17: Row addr set, 4 args, no delay:
      0x00, 0x00,                       //     XSTART = 0
      0x00, 0x7F,
    ST7735_GMCTRP1, 16      ,           // 18: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      ,           // 19: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735_NORON  ,    ST7735_DELAY,    // 20: Normal display on, no args, w/delay
      10,                               //     10 ms delay
    ST7735_DISPON ,    ST7735_DELAY,    // 21: Main screen turn on, no args w/delay
      100    
};
// #############################################################################

/* *** Function prototypes. *** */
void ST7735_Reset();
void ST7735_WriteCommand(uint8 command);
void ST7735_WriteData(uint8 data[], uint8 length);
void ST7735_WriteDataByte(uint8_t data);
void ST7735_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

// #############################################################################
// Companion code to the above tables. Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void ST7735_commandList(uint8_t const *addr) 
{
    uint8_t numCommands, numArgs;
    uint16_t ms;

    numCommands = *(addr++);                        // Number of commands to follow
    while(numCommands--) 
    { 
        ST7735_WriteCommand(*(addr++));            // Read, issue command    
        numArgs = *(addr++);                        // Number of args to follow
        ms = numArgs & ST7735_DELAY;                       // If hibit set, delay follows args
        numArgs &= ~ST7735_DELAY;                          // Mask out delay bit
        ST7735_WriteData((uint8_t *)addr,numArgs);    
        addr += numArgs;
        if (ms) 
        {
            ms = *(addr++);                         // Read post-command delay time (ms)
            if(ms == 255) ms = 500; 
            CyDelay(ms);                            // Delay in ms
        }   
    }
}
// #############################################################################
void ST7735_Init(uint8_t rotation)
{ 
    /* *** Initialize hardware. **** */
    DISP_RESET_Write(1);                    // DISP_RESET is active low. So initialize it to high level.
    DISP_CD_Write(1);                       // DISP_CD. Ready for data mode, the most
                                            //          common mode.       
    /* *** Initialize display. *** */
    ST7735_Reset();
    ST7735_commandList(LCD_initData);
    ST7735_rotation = rotation;
    ST7735_setRotation(ST7735_rotation); //will init global structure and register hardware accelerators
    
}
// #############################################################################
void ST7735_Reset()
{
    mmDisplayReset_Write(0); CyDelay(50);
    mmDisplayReset_Write(1); CyDelay(120);
}
// #############################################################################
void ST7735_WriteCommand(uint8 command)
{
    mmDisplayCD_Write(0);  
    mmSPI_SpiUartWriteTxData(command);
    mmSPI_SpiUartClearRxBuffer();
}
// #############################################################################
void ST7735_WriteData(uint8 data[], uint8 length)
{
    mmDisplayCD_Write(1);    
    mmSPI_SpiUartPutArray(data, length);
    mmSPI_SpiUartClearRxBuffer();
}
// #############################################################################
void ST7735_WriteDataByte(uint8_t data)
{
    mmDisplayCD_Write(1);
    mmSPI_SpiUartWriteTxData(data);
    mmSPI_SpiUartClearRxBuffer();
}
// #############################################################################
uint16_t ST7735_Color565(uint8_t r, uint8_t g, uint8_t b) 
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// #############################################################################
void ST7735_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) 
{
    uint8 ddata[4];
    ST7735_WriteCommand(ST7735_CASET); // Column addr set
    ddata[0] = 0x00; ddata[1] = x0+ST7735_colstart;
    ddata[2] = 0x00; ddata[3] = x1+ST7735_colstart;
    ST7735_WriteData(ddata, 4);

    ST7735_WriteCommand(ST7735_RASET); // Row addr set
    ddata[0] = 0x00; ddata[1] = y0+ST7735_rowstart;
    ddata[2] = 0x00; ddata[3] = y1+ST7735_rowstart;
    ST7735_WriteData(ddata, 4);

    ST7735_WriteCommand(ST7735_RAMWR); // write to RAM
}
// #############################################################################
void ST7735_PSet(UG_S16 x, UG_S16 y, UG_COLOR c)
{
    uint8 ddata[4];

    if((x < 0) ||(x >= ST7735_width) || (y < 0) || (y >= ST7735_height)) return;

    ST7735_setAddrWindow(x, y, x+1, y+1);

#if defined USE_COLOR_RGB565
    
    ddata[0] = (c >> 8); ddata[1] = c;
    ST7735_WriteData(ddata, 2);
    
#endif    

#if defined USE_COLOR_RGB888
    
    ddata[0] = (uint8)(c);
    ddata[1] = (uint8)(c >> 8);
    ddata[2] = (uint8)(c >> 16);
    
    ST7735_WriteData(ddata, 3); 
    
#endif    

}
// #############################################################################
UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
    uint8 loopx, loopy;
    uint8 ddata[4];
    
    if((x1 < 0) ||(x1 >= ST7735_width) || (y1 < 0) || (y1 >= ST7735_height)) 
    {
        return UG_RESULT_FAIL;      
    }
    if((x2 < 0) ||(x2 >= ST7735_width) || (y2 < 0) || (y2 >= ST7735_height)) 
    {
        return UG_RESULT_FAIL;      
    }

    ST7735_setAddrWindow(x1, y1, x2, y2);      
    
#if defined USE_COLOR_RGB565
       
    for (loopx = x1; loopx < (x2 + 1); loopx++)
    {
        for (loopy = y1; loopy < (y2 + 1); loopy++)
        {
            ddata[0] = (c >> 8); ddata[1] = c;
            ST7735_WriteData(ddata, 2); 
        }
    }
    
#endif    

#if defined USE_COLOR_RGB888
    
    ddata[0] = (uint8)(c);
    ddata[1] = (uint8)(c >> 8);
    ddata[2] = (uint8)(c >> 16);
    
    for (loopx = x1; loopx < (x2 + 1); loopx++)
    {
        for (loopy = y1; loopy < (y2 + 1); loopy++)
        {
            ST7735_WriteData(ddata, 3); 
        }
    }
    
#endif    
    return UG_RESULT_OK;
}
// #############################################################################
UG_RESULT HW_DrawLine( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c )
{
    if((x1 < 0) ||(x1 >= ST7735_width) || (y1 < 0) || (y1 >= ST7735_height)) return UG_RESULT_FAIL;
    if((x2 < 0) ||(x2 >= ST7735_width) || (y2 < 0) || (y2 >= ST7735_height)) return UG_RESULT_FAIL;
    
    // If it is a vertical or a horizontal line, draw it.
    // If not, then use original drawline routine.
    if ((x1 == x2) || (y1 == y2)) 
    {
        HW_FillFrame(x1, y1, x2, y2, c);
        return UG_RESULT_OK;
    }
    
    return UG_RESULT_FAIL;
}
// #############################################################################

void ST7735_setRotation(uint8_t m) 
{
    ST7735_WriteCommand(ST7735_MADCTL);

    ST7735_rotation = m % 4; // can't be higher than 3
    switch (ST7735_rotation) 
    {
        case 0:
                ST7735_WriteDataByte(ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR);
                ST7735_width = DISPLAY_WIDTH;
                ST7735_height = DISPLAY_HEIGHT;
                ST7735_colstart = 0;      
                ST7735_rowstart = ST7735_RAM_OFFSET;
                break;
        case 1: 
                ST7735_WriteDataByte(ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR);
                ST7735_width = DISPLAY_HEIGHT;
                ST7735_height = DISPLAY_WIDTH;
                ST7735_colstart = ST7735_RAM_OFFSET;      
                ST7735_rowstart = 0;
                break;
        case 2:
                ST7735_WriteDataByte(ST7735_MADCTL_BGR);
                ST7735_width = DISPLAY_WIDTH;
                ST7735_height = DISPLAY_HEIGHT;
                ST7735_colstart = 0;      
                ST7735_rowstart = 0;
                break;
        case 3:
                ST7735_WriteDataByte(ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_BGR);
                ST7735_width = DISPLAY_HEIGHT;
                ST7735_height = DISPLAY_WIDTH;
                ST7735_colstart = 0;      
                ST7735_rowstart = 0;
                break;
    }
    //re-init the gui with new width/height settings
    UG_Init(&gui7735, ST7735_PSet, ST7735_width, ST7735_height);
    // re-register accelerators, as they are cleared in the init func.
    UG_DriverRegister(DRIVER_FILL_FRAME, (void*)HW_FillFrame);
    UG_DriverRegister(DRIVER_DRAW_LINE, (void*)HW_DrawLine);
    UG_DriverEnable( DRIVER_DRAW_LINE );
    UG_DriverEnable( DRIVER_FILL_FRAME );
}
// #############################################################################
uint8_t ST7735_getRotation(void)
{
    return ST7735_rotation;
}
// #############################################################################
void ST7735_invertDisplay(uint8_t i) 
{
    ST7735_WriteCommand(i ? ST7735_INVON : ST7735_INVOFF);
}
// #############################################################################

/* [] END OF FILE */
