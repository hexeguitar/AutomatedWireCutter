/*******************************************************************************
* File Name: ClockLpress.h
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

#if !defined(CY_CLOCK_ClockLpress_H)
#define CY_CLOCK_ClockLpress_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void ClockLpress_StartEx(uint32 alignClkDiv);
#define ClockLpress_Start() \
    ClockLpress_StartEx(ClockLpress__PA_DIV_ID)

#else

void ClockLpress_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void ClockLpress_Stop(void);

void ClockLpress_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 ClockLpress_GetDividerRegister(void);
uint8  ClockLpress_GetFractionalDividerRegister(void);

#define ClockLpress_Enable()                         ClockLpress_Start()
#define ClockLpress_Disable()                        ClockLpress_Stop()
#define ClockLpress_SetDividerRegister(clkDivider, reset)  \
    ClockLpress_SetFractionalDividerRegister((clkDivider), 0u)
#define ClockLpress_SetDivider(clkDivider)           ClockLpress_SetDividerRegister((clkDivider), 1u)
#define ClockLpress_SetDividerValue(clkDivider)      ClockLpress_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define ClockLpress_DIV_ID     ClockLpress__DIV_ID

#define ClockLpress_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define ClockLpress_CTRL_REG   (*(reg32 *)ClockLpress__CTRL_REGISTER)
#define ClockLpress_DIV_REG    (*(reg32 *)ClockLpress__DIV_REGISTER)

#define ClockLpress_CMD_DIV_SHIFT          (0u)
#define ClockLpress_CMD_PA_DIV_SHIFT       (8u)
#define ClockLpress_CMD_DISABLE_SHIFT      (30u)
#define ClockLpress_CMD_ENABLE_SHIFT       (31u)

#define ClockLpress_CMD_DISABLE_MASK       ((uint32)((uint32)1u << ClockLpress_CMD_DISABLE_SHIFT))
#define ClockLpress_CMD_ENABLE_MASK        ((uint32)((uint32)1u << ClockLpress_CMD_ENABLE_SHIFT))

#define ClockLpress_DIV_FRAC_MASK  (0x000000F8u)
#define ClockLpress_DIV_FRAC_SHIFT (3u)
#define ClockLpress_DIV_INT_MASK   (0xFFFFFF00u)
#define ClockLpress_DIV_INT_SHIFT  (8u)

#else 

#define ClockLpress_DIV_REG        (*(reg32 *)ClockLpress__REGISTER)
#define ClockLpress_ENABLE_REG     ClockLpress_DIV_REG
#define ClockLpress_DIV_FRAC_MASK  ClockLpress__FRAC_MASK
#define ClockLpress_DIV_FRAC_SHIFT (16u)
#define ClockLpress_DIV_INT_MASK   ClockLpress__DIVIDER_MASK
#define ClockLpress_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_ClockLpress_H) */

/* [] END OF FILE */
