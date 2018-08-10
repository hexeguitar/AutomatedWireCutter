/*******************************************************************************
* File Name: ADC12_intClock.h
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

#if !defined(CY_CLOCK_ADC12_intClock_H)
#define CY_CLOCK_ADC12_intClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void ADC12_intClock_StartEx(uint32 alignClkDiv);
#define ADC12_intClock_Start() \
    ADC12_intClock_StartEx(ADC12_intClock__PA_DIV_ID)

#else

void ADC12_intClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void ADC12_intClock_Stop(void);

void ADC12_intClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 ADC12_intClock_GetDividerRegister(void);
uint8  ADC12_intClock_GetFractionalDividerRegister(void);

#define ADC12_intClock_Enable()                         ADC12_intClock_Start()
#define ADC12_intClock_Disable()                        ADC12_intClock_Stop()
#define ADC12_intClock_SetDividerRegister(clkDivider, reset)  \
    ADC12_intClock_SetFractionalDividerRegister((clkDivider), 0u)
#define ADC12_intClock_SetDivider(clkDivider)           ADC12_intClock_SetDividerRegister((clkDivider), 1u)
#define ADC12_intClock_SetDividerValue(clkDivider)      ADC12_intClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define ADC12_intClock_DIV_ID     ADC12_intClock__DIV_ID

#define ADC12_intClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define ADC12_intClock_CTRL_REG   (*(reg32 *)ADC12_intClock__CTRL_REGISTER)
#define ADC12_intClock_DIV_REG    (*(reg32 *)ADC12_intClock__DIV_REGISTER)

#define ADC12_intClock_CMD_DIV_SHIFT          (0u)
#define ADC12_intClock_CMD_PA_DIV_SHIFT       (8u)
#define ADC12_intClock_CMD_DISABLE_SHIFT      (30u)
#define ADC12_intClock_CMD_ENABLE_SHIFT       (31u)

#define ADC12_intClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << ADC12_intClock_CMD_DISABLE_SHIFT))
#define ADC12_intClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << ADC12_intClock_CMD_ENABLE_SHIFT))

#define ADC12_intClock_DIV_FRAC_MASK  (0x000000F8u)
#define ADC12_intClock_DIV_FRAC_SHIFT (3u)
#define ADC12_intClock_DIV_INT_MASK   (0xFFFFFF00u)
#define ADC12_intClock_DIV_INT_SHIFT  (8u)

#else 

#define ADC12_intClock_DIV_REG        (*(reg32 *)ADC12_intClock__REGISTER)
#define ADC12_intClock_ENABLE_REG     ADC12_intClock_DIV_REG
#define ADC12_intClock_DIV_FRAC_MASK  ADC12_intClock__FRAC_MASK
#define ADC12_intClock_DIV_FRAC_SHIFT (16u)
#define ADC12_intClock_DIV_INT_MASK   ADC12_intClock__DIVIDER_MASK
#define ADC12_intClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_ADC12_intClock_H) */

/* [] END OF FILE */
