/*******************************************************************************
* File Name: SysTimers.h
* Version 1.2
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

#if !defined(CY_SYSTIMERS_SysTimers_H)
#define CY_SYSTIMERS_SysTimers_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define SysTimers_TIMERS           4
#define SysTimers_TIMER_MASK       (4-1)
#define SysTimers_RESOLUTION       1000
#define SysTimers_TIMER_MODE       0
#define SysTimers_TICKS_PER_SECOND 1000
#define SysTimers_SYSTICK_IRQN     (uint8)(SysTick_IRQn + 16)  /* Offset between user and system vectors */


#define SysTimers_MODE_FAST_IRQ  1u
#define SysTimers_MODE_NORMAL    0u
#define SysTimers_ALARM_CNT_MASK 0x0000FFFF
#define SysTimers_ALARM_CLR_MASK 0x80000000
#define SysTimers_ALARM          0x10000000
#define SysTimers_IN_USE         0x80000000
#define SysTimers_NOT_IN_USE     0x00000000
#define SysTimers_RELOAD         0x00000000
#define SysTimers_INVALID_TIMER  0x00000000
#define SysTimers_INVALID_PERIOD 0xFFF00000
#define SysTimers_SYS_CLOCK      0



void    SysTimers_Start(void);
void    SysTimers_Stop(void);
uint32  SysTimers_GetTimer(uint32 timerValue);
uint32  SysTimers_GetTimerStatus(uint32 timerID);
uint32  SysTimers_GetTimerValue(uint32 timerID);
uint32  SysTimers_GetSysTickValue(void);
void    SysTimers_ResetTimer(uint32 timerID, uint32 timerValue);
void    SysTimers_ReleaseTimer(uint32 timerID);
void    SysTimers_ReleaseAllTimers(void);

CY_ISR_PROTO(SysTimers_SysTickISR);

#if (CY_PSOC3 || CY_PSOC5LP)
    #define  SysTimers_SYSTICK_PERIOD     ((BCLK__BUS_CLK__HZ)/1000)
#elif (CY_PSOC4)
    #define  SysTimers_SYSTICK_PERIOD     ((CYDEV_BCLK__SYSCLK__HZ)/1000)
#endif /* CY_PSOC5A */

#endif

//[] END OF FILE
