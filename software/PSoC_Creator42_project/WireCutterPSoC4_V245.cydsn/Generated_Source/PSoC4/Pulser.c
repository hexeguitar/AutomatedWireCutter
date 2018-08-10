/*******************************************************************************
* File Name: Pulser.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the Pulser
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

#include "Pulser.h"

uint8 Pulser_initVar = 0u;


/*******************************************************************************
* Function Name: Pulser_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default Pulser configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (Pulser__QUAD == Pulser_CONFIG)
        Pulser_CONTROL_REG = Pulser_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Pulser_TRIG_CONTROL1_REG  = Pulser_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        Pulser_SetInterruptMode(Pulser_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        Pulser_SetCounterMode(Pulser_COUNT_DOWN);
        Pulser_WritePeriod(Pulser_QUAD_PERIOD_INIT_VALUE);
        Pulser_WriteCounter(Pulser_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (Pulser__QUAD == Pulser_CONFIG) */

    #if (Pulser__TIMER == Pulser_CONFIG)
        Pulser_CONTROL_REG = Pulser_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Pulser_TRIG_CONTROL1_REG  = Pulser_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Pulser_SetInterruptMode(Pulser_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        Pulser_WritePeriod(Pulser_TC_PERIOD_VALUE );

        #if (Pulser__COMPARE == Pulser_TC_COMP_CAP_MODE)
            Pulser_WriteCompare(Pulser_TC_COMPARE_VALUE);

            #if (1u == Pulser_TC_COMPARE_SWAP)
                Pulser_SetCompareSwap(1u);
                Pulser_WriteCompareBuf(Pulser_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == Pulser_TC_COMPARE_SWAP) */
        #endif  /* (Pulser__COMPARE == Pulser_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (Pulser_CY_TCPWM_V2 && Pulser_TIMER_UPDOWN_CNT_USED && !Pulser_CY_TCPWM_4000)
            Pulser_WriteCounter(1u);
        #elif(Pulser__COUNT_DOWN == Pulser_TC_COUNTER_MODE)
            Pulser_WriteCounter(Pulser_TC_PERIOD_VALUE);
        #else
            Pulser_WriteCounter(0u);
        #endif /* (Pulser_CY_TCPWM_V2 && Pulser_TIMER_UPDOWN_CNT_USED && !Pulser_CY_TCPWM_4000) */
    #endif  /* (Pulser__TIMER == Pulser_CONFIG) */

    #if (Pulser__PWM_SEL == Pulser_CONFIG)
        Pulser_CONTROL_REG = Pulser_CTRL_PWM_BASE_CONFIG;

        #if (Pulser__PWM_PR == Pulser_PWM_MODE)
            Pulser_CONTROL_REG |= Pulser_CTRL_PWM_RUN_MODE;
            Pulser_WriteCounter(Pulser_PWM_PR_INIT_VALUE);
        #else
            Pulser_CONTROL_REG |= Pulser_CTRL_PWM_ALIGN | Pulser_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (Pulser_CY_TCPWM_V2 && Pulser_PWM_UPDOWN_CNT_USED && !Pulser_CY_TCPWM_4000)
                Pulser_WriteCounter(1u);
            #elif (Pulser__RIGHT == Pulser_PWM_ALIGN)
                Pulser_WriteCounter(Pulser_PWM_PERIOD_VALUE);
            #else 
                Pulser_WriteCounter(0u);
            #endif  /* (Pulser_CY_TCPWM_V2 && Pulser_PWM_UPDOWN_CNT_USED && !Pulser_CY_TCPWM_4000) */
        #endif  /* (Pulser__PWM_PR == Pulser_PWM_MODE) */

        #if (Pulser__PWM_DT == Pulser_PWM_MODE)
            Pulser_CONTROL_REG |= Pulser_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (Pulser__PWM_DT == Pulser_PWM_MODE) */

        #if (Pulser__PWM == Pulser_PWM_MODE)
            Pulser_CONTROL_REG |= Pulser_CTRL_PWM_PRESCALER;
        #endif  /* (Pulser__PWM == Pulser_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        Pulser_TRIG_CONTROL1_REG  = Pulser_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Pulser_SetInterruptMode(Pulser_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (Pulser__PWM_PR == Pulser_PWM_MODE)
            Pulser_TRIG_CONTROL2_REG =
                    (Pulser_CC_MATCH_NO_CHANGE    |
                    Pulser_OVERLOW_NO_CHANGE      |
                    Pulser_UNDERFLOW_NO_CHANGE);
        #else
            #if (Pulser__LEFT == Pulser_PWM_ALIGN)
                Pulser_TRIG_CONTROL2_REG = Pulser_PWM_MODE_LEFT;
            #endif  /* ( Pulser_PWM_LEFT == Pulser_PWM_ALIGN) */

            #if (Pulser__RIGHT == Pulser_PWM_ALIGN)
                Pulser_TRIG_CONTROL2_REG = Pulser_PWM_MODE_RIGHT;
            #endif  /* ( Pulser_PWM_RIGHT == Pulser_PWM_ALIGN) */

            #if (Pulser__CENTER == Pulser_PWM_ALIGN)
                Pulser_TRIG_CONTROL2_REG = Pulser_PWM_MODE_CENTER;
            #endif  /* ( Pulser_PWM_CENTER == Pulser_PWM_ALIGN) */

            #if (Pulser__ASYMMETRIC == Pulser_PWM_ALIGN)
                Pulser_TRIG_CONTROL2_REG = Pulser_PWM_MODE_ASYM;
            #endif  /* (Pulser__ASYMMETRIC == Pulser_PWM_ALIGN) */
        #endif  /* (Pulser__PWM_PR == Pulser_PWM_MODE) */

        /* Set other values from customizer */
        Pulser_WritePeriod(Pulser_PWM_PERIOD_VALUE );
        Pulser_WriteCompare(Pulser_PWM_COMPARE_VALUE);

        #if (1u == Pulser_PWM_COMPARE_SWAP)
            Pulser_SetCompareSwap(1u);
            Pulser_WriteCompareBuf(Pulser_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == Pulser_PWM_COMPARE_SWAP) */

        #if (1u == Pulser_PWM_PERIOD_SWAP)
            Pulser_SetPeriodSwap(1u);
            Pulser_WritePeriodBuf(Pulser_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == Pulser_PWM_PERIOD_SWAP) */
    #endif  /* (Pulser__PWM_SEL == Pulser_CONFIG) */
    
}


/*******************************************************************************
* Function Name: Pulser_Enable
********************************************************************************
*
* Summary:
*  Enables the Pulser.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    Pulser_BLOCK_CONTROL_REG |= Pulser_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (Pulser__PWM_SEL == Pulser_CONFIG)
        #if (0u == Pulser_PWM_START_SIGNAL_PRESENT)
            Pulser_TriggerCommand(Pulser_MASK, Pulser_CMD_START);
        #endif /* (0u == Pulser_PWM_START_SIGNAL_PRESENT) */
    #endif /* (Pulser__PWM_SEL == Pulser_CONFIG) */

    #if (Pulser__TIMER == Pulser_CONFIG)
        #if (0u == Pulser_TC_START_SIGNAL_PRESENT)
            Pulser_TriggerCommand(Pulser_MASK, Pulser_CMD_START);
        #endif /* (0u == Pulser_TC_START_SIGNAL_PRESENT) */
    #endif /* (Pulser__TIMER == Pulser_CONFIG) */
    
    #if (Pulser__QUAD == Pulser_CONFIG)
        #if (0u != Pulser_QUAD_AUTO_START)
            Pulser_TriggerCommand(Pulser_MASK, Pulser_CMD_RELOAD);
        #endif /* (0u != Pulser_QUAD_AUTO_START) */
    #endif  /* (Pulser__QUAD == Pulser_CONFIG) */
}


/*******************************************************************************
* Function Name: Pulser_Start
********************************************************************************
*
* Summary:
*  Initializes the Pulser with default customizer
*  values when called the first time and enables the Pulser.
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
*  Pulser_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time Pulser_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the Pulser_Start() routine.
*
*******************************************************************************/
void Pulser_Start(void)
{
    if (0u == Pulser_initVar)
    {
        Pulser_Init();
        Pulser_initVar = 1u;
    }

    Pulser_Enable();
}


/*******************************************************************************
* Function Name: Pulser_Stop
********************************************************************************
*
* Summary:
*  Disables the Pulser.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_BLOCK_CONTROL_REG &= (uint32)~Pulser_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the Pulser. This function is used when
*  configured as a generic Pulser and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the Pulser to operate in
*   Values:
*   - Pulser_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - Pulser_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - Pulser_MODE_QUAD - Quadrature decoder
*         - Pulser_MODE_PWM - PWM
*         - Pulser_MODE_PWM_DT - PWM with dead time
*         - Pulser_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_CONTROL_REG &= (uint32)~Pulser_MODE_MASK;
    Pulser_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - Pulser_MODE_X1 - Counts on phi 1 rising
*         - Pulser_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - Pulser_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_CONTROL_REG &= (uint32)~Pulser_QUAD_MODE_MASK;
    Pulser_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - Pulser_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - Pulser_PRESCALE_DIVBY2    - Divide by 2
*         - Pulser_PRESCALE_DIVBY4    - Divide by 4
*         - Pulser_PRESCALE_DIVBY8    - Divide by 8
*         - Pulser_PRESCALE_DIVBY16   - Divide by 16
*         - Pulser_PRESCALE_DIVBY32   - Divide by 32
*         - Pulser_PRESCALE_DIVBY64   - Divide by 64
*         - Pulser_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_CONTROL_REG &= (uint32)~Pulser_PRESCALER_MASK;
    Pulser_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the Pulser runs
*  continuously or stops when terminal count is reached.  By default the
*  Pulser operates in the continuous mode.
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
void Pulser_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_CONTROL_REG &= (uint32)~Pulser_ONESHOT_MASK;
    Pulser_CONTROL_REG |= ((uint32)((oneShotEnable & Pulser_1BIT_MASK) <<
                                                               Pulser_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_SetPWMMode
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
void Pulser_SetPWMMode(uint32 modeMask)
{
    Pulser_TRIG_CONTROL2_REG = (modeMask & Pulser_6BIT_MASK);
}



/*******************************************************************************
* Function Name: Pulser_SetPWMSyncKill
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
void Pulser_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_CONTROL_REG &= (uint32)~Pulser_PWM_SYNC_KILL_MASK;
    Pulser_CONTROL_REG |= ((uint32)((syncKillEnable & Pulser_1BIT_MASK)  <<
                                               Pulser_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_SetPWMStopOnKill
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
void Pulser_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_CONTROL_REG &= (uint32)~Pulser_PWM_STOP_KILL_MASK;
    Pulser_CONTROL_REG |= ((uint32)((stopOnKillEnable & Pulser_1BIT_MASK)  <<
                                                         Pulser_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_SetPWMDeadTime
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
void Pulser_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_CONTROL_REG &= (uint32)~Pulser_PRESCALER_MASK;
    Pulser_CONTROL_REG |= ((uint32)((deadTime & Pulser_8BIT_MASK) <<
                                                          Pulser_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_SetPWMInvert
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
*         - Pulser_INVERT_LINE   - Inverts the line output
*         - Pulser_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_CONTROL_REG &= (uint32)~Pulser_INV_OUT_MASK;
    Pulser_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: Pulser_WriteCounter
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
void Pulser_WriteCounter(uint32 count)
{
    Pulser_COUNTER_REG = (count & Pulser_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Pulser_ReadCounter
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
uint32 Pulser_ReadCounter(void)
{
    return (Pulser_COUNTER_REG & Pulser_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Pulser_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - Pulser_COUNT_UP       - Counts up
*     - Pulser_COUNT_DOWN     - Counts down
*     - Pulser_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - Pulser_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_CONTROL_REG &= (uint32)~Pulser_UPDOWN_MASK;
    Pulser_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_WritePeriod
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
void Pulser_WritePeriod(uint32 period)
{
    Pulser_PERIOD_REG = (period & Pulser_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Pulser_ReadPeriod
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
uint32 Pulser_ReadPeriod(void)
{
    return (Pulser_PERIOD_REG & Pulser_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Pulser_SetCompareSwap
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
void Pulser_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_CONTROL_REG &= (uint32)~Pulser_RELOAD_CC_MASK;
    Pulser_CONTROL_REG |= (swapEnable & Pulser_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_WritePeriodBuf
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
void Pulser_WritePeriodBuf(uint32 periodBuf)
{
    Pulser_PERIOD_BUF_REG = (periodBuf & Pulser_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Pulser_ReadPeriodBuf
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
uint32 Pulser_ReadPeriodBuf(void)
{
    return (Pulser_PERIOD_BUF_REG & Pulser_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Pulser_SetPeriodSwap
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
void Pulser_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_CONTROL_REG &= (uint32)~Pulser_RELOAD_PERIOD_MASK;
    Pulser_CONTROL_REG |= ((uint32)((swapEnable & Pulser_1BIT_MASK) <<
                                                            Pulser_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_WriteCompare
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
void Pulser_WriteCompare(uint32 compare)
{
    #if (Pulser_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Pulser_CY_TCPWM_4000) */

    #if (Pulser_CY_TCPWM_4000)
        currentMode = ((Pulser_CONTROL_REG & Pulser_UPDOWN_MASK) >> Pulser_UPDOWN_SHIFT);

        if (((uint32)Pulser__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)Pulser__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (Pulser_CY_TCPWM_4000) */
    
    Pulser_COMP_CAP_REG = (compare & Pulser_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Pulser_ReadCompare
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
uint32 Pulser_ReadCompare(void)
{
    #if (Pulser_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Pulser_CY_TCPWM_4000) */

    #if (Pulser_CY_TCPWM_4000)
        currentMode = ((Pulser_CONTROL_REG & Pulser_UPDOWN_MASK) >> Pulser_UPDOWN_SHIFT);
        
        regVal = Pulser_COMP_CAP_REG;
        
        if (((uint32)Pulser__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Pulser__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Pulser_16BIT_MASK);
    #else
        return (Pulser_COMP_CAP_REG & Pulser_16BIT_MASK);
    #endif /* (Pulser_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Pulser_WriteCompareBuf
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
void Pulser_WriteCompareBuf(uint32 compareBuf)
{
    #if (Pulser_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Pulser_CY_TCPWM_4000) */

    #if (Pulser_CY_TCPWM_4000)
        currentMode = ((Pulser_CONTROL_REG & Pulser_UPDOWN_MASK) >> Pulser_UPDOWN_SHIFT);

        if (((uint32)Pulser__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)Pulser__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (Pulser_CY_TCPWM_4000) */
    
    Pulser_COMP_CAP_BUF_REG = (compareBuf & Pulser_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Pulser_ReadCompareBuf
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
uint32 Pulser_ReadCompareBuf(void)
{
    #if (Pulser_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Pulser_CY_TCPWM_4000) */

    #if (Pulser_CY_TCPWM_4000)
        currentMode = ((Pulser_CONTROL_REG & Pulser_UPDOWN_MASK) >> Pulser_UPDOWN_SHIFT);

        regVal = Pulser_COMP_CAP_BUF_REG;
        
        if (((uint32)Pulser__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Pulser__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Pulser_16BIT_MASK);
    #else
        return (Pulser_COMP_CAP_BUF_REG & Pulser_16BIT_MASK);
    #endif /* (Pulser_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Pulser_ReadCapture
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
uint32 Pulser_ReadCapture(void)
{
    return (Pulser_COMP_CAP_REG & Pulser_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Pulser_ReadCaptureBuf
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
uint32 Pulser_ReadCaptureBuf(void)
{
    return (Pulser_COMP_CAP_BUF_REG & Pulser_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Pulser_SetCaptureMode
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
*     - Pulser_TRIG_LEVEL     - Level
*     - Pulser_TRIG_RISING    - Rising edge
*     - Pulser_TRIG_FALLING   - Falling edge
*     - Pulser_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_TRIG_CONTROL1_REG &= (uint32)~Pulser_CAPTURE_MASK;
    Pulser_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Pulser_TRIG_LEVEL     - Level
*     - Pulser_TRIG_RISING    - Rising edge
*     - Pulser_TRIG_FALLING   - Falling edge
*     - Pulser_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_TRIG_CONTROL1_REG &= (uint32)~Pulser_RELOAD_MASK;
    Pulser_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Pulser_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Pulser_TRIG_LEVEL     - Level
*     - Pulser_TRIG_RISING    - Rising edge
*     - Pulser_TRIG_FALLING   - Falling edge
*     - Pulser_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_TRIG_CONTROL1_REG &= (uint32)~Pulser_START_MASK;
    Pulser_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Pulser_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Pulser_TRIG_LEVEL     - Level
*     - Pulser_TRIG_RISING    - Rising edge
*     - Pulser_TRIG_FALLING   - Falling edge
*     - Pulser_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_TRIG_CONTROL1_REG &= (uint32)~Pulser_STOP_MASK;
    Pulser_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Pulser_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Pulser_TRIG_LEVEL     - Level
*     - Pulser_TRIG_RISING    - Rising edge
*     - Pulser_TRIG_FALLING   - Falling edge
*     - Pulser_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_TRIG_CONTROL1_REG &= (uint32)~Pulser_COUNT_MASK;
    Pulser_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Pulser_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_TriggerCommand
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
*     - Pulser_CMD_CAPTURE    - Trigger Capture/Switch command
*     - Pulser_CMD_RELOAD     - Trigger Reload/Index command
*     - Pulser_CMD_STOP       - Trigger Stop/Kill command
*     - Pulser_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Pulser_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pulser_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the Pulser.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - Pulser_STATUS_DOWN    - Set if counting down
*     - Pulser_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 Pulser_ReadStatus(void)
{
    return ((Pulser_STATUS_REG >> Pulser_RUNNING_STATUS_SHIFT) |
            (Pulser_STATUS_REG & Pulser_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: Pulser_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - Pulser_INTR_MASK_TC       - Terminal count mask
*     - Pulser_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_SetInterruptMode(uint32 interruptMask)
{
    Pulser_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: Pulser_GetInterruptSourceMasked
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
*     - Pulser_INTR_MASK_TC       - Terminal count mask
*     - Pulser_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Pulser_GetInterruptSourceMasked(void)
{
    return (Pulser_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: Pulser_GetInterruptSource
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
*     - Pulser_INTR_MASK_TC       - Terminal count mask
*     - Pulser_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Pulser_GetInterruptSource(void)
{
    return (Pulser_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: Pulser_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - Pulser_INTR_MASK_TC       - Terminal count mask
*     - Pulser_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_ClearInterrupt(uint32 interruptMask)
{
    Pulser_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: Pulser_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - Pulser_INTR_MASK_TC       - Terminal count mask
*     - Pulser_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Pulser_SetInterrupt(uint32 interruptMask)
{
    Pulser_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
