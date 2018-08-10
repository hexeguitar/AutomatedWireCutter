/*******************************************************************************
* File Name: ugui_st7735.h  
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
*  1.0 - April, 2016.       First version.
*
********************************************************************************
* Copyright (c) 2015 Jesús Rodríguez Cacabelos
* This driver follows the same license than the uGui library.
* http:
*******************************************************************************/

#if !defined(PSOC_UGUI_ST7735_H)
#define PSOC_UGUI_ST7735_H

#include "ugui.h"
    
/* *** Configuration. *** */

    /* Select SPI mode used. SCB mode or UDB based. Uncomment used mode and comment the other. */
    /* Only for PSoC 4. Not used in PSoC 5LP. */
#define USING_SPI_SCB
//#define USING_SPI_UDB
    
    /* Put here the same name of the SPI bus module you have used in schematic. */
#define SPI_NAME                SPI 
    
    /* Set here the name you have given to display CD and RESET signals in your schematic. */
#define DISPLAY_RESET           DISP_RESET
#define DISPLAY_CD              DISP_CD
    
    /* Set here your display resolution. */
#define DISPLAY_WIDTH           128
#define DISPLAY_HEIGHT          128
    
    /**** 
    Realize that ST7735 chip max bits per pixel is 6 (RGB666), so if you configure uGui
    to use RGB888, two lower bits of every color will be discarded by the chip.
    *****/
    
/* *** Configuration <END>. *** */
    
    /* Text substitution macros to avoid changing function names related to SPI API
       if SPI module name is changed in schematic. */

#define CONCATENATE(name, function)                 CONCAT(name, function)
#define CONCAT(name, function)                      name##function

#if (CY_PSOC4)

    #if defined USING_SPI_SCB
        #define mmSPI_SpiUartPutArray(a, b)                 CONCATENATE(SPI_NAME, _SpiUartPutArray(a, b)) 
        #define mmSPI_SpiUartWriteTxData(value)             CONCATENATE(SPI_NAME, _SpiUartWriteTxData(value))
        #define mmSPI_SpiUartGetTxBufferSize()              CONCATENATE(SPI_NAME, _SpiUartGetTxBufferSize())
        #define mmSPI_SpiUartGetRxBufferSize()              CONCATENATE(SPI_NAME, _SpiUartGetRxBufferSize())        
        #define mmSPI_SpiUartClearRxBuffer()                CONCATENATE(SPI_NAME, _SpiUartClearRxBuffer())
        #define mmSPI_SpiUartReadRxData()                   CONCATENATE(SPI_NAME, _SpiUartReadRxData())
        #define mmSPI_SpiIsBusBusy()                        CONCATENATE(SPI_NAME, _SpiIsBusBusy())
        #define mmSPI_Start()                               CONCATENATE(SPI_NAME, _Start())
    #endif
    
    #if defined USING_SPI_UDB
        #define mmSPI_SpiUartPutArray(a, b)                 CONCATENATE(SPI_NAME, _PutArray(a, b)) 
        #define mmSPI_SpiUartWriteTxData(value)             CONCATENATE(SPI_NAME, _WriteTxData(value))
        #define mmSPI_SpiUartGetTxBufferSize()              CONCATENATE(SPI_NAME, _GetTxBufferSize())
        #define mmSPI_SpiUartGetRxBufferSize()              CONCATENATE(SPI_NAME, _GetRxBufferSize())
    #endif
    
#endif    

#if (CY_PSOC5LP) 

        #define mmSPI_SpiUartPutArray(a, b)                 CONCATENATE(SPI_NAME, _PutArray(a, b)) 
        #define mmSPI_SpiUartWriteTxData(value)             CONCATENATE(SPI_NAME, _WriteTxData(value))
        #define mmSPI_SpiUartGetTxBufferSize()              CONCATENATE(SPI_NAME, _GetTxBufferSize())
        #define mmSPI_SpiUartGetRxBufferSize()              CONCATENATE(SPI_NAME, _GetRxBufferSize())
    
#endif 

#define mmDisplayReset_Write(value) CONCATENATE(DISPLAY_RESET, _Write(value))
#define mmDisplayCD_Write(value)    CONCATENATE(DISPLAY_CD, _Write(value))
    
/* *** Global GUI structure for this. *** */
extern UG_GUI gui7735;

/* *** Function prototypes. *** */
void ST7735_Init(uint8_t rotation);
void ST7735_PSet(UG_S16 x, UG_S16 y, UG_COLOR c);
void ST7735_setRotation(uint8_t m);
uint8_t ST7735_getRotation(void);
uint16_t ST7735_Color565(uint8_t r, uint8_t g, uint8_t b);
void ST7735_invertDisplay(uint8_t i);


    // Accelerators.
UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
UG_RESULT HW_DrawLine(UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c );



#endif /* End PSOC_UGUI_ST7735_H */    

/* [] END OF FILE */

