/*******************************************************************************
* File Name: Beep.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the Beep
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

#include "Beep.h"

uint8 Beep_initVar = 0u;


/*******************************************************************************
* Function Name: Beep_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default Beep configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Beep_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (Beep__QUAD == Beep_CONFIG)
        Beep_CONTROL_REG = Beep_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Beep_TRIG_CONTROL1_REG  = Beep_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        Beep_SetInterruptMode(Beep_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        Beep_SetCounterMode(Beep_COUNT_DOWN);
        Beep_WritePeriod(Beep_QUAD_PERIOD_INIT_VALUE);
        Beep_WriteCounter(Beep_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (Beep__QUAD == Beep_CONFIG) */

    #if (Beep__TIMER == Beep_CONFIG)
        Beep_CONTROL_REG = Beep_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Beep_TRIG_CONTROL1_REG  = Beep_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Beep_SetInterruptMode(Beep_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        Beep_WritePeriod(Beep_TC_PERIOD_VALUE );

        #if (Beep__COMPARE == Beep_TC_COMP_CAP_MODE)
            Beep_WriteCompare(Beep_TC_COMPARE_VALUE);

            #if (1u == Beep_TC_COMPARE_SWAP)
                Beep_SetCompareSwap(1u);
                Beep_WriteCompareBuf(Beep_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == Beep_TC_COMPARE_SWAP) */
        #endif  /* (Beep__COMPARE == Beep_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (Beep_CY_TCPWM_V2 && Beep_TIMER_UPDOWN_CNT_USED && !Beep_CY_TCPWM_4000)
            Beep_WriteCounter(1u);
        #elif(Beep__COUNT_DOWN == Beep_TC_COUNTER_MODE)
            Beep_WriteCounter(Beep_TC_PERIOD_VALUE);
        #else
            Beep_WriteCounter(0u);
        #endif /* (Beep_CY_TCPWM_V2 && Beep_TIMER_UPDOWN_CNT_USED && !Beep_CY_TCPWM_4000) */
    #endif  /* (Beep__TIMER == Beep_CONFIG) */

    #if (Beep__PWM_SEL == Beep_CONFIG)
        Beep_CONTROL_REG = Beep_CTRL_PWM_BASE_CONFIG;

        #if (Beep__PWM_PR == Beep_PWM_MODE)
            Beep_CONTROL_REG |= Beep_CTRL_PWM_RUN_MODE;
            Beep_WriteCounter(Beep_PWM_PR_INIT_VALUE);
        #else
            Beep_CONTROL_REG |= Beep_CTRL_PWM_ALIGN | Beep_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (Beep_CY_TCPWM_V2 && Beep_PWM_UPDOWN_CNT_USED && !Beep_CY_TCPWM_4000)
                Beep_WriteCounter(1u);
            #elif (Beep__RIGHT == Beep_PWM_ALIGN)
                Beep_WriteCounter(Beep_PWM_PERIOD_VALUE);
            #else 
                Beep_WriteCounter(0u);
            #endif  /* (Beep_CY_TCPWM_V2 && Beep_PWM_UPDOWN_CNT_USED && !Beep_CY_TCPWM_4000) */
        #endif  /* (Beep__PWM_PR == Beep_PWM_MODE) */

        #if (Beep__PWM_DT == Beep_PWM_MODE)
            Beep_CONTROL_REG |= Beep_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (Beep__PWM_DT == Beep_PWM_MODE) */

        #if (Beep__PWM == Beep_PWM_MODE)
            Beep_CONTROL_REG |= Beep_CTRL_PWM_PRESCALER;
        #endif  /* (Beep__PWM == Beep_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        Beep_TRIG_CONTROL1_REG  = Beep_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Beep_SetInterruptMode(Beep_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (Beep__PWM_PR == Beep_PWM_MODE)
            Beep_TRIG_CONTROL2_REG =
                    (Beep_CC_MATCH_NO_CHANGE    |
                    Beep_OVERLOW_NO_CHANGE      |
                    Beep_UNDERFLOW_NO_CHANGE);
        #else
            #if (Beep__LEFT == Beep_PWM_ALIGN)
                Beep_TRIG_CONTROL2_REG = Beep_PWM_MODE_LEFT;
            #endif  /* ( Beep_PWM_LEFT == Beep_PWM_ALIGN) */

            #if (Beep__RIGHT == Beep_PWM_ALIGN)
                Beep_TRIG_CONTROL2_REG = Beep_PWM_MODE_RIGHT;
            #endif  /* ( Beep_PWM_RIGHT == Beep_PWM_ALIGN) */

            #if (Beep__CENTER == Beep_PWM_ALIGN)
                Beep_TRIG_CONTROL2_REG = Beep_PWM_MODE_CENTER;
            #endif  /* ( Beep_PWM_CENTER == Beep_PWM_ALIGN) */

            #if (Beep__ASYMMETRIC == Beep_PWM_ALIGN)
                Beep_TRIG_CONTROL2_REG = Beep_PWM_MODE_ASYM;
            #endif  /* (Beep__ASYMMETRIC == Beep_PWM_ALIGN) */
        #endif  /* (Beep__PWM_PR == Beep_PWM_MODE) */

        /* Set other values from customizer */
        Beep_WritePeriod(Beep_PWM_PERIOD_VALUE );
        Beep_WriteCompare(Beep_PWM_COMPARE_VALUE);

        #if (1u == Beep_PWM_COMPARE_SWAP)
            Beep_SetCompareSwap(1u);
            Beep_WriteCompareBuf(Beep_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == Beep_PWM_COMPARE_SWAP) */

        #if (1u == Beep_PWM_PERIOD_SWAP)
            Beep_SetPeriodSwap(1u);
            Beep_WritePeriodBuf(Beep_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == Beep_PWM_PERIOD_SWAP) */
    #endif  /* (Beep__PWM_SEL == Beep_CONFIG) */
    
}


/*******************************************************************************
* Function Name: Beep_Enable
********************************************************************************
*
* Summary:
*  Enables the Beep.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Beep_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    Beep_BLOCK_CONTROL_REG |= Beep_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (Beep__PWM_SEL == Beep_CONFIG)
        #if (0u == Beep_PWM_START_SIGNAL_PRESENT)
            Beep_TriggerCommand(Beep_MASK, Beep_CMD_START);
        #endif /* (0u == Beep_PWM_START_SIGNAL_PRESENT) */
    #endif /* (Beep__PWM_SEL == Beep_CONFIG) */

    #if (Beep__TIMER == Beep_CONFIG)
        #if (0u == Beep_TC_START_SIGNAL_PRESENT)
            Beep_TriggerCommand(Beep_MASK, Beep_CMD_START);
        #endif /* (0u == Beep_TC_START_SIGNAL_PRESENT) */
    #endif /* (Beep__TIMER == Beep_CONFIG) */
    
    #if (Beep__QUAD == Beep_CONFIG)
        #if (0u != Beep_QUAD_AUTO_START)
            Beep_TriggerCommand(Beep_MASK, Beep_CMD_RELOAD);
        #endif /* (0u != Beep_QUAD_AUTO_START) */
    #endif  /* (Beep__QUAD == Beep_CONFIG) */
}


/*******************************************************************************
* Function Name: Beep_Start
********************************************************************************
*
* Summary:
*  Initializes the Beep with default customizer
*  values when called the first time and enables the Beep.
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
*  Beep_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time Beep_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the Beep_Start() routine.
*
*******************************************************************************/
void Beep_Start(void)
{
    if (0u == Beep_initVar)
    {
        Beep_Init();
        Beep_initVar = 1u;
    }

    Beep_Enable();
}


/*******************************************************************************
* Function Name: Beep_Stop
********************************************************************************
*
* Summary:
*  Disables the Beep.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Beep_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_BLOCK_CONTROL_REG &= (uint32)~Beep_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the Beep. This function is used when
*  configured as a generic Beep and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the Beep to operate in
*   Values:
*   - Beep_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - Beep_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - Beep_MODE_QUAD - Quadrature decoder
*         - Beep_MODE_PWM - PWM
*         - Beep_MODE_PWM_DT - PWM with dead time
*         - Beep_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_CONTROL_REG &= (uint32)~Beep_MODE_MASK;
    Beep_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - Beep_MODE_X1 - Counts on phi 1 rising
*         - Beep_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - Beep_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_CONTROL_REG &= (uint32)~Beep_QUAD_MODE_MASK;
    Beep_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - Beep_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - Beep_PRESCALE_DIVBY2    - Divide by 2
*         - Beep_PRESCALE_DIVBY4    - Divide by 4
*         - Beep_PRESCALE_DIVBY8    - Divide by 8
*         - Beep_PRESCALE_DIVBY16   - Divide by 16
*         - Beep_PRESCALE_DIVBY32   - Divide by 32
*         - Beep_PRESCALE_DIVBY64   - Divide by 64
*         - Beep_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_CONTROL_REG &= (uint32)~Beep_PRESCALER_MASK;
    Beep_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the Beep runs
*  continuously or stops when terminal count is reached.  By default the
*  Beep operates in the continuous mode.
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
void Beep_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_CONTROL_REG &= (uint32)~Beep_ONESHOT_MASK;
    Beep_CONTROL_REG |= ((uint32)((oneShotEnable & Beep_1BIT_MASK) <<
                                                               Beep_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_SetPWMMode
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
void Beep_SetPWMMode(uint32 modeMask)
{
    Beep_TRIG_CONTROL2_REG = (modeMask & Beep_6BIT_MASK);
}



/*******************************************************************************
* Function Name: Beep_SetPWMSyncKill
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
void Beep_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_CONTROL_REG &= (uint32)~Beep_PWM_SYNC_KILL_MASK;
    Beep_CONTROL_REG |= ((uint32)((syncKillEnable & Beep_1BIT_MASK)  <<
                                               Beep_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_SetPWMStopOnKill
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
void Beep_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_CONTROL_REG &= (uint32)~Beep_PWM_STOP_KILL_MASK;
    Beep_CONTROL_REG |= ((uint32)((stopOnKillEnable & Beep_1BIT_MASK)  <<
                                                         Beep_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_SetPWMDeadTime
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
void Beep_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_CONTROL_REG &= (uint32)~Beep_PRESCALER_MASK;
    Beep_CONTROL_REG |= ((uint32)((deadTime & Beep_8BIT_MASK) <<
                                                          Beep_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_SetPWMInvert
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
*         - Beep_INVERT_LINE   - Inverts the line output
*         - Beep_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_CONTROL_REG &= (uint32)~Beep_INV_OUT_MASK;
    Beep_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: Beep_WriteCounter
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
void Beep_WriteCounter(uint32 count)
{
    Beep_COUNTER_REG = (count & Beep_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Beep_ReadCounter
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
uint32 Beep_ReadCounter(void)
{
    return (Beep_COUNTER_REG & Beep_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Beep_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - Beep_COUNT_UP       - Counts up
*     - Beep_COUNT_DOWN     - Counts down
*     - Beep_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - Beep_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_CONTROL_REG &= (uint32)~Beep_UPDOWN_MASK;
    Beep_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_WritePeriod
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
void Beep_WritePeriod(uint32 period)
{
    Beep_PERIOD_REG = (period & Beep_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Beep_ReadPeriod
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
uint32 Beep_ReadPeriod(void)
{
    return (Beep_PERIOD_REG & Beep_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Beep_SetCompareSwap
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
void Beep_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_CONTROL_REG &= (uint32)~Beep_RELOAD_CC_MASK;
    Beep_CONTROL_REG |= (swapEnable & Beep_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_WritePeriodBuf
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
void Beep_WritePeriodBuf(uint32 periodBuf)
{
    Beep_PERIOD_BUF_REG = (periodBuf & Beep_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Beep_ReadPeriodBuf
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
uint32 Beep_ReadPeriodBuf(void)
{
    return (Beep_PERIOD_BUF_REG & Beep_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Beep_SetPeriodSwap
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
void Beep_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_CONTROL_REG &= (uint32)~Beep_RELOAD_PERIOD_MASK;
    Beep_CONTROL_REG |= ((uint32)((swapEnable & Beep_1BIT_MASK) <<
                                                            Beep_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_WriteCompare
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
void Beep_WriteCompare(uint32 compare)
{
    #if (Beep_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Beep_CY_TCPWM_4000) */

    #if (Beep_CY_TCPWM_4000)
        currentMode = ((Beep_CONTROL_REG & Beep_UPDOWN_MASK) >> Beep_UPDOWN_SHIFT);

        if (((uint32)Beep__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)Beep__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (Beep_CY_TCPWM_4000) */
    
    Beep_COMP_CAP_REG = (compare & Beep_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Beep_ReadCompare
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
uint32 Beep_ReadCompare(void)
{
    #if (Beep_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Beep_CY_TCPWM_4000) */

    #if (Beep_CY_TCPWM_4000)
        currentMode = ((Beep_CONTROL_REG & Beep_UPDOWN_MASK) >> Beep_UPDOWN_SHIFT);
        
        regVal = Beep_COMP_CAP_REG;
        
        if (((uint32)Beep__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Beep__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Beep_16BIT_MASK);
    #else
        return (Beep_COMP_CAP_REG & Beep_16BIT_MASK);
    #endif /* (Beep_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Beep_WriteCompareBuf
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
void Beep_WriteCompareBuf(uint32 compareBuf)
{
    #if (Beep_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Beep_CY_TCPWM_4000) */

    #if (Beep_CY_TCPWM_4000)
        currentMode = ((Beep_CONTROL_REG & Beep_UPDOWN_MASK) >> Beep_UPDOWN_SHIFT);

        if (((uint32)Beep__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)Beep__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (Beep_CY_TCPWM_4000) */
    
    Beep_COMP_CAP_BUF_REG = (compareBuf & Beep_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Beep_ReadCompareBuf
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
uint32 Beep_ReadCompareBuf(void)
{
    #if (Beep_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Beep_CY_TCPWM_4000) */

    #if (Beep_CY_TCPWM_4000)
        currentMode = ((Beep_CONTROL_REG & Beep_UPDOWN_MASK) >> Beep_UPDOWN_SHIFT);

        regVal = Beep_COMP_CAP_BUF_REG;
        
        if (((uint32)Beep__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Beep__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Beep_16BIT_MASK);
    #else
        return (Beep_COMP_CAP_BUF_REG & Beep_16BIT_MASK);
    #endif /* (Beep_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Beep_ReadCapture
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
uint32 Beep_ReadCapture(void)
{
    return (Beep_COMP_CAP_REG & Beep_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Beep_ReadCaptureBuf
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
uint32 Beep_ReadCaptureBuf(void)
{
    return (Beep_COMP_CAP_BUF_REG & Beep_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Beep_SetCaptureMode
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
*     - Beep_TRIG_LEVEL     - Level
*     - Beep_TRIG_RISING    - Rising edge
*     - Beep_TRIG_FALLING   - Falling edge
*     - Beep_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_TRIG_CONTROL1_REG &= (uint32)~Beep_CAPTURE_MASK;
    Beep_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Beep_TRIG_LEVEL     - Level
*     - Beep_TRIG_RISING    - Rising edge
*     - Beep_TRIG_FALLING   - Falling edge
*     - Beep_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_TRIG_CONTROL1_REG &= (uint32)~Beep_RELOAD_MASK;
    Beep_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Beep_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Beep_TRIG_LEVEL     - Level
*     - Beep_TRIG_RISING    - Rising edge
*     - Beep_TRIG_FALLING   - Falling edge
*     - Beep_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_TRIG_CONTROL1_REG &= (uint32)~Beep_START_MASK;
    Beep_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Beep_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Beep_TRIG_LEVEL     - Level
*     - Beep_TRIG_RISING    - Rising edge
*     - Beep_TRIG_FALLING   - Falling edge
*     - Beep_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_TRIG_CONTROL1_REG &= (uint32)~Beep_STOP_MASK;
    Beep_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Beep_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Beep_TRIG_LEVEL     - Level
*     - Beep_TRIG_RISING    - Rising edge
*     - Beep_TRIG_FALLING   - Falling edge
*     - Beep_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_TRIG_CONTROL1_REG &= (uint32)~Beep_COUNT_MASK;
    Beep_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Beep_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_TriggerCommand
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
*     - Beep_CMD_CAPTURE    - Trigger Capture/Switch command
*     - Beep_CMD_RELOAD     - Trigger Reload/Index command
*     - Beep_CMD_STOP       - Trigger Stop/Kill command
*     - Beep_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void Beep_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Beep_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Beep_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the Beep.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - Beep_STATUS_DOWN    - Set if counting down
*     - Beep_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 Beep_ReadStatus(void)
{
    return ((Beep_STATUS_REG >> Beep_RUNNING_STATUS_SHIFT) |
            (Beep_STATUS_REG & Beep_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: Beep_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - Beep_INTR_MASK_TC       - Terminal count mask
*     - Beep_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SetInterruptMode(uint32 interruptMask)
{
    Beep_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: Beep_GetInterruptSourceMasked
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
*     - Beep_INTR_MASK_TC       - Terminal count mask
*     - Beep_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Beep_GetInterruptSourceMasked(void)
{
    return (Beep_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: Beep_GetInterruptSource
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
*     - Beep_INTR_MASK_TC       - Terminal count mask
*     - Beep_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Beep_GetInterruptSource(void)
{
    return (Beep_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: Beep_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - Beep_INTR_MASK_TC       - Terminal count mask
*     - Beep_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Beep_ClearInterrupt(uint32 interruptMask)
{
    Beep_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: Beep_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - Beep_INTR_MASK_TC       - Terminal count mask
*     - Beep_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SetInterrupt(uint32 interruptMask)
{
    Beep_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
