/*******************************************************************************
* File Name: WireSnsTimeout.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the WireSnsTimeout
*  component
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "WireSnsTimeout.h"

uint8 WireSnsTimeout_initVar = 0u;


/*******************************************************************************
* Function Name: WireSnsTimeout_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default WireSnsTimeout configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (WireSnsTimeout__QUAD == WireSnsTimeout_CONFIG)
        WireSnsTimeout_CONTROL_REG = WireSnsTimeout_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        WireSnsTimeout_TRIG_CONTROL1_REG  = WireSnsTimeout_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        WireSnsTimeout_SetInterruptMode(WireSnsTimeout_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        WireSnsTimeout_SetCounterMode(WireSnsTimeout_COUNT_DOWN);
        WireSnsTimeout_WritePeriod(WireSnsTimeout_QUAD_PERIOD_INIT_VALUE);
        WireSnsTimeout_WriteCounter(WireSnsTimeout_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (WireSnsTimeout__QUAD == WireSnsTimeout_CONFIG) */

    #if (WireSnsTimeout__TIMER == WireSnsTimeout_CONFIG)
        WireSnsTimeout_CONTROL_REG = WireSnsTimeout_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        WireSnsTimeout_TRIG_CONTROL1_REG  = WireSnsTimeout_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        WireSnsTimeout_SetInterruptMode(WireSnsTimeout_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        WireSnsTimeout_WritePeriod(WireSnsTimeout_TC_PERIOD_VALUE );

        #if (WireSnsTimeout__COMPARE == WireSnsTimeout_TC_COMP_CAP_MODE)
            WireSnsTimeout_WriteCompare(WireSnsTimeout_TC_COMPARE_VALUE);

            #if (1u == WireSnsTimeout_TC_COMPARE_SWAP)
                WireSnsTimeout_SetCompareSwap(1u);
                WireSnsTimeout_WriteCompareBuf(WireSnsTimeout_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == WireSnsTimeout_TC_COMPARE_SWAP) */
        #endif  /* (WireSnsTimeout__COMPARE == WireSnsTimeout_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (WireSnsTimeout_CY_TCPWM_V2 && WireSnsTimeout_TIMER_UPDOWN_CNT_USED && !WireSnsTimeout_CY_TCPWM_4000)
            WireSnsTimeout_WriteCounter(1u);
        #elif(WireSnsTimeout__COUNT_DOWN == WireSnsTimeout_TC_COUNTER_MODE)
            WireSnsTimeout_WriteCounter(WireSnsTimeout_TC_PERIOD_VALUE);
        #else
            WireSnsTimeout_WriteCounter(0u);
        #endif /* (WireSnsTimeout_CY_TCPWM_V2 && WireSnsTimeout_TIMER_UPDOWN_CNT_USED && !WireSnsTimeout_CY_TCPWM_4000) */
    #endif  /* (WireSnsTimeout__TIMER == WireSnsTimeout_CONFIG) */

    #if (WireSnsTimeout__PWM_SEL == WireSnsTimeout_CONFIG)
        WireSnsTimeout_CONTROL_REG = WireSnsTimeout_CTRL_PWM_BASE_CONFIG;

        #if (WireSnsTimeout__PWM_PR == WireSnsTimeout_PWM_MODE)
            WireSnsTimeout_CONTROL_REG |= WireSnsTimeout_CTRL_PWM_RUN_MODE;
            WireSnsTimeout_WriteCounter(WireSnsTimeout_PWM_PR_INIT_VALUE);
        #else
            WireSnsTimeout_CONTROL_REG |= WireSnsTimeout_CTRL_PWM_ALIGN | WireSnsTimeout_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (WireSnsTimeout_CY_TCPWM_V2 && WireSnsTimeout_PWM_UPDOWN_CNT_USED && !WireSnsTimeout_CY_TCPWM_4000)
                WireSnsTimeout_WriteCounter(1u);
            #elif (WireSnsTimeout__RIGHT == WireSnsTimeout_PWM_ALIGN)
                WireSnsTimeout_WriteCounter(WireSnsTimeout_PWM_PERIOD_VALUE);
            #else 
                WireSnsTimeout_WriteCounter(0u);
            #endif  /* (WireSnsTimeout_CY_TCPWM_V2 && WireSnsTimeout_PWM_UPDOWN_CNT_USED && !WireSnsTimeout_CY_TCPWM_4000) */
        #endif  /* (WireSnsTimeout__PWM_PR == WireSnsTimeout_PWM_MODE) */

        #if (WireSnsTimeout__PWM_DT == WireSnsTimeout_PWM_MODE)
            WireSnsTimeout_CONTROL_REG |= WireSnsTimeout_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (WireSnsTimeout__PWM_DT == WireSnsTimeout_PWM_MODE) */

        #if (WireSnsTimeout__PWM == WireSnsTimeout_PWM_MODE)
            WireSnsTimeout_CONTROL_REG |= WireSnsTimeout_CTRL_PWM_PRESCALER;
        #endif  /* (WireSnsTimeout__PWM == WireSnsTimeout_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        WireSnsTimeout_TRIG_CONTROL1_REG  = WireSnsTimeout_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        WireSnsTimeout_SetInterruptMode(WireSnsTimeout_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (WireSnsTimeout__PWM_PR == WireSnsTimeout_PWM_MODE)
            WireSnsTimeout_TRIG_CONTROL2_REG =
                    (WireSnsTimeout_CC_MATCH_NO_CHANGE    |
                    WireSnsTimeout_OVERLOW_NO_CHANGE      |
                    WireSnsTimeout_UNDERFLOW_NO_CHANGE);
        #else
            #if (WireSnsTimeout__LEFT == WireSnsTimeout_PWM_ALIGN)
                WireSnsTimeout_TRIG_CONTROL2_REG = WireSnsTimeout_PWM_MODE_LEFT;
            #endif  /* ( WireSnsTimeout_PWM_LEFT == WireSnsTimeout_PWM_ALIGN) */

            #if (WireSnsTimeout__RIGHT == WireSnsTimeout_PWM_ALIGN)
                WireSnsTimeout_TRIG_CONTROL2_REG = WireSnsTimeout_PWM_MODE_RIGHT;
            #endif  /* ( WireSnsTimeout_PWM_RIGHT == WireSnsTimeout_PWM_ALIGN) */

            #if (WireSnsTimeout__CENTER == WireSnsTimeout_PWM_ALIGN)
                WireSnsTimeout_TRIG_CONTROL2_REG = WireSnsTimeout_PWM_MODE_CENTER;
            #endif  /* ( WireSnsTimeout_PWM_CENTER == WireSnsTimeout_PWM_ALIGN) */

            #if (WireSnsTimeout__ASYMMETRIC == WireSnsTimeout_PWM_ALIGN)
                WireSnsTimeout_TRIG_CONTROL2_REG = WireSnsTimeout_PWM_MODE_ASYM;
            #endif  /* (WireSnsTimeout__ASYMMETRIC == WireSnsTimeout_PWM_ALIGN) */
        #endif  /* (WireSnsTimeout__PWM_PR == WireSnsTimeout_PWM_MODE) */

        /* Set other values from customizer */
        WireSnsTimeout_WritePeriod(WireSnsTimeout_PWM_PERIOD_VALUE );
        WireSnsTimeout_WriteCompare(WireSnsTimeout_PWM_COMPARE_VALUE);

        #if (1u == WireSnsTimeout_PWM_COMPARE_SWAP)
            WireSnsTimeout_SetCompareSwap(1u);
            WireSnsTimeout_WriteCompareBuf(WireSnsTimeout_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == WireSnsTimeout_PWM_COMPARE_SWAP) */

        #if (1u == WireSnsTimeout_PWM_PERIOD_SWAP)
            WireSnsTimeout_SetPeriodSwap(1u);
            WireSnsTimeout_WritePeriodBuf(WireSnsTimeout_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == WireSnsTimeout_PWM_PERIOD_SWAP) */
    #endif  /* (WireSnsTimeout__PWM_SEL == WireSnsTimeout_CONFIG) */
    
}


/*******************************************************************************
* Function Name: WireSnsTimeout_Enable
********************************************************************************
*
* Summary:
*  Enables the WireSnsTimeout.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    WireSnsTimeout_BLOCK_CONTROL_REG |= WireSnsTimeout_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (WireSnsTimeout__PWM_SEL == WireSnsTimeout_CONFIG)
        #if (0u == WireSnsTimeout_PWM_START_SIGNAL_PRESENT)
            WireSnsTimeout_TriggerCommand(WireSnsTimeout_MASK, WireSnsTimeout_CMD_START);
        #endif /* (0u == WireSnsTimeout_PWM_START_SIGNAL_PRESENT) */
    #endif /* (WireSnsTimeout__PWM_SEL == WireSnsTimeout_CONFIG) */

    #if (WireSnsTimeout__TIMER == WireSnsTimeout_CONFIG)
        #if (0u == WireSnsTimeout_TC_START_SIGNAL_PRESENT)
            WireSnsTimeout_TriggerCommand(WireSnsTimeout_MASK, WireSnsTimeout_CMD_START);
        #endif /* (0u == WireSnsTimeout_TC_START_SIGNAL_PRESENT) */
    #endif /* (WireSnsTimeout__TIMER == WireSnsTimeout_CONFIG) */
    
    #if (WireSnsTimeout__QUAD == WireSnsTimeout_CONFIG)
        #if (0u != WireSnsTimeout_QUAD_AUTO_START)
            WireSnsTimeout_TriggerCommand(WireSnsTimeout_MASK, WireSnsTimeout_CMD_RELOAD);
        #endif /* (0u != WireSnsTimeout_QUAD_AUTO_START) */
    #endif  /* (WireSnsTimeout__QUAD == WireSnsTimeout_CONFIG) */
}


/*******************************************************************************
* Function Name: WireSnsTimeout_Start
********************************************************************************
*
* Summary:
*  Initializes the WireSnsTimeout with default customizer
*  values when called the first time and enables the WireSnsTimeout.
*  For subsequent calls the configuration is left unchanged and the component is
*  just enabled.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  WireSnsTimeout_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time WireSnsTimeout_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the WireSnsTimeout_Start() routine.
*
*******************************************************************************/
void WireSnsTimeout_Start(void)
{
    if (0u == WireSnsTimeout_initVar)
    {
        WireSnsTimeout_Init();
        WireSnsTimeout_initVar = 1u;
    }

    WireSnsTimeout_Enable();
}


/*******************************************************************************
* Function Name: WireSnsTimeout_Stop
********************************************************************************
*
* Summary:
*  Disables the WireSnsTimeout.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_BLOCK_CONTROL_REG &= (uint32)~WireSnsTimeout_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the WireSnsTimeout. This function is used when
*  configured as a generic WireSnsTimeout and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the WireSnsTimeout to operate in
*   Values:
*   - WireSnsTimeout_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - WireSnsTimeout_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - WireSnsTimeout_MODE_QUAD - Quadrature decoder
*         - WireSnsTimeout_MODE_PWM - PWM
*         - WireSnsTimeout_MODE_PWM_DT - PWM with dead time
*         - WireSnsTimeout_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_CONTROL_REG &= (uint32)~WireSnsTimeout_MODE_MASK;
    WireSnsTimeout_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - WireSnsTimeout_MODE_X1 - Counts on phi 1 rising
*         - WireSnsTimeout_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - WireSnsTimeout_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_CONTROL_REG &= (uint32)~WireSnsTimeout_QUAD_MODE_MASK;
    WireSnsTimeout_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - WireSnsTimeout_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - WireSnsTimeout_PRESCALE_DIVBY2    - Divide by 2
*         - WireSnsTimeout_PRESCALE_DIVBY4    - Divide by 4
*         - WireSnsTimeout_PRESCALE_DIVBY8    - Divide by 8
*         - WireSnsTimeout_PRESCALE_DIVBY16   - Divide by 16
*         - WireSnsTimeout_PRESCALE_DIVBY32   - Divide by 32
*         - WireSnsTimeout_PRESCALE_DIVBY64   - Divide by 64
*         - WireSnsTimeout_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_CONTROL_REG &= (uint32)~WireSnsTimeout_PRESCALER_MASK;
    WireSnsTimeout_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the WireSnsTimeout runs
*  continuously or stops when terminal count is reached.  By default the
*  WireSnsTimeout operates in the continuous mode.
*
* Parameters:
*  oneShotEnable
*   Values:
*     - 0 - Continuous
*     - 1 - Enable One Shot
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_CONTROL_REG &= (uint32)~WireSnsTimeout_ONESHOT_MASK;
    WireSnsTimeout_CONTROL_REG |= ((uint32)((oneShotEnable & WireSnsTimeout_1BIT_MASK) <<
                                                               WireSnsTimeout_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetPWMMode
********************************************************************************
*
* Summary:
*  Writes the control register that determines what mode of operation the PWM
*  output lines are driven in.  There is a setting for what to do on a
*  comparison match (CC_MATCH), on an overflow (OVERFLOW) and on an underflow
*  (UNDERFLOW).  The value for each of the three must be ORed together to form
*  the mode.
*
* Parameters:
*  modeMask: A combination of three mode settings.  Mask must include a value
*  for each of the three or use one of the preconfigured PWM settings.
*   Values:
*     - CC_MATCH_SET        - Set on comparison match
*     - CC_MATCH_CLEAR      - Clear on comparison match
*     - CC_MATCH_INVERT     - Invert on comparison match
*     - CC_MATCH_NO_CHANGE  - No change on comparison match
*     - OVERLOW_SET         - Set on overflow
*     - OVERLOW_CLEAR       - Clear on  overflow
*     - OVERLOW_INVERT      - Invert on overflow
*     - OVERLOW_NO_CHANGE   - No change on overflow
*     - UNDERFLOW_SET       - Set on underflow
*     - UNDERFLOW_CLEAR     - Clear on underflow
*     - UNDERFLOW_INVERT    - Invert on underflow
*     - UNDERFLOW_NO_CHANGE - No change on underflow
*     - PWM_MODE_LEFT       - Setting for left aligned PWM.  Should be combined
*                             with up counting mode
*     - PWM_MODE_RIGHT      - Setting for right aligned PWM.  Should be combined
*                             with down counting mode
*     - PWM_MODE_CENTER     - Setting for center aligned PWM.  Should be
*                             combined with up/down 0 mode
*     - PWM_MODE_ASYM       - Setting for asymmetric PWM.  Should be combined
*                             with up/down 1 mode
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetPWMMode(uint32 modeMask)
{
    WireSnsTimeout_TRIG_CONTROL2_REG = (modeMask & WireSnsTimeout_6BIT_MASK);
}



/*******************************************************************************
* Function Name: WireSnsTimeout_SetPWMSyncKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes asynchronous or synchronous kill operation.  By default the kill
*  operation is asynchronous.  This functionality is only applicable to the PWM
*  and PWM with dead time modes.
*
*  For Synchronous mode the kill signal disables both the line and line_n
*  signals until the next terminal count.
*
*  For Asynchronous mode the kill signal disables both the line and line_n
*  signals when the kill signal is present.  This mode should only be used
*  when the kill signal (stop input) is configured in the pass through mode
*  (Level sensitive signal).

*
* Parameters:
*  syncKillEnable
*   Values:
*     - 0 - Asynchronous
*     - 1 - Synchronous
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_CONTROL_REG &= (uint32)~WireSnsTimeout_PWM_SYNC_KILL_MASK;
    WireSnsTimeout_CONTROL_REG |= ((uint32)((syncKillEnable & WireSnsTimeout_1BIT_MASK)  <<
                                               WireSnsTimeout_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetPWMStopOnKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes the PWM counter to stop.  By default the kill operation does not stop
*  the counter.  This functionality is only applicable to the three PWM modes.
*
*
* Parameters:
*  stopOnKillEnable
*   Values:
*     - 0 - Don't stop
*     - 1 - Stop
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_CONTROL_REG &= (uint32)~WireSnsTimeout_PWM_STOP_KILL_MASK;
    WireSnsTimeout_CONTROL_REG |= ((uint32)((stopOnKillEnable & WireSnsTimeout_1BIT_MASK)  <<
                                                         WireSnsTimeout_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetPWMDeadTime
********************************************************************************
*
* Summary:
*  Writes the dead time control value.  This value delays the rising edge of
*  both the line and line_n signals the designated number of cycles resulting
*  in both signals being inactive for that many cycles.  This functionality is
*  only applicable to the PWM in the dead time mode.

*
* Parameters:
*  Dead time to insert
*   Values: 0 to 255
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_CONTROL_REG &= (uint32)~WireSnsTimeout_PRESCALER_MASK;
    WireSnsTimeout_CONTROL_REG |= ((uint32)((deadTime & WireSnsTimeout_8BIT_MASK) <<
                                                          WireSnsTimeout_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetPWMInvert
********************************************************************************
*
* Summary:
*  Writes the bits that control whether the line and line_n outputs are
*  inverted from their normal output values.  This functionality is only
*  applicable to the three PWM modes.
*
* Parameters:
*  mask: Mask of outputs to invert.
*   Values:
*         - WireSnsTimeout_INVERT_LINE   - Inverts the line output
*         - WireSnsTimeout_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_CONTROL_REG &= (uint32)~WireSnsTimeout_INV_OUT_MASK;
    WireSnsTimeout_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: WireSnsTimeout_WriteCounter
********************************************************************************
*
* Summary:
*  Writes a new 16bit counter value directly into the counter register, thus
*  setting the counter (not the period) to the value written. It is not
*  advised to write to this field when the counter is running.
*
* Parameters:
*  count: value to write
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_WriteCounter(uint32 count)
{
    WireSnsTimeout_COUNTER_REG = (count & WireSnsTimeout_16BIT_MASK);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_ReadCounter
********************************************************************************
*
* Summary:
*  Reads the current counter value.
*
* Parameters:
*  None
*
* Return:
*  Current counter value
*
*******************************************************************************/
uint32 WireSnsTimeout_ReadCounter(void)
{
    return (WireSnsTimeout_COUNTER_REG & WireSnsTimeout_16BIT_MASK);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - WireSnsTimeout_COUNT_UP       - Counts up
*     - WireSnsTimeout_COUNT_DOWN     - Counts down
*     - WireSnsTimeout_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - WireSnsTimeout_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_CONTROL_REG &= (uint32)~WireSnsTimeout_UPDOWN_MASK;
    WireSnsTimeout_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_WritePeriod
********************************************************************************
*
* Summary:
*  Writes the 16 bit period register with the new period value.
*  To cause the counter to count for N cycles this register should be written
*  with N-1 (counts from 0 to period inclusive).
*
* Parameters:
*  period: Period value
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_WritePeriod(uint32 period)
{
    WireSnsTimeout_PERIOD_REG = (period & WireSnsTimeout_16BIT_MASK);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the 16 bit period register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 WireSnsTimeout_ReadPeriod(void)
{
    return (WireSnsTimeout_PERIOD_REG & WireSnsTimeout_16BIT_MASK);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetCompareSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the compare registers are
*  swapped. When enabled in the Timer/Counter mode(without capture) the swap
*  occurs at a TC event. In the PWM mode the swap occurs at the next TC event
*  following a hardware switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_CONTROL_REG &= (uint32)~WireSnsTimeout_RELOAD_CC_MASK;
    WireSnsTimeout_CONTROL_REG |= (swapEnable & WireSnsTimeout_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_WritePeriodBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit period buf register with the new period value.
*
* Parameters:
*  periodBuf: Period value
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_WritePeriodBuf(uint32 periodBuf)
{
    WireSnsTimeout_PERIOD_BUF_REG = (periodBuf & WireSnsTimeout_16BIT_MASK);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_ReadPeriodBuf
********************************************************************************
*
* Summary:
*  Reads the 16 bit period buf register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 WireSnsTimeout_ReadPeriodBuf(void)
{
    return (WireSnsTimeout_PERIOD_BUF_REG & WireSnsTimeout_16BIT_MASK);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetPeriodSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the period registers are
*  swapped. When enabled in Timer/Counter mode the swap occurs at a TC event.
*  In the PWM mode the swap occurs at the next TC event following a hardware
*  switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_CONTROL_REG &= (uint32)~WireSnsTimeout_RELOAD_PERIOD_MASK;
    WireSnsTimeout_CONTROL_REG |= ((uint32)((swapEnable & WireSnsTimeout_1BIT_MASK) <<
                                                            WireSnsTimeout_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_WriteCompare
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compare: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void WireSnsTimeout_WriteCompare(uint32 compare)
{
    #if (WireSnsTimeout_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (WireSnsTimeout_CY_TCPWM_4000) */

    #if (WireSnsTimeout_CY_TCPWM_4000)
        currentMode = ((WireSnsTimeout_CONTROL_REG & WireSnsTimeout_UPDOWN_MASK) >> WireSnsTimeout_UPDOWN_SHIFT);

        if (((uint32)WireSnsTimeout__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)WireSnsTimeout__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (WireSnsTimeout_CY_TCPWM_4000) */
    
    WireSnsTimeout_COMP_CAP_REG = (compare & WireSnsTimeout_16BIT_MASK);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_ReadCompare
********************************************************************************
*
* Summary:
*  Reads the compare register. Not applicable for Timer/Counter with Capture
*  or in Quadrature Decoder modes.
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
* Parameters:
*  None
*
* Return:
*  Compare value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 WireSnsTimeout_ReadCompare(void)
{
    #if (WireSnsTimeout_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (WireSnsTimeout_CY_TCPWM_4000) */

    #if (WireSnsTimeout_CY_TCPWM_4000)
        currentMode = ((WireSnsTimeout_CONTROL_REG & WireSnsTimeout_UPDOWN_MASK) >> WireSnsTimeout_UPDOWN_SHIFT);
        
        regVal = WireSnsTimeout_COMP_CAP_REG;
        
        if (((uint32)WireSnsTimeout__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)WireSnsTimeout__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & WireSnsTimeout_16BIT_MASK);
    #else
        return (WireSnsTimeout_COMP_CAP_REG & WireSnsTimeout_16BIT_MASK);
    #endif /* (WireSnsTimeout_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: WireSnsTimeout_WriteCompareBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare buffer register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compareBuf: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void WireSnsTimeout_WriteCompareBuf(uint32 compareBuf)
{
    #if (WireSnsTimeout_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (WireSnsTimeout_CY_TCPWM_4000) */

    #if (WireSnsTimeout_CY_TCPWM_4000)
        currentMode = ((WireSnsTimeout_CONTROL_REG & WireSnsTimeout_UPDOWN_MASK) >> WireSnsTimeout_UPDOWN_SHIFT);

        if (((uint32)WireSnsTimeout__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)WireSnsTimeout__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (WireSnsTimeout_CY_TCPWM_4000) */
    
    WireSnsTimeout_COMP_CAP_BUF_REG = (compareBuf & WireSnsTimeout_16BIT_MASK);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_ReadCompareBuf
********************************************************************************
*
* Summary:
*  Reads the compare buffer register. Not applicable for Timer/Counter with
*  Capture or in Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Compare buffer value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 WireSnsTimeout_ReadCompareBuf(void)
{
    #if (WireSnsTimeout_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (WireSnsTimeout_CY_TCPWM_4000) */

    #if (WireSnsTimeout_CY_TCPWM_4000)
        currentMode = ((WireSnsTimeout_CONTROL_REG & WireSnsTimeout_UPDOWN_MASK) >> WireSnsTimeout_UPDOWN_SHIFT);

        regVal = WireSnsTimeout_COMP_CAP_BUF_REG;
        
        if (((uint32)WireSnsTimeout__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)WireSnsTimeout__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & WireSnsTimeout_16BIT_MASK);
    #else
        return (WireSnsTimeout_COMP_CAP_BUF_REG & WireSnsTimeout_16BIT_MASK);
    #endif /* (WireSnsTimeout_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: WireSnsTimeout_ReadCapture
********************************************************************************
*
* Summary:
*  Reads the captured counter value. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture value
*
*******************************************************************************/
uint32 WireSnsTimeout_ReadCapture(void)
{
    return (WireSnsTimeout_COMP_CAP_REG & WireSnsTimeout_16BIT_MASK);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_ReadCaptureBuf
********************************************************************************
*
* Summary:
*  Reads the capture buffer register. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture buffer value
*
*******************************************************************************/
uint32 WireSnsTimeout_ReadCaptureBuf(void)
{
    return (WireSnsTimeout_COMP_CAP_BUF_REG & WireSnsTimeout_16BIT_MASK);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetCaptureMode
********************************************************************************
*
* Summary:
*  Sets the capture trigger mode. For PWM mode this is the switch input.
*  This input is not applicable to the Timer/Counter without Capture and
*  Quadrature Decoder modes.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - WireSnsTimeout_TRIG_LEVEL     - Level
*     - WireSnsTimeout_TRIG_RISING    - Rising edge
*     - WireSnsTimeout_TRIG_FALLING   - Falling edge
*     - WireSnsTimeout_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_TRIG_CONTROL1_REG &= (uint32)~WireSnsTimeout_CAPTURE_MASK;
    WireSnsTimeout_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - WireSnsTimeout_TRIG_LEVEL     - Level
*     - WireSnsTimeout_TRIG_RISING    - Rising edge
*     - WireSnsTimeout_TRIG_FALLING   - Falling edge
*     - WireSnsTimeout_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_TRIG_CONTROL1_REG &= (uint32)~WireSnsTimeout_RELOAD_MASK;
    WireSnsTimeout_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << WireSnsTimeout_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - WireSnsTimeout_TRIG_LEVEL     - Level
*     - WireSnsTimeout_TRIG_RISING    - Rising edge
*     - WireSnsTimeout_TRIG_FALLING   - Falling edge
*     - WireSnsTimeout_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_TRIG_CONTROL1_REG &= (uint32)~WireSnsTimeout_START_MASK;
    WireSnsTimeout_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << WireSnsTimeout_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - WireSnsTimeout_TRIG_LEVEL     - Level
*     - WireSnsTimeout_TRIG_RISING    - Rising edge
*     - WireSnsTimeout_TRIG_FALLING   - Falling edge
*     - WireSnsTimeout_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_TRIG_CONTROL1_REG &= (uint32)~WireSnsTimeout_STOP_MASK;
    WireSnsTimeout_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << WireSnsTimeout_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - WireSnsTimeout_TRIG_LEVEL     - Level
*     - WireSnsTimeout_TRIG_RISING    - Rising edge
*     - WireSnsTimeout_TRIG_FALLING   - Falling edge
*     - WireSnsTimeout_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_TRIG_CONTROL1_REG &= (uint32)~WireSnsTimeout_COUNT_MASK;
    WireSnsTimeout_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << WireSnsTimeout_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_TriggerCommand
********************************************************************************
*
* Summary:
*  Triggers the designated command to occur on the designated TCPWM instances.
*  The mask can be used to apply this command simultaneously to more than one
*  instance.  This allows multiple TCPWM instances to be synchronized.
*
* Parameters:
*  mask: A combination of mask bits for each instance of the TCPWM that the
*        command should apply to.  This function from one instance can be used
*        to apply the command to any of the instances in the design.
*        The mask value for a specific instance is available with the MASK
*        define.
*  command: Enumerated command values. Capture command only applicable for
*           Timer/Counter with Capture and PWM modes.
*   Values:
*     - WireSnsTimeout_CMD_CAPTURE    - Trigger Capture/Switch command
*     - WireSnsTimeout_CMD_RELOAD     - Trigger Reload/Index command
*     - WireSnsTimeout_CMD_STOP       - Trigger Stop/Kill command
*     - WireSnsTimeout_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    WireSnsTimeout_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the WireSnsTimeout.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - WireSnsTimeout_STATUS_DOWN    - Set if counting down
*     - WireSnsTimeout_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 WireSnsTimeout_ReadStatus(void)
{
    return ((WireSnsTimeout_STATUS_REG >> WireSnsTimeout_RUNNING_STATUS_SHIFT) |
            (WireSnsTimeout_STATUS_REG & WireSnsTimeout_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - WireSnsTimeout_INTR_MASK_TC       - Terminal count mask
*     - WireSnsTimeout_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetInterruptMode(uint32 interruptMask)
{
    WireSnsTimeout_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: WireSnsTimeout_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Gets the interrupt requests masked by the interrupt mask.
*
* Parameters:
*   None
*
* Return:
*  Masked interrupt source
*   Values:
*     - WireSnsTimeout_INTR_MASK_TC       - Terminal count mask
*     - WireSnsTimeout_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 WireSnsTimeout_GetInterruptSourceMasked(void)
{
    return (WireSnsTimeout_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the interrupt requests (without masking).
*
* Parameters:
*  None
*
* Return:
*  Interrupt request value
*   Values:
*     - WireSnsTimeout_INTR_MASK_TC       - Terminal count mask
*     - WireSnsTimeout_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 WireSnsTimeout_GetInterruptSource(void)
{
    return (WireSnsTimeout_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: WireSnsTimeout_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - WireSnsTimeout_INTR_MASK_TC       - Terminal count mask
*     - WireSnsTimeout_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_ClearInterrupt(uint32 interruptMask)
{
    WireSnsTimeout_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: WireSnsTimeout_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - WireSnsTimeout_INTR_MASK_TC       - Terminal count mask
*     - WireSnsTimeout_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void WireSnsTimeout_SetInterrupt(uint32 interruptMask)
{
    WireSnsTimeout_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
