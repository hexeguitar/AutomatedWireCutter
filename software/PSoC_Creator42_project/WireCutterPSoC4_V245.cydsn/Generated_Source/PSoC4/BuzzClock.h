/*******************************************************************************
* File Name: BuzzClock.h
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

#if !defined(CY_CLOCK_BuzzClock_H)
#define CY_CLOCK_BuzzClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void BuzzClock_StartEx(uint32 alignClkDiv);
#define BuzzClock_Start() \
    BuzzClock_StartEx(BuzzClock__PA_DIV_ID)

#else

void BuzzClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void BuzzClock_Stop(void);

void BuzzClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 BuzzClock_GetDividerRegister(void);
uint8  BuzzClock_GetFractionalDividerRegister(void);

#define BuzzClock_Enable()                         BuzzClock_Start()
#define BuzzClock_Disable()                        BuzzClock_Stop()
#define BuzzClock_SetDividerRegister(clkDivider, reset)  \
    BuzzClock_SetFractionalDividerRegister((clkDivider), 0u)
#define BuzzClock_SetDivider(clkDivider)           BuzzClock_SetDividerRegister((clkDivider), 1u)
#define BuzzClock_SetDividerValue(clkDivider)      BuzzClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define BuzzClock_DIV_ID     BuzzClock__DIV_ID

#define BuzzClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define BuzzClock_CTRL_REG   (*(reg32 *)BuzzClock__CTRL_REGISTER)
#define BuzzClock_DIV_REG    (*(reg32 *)BuzzClock__DIV_REGISTER)

#define BuzzClock_CMD_DIV_SHIFT          (0u)
#define BuzzClock_CMD_PA_DIV_SHIFT       (8u)
#define BuzzClock_CMD_DISABLE_SHIFT      (30u)
#define BuzzClock_CMD_ENABLE_SHIFT       (31u)

#define BuzzClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << BuzzClock_CMD_DISABLE_SHIFT))
#define BuzzClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << BuzzClock_CMD_ENABLE_SHIFT))

#define BuzzClock_DIV_FRAC_MASK  (0x000000F8u)
#define BuzzClock_DIV_FRAC_SHIFT (3u)
#define BuzzClock_DIV_INT_MASK   (0xFFFFFF00u)
#define BuzzClock_DIV_INT_SHIFT  (8u)

#else 

#define BuzzClock_DIV_REG        (*(reg32 *)BuzzClock__REGISTER)
#define BuzzClock_ENABLE_REG     BuzzClock_DIV_REG
#define BuzzClock_DIV_FRAC_MASK  BuzzClock__FRAC_MASK
#define BuzzClock_DIV_FRAC_SHIFT (16u)
#define BuzzClock_DIV_INT_MASK   BuzzClock__DIVIDER_MASK
#define BuzzClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_BuzzClock_H) */

/* [] END OF FILE */
