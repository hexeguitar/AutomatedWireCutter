/*******************************************************************************
* File Name: Servo.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the Servo
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

#include "Servo.h"

uint8 Servo_initVar = 0u;


/*******************************************************************************
* Function Name: Servo_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default Servo configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (Servo__QUAD == Servo_CONFIG)
        Servo_CONTROL_REG = Servo_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Servo_TRIG_CONTROL1_REG  = Servo_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        Servo_SetInterruptMode(Servo_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        Servo_SetCounterMode(Servo_COUNT_DOWN);
        Servo_WritePeriod(Servo_QUAD_PERIOD_INIT_VALUE);
        Servo_WriteCounter(Servo_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (Servo__QUAD == Servo_CONFIG) */

    #if (Servo__TIMER == Servo_CONFIG)
        Servo_CONTROL_REG = Servo_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Servo_TRIG_CONTROL1_REG  = Servo_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Servo_SetInterruptMode(Servo_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        Servo_WritePeriod(Servo_TC_PERIOD_VALUE );

        #if (Servo__COMPARE == Servo_TC_COMP_CAP_MODE)
            Servo_WriteCompare(Servo_TC_COMPARE_VALUE);

            #if (1u == Servo_TC_COMPARE_SWAP)
                Servo_SetCompareSwap(1u);
                Servo_WriteCompareBuf(Servo_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == Servo_TC_COMPARE_SWAP) */
        #endif  /* (Servo__COMPARE == Servo_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (Servo_CY_TCPWM_V2 && Servo_TIMER_UPDOWN_CNT_USED && !Servo_CY_TCPWM_4000)
            Servo_WriteCounter(1u);
        #elif(Servo__COUNT_DOWN == Servo_TC_COUNTER_MODE)
            Servo_WriteCounter(Servo_TC_PERIOD_VALUE);
        #else
            Servo_WriteCounter(0u);
        #endif /* (Servo_CY_TCPWM_V2 && Servo_TIMER_UPDOWN_CNT_USED && !Servo_CY_TCPWM_4000) */
    #endif  /* (Servo__TIMER == Servo_CONFIG) */

    #if (Servo__PWM_SEL == Servo_CONFIG)
        Servo_CONTROL_REG = Servo_CTRL_PWM_BASE_CONFIG;

        #if (Servo__PWM_PR == Servo_PWM_MODE)
            Servo_CONTROL_REG |= Servo_CTRL_PWM_RUN_MODE;
            Servo_WriteCounter(Servo_PWM_PR_INIT_VALUE);
        #else
            Servo_CONTROL_REG |= Servo_CTRL_PWM_ALIGN | Servo_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (Servo_CY_TCPWM_V2 && Servo_PWM_UPDOWN_CNT_USED && !Servo_CY_TCPWM_4000)
                Servo_WriteCounter(1u);
            #elif (Servo__RIGHT == Servo_PWM_ALIGN)
                Servo_WriteCounter(Servo_PWM_PERIOD_VALUE);
            #else 
                Servo_WriteCounter(0u);
            #endif  /* (Servo_CY_TCPWM_V2 && Servo_PWM_UPDOWN_CNT_USED && !Servo_CY_TCPWM_4000) */
        #endif  /* (Servo__PWM_PR == Servo_PWM_MODE) */

        #if (Servo__PWM_DT == Servo_PWM_MODE)
            Servo_CONTROL_REG |= Servo_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (Servo__PWM_DT == Servo_PWM_MODE) */

        #if (Servo__PWM == Servo_PWM_MODE)
            Servo_CONTROL_REG |= Servo_CTRL_PWM_PRESCALER;
        #endif  /* (Servo__PWM == Servo_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        Servo_TRIG_CONTROL1_REG  = Servo_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Servo_SetInterruptMode(Servo_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (Servo__PWM_PR == Servo_PWM_MODE)
            Servo_TRIG_CONTROL2_REG =
                    (Servo_CC_MATCH_NO_CHANGE    |
                    Servo_OVERLOW_NO_CHANGE      |
                    Servo_UNDERFLOW_NO_CHANGE);
        #else
            #if (Servo__LEFT == Servo_PWM_ALIGN)
                Servo_TRIG_CONTROL2_REG = Servo_PWM_MODE_LEFT;
            #endif  /* ( Servo_PWM_LEFT == Servo_PWM_ALIGN) */

            #if (Servo__RIGHT == Servo_PWM_ALIGN)
                Servo_TRIG_CONTROL2_REG = Servo_PWM_MODE_RIGHT;
            #endif  /* ( Servo_PWM_RIGHT == Servo_PWM_ALIGN) */

            #if (Servo__CENTER == Servo_PWM_ALIGN)
                Servo_TRIG_CONTROL2_REG = Servo_PWM_MODE_CENTER;
            #endif  /* ( Servo_PWM_CENTER == Servo_PWM_ALIGN) */

            #if (Servo__ASYMMETRIC == Servo_PWM_ALIGN)
                Servo_TRIG_CONTROL2_REG = Servo_PWM_MODE_ASYM;
            #endif  /* (Servo__ASYMMETRIC == Servo_PWM_ALIGN) */
        #endif  /* (Servo__PWM_PR == Servo_PWM_MODE) */

        /* Set other values from customizer */
        Servo_WritePeriod(Servo_PWM_PERIOD_VALUE );
        Servo_WriteCompare(Servo_PWM_COMPARE_VALUE);

        #if (1u == Servo_PWM_COMPARE_SWAP)
            Servo_SetCompareSwap(1u);
            Servo_WriteCompareBuf(Servo_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == Servo_PWM_COMPARE_SWAP) */

        #if (1u == Servo_PWM_PERIOD_SWAP)
            Servo_SetPeriodSwap(1u);
            Servo_WritePeriodBuf(Servo_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == Servo_PWM_PERIOD_SWAP) */
    #endif  /* (Servo__PWM_SEL == Servo_CONFIG) */
    
}


/*******************************************************************************
* Function Name: Servo_Enable
********************************************************************************
*
* Summary:
*  Enables the Servo.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    Servo_BLOCK_CONTROL_REG |= Servo_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (Servo__PWM_SEL == Servo_CONFIG)
        #if (0u == Servo_PWM_START_SIGNAL_PRESENT)
            Servo_TriggerCommand(Servo_MASK, Servo_CMD_START);
        #endif /* (0u == Servo_PWM_START_SIGNAL_PRESENT) */
    #endif /* (Servo__PWM_SEL == Servo_CONFIG) */

    #if (Servo__TIMER == Servo_CONFIG)
        #if (0u == Servo_TC_START_SIGNAL_PRESENT)
            Servo_TriggerCommand(Servo_MASK, Servo_CMD_START);
        #endif /* (0u == Servo_TC_START_SIGNAL_PRESENT) */
    #endif /* (Servo__TIMER == Servo_CONFIG) */
    
    #if (Servo__QUAD == Servo_CONFIG)
        #if (0u != Servo_QUAD_AUTO_START)
            Servo_TriggerCommand(Servo_MASK, Servo_CMD_RELOAD);
        #endif /* (0u != Servo_QUAD_AUTO_START) */
    #endif  /* (Servo__QUAD == Servo_CONFIG) */
}


/*******************************************************************************
* Function Name: Servo_Start
********************************************************************************
*
* Summary:
*  Initializes the Servo with default customizer
*  values when called the first time and enables the Servo.
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
*  Servo_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time Servo_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the Servo_Start() routine.
*
*******************************************************************************/
void Servo_Start(void)
{
    if (0u == Servo_initVar)
    {
        Servo_Init();
        Servo_initVar = 1u;
    }

    Servo_Enable();
}


/*******************************************************************************
* Function Name: Servo_Stop
********************************************************************************
*
* Summary:
*  Disables the Servo.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Servo_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_BLOCK_CONTROL_REG &= (uint32)~Servo_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the Servo. This function is used when
*  configured as a generic Servo and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the Servo to operate in
*   Values:
*   - Servo_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - Servo_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - Servo_MODE_QUAD - Quadrature decoder
*         - Servo_MODE_PWM - PWM
*         - Servo_MODE_PWM_DT - PWM with dead time
*         - Servo_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void Servo_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_CONTROL_REG &= (uint32)~Servo_MODE_MASK;
    Servo_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - Servo_MODE_X1 - Counts on phi 1 rising
*         - Servo_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - Servo_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void Servo_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_CONTROL_REG &= (uint32)~Servo_QUAD_MODE_MASK;
    Servo_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - Servo_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - Servo_PRESCALE_DIVBY2    - Divide by 2
*         - Servo_PRESCALE_DIVBY4    - Divide by 4
*         - Servo_PRESCALE_DIVBY8    - Divide by 8
*         - Servo_PRESCALE_DIVBY16   - Divide by 16
*         - Servo_PRESCALE_DIVBY32   - Divide by 32
*         - Servo_PRESCALE_DIVBY64   - Divide by 64
*         - Servo_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void Servo_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_CONTROL_REG &= (uint32)~Servo_PRESCALER_MASK;
    Servo_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the Servo runs
*  continuously or stops when terminal count is reached.  By default the
*  Servo operates in the continuous mode.
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
void Servo_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_CONTROL_REG &= (uint32)~Servo_ONESHOT_MASK;
    Servo_CONTROL_REG |= ((uint32)((oneShotEnable & Servo_1BIT_MASK) <<
                                                               Servo_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_SetPWMMode
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
void Servo_SetPWMMode(uint32 modeMask)
{
    Servo_TRIG_CONTROL2_REG = (modeMask & Servo_6BIT_MASK);
}



/*******************************************************************************
* Function Name: Servo_SetPWMSyncKill
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
void Servo_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_CONTROL_REG &= (uint32)~Servo_PWM_SYNC_KILL_MASK;
    Servo_CONTROL_REG |= ((uint32)((syncKillEnable & Servo_1BIT_MASK)  <<
                                               Servo_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_SetPWMStopOnKill
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
void Servo_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_CONTROL_REG &= (uint32)~Servo_PWM_STOP_KILL_MASK;
    Servo_CONTROL_REG |= ((uint32)((stopOnKillEnable & Servo_1BIT_MASK)  <<
                                                         Servo_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_SetPWMDeadTime
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
void Servo_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_CONTROL_REG &= (uint32)~Servo_PRESCALER_MASK;
    Servo_CONTROL_REG |= ((uint32)((deadTime & Servo_8BIT_MASK) <<
                                                          Servo_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_SetPWMInvert
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
*         - Servo_INVERT_LINE   - Inverts the line output
*         - Servo_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void Servo_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_CONTROL_REG &= (uint32)~Servo_INV_OUT_MASK;
    Servo_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: Servo_WriteCounter
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
void Servo_WriteCounter(uint32 count)
{
    Servo_COUNTER_REG = (count & Servo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_ReadCounter
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
uint32 Servo_ReadCounter(void)
{
    return (Servo_COUNTER_REG & Servo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - Servo_COUNT_UP       - Counts up
*     - Servo_COUNT_DOWN     - Counts down
*     - Servo_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - Servo_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void Servo_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_CONTROL_REG &= (uint32)~Servo_UPDOWN_MASK;
    Servo_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_WritePeriod
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
void Servo_WritePeriod(uint32 period)
{
    Servo_PERIOD_REG = (period & Servo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_ReadPeriod
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
uint32 Servo_ReadPeriod(void)
{
    return (Servo_PERIOD_REG & Servo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_SetCompareSwap
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
void Servo_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_CONTROL_REG &= (uint32)~Servo_RELOAD_CC_MASK;
    Servo_CONTROL_REG |= (swapEnable & Servo_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_WritePeriodBuf
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
void Servo_WritePeriodBuf(uint32 periodBuf)
{
    Servo_PERIOD_BUF_REG = (periodBuf & Servo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_ReadPeriodBuf
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
uint32 Servo_ReadPeriodBuf(void)
{
    return (Servo_PERIOD_BUF_REG & Servo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_SetPeriodSwap
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
void Servo_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_CONTROL_REG &= (uint32)~Servo_RELOAD_PERIOD_MASK;
    Servo_CONTROL_REG |= ((uint32)((swapEnable & Servo_1BIT_MASK) <<
                                                            Servo_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_WriteCompare
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
void Servo_WriteCompare(uint32 compare)
{
    #if (Servo_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Servo_CY_TCPWM_4000) */

    #if (Servo_CY_TCPWM_4000)
        currentMode = ((Servo_CONTROL_REG & Servo_UPDOWN_MASK) >> Servo_UPDOWN_SHIFT);

        if (((uint32)Servo__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)Servo__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (Servo_CY_TCPWM_4000) */
    
    Servo_COMP_CAP_REG = (compare & Servo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_ReadCompare
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
uint32 Servo_ReadCompare(void)
{
    #if (Servo_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Servo_CY_TCPWM_4000) */

    #if (Servo_CY_TCPWM_4000)
        currentMode = ((Servo_CONTROL_REG & Servo_UPDOWN_MASK) >> Servo_UPDOWN_SHIFT);
        
        regVal = Servo_COMP_CAP_REG;
        
        if (((uint32)Servo__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Servo__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Servo_16BIT_MASK);
    #else
        return (Servo_COMP_CAP_REG & Servo_16BIT_MASK);
    #endif /* (Servo_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Servo_WriteCompareBuf
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
void Servo_WriteCompareBuf(uint32 compareBuf)
{
    #if (Servo_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Servo_CY_TCPWM_4000) */

    #if (Servo_CY_TCPWM_4000)
        currentMode = ((Servo_CONTROL_REG & Servo_UPDOWN_MASK) >> Servo_UPDOWN_SHIFT);

        if (((uint32)Servo__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)Servo__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (Servo_CY_TCPWM_4000) */
    
    Servo_COMP_CAP_BUF_REG = (compareBuf & Servo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_ReadCompareBuf
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
uint32 Servo_ReadCompareBuf(void)
{
    #if (Servo_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Servo_CY_TCPWM_4000) */

    #if (Servo_CY_TCPWM_4000)
        currentMode = ((Servo_CONTROL_REG & Servo_UPDOWN_MASK) >> Servo_UPDOWN_SHIFT);

        regVal = Servo_COMP_CAP_BUF_REG;
        
        if (((uint32)Servo__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Servo__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Servo_16BIT_MASK);
    #else
        return (Servo_COMP_CAP_BUF_REG & Servo_16BIT_MASK);
    #endif /* (Servo_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Servo_ReadCapture
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
uint32 Servo_ReadCapture(void)
{
    return (Servo_COMP_CAP_REG & Servo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_ReadCaptureBuf
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
uint32 Servo_ReadCaptureBuf(void)
{
    return (Servo_COMP_CAP_BUF_REG & Servo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Servo_SetCaptureMode
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
*     - Servo_TRIG_LEVEL     - Level
*     - Servo_TRIG_RISING    - Rising edge
*     - Servo_TRIG_FALLING   - Falling edge
*     - Servo_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Servo_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_TRIG_CONTROL1_REG &= (uint32)~Servo_CAPTURE_MASK;
    Servo_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Servo_TRIG_LEVEL     - Level
*     - Servo_TRIG_RISING    - Rising edge
*     - Servo_TRIG_FALLING   - Falling edge
*     - Servo_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Servo_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_TRIG_CONTROL1_REG &= (uint32)~Servo_RELOAD_MASK;
    Servo_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Servo_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Servo_TRIG_LEVEL     - Level
*     - Servo_TRIG_RISING    - Rising edge
*     - Servo_TRIG_FALLING   - Falling edge
*     - Servo_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Servo_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_TRIG_CONTROL1_REG &= (uint32)~Servo_START_MASK;
    Servo_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Servo_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Servo_TRIG_LEVEL     - Level
*     - Servo_TRIG_RISING    - Rising edge
*     - Servo_TRIG_FALLING   - Falling edge
*     - Servo_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Servo_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_TRIG_CONTROL1_REG &= (uint32)~Servo_STOP_MASK;
    Servo_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Servo_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Servo_TRIG_LEVEL     - Level
*     - Servo_TRIG_RISING    - Rising edge
*     - Servo_TRIG_FALLING   - Falling edge
*     - Servo_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Servo_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_TRIG_CONTROL1_REG &= (uint32)~Servo_COUNT_MASK;
    Servo_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Servo_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_TriggerCommand
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
*     - Servo_CMD_CAPTURE    - Trigger Capture/Switch command
*     - Servo_CMD_RELOAD     - Trigger Reload/Index command
*     - Servo_CMD_STOP       - Trigger Stop/Kill command
*     - Servo_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void Servo_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Servo_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Servo_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the Servo.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - Servo_STATUS_DOWN    - Set if counting down
*     - Servo_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 Servo_ReadStatus(void)
{
    return ((Servo_STATUS_REG >> Servo_RUNNING_STATUS_SHIFT) |
            (Servo_STATUS_REG & Servo_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: Servo_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - Servo_INTR_MASK_TC       - Terminal count mask
*     - Servo_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Servo_SetInterruptMode(uint32 interruptMask)
{
    Servo_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: Servo_GetInterruptSourceMasked
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
*     - Servo_INTR_MASK_TC       - Terminal count mask
*     - Servo_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Servo_GetInterruptSourceMasked(void)
{
    return (Servo_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: Servo_GetInterruptSource
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
*     - Servo_INTR_MASK_TC       - Terminal count mask
*     - Servo_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Servo_GetInterruptSource(void)
{
    return (Servo_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: Servo_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - Servo_INTR_MASK_TC       - Terminal count mask
*     - Servo_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Servo_ClearInterrupt(uint32 interruptMask)
{
    Servo_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: Servo_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - Servo_INTR_MASK_TC       - Terminal count mask
*     - Servo_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Servo_SetInterrupt(uint32 interruptMask)
{
    Servo_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
