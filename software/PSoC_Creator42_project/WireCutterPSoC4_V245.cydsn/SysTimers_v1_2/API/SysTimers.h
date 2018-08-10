/*******************************************************************************
* File Name: `$INSTANCE_NAME`.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This header file contains registers and constants associated with the
*  System Timer Component
*
*  6-26-2013  MEH  Initial Creation

* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SYSTIMERS_`$INSTANCE_NAME`_H)
#define CY_SYSTIMERS_`$INSTANCE_NAME`_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define `$INSTANCE_NAME`_TIMERS           `$Number_of_Timers`
#define `$INSTANCE_NAME`_TIMER_MASK       (`$Number_of_Timers`-1)
#define `$INSTANCE_NAME`_RESOLUTION       `$Timer_Resolution`
#define `$INSTANCE_NAME`_TIMER_MODE       `$Timer_Mode`
#define `$INSTANCE_NAME`_TICKS_PER_SECOND `$Timer_Resolution`
#define `$INSTANCE_NAME`_SYSTICK_IRQN     (uint8)(SysTick_IRQn + 16)  /* Offset between user and system vectors */


#define `$INSTANCE_NAME`_MODE_FAST_IRQ  1u
#define `$INSTANCE_NAME`_MODE_NORMAL    0u
#define `$INSTANCE_NAME`_ALARM_CNT_MASK 0x0000FFFF
#define `$INSTANCE_NAME`_ALARM_CLR_MASK 0x80000000
#define `$INSTANCE_NAME`_ALARM          0x10000000
#define `$INSTANCE_NAME`_IN_USE         0x80000000
#define `$INSTANCE_NAME`_NOT_IN_USE     0x00000000
#define `$INSTANCE_NAME`_RELOAD         0x00000000
#define `$INSTANCE_NAME`_INVALID_TIMER  0x00000000
#define `$INSTANCE_NAME`_INVALID_PERIOD 0xFFF00000
#define `$INSTANCE_NAME`_SYS_CLOCK      0



void    `$INSTANCE_NAME`_Start(void);
void    `$INSTANCE_NAME`_Stop(void);
uint32  `$INSTANCE_NAME`_GetTimer(uint32 timerValue);
uint32  `$INSTANCE_NAME`_GetTimerStatus(uint32 timerID);
uint32  `$INSTANCE_NAME`_GetTimerValue(uint32 timerID);
uint32  `$INSTANCE_NAME`_GetSysTickValue(void);
void    `$INSTANCE_NAME`_ResetTimer(uint32 timerID, uint32 timerValue);
void    `$INSTANCE_NAME`_ReleaseTimer(uint32 timerID);
void    `$INSTANCE_NAME`_ReleaseAllTimers(void);

CY_ISR_PROTO(`$INSTANCE_NAME`_SysTickISR);

#if (CY_PSOC3 || CY_PSOC5LP)
    #define  `$INSTANCE_NAME`_SYSTICK_PERIOD     ((BCLK__BUS_CLK__HZ)/`$Timer_Resolution`)
#elif (CY_PSOC4)
    #define  `$INSTANCE_NAME`_SYSTICK_PERIOD     ((CYDEV_BCLK__SYSCLK__HZ)/`$Timer_Resolution`)
#endif /* CY_PSOC5A */

#endif

//[] END OF FILE
