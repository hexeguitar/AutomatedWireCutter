/*******************************************************************************
* File Name: SysTimers.c
* Version 1.2
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
#include "SysTimers.h"

volatile uint32  SysTimers_TimerSCount[SysTimers_TIMERS];
volatile uint32  SysTimers_TimerPeriod[SysTimers_TIMERS];
volatile uint32  SysTimers_TimerStatus[SysTimers_TIMERS];
volatile uint32  SysTimers_SysTickCount;

uint8 SysTimers_initVar = 0u;

/*******************************************************************************
* Function Name: SysTimers_Start
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
void    SysTimers_Start(void)
{

	if( SysTimers_initVar == 0 )
	{
	    SysTimers_initVar = 1; 
		SysTimers_ReleaseAllTimers();
	    
		CyIntSetSysVector(SysTimers_SYSTICK_IRQN, SysTimers_SysTickISR);
    }
	SysTick_Config(SysTimers_SYSTICK_PERIOD);
}


/*******************************************************************************
* Function Name: SysTimers_SysTickISR
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
CY_ISR(SysTimers_SysTickISR)
{

#if(SysTimers_TIMER_MODE == SysTimers_MODE_FAST_IRQ)
    /* Fast efficient ISR */
    SysTimers_SysTickCount++;
	
#else  /* This method takes longer but all timers are kept up to date */
    static uint32 index;
	SysTimers_SysTickCount++;
	
	for(index = 0; index < SysTimers_TIMERS; index++)
	{
		if(SysTimers_TimerStatus[index] & SysTimers_IN_USE)
		{
	        SysTimers_TimerSCount[index]--;
		    if(SysTimers_TimerSCount[index] == 0)
		    {
		        SysTimers_TimerStatus[index] |= SysTimers_ALARM;
		    	SysTimers_TimerStatus[index]++;  /* Increment the alarm count */
		    	SysTimers_TimerSCount[index]  = SysTimers_TimerPeriod[index];
		    }
		}
	}
#endif
}

/*******************************************************************************
* Function Name: SysTimers_Stop
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
void    SysTimers_Stop(void)
{
    SysTick->CTRL  = 0u;  /* Disable interrupt and deactivate timer */
}

/*******************************************************************************
* Function Name: SysTimers_GetTimer
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
uint32  SysTimers_GetTimer(uint32 timerValue )
{
    uint32 index;
	uint32 timerID = SysTimers_INVALID_TIMER;
	
	/* Search for a unused timer */	
	for(index=0; index < SysTimers_TIMERS; index++)
	{
		if(!(SysTimers_TimerStatus[index] & SysTimers_IN_USE))
		{
		    #if(SysTimers_TIMER_MODE == SysTimers_MODE_FAST_IRQ)
			    SysTimers_TimerSCount[index]  = SysTimers_SysTickCount;
		    #else
			    SysTimers_TimerSCount[index] = timerValue;
			#endif
			SysTimers_TimerPeriod[index] = timerValue;
			SysTimers_TimerStatus[index] = SysTimers_IN_USE;
			timerID = index + 1;
			break;
		}
	}
	return(timerID);
}
/*******************************************************************************
* Function Name: SysTimers_GetTimerStatus
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
uint32  SysTimers_GetTimerStatus(uint32 timerID)
{
    uint32 status = 0;
	
	if(timerID > 0)
	{
	#if(SysTimers_TIMER_MODE == SysTimers_MODE_FAST_IRQ)
	    uint32 delta;
	    uint32 sysTickCnt;
	    timerID--;   // Timers start at index 1, but arrays start at 0
	    timerID = timerID & SysTimers_TIMER_MASK;  // Mask only valid timers
		
		if((SysTimers_TimerStatus[timerID] & SysTimers_IN_USE) != 0)
		{
		    sysTickCnt = SysTimers_SysTickCount;
		
			delta = sysTickCnt - SysTimers_TimerSCount[timerID];
			if(delta >= SysTimers_TimerPeriod[timerID] )
			{
			    status = 1;
				SysTimers_TimerSCount[timerID] += SysTimers_TimerPeriod[timerID];
			}
		}
	#else
	    timerID--;   // Timers start at index 1, but arrays start at 0
	    timerID = timerID & SysTimers_TIMER_MASK;   // Mask only valid timers
		if((SysTimers_TimerStatus[timerID] & SysTimers_IN_USE) != 0)
		{
			status = SysTimers_TimerStatus[timerID] & SysTimers_ALARM_CNT_MASK;
           
		    if(status != 0)
			{
			    SysTimers_TimerStatus[timerID] &= SysTimers_ALARM_CLR_MASK;  // Reset timer count
			
			}
		}
    #endif
	}
	
    return(status);
}

/*******************************************************************************
* Function Name: SysTimers_GetTimerValue
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
uint32  SysTimers_GetTimerValue(uint32 timerID)
{
    uint32 timerValue;
    timerID--;
	timerID = timerID & SysTimers_TIMER_MASK;
	
    #if(SysTimers_TIMER_MODE == SysTimers_MODE_FAST_IRQ)
	    timerValue = SysTimers_SysTickCount - SysTimers_TimerSCount[timerID];
	#else
        timerValue = SysTimers_TimerSCount[timerID];
	#endif
	return(timerValue);
}

/*******************************************************************************
* Function Name: SysTimers_GetSysTickValue
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
uint32  SysTimers_GetSysTickValue(void)
{
    return(SysTimers_SysTickCount);
}

/*******************************************************************************
* Function Name: SysTimers_ResetTimer
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
void  SysTimers_ResetTimer(uint32 timerID, uint32 timerValue)
{
    timerID--;
	timerID = timerID & SysTimers_TIMER_MASK;
	if(timerValue != 0)  /* If timerValue not zero, load new period */
	{
	    SysTimers_TimerPeriod[timerID] = timerValue;
	}
	#if(SysTimers_TIMER_MODE == SysTimers_MODE_FAST_IRQ)
        SysTimers_TimerSCount[timerID] = SysTimers_SysTickCount;
	#else
	    SysTimers_TimerSCount[timerID] = SysTimers_TimerPeriod[timerID];
	#endif
	
	SysTimers_TimerStatus[timerID] &= SysTimers_ALARM_CLR_MASK;
}
/*******************************************************************************
* Function Name: SysTimers_ReleaseTimer
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
void  SysTimers_ReleaseTimer(uint32 timerID)
{
	timerID--;
	SysTimers_TimerStatus[timerID] = SysTimers_NOT_IN_USE;
	SysTimers_TimerSCount[timerID] = SysTimers_INVALID_PERIOD;
	SysTimers_TimerPeriod[timerID] = SysTimers_INVALID_PERIOD;
}
/*******************************************************************************
* Function Name: SysTimers_ReleaseAllTimers
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
void  SysTimers_ReleaseAllTimers(void)
{
    uint32 index;
	
	for(index = 1; index <= SysTimers_TIMERS; index++)
	{
		SysTimers_ReleaseTimer(index);
	}
}

/* [] END OF FILE */
