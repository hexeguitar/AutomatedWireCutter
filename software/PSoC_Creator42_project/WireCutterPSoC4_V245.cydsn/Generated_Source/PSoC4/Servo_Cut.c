/*******************************************************************************
* File Name: Servo_Cut.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the Servo_Cut
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

#include "Servo_Cut.h"

uint8 Servo_Cut_initVar = 0u;


/*******************************************************************************
* Function Name: Servo_Cut_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default Servo_Cut configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (Servo_Cut__QUAD == Servo_Cut_CONFIG)
        Servo_Cut_CONTROL_REG = Servo_Cut_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Servo_Cut_TRIG_CONTROL1_REG  = Servo_Cut_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        Servo_Cut_SetInterruptMode(Servo_Cut_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        Servo_Cut_SetCounterMode(Servo_Cut_COUNT_DOWN);
        Servo_Cut_WritePeriod(Servo_Cut_QUAD_PERIOD_INIT_VALUE);
        Servo_Cut_WriteCounter(Servo_Cut_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (Servo_Cut__QUAD == Servo_Cut_CONFIG) */

    #if (Servo_Cut__TIMER == Servo_Cut_CONFIG)
        Servo_Cut_CONTROL_REG = Servo_Cut_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Servo_Cut_TRIG_CONTROL1_REG  = Servo_Cut_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Servo_Cut_SetInterruptMode(Servo_Cut_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        Servo_Cut_WritePeriod(Servo_Cut_TC_PERIOD_VALUE );

        #if (Servo_Cut__COMPARE == Servo_Cut_TC_COMP_CAP_MODE)
            Servo_Cut_WriteCompare(Servo_Cut_TC_COMPARE_VALUE);

            #if (1u == Servo_Cut_TC_COMPARE_SWAP)
                Servo_Cut_SetCompareSwap(1u);
                Servo_Cut_WriteCompareBuf(Servo_Cut_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == Servo_Cut_TC_COMPARE_SWAP) */
        #endif  /* (Servo_Cut__COMPARE == Servo_Cut_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (Servo_Cut_CY_TCPWM_V2 && Servo_Cut_TIMER_UPDOWN_CNT_USED && !Servo_Cut_CY_TCPWM_4000)
            Servo_Cut_WriteCounter(1u);
        #elif(Servo_Cut__COUNT_DOWN == Servo_Cut_TC_COUNTER_MODE)
            Servo_Cut_WriteCounter(Servo_Cut_TC_PERIOD_VALUE);
        #else
            Servo_Cut_WriteCounter(0u);
        #endif /* (Servo_Cut_CY_TCPWM_V2 && Servo_Cut_TIMER_UPDOWN_CNT_USED && !Servo_Cut_CY_TCPWM_4000) */
    #endif  /* (Servo_Cut__TIMER == Servo_Cut_CONFIG) */

    #if (Servo_Cut__PWM_SEL == Servo_Cut_CONFIG)
        Servo_Cut_CONTROL_REG = Servo_Cut_CTRL_PWM_BASE_CONFIG;

        #if (Servo_Cut__PWM_PR == Servo_Cut_PWM_MODE)
            Servo_Cut_CONTROL_REG |= Servo_Cut_CTRL_PWM_RUN_MODE;
            Servo_Cut_WriteCounter(Servo_Cut_PWM_PR_INIT_VALUE);
        #else
            Servo_Cut_CONTROL_REG |= Servo_Cut_CTRL_PWM_ALIGN | Servo_Cut_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (Servo_Cut_CY_TCPWM_V2 && Servo_Cut_PWM_UPDOWN_CNT_USED && !Servo_Cut_CY_TCPWM_4000)
                Servo_Cut_WriteCounter(1u);
            #elif (Servo_Cut__RIGHT == Servo_Cut_PWM_ALIGN)
                Servo_Cut_WriteCounter(Servo_Cut_PWM_PERIOD_VALUE);
            #else 
                Servo_Cut_WriteCounter(0u);
            #endif  /* (Servo_Cut_CY_TCPWM_V2 && Servo_Cut_PWM_UPDOWN_CNT_USED && !Servo_Cut_CY_TCPWM_4000) */
        #endif  /* (Servo_Cut__PWM_PR == Servo_Cut_PWM_MODE) */

        #if (Servo_Cut__PWM_DT == Servo_Cut_PWM_MODE)
            Servo_Cut_CONTROL_REG |= Servo_Cut_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (Servo_Cut__PWM_DT == Servo_Cut_PWM_MODE) */

        #if (Servo_Cut__PWM == Servo_Cut_PWM_MODE)
            Servo_Cut_CONTROL_REG |= Servo_Cut_CTRL_PWM_PRESCALER;
        #endif  /* (Servo_Cut__PWM == Servo_Cut_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        Servo_Cut_TRIG_CONTROL1_REG  = Servo_Cut_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Servo_Cut_SetInterruptMode(Servo_Cut_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (Servo_Cut__PWM_PR == Servo_Cut_PWM_MODE)
            Servo_Cut_TRIG_CONTROL2_REG =
                    (Servo_Cut_CC_MATCH_NO_CHANGE    |
                    Servo_Cut_OVERLOW_NO_CHANGE      |
                    Servo_Cut_UNDERFLOW_NO_CHANGE);
        #else
            #if (Servo_Cut__LEFT == Servo_Cut_PWM_ALIGN)
                Servo_Cut_TRIG_CONTROL2_REG = Servo_Cut_PWM_MODE_LEFT;
            #endif  /* ( Servo_Cut_PWM_LEFT == Servo_Cut_PWM_ALIGN) */

            #if (Servo_Cut__RIGHT == Servo_Cut_PWM_ALIGN)
                Servo_Cut_TRIG_CONTROL2_REG = Servo_Cut_PWM_MODE_RIGHT;
            #endif  /* ( Servo_Cut_PWM_RIGHT == Servo_Cut_PWM_ALIGN) */

            #if (Servo_Cut__CENTER == Servo_Cut_PWM_ALIGN)
                Servo_Cut_TRIG_CONTROL2_REG = Servo_Cut_PWM_MODE_CENTER;
            #endif  /* ( Servo_Cut_PWM_CENTER == Servo_Cut_PWM_ALIGN) */

            #if (Servo_Cut__ASYMMETRIC == Servo_Cut_PWM_ALIGN)
                Servo_Cut_TRIG_CONTROL2_REG = Servo_Cut_PWM_MODE_ASYM;
            #endif  /* (Servo_Cut__ASYMMETRIC == Servo_Cut_PWM_ALIGN) */
        #endif  /* (Servo_Cut__PWM_PR == Servo_Cut_PWM_MODE) */

        /* Set other values from customizer */
        Servo_Cut_WritePeriod(Servo_Cut_PWM_PERIOD_VALUE );
        Servo_Cut_WriteCompare(Servo_Cut_PWM_COMPARE_VALUE);

        #if (1u == Servo_Cut_PWM_COMPARE_SWAP)
            Servo_Cut_SetCompareSwap(1u);
            Servo_Cut_WriteCompareBuf(Servo_Cut_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == Servo_Cut_PWM_COMPARE_SWAP) */

        #if (1u == Servo_Cut_PWM_PERIOD_SWAP)
            Servo_Cut_SetPeriodSwap(1u);
            Servo_Cut_WritePeriodBuf(Servo_Cut_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == Servo_Cut_PWM_PERIOD_SWAP) */
    #endif  /* (Servo_Cut__PWM_SEL == Servo_Cut_CONFIG) */
    
}


/*******************************************************************************
* Function Name: Servo_Cut_Enable
********************************************************************************
*
* Summary:
*  Enables the Servo_Cut.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    Servo_Cut_BLOCK_CONTROL_REG |= Servo_Cut_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (Servo_Cut__PWM_SEL == Servo_Cut_CONFIG)
        #if (0u == Servo_Cut_PWM_START_SIGNAL_PRESENT)
            Servo_Cut_TriggerCommand(Servo_Cut_MASK, Servo_Cut_CMD_START);
        #endif /* (0u == Servo_Cut_PWM_START_SIGNAL_PRESENT) */
    #endif /* (Servo_Cut__PWM_SEL == Servo_Cut_CONFIG) */

    #if (Servo_Cut__TIMER == Servo_Cut_CONFIG)
        #if (0u == Servo_Cut_TC_START_SIGNAL_PRESENT)
            Servo_Cut_TriggerCommand(Servo_Cut_MASK, Servo_Cut_CMD_START);
        #endif /* (0u == Servo_Cut_TC_START_SIGNAL_PRESENT) */
    #endif /* (Servo_Cut__TIMER == Servo_Cut_CONFIG) */
    
    #if (Servo_Cut__QUAD == Servo_Cut_CONFIG)
        #if (0u != Servo_Cut_QUAD_AUTO_START)
            Servo_Cut_TriggerCommand(Servo_Cut_MASK, Servo_Cut_CMD_RELOAD);
        #endif /* (0u != Servo_Cut_QUAD_AUTO_START) */
    #endif  /* (Servo_Cut__QUAD == Servo_Cut_CONFIG) */
}


/*******************************************************************************
* Function Name: Servo_Cut_Start
********************************************************************************
*
* Summary:
*  Initializes the Servo_Cut with default customizer
*  values when called the first time and enables the Servo_Cut.
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
*  Servo_Cut_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time Servo_Cut_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the Servo_Cut_Start() routine.
*
*******************************************************************************/
void Servo_Cut_Start(void)
{
    if (0u == Servo_Cut_initVar)
    {
        Servo_Cut_Init();
        Servo_Cut_initVar = 1u;
    }

    Servo_Cut_Enable();
}


/*******************************************************************************
* Function Name: Servo_Cut_Stop
********************************************************************************
*
* Summary:
*  Disables the Servo_Cut.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_BLOCK_CONTROL_REG &= (uint32)~Servo_Cut_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the Servo_Cut. This function is used when
*  configured as a generic Servo_Cut and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the Servo_Cut to operate in
*   Values:
*   - Servo_Cut_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - Servo_Cut_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - Servo_Cut_MODE_QUAD - Quadrature decoder
*         - Servo_Cut_MODE_PWM - PWM
*         - Servo_Cut_MODE_PWM_DT - PWM with dead time
*         - Servo_Cut_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_CONTROL_REG &= (uint32)~Servo_Cut_MODE_MASK;
    Servo_Cut_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - Servo_Cut_MODE_X1 - Counts on phi 1 rising
*         - Servo_Cut_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - Servo_Cut_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_CONTROL_REG &= (uint32)~Servo_Cut_QUAD_MODE_MASK;
    Servo_Cut_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - Servo_Cut_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - Servo_Cut_PRESCALE_DIVBY2    - Divide by 2
*         - Servo_Cut_PRESCALE_DIVBY4    - Divide by 4
*         - Servo_Cut_PRESCALE_DIVBY8    - Divide by 8
*         - Servo_Cut_PRESCALE_DIVBY16   - Divide by 16
*         - Servo_Cut_PRESCALE_DIVBY32   - Divide by 32
*         - Servo_Cut_PRESCALE_DIVBY64   - Divide by 64
*         - Servo_Cut_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_CONTROL_REG &= (uint32)~Servo_Cut_PRESCALER_MASK;
    Servo_Cut_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the Servo_Cut runs
*  continuously or stops when terminal count is reached.  By default the
*  Servo_Cut operates in the continuous mode.
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
void Servo_Cut_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_CONTROL_REG &= (uint32)~Servo_Cut_ONESHOT_MASK;
    Servo_Cut_CONTROL_REG |= ((uint32)((oneShotEnable & Servo_Cut_1BIT_MASK) <<
                                                               Servo_Cut_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetPWMMode
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
void Servo_Cut_SetPWMMode(uint32 modeMask)
{
    Servo_Cut_TRIG_CONTROL2_REG = (modeMask & Servo_Cut_6BIT_MASK);
}



/*******************************************************************************
* Function Name: Servo_Cut_SetPWMSyncKill
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
void Servo_Cut_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_CONTROL_REG &= (uint32)~Servo_Cut_PWM_SYNC_KILL_MASK;
    Servo_Cut_CONTROL_REG |= ((uint32)((syncKillEnable & Servo_Cut_1BIT_MASK)  <<
                                               Servo_Cut_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetPWMStopOnKill
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
void Servo_Cut_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_CONTROL_REG &= (uint32)~Servo_Cut_PWM_STOP_KILL_MASK;
    Servo_Cut_CONTROL_REG |= ((uint32)((stopOnKillEnable & Servo_Cut_1BIT_MASK)  <<
                                                         Servo_Cut_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetPWMDeadTime
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
void Servo_Cut_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_CONTROL_REG &= (uint32)~Servo_Cut_PRESCALER_MASK;
    Servo_Cut_CONTROL_REG |= ((uint32)((deadTime & Servo_Cut_8BIT_MASK) <<
                                                          Servo_Cut_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetPWMInvert
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
*         - Servo_Cut_INVERT_LINE   - Inverts the line output
*         - Servo_Cut_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_CONTROL_REG &= (uint32)~Servo_Cut_INV_OUT_MASK;
    Servo_Cut_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: Servo_Cut_WriteCounter
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
void Servo_Cut_WriteCounter(uint32 count)
{
    Servo_Cut_COUNTER_REG = (count & Servo_Cut_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_Cut_ReadCounter
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
uint32 Servo_Cut_ReadCounter(void)
{
    return (Servo_Cut_COUNTER_REG & Servo_Cut_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - Servo_Cut_COUNT_UP       - Counts up
*     - Servo_Cut_COUNT_DOWN     - Counts down
*     - Servo_Cut_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - Servo_Cut_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_CONTROL_REG &= (uint32)~Servo_Cut_UPDOWN_MASK;
    Servo_Cut_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_WritePeriod
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
void Servo_Cut_WritePeriod(uint32 period)
{
    Servo_Cut_PERIOD_REG = (period & Servo_Cut_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_Cut_ReadPeriod
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
uint32 Servo_Cut_ReadPeriod(void)
{
    return (Servo_Cut_PERIOD_REG & Servo_Cut_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetCompareSwap
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
void Servo_Cut_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_CONTROL_REG &= (uint32)~Servo_Cut_RELOAD_CC_MASK;
    Servo_Cut_CONTROL_REG |= (swapEnable & Servo_Cut_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_WritePeriodBuf
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
void Servo_Cut_WritePeriodBuf(uint32 periodBuf)
{
    Servo_Cut_PERIOD_BUF_REG = (periodBuf & Servo_Cut_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_Cut_ReadPeriodBuf
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
uint32 Servo_Cut_ReadPeriodBuf(void)
{
    return (Servo_Cut_PERIOD_BUF_REG & Servo_Cut_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetPeriodSwap
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
void Servo_Cut_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_CONTROL_REG &= (uint32)~Servo_Cut_RELOAD_PERIOD_MASK;
    Servo_Cut_CONTROL_REG |= ((uint32)((swapEnable & Servo_Cut_1BIT_MASK) <<
                                                            Servo_Cut_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_WriteCompare
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
void Servo_Cut_WriteCompare(uint32 compare)
{
    #if (Servo_Cut_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Servo_Cut_CY_TCPWM_4000) */

    #if (Servo_Cut_CY_TCPWM_4000)
        currentMode = ((Servo_Cut_CONTROL_REG & Servo_Cut_UPDOWN_MASK) >> Servo_Cut_UPDOWN_SHIFT);

        if (((uint32)Servo_Cut__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)Servo_Cut__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (Servo_Cut_CY_TCPWM_4000) */
    
    Servo_Cut_COMP_CAP_REG = (compare & Servo_Cut_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_Cut_ReadCompare
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
uint32 Servo_Cut_ReadCompare(void)
{
    #if (Servo_Cut_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Servo_Cut_CY_TCPWM_4000) */

    #if (Servo_Cut_CY_TCPWM_4000)
        currentMode = ((Servo_Cut_CONTROL_REG & Servo_Cut_UPDOWN_MASK) >> Servo_Cut_UPDOWN_SHIFT);
        
        regVal = Servo_Cut_COMP_CAP_REG;
        
        if (((uint32)Servo_Cut__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Servo_Cut__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Servo_Cut_16BIT_MASK);
    #else
        return (Servo_Cut_COMP_CAP_REG & Servo_Cut_16BIT_MASK);
    #endif /* (Servo_Cut_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Servo_Cut_WriteCompareBuf
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
void Servo_Cut_WriteCompareBuf(uint32 compareBuf)
{
    #if (Servo_Cut_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Servo_Cut_CY_TCPWM_4000) */

    #if (Servo_Cut_CY_TCPWM_4000)
        currentMode = ((Servo_Cut_CONTROL_REG & Servo_Cut_UPDOWN_MASK) >> Servo_Cut_UPDOWN_SHIFT);

        if (((uint32)Servo_Cut__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)Servo_Cut__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (Servo_Cut_CY_TCPWM_4000) */
    
    Servo_Cut_COMP_CAP_BUF_REG = (compareBuf & Servo_Cut_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_Cut_ReadCompareBuf
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
uint32 Servo_Cut_ReadCompareBuf(void)
{
    #if (Servo_Cut_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Servo_Cut_CY_TCPWM_4000) */

    #if (Servo_Cut_CY_TCPWM_4000)
        currentMode = ((Servo_Cut_CONTROL_REG & Servo_Cut_UPDOWN_MASK) >> Servo_Cut_UPDOWN_SHIFT);

        regVal = Servo_Cut_COMP_CAP_BUF_REG;
        
        if (((uint32)Servo_Cut__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Servo_Cut__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Servo_Cut_16BIT_MASK);
    #else
        return (Servo_Cut_COMP_CAP_BUF_REG & Servo_Cut_16BIT_MASK);
    #endif /* (Servo_Cut_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Servo_Cut_ReadCapture
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
uint32 Servo_Cut_ReadCapture(void)
{
    return (Servo_Cut_COMP_CAP_REG & Servo_Cut_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_Cut_ReadCaptureBuf
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
uint32 Servo_Cut_ReadCaptureBuf(void)
{
    return (Servo_Cut_COMP_CAP_BUF_REG & Servo_Cut_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetCaptureMode
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
*     - Servo_Cut_TRIG_LEVEL     - Level
*     - Servo_Cut_TRIG_RISING    - Rising edge
*     - Servo_Cut_TRIG_FALLING   - Falling edge
*     - Servo_Cut_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_TRIG_CONTROL1_REG &= (uint32)~Servo_Cut_CAPTURE_MASK;
    Servo_Cut_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Servo_Cut_TRIG_LEVEL     - Level
*     - Servo_Cut_TRIG_RISING    - Rising edge
*     - Servo_Cut_TRIG_FALLING   - Falling edge
*     - Servo_Cut_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_TRIG_CONTROL1_REG &= (uint32)~Servo_Cut_RELOAD_MASK;
    Servo_Cut_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Servo_Cut_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Servo_Cut_TRIG_LEVEL     - Level
*     - Servo_Cut_TRIG_RISING    - Rising edge
*     - Servo_Cut_TRIG_FALLING   - Falling edge
*     - Servo_Cut_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_TRIG_CONTROL1_REG &= (uint32)~Servo_Cut_START_MASK;
    Servo_Cut_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Servo_Cut_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Servo_Cut_TRIG_LEVEL     - Level
*     - Servo_Cut_TRIG_RISING    - Rising edge
*     - Servo_Cut_TRIG_FALLING   - Falling edge
*     - Servo_Cut_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_TRIG_CONTROL1_REG &= (uint32)~Servo_Cut_STOP_MASK;
    Servo_Cut_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Servo_Cut_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Servo_Cut_TRIG_LEVEL     - Level
*     - Servo_Cut_TRIG_RISING    - Rising edge
*     - Servo_Cut_TRIG_FALLING   - Falling edge
*     - Servo_Cut_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_TRIG_CONTROL1_REG &= (uint32)~Servo_Cut_COUNT_MASK;
    Servo_Cut_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Servo_Cut_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_TriggerCommand
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
*     - Servo_Cut_CMD_CAPTURE    - Trigger Capture/Switch command
*     - Servo_Cut_CMD_RELOAD     - Trigger Reload/Index command
*     - Servo_Cut_CMD_STOP       - Trigger Stop/Kill command
*     - Servo_Cut_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_Cut_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_Cut_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the Servo_Cut.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - Servo_Cut_STATUS_DOWN    - Set if counting down
*     - Servo_Cut_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 Servo_Cut_ReadStatus(void)
{
    return ((Servo_Cut_STATUS_REG >> Servo_Cut_RUNNING_STATUS_SHIFT) |
            (Servo_Cut_STATUS_REG & Servo_Cut_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: Servo_Cut_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - Servo_Cut_INTR_MASK_TC       - Terminal count mask
*     - Servo_Cut_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_SetInterruptMode(uint32 interruptMask)
{
    Servo_Cut_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: Servo_Cut_GetInterruptSourceMasked
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
*     - Servo_Cut_INTR_MASK_TC       - Terminal count mask
*     - Servo_Cut_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Servo_Cut_GetInterruptSourceMasked(void)
{
    return (Servo_Cut_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: Servo_Cut_GetInterruptSource
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
*     - Servo_Cut_INTR_MASK_TC       - Terminal count mask
*     - Servo_Cut_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Servo_Cut_GetInterruptSource(void)
{
    return (Servo_Cut_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: Servo_Cut_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - Servo_Cut_INTR_MASK_TC       - Terminal count mask
*     - Servo_Cut_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_ClearInterrupt(uint32 interruptMask)
{
    Servo_Cut_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: Servo_Cut_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - Servo_Cut_INTR_MASK_TC       - Terminal count mask
*     - Servo_Cut_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Cut_SetInterrupt(uint32 interruptMask)
{
    Servo_Cut_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
