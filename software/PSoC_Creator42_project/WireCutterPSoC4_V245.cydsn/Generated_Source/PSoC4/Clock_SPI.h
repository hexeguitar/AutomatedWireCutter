/*******************************************************************************
* File Name: Clock_SPI.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Clock_SPI_H)
#define CY_CLOCK_Clock_SPI_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Clock_SPI_StartEx(uint32 alignClkDiv);
#define Clock_SPI_Start() \
    Clock_SPI_StartEx(Clock_SPI__PA_DIV_ID)

#else

void Clock_SPI_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Clock_SPI_Stop(void);

void Clock_SPI_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Clock_SPI_GetDividerRegister(void);
uint8  Clock_SPI_GetFractionalDividerRegister(void);

#define Clock_SPI_Enable()                         Clock_SPI_Start()
#define Clock_SPI_Disable()                        Clock_SPI_Stop()
#define Clock_SPI_SetDividerRegister(clkDivider, reset)  \
    Clock_SPI_SetFractionalDividerRegister((clkDivider), 0u)
#define Clock_SPI_SetDivider(clkDivider)           Clock_SPI_SetDividerRegister((clkDivider), 1u)
#define Clock_SPI_SetDividerValue(clkDivider)      Clock_SPI_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Clock_SPI_DIV_ID     Clock_SPI__DIV_ID

#define Clock_SPI_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Clock_SPI_CTRL_REG   (*(reg32 *)Clock_SPI__CTRL_REGISTER)
#define Clock_SPI_DIV_REG    (*(reg32 *)Clock_SPI__DIV_REGISTER)

#define Clock_SPI_CMD_DIV_SHIFT          (0u)
#define Clock_SPI_CMD_PA_DIV_SHIFT       (8u)
#define Clock_SPI_CMD_DISABLE_SHIFT      (30u)
#define Clock_SPI_CMD_ENABLE_SHIFT       (31u)

#define Clock_SPI_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Clock_SPI_CMD_DISABLE_SHIFT))
#define Clock_SPI_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Clock_SPI_CMD_ENABLE_SHIFT))

#define Clock_SPI_DIV_FRAC_MASK  (0x000000F8u)
#define Clock_SPI_DIV_FRAC_SHIFT (3u)
#define Clock_SPI_DIV_INT_MASK   (0xFFFFFF00u)
#define Clock_SPI_DIV_INT_SHIFT  (8u)

#else 

#define Clock_SPI_DIV_REG        (*(reg32 *)Clock_SPI__REGISTER)
#define Clock_SPI_ENABLE_REG     Clock_SPI_DIV_REG
#define Clock_SPI_DIV_FRAC_MASK  Clock_SPI__FRAC_MASK
#define Clock_SPI_DIV_FRAC_SHIFT (16u)
#define Clock_SPI_DIV_INT_MASK   Clock_SPI__DIVIDER_MASK
#define Clock_SPI_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Clock_SPI_H) */

/* [] END OF FILE */
