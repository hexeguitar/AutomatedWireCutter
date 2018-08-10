/*******************************************************************************
* File Name: `$INSTANCE_NAME`.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides source code for the System Timers component's API.
*
* 6-26-2013  MEH  Initial Creation
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CyLib.h"
#include <project.h>
#include "`$INSTANCE_NAME`.h"

volatile uint32  `$INSTANCE_NAME`_TimerSCount[`$INSTANCE_NAME`_TIMERS];
volatile uint32  `$INSTANCE_NAME`_TimerPeriod[`$INSTANCE_NAME`_TIMERS];
volatile uint32  `$INSTANCE_NAME`_TimerStatus[`$INSTANCE_NAME`_TIMERS];
volatile uint32  `$INSTANCE_NAME`_SysTickCount;

uint8 `$INSTANCE_NAME`_initVar = 0u;

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start
********************************************************************************
*
* Summary:
*  Initializes all the timers to initial condition.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void    `$INSTANCE_NAME`_Start(void)
{

	if( `$INSTANCE_NAME`_initVar == 0 )
	{
	    `$INSTANCE_NAME`_initVar = 1; 
		`$INSTANCE_NAME`_ReleaseAllTimers();
	    
		CyIntSetSysVector(`$INSTANCE_NAME`_SYSTICK_IRQN, `$INSTANCE_NAME`_SysTickISR);
    }
	SysTick_Config(`$INSTANCE_NAME`_SYSTICK_PERIOD);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SysTickISR
********************************************************************************
*
* Summary:
*  The ISR takes care of all the timing functions.  The user sets the interrupt
*  (or resolution) of the timers which determine the rate at which the ISR is
*  triggered.  The ISR overhead is a function of the interrupt frequency, the
*  number of timers, and the System clock setting.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_SysTickISR)
{

#if(`$INSTANCE_NAME`_TIMER_MODE == `$INSTANCE_NAME`_MODE_FAST_IRQ)
    /* Fast efficient ISR */
    `$INSTANCE_NAME`_SysTickCount++;
	
#else  /* This method takes longer but all timers are kept up to date */
    static uint32 index;
	`$INSTANCE_NAME`_SysTickCount++;
	
	for(index = 0; index < `$INSTANCE_NAME`_TIMERS; index++)
	{
		if(`$INSTANCE_NAME`_TimerStatus[index] & `$INSTANCE_NAME`_IN_USE)
		{
	        `$INSTANCE_NAME`_TimerSCount[index]--;
		    if(`$INSTANCE_NAME`_TimerSCount[index] == 0)
		    {
		        `$INSTANCE_NAME`_TimerStatus[index] |= `$INSTANCE_NAME`_ALARM;
		    	`$INSTANCE_NAME`_TimerStatus[index]++;  /* Increment the alarm count */
		    	`$INSTANCE_NAME`_TimerSCount[index]  = `$INSTANCE_NAME`_TimerPeriod[index];
		    }
		}
	}
#endif
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Stop
********************************************************************************
*
* Summary:
*  Stops the counters by disabling interrupts
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void    `$INSTANCE_NAME`_Stop(void)
{
    SysTick->CTRL  = 0u;  /* Disable interrupt and deactivate timer */
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_GetTimer
********************************************************************************
*
* Summary:
*  Reserve a timer to be used with the specified timout value.  A zero will be
*  return if no timers are available.  The user should check to make sure the
*  return value is non-zero.
*
* Parameters:
*  uint32 timerValue:  Timer period in sysTicks.
*
* Return:
*  A non-zero value will be return if a timer has been assigned.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint32  `$INSTANCE_NAME`_GetTimer(uint32 timerValue )
{
    uint32 index;
	uint32 timerID = `$INSTANCE_NAME`_INVALID_TIMER;
	
	/* Search for a unused timer */	
	for(index=0; index < `$INSTANCE_NAME`_TIMERS; index++)
	{
		if(!(`$INSTANCE_NAME`_TimerStatus[index] & `$INSTANCE_NAME`_IN_USE))
		{
		    #if(`$INSTANCE_NAME`_TIMER_MODE == `$INSTANCE_NAME`_MODE_FAST_IRQ)
			    `$INSTANCE_NAME`_TimerSCount[index]  = `$INSTANCE_NAME`_SysTickCount;
		    #else
			    `$INSTANCE_NAME`_TimerSCount[index] = timerValue;
			#endif
			`$INSTANCE_NAME`_TimerPeriod[index] = timerValue;
			`$INSTANCE_NAME`_TimerStatus[index] = `$INSTANCE_NAME`_IN_USE;
			timerID = index + 1;
			break;
		}
	}
	return(timerID);
}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_GetTimerStatus
********************************************************************************
*
* Summary:
*  Returns the status for the given timer.
*
* Parameters:
*  uint32 timerID:  The ID of the timer to be queried. 
*
* Return:
*  Status of timer, invalid, alarm triggered, or running.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint32  `$INSTANCE_NAME`_GetTimerStatus(uint32 timerID)
{
    uint32 status = 0;
	
	if(timerID > 0)
	{
	#if(`$INSTANCE_NAME`_TIMER_MODE == `$INSTANCE_NAME`_MODE_FAST_IRQ)
	    uint32 delta;
	    uint32 sysTickCnt;
	    timerID--;   // Timers start at index 1, but arrays start at 0
	    timerID = timerID & `$INSTANCE_NAME`_TIMER_MASK;  // Mask only valid timers
		
		if((`$INSTANCE_NAME`_TimerStatus[timerID] & `$INSTANCE_NAME`_IN_USE) != 0)
		{
		    sysTickCnt = `$INSTANCE_NAME`_SysTickCount;
		
			delta = sysTickCnt - `$INSTANCE_NAME`_TimerSCount[timerID];
			if(delta >= `$INSTANCE_NAME`_TimerPeriod[timerID] )
			{
			    status = 1;
				`$INSTANCE_NAME`_TimerSCount[timerID] += `$INSTANCE_NAME`_TimerPeriod[timerID];
			}
		}
	#else
	    timerID--;   // Timers start at index 1, but arrays start at 0
	    timerID = timerID & `$INSTANCE_NAME`_TIMER_MASK;   // Mask only valid timers
		if((`$INSTANCE_NAME`_TimerStatus[timerID] & `$INSTANCE_NAME`_IN_USE) != 0)
		{
			status = `$INSTANCE_NAME`_TimerStatus[timerID] & `$INSTANCE_NAME`_ALARM_CNT_MASK;
           
		    if(status != 0)
			{
			    `$INSTANCE_NAME`_TimerStatus[timerID] &= `$INSTANCE_NAME`_ALARM_CLR_MASK;  // Reset timer count
			
			}
		}
    #endif
	}
	
    return(status);
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_GetTimerValue
********************************************************************************
*
* Summary:
*  Returns the time left on the timer.
*
* Parameters:
*  uint32 timerID:  The ID of the timer to be queried. 
*
* Return:
*  The number of sysClocks before tiemr expires.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint32  `$INSTANCE_NAME`_GetTimerValue(uint32 timerID)
{
    uint32 timerValue;
    timerID--;
	timerID = timerID & `$INSTANCE_NAME`_TIMER_MASK;
	
    #if(`$INSTANCE_NAME`_TIMER_MODE == `$INSTANCE_NAME`_MODE_FAST_IRQ)
	    timerValue = `$INSTANCE_NAME`_SysTickCount - `$INSTANCE_NAME`_TimerSCount[timerID];
	#else
        timerValue = `$INSTANCE_NAME`_TimerSCount[timerID];
	#endif
	return(timerValue);
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_GetSysTickValue
********************************************************************************
*
* Summary:
*  Returns the sysTick counter value.
*
* Parameters:
*  none 
*
* Return:
*  Value of sysTick since reset.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint32  `$INSTANCE_NAME`_GetSysTickValue(void)
{
    return(`$INSTANCE_NAME`_SysTickCount);
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ResetTimer
********************************************************************************
*
* Summary:
*  Reset timer to new or old period.
*
* Parameters:
*  uint32 timerID:  The timers identification number.
*  uint32 timerValue:  New timer period, if zero reload with old value.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void  `$INSTANCE_NAME`_ResetTimer(uint32 timerID, uint32 timerValue)
{
    timerID--;
	timerID = timerID & `$INSTANCE_NAME`_TIMER_MASK;
	if(timerValue != 0)  /* If timerValue not zero, load new period */
	{
	    `$INSTANCE_NAME`_TimerPeriod[timerID] = timerValue;
	}
	#if(`$INSTANCE_NAME`_TIMER_MODE == `$INSTANCE_NAME`_MODE_FAST_IRQ)
        `$INSTANCE_NAME`_TimerSCount[timerID] = `$INSTANCE_NAME`_SysTickCount;
	#else
	    `$INSTANCE_NAME`_TimerSCount[timerID] = `$INSTANCE_NAME`_TimerPeriod[timerID];
	#endif
	
	`$INSTANCE_NAME`_TimerStatus[timerID] &= `$INSTANCE_NAME`_ALARM_CLR_MASK;
}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ReleaseTimer
********************************************************************************
*
* Summary:
*  Return timer to the queue, so that it may be used again.
*
* Parameters:
*  uint32 timerID:  ID of the timer to release.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void  `$INSTANCE_NAME`_ReleaseTimer(uint32 timerID)
{
	timerID--;
	`$INSTANCE_NAME`_TimerStatus[timerID] = `$INSTANCE_NAME`_NOT_IN_USE;
	`$INSTANCE_NAME`_TimerSCount[timerID] = `$INSTANCE_NAME`_INVALID_PERIOD;
	`$INSTANCE_NAME`_TimerPeriod[timerID] = `$INSTANCE_NAME`_INVALID_PERIOD;
}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ReleaseAllTimers
********************************************************************************
*
* Summary:
*  Return all timers to the queue.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void  `$INSTANCE_NAME`_ReleaseAllTimers(void)
{
    uint32 index;
	
	for(index = 1; index <= `$INSTANCE_NAME`_TIMERS; index++)
	{
		`$INSTANCE_NAME`_ReleaseTimer(index);
	}
}

/* [] END OF FILE */
