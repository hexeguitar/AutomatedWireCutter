/*******************************************************************************
* File Name: ClockKey.h
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

#if !defined(CY_CLOCK_ClockKey_H)
#define CY_CLOCK_ClockKey_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void ClockKey_StartEx(uint32 alignClkDiv);
#define ClockKey_Start() \
    ClockKey_StartEx(ClockKey__PA_DIV_ID)

#else

void ClockKey_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void ClockKey_Stop(void);

void ClockKey_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 ClockKey_GetDividerRegister(void);
uint8  ClockKey_GetFractionalDividerRegister(void);

#define ClockKey_Enable()                         ClockKey_Start()
#define ClockKey_Disable()                        ClockKey_Stop()
#define ClockKey_SetDividerRegister(clkDivider, reset)  \
    ClockKey_SetFractionalDividerRegister((clkDivider), 0u)
#define ClockKey_SetDivider(clkDivider)           ClockKey_SetDividerRegister((clkDivider), 1u)
#define ClockKey_SetDividerValue(clkDivider)      ClockKey_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define ClockKey_DIV_ID     ClockKey__DIV_ID

#define ClockKey_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define ClockKey_CTRL_REG   (*(reg32 *)ClockKey__CTRL_REGISTER)
#define ClockKey_DIV_REG    (*(reg32 *)ClockKey__DIV_REGISTER)

#define ClockKey_CMD_DIV_SHIFT          (0u)
#define ClockKey_CMD_PA_DIV_SHIFT       (8u)
#define ClockKey_CMD_DISABLE_SHIFT      (30u)
#define ClockKey_CMD_ENABLE_SHIFT       (31u)

#define ClockKey_CMD_DISABLE_MASK       ((uint32)((uint32)1u << ClockKey_CMD_DISABLE_SHIFT))
#define ClockKey_CMD_ENABLE_MASK        ((uint32)((uint32)1u << ClockKey_CMD_ENABLE_SHIFT))

#define ClockKey_DIV_FRAC_MASK  (0x000000F8u)
#define ClockKey_DIV_FRAC_SHIFT (3u)
#define ClockKey_DIV_INT_MASK   (0xFFFFFF00u)
#define ClockKey_DIV_INT_SHIFT  (8u)

#else 

#define ClockKey_DIV_REG        (*(reg32 *)ClockKey__REGISTER)
#define ClockKey_ENABLE_REG     ClockKey_DIV_REG
#define ClockKey_DIV_FRAC_MASK  ClockKey__FRAC_MASK
#define ClockKey_DIV_FRAC_SHIFT (16u)
#define ClockKey_DIV_INT_MASK   ClockKey__DIVIDER_MASK
#define ClockKey_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_ClockKey_H) */

/* [] END OF FILE */
