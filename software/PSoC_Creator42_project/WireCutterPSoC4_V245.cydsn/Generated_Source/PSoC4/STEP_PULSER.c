/*******************************************************************************
* File Name: STEP_PULSER.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the STEP_PULSER
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

#include "STEP_PULSER.h"

uint8 STEP_PULSER_initVar = 0u;


/*******************************************************************************
* Function Name: STEP_PULSER_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default STEP_PULSER configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (STEP_PULSER__QUAD == STEP_PULSER_CONFIG)
        STEP_PULSER_CONTROL_REG = STEP_PULSER_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        STEP_PULSER_TRIG_CONTROL1_REG  = STEP_PULSER_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        STEP_PULSER_SetInterruptMode(STEP_PULSER_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        STEP_PULSER_SetCounterMode(STEP_PULSER_COUNT_DOWN);
        STEP_PULSER_WritePeriod(STEP_PULSER_QUAD_PERIOD_INIT_VALUE);
        STEP_PULSER_WriteCounter(STEP_PULSER_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (STEP_PULSER__QUAD == STEP_PULSER_CONFIG) */

    #if (STEP_PULSER__TIMER == STEP_PULSER_CONFIG)
        STEP_PULSER_CONTROL_REG = STEP_PULSER_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        STEP_PULSER_TRIG_CONTROL1_REG  = STEP_PULSER_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        STEP_PULSER_SetInterruptMode(STEP_PULSER_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        STEP_PULSER_WritePeriod(STEP_PULSER_TC_PERIOD_VALUE );

        #if (STEP_PULSER__COMPARE == STEP_PULSER_TC_COMP_CAP_MODE)
            STEP_PULSER_WriteCompare(STEP_PULSER_TC_COMPARE_VALUE);

            #if (1u == STEP_PULSER_TC_COMPARE_SWAP)
                STEP_PULSER_SetCompareSwap(1u);
                STEP_PULSER_WriteCompareBuf(STEP_PULSER_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == STEP_PULSER_TC_COMPARE_SWAP) */
        #endif  /* (STEP_PULSER__COMPARE == STEP_PULSER_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (STEP_PULSER_CY_TCPWM_V2 && STEP_PULSER_TIMER_UPDOWN_CNT_USED && !STEP_PULSER_CY_TCPWM_4000)
            STEP_PULSER_WriteCounter(1u);
        #elif(STEP_PULSER__COUNT_DOWN == STEP_PULSER_TC_COUNTER_MODE)
            STEP_PULSER_WriteCounter(STEP_PULSER_TC_PERIOD_VALUE);
        #else
            STEP_PULSER_WriteCounter(0u);
        #endif /* (STEP_PULSER_CY_TCPWM_V2 && STEP_PULSER_TIMER_UPDOWN_CNT_USED && !STEP_PULSER_CY_TCPWM_4000) */
    #endif  /* (STEP_PULSER__TIMER == STEP_PULSER_CONFIG) */

    #if (STEP_PULSER__PWM_SEL == STEP_PULSER_CONFIG)
        STEP_PULSER_CONTROL_REG = STEP_PULSER_CTRL_PWM_BASE_CONFIG;

        #if (STEP_PULSER__PWM_PR == STEP_PULSER_PWM_MODE)
            STEP_PULSER_CONTROL_REG |= STEP_PULSER_CTRL_PWM_RUN_MODE;
            STEP_PULSER_WriteCounter(STEP_PULSER_PWM_PR_INIT_VALUE);
        #else
            STEP_PULSER_CONTROL_REG |= STEP_PULSER_CTRL_PWM_ALIGN | STEP_PULSER_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (STEP_PULSER_CY_TCPWM_V2 && STEP_PULSER_PWM_UPDOWN_CNT_USED && !STEP_PULSER_CY_TCPWM_4000)
                STEP_PULSER_WriteCounter(1u);
            #elif (STEP_PULSER__RIGHT == STEP_PULSER_PWM_ALIGN)
                STEP_PULSER_WriteCounter(STEP_PULSER_PWM_PERIOD_VALUE);
            #else 
                STEP_PULSER_WriteCounter(0u);
            #endif  /* (STEP_PULSER_CY_TCPWM_V2 && STEP_PULSER_PWM_UPDOWN_CNT_USED && !STEP_PULSER_CY_TCPWM_4000) */
        #endif  /* (STEP_PULSER__PWM_PR == STEP_PULSER_PWM_MODE) */

        #if (STEP_PULSER__PWM_DT == STEP_PULSER_PWM_MODE)
            STEP_PULSER_CONTROL_REG |= STEP_PULSER_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (STEP_PULSER__PWM_DT == STEP_PULSER_PWM_MODE) */

        #if (STEP_PULSER__PWM == STEP_PULSER_PWM_MODE)
            STEP_PULSER_CONTROL_REG |= STEP_PULSER_CTRL_PWM_PRESCALER;
        #endif  /* (STEP_PULSER__PWM == STEP_PULSER_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        STEP_PULSER_TRIG_CONTROL1_REG  = STEP_PULSER_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        STEP_PULSER_SetInterruptMode(STEP_PULSER_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (STEP_PULSER__PWM_PR == STEP_PULSER_PWM_MODE)
            STEP_PULSER_TRIG_CONTROL2_REG =
                    (STEP_PULSER_CC_MATCH_NO_CHANGE    |
                    STEP_PULSER_OVERLOW_NO_CHANGE      |
                    STEP_PULSER_UNDERFLOW_NO_CHANGE);
        #else
            #if (STEP_PULSER__LEFT == STEP_PULSER_PWM_ALIGN)
                STEP_PULSER_TRIG_CONTROL2_REG = STEP_PULSER_PWM_MODE_LEFT;
            #endif  /* ( STEP_PULSER_PWM_LEFT == STEP_PULSER_PWM_ALIGN) */

            #if (STEP_PULSER__RIGHT == STEP_PULSER_PWM_ALIGN)
                STEP_PULSER_TRIG_CONTROL2_REG = STEP_PULSER_PWM_MODE_RIGHT;
            #endif  /* ( STEP_PULSER_PWM_RIGHT == STEP_PULSER_PWM_ALIGN) */

            #if (STEP_PULSER__CENTER == STEP_PULSER_PWM_ALIGN)
                STEP_PULSER_TRIG_CONTROL2_REG = STEP_PULSER_PWM_MODE_CENTER;
            #endif  /* ( STEP_PULSER_PWM_CENTER == STEP_PULSER_PWM_ALIGN) */

            #if (STEP_PULSER__ASYMMETRIC == STEP_PULSER_PWM_ALIGN)
                STEP_PULSER_TRIG_CONTROL2_REG = STEP_PULSER_PWM_MODE_ASYM;
            #endif  /* (STEP_PULSER__ASYMMETRIC == STEP_PULSER_PWM_ALIGN) */
        #endif  /* (STEP_PULSER__PWM_PR == STEP_PULSER_PWM_MODE) */

        /* Set other values from customizer */
        STEP_PULSER_WritePeriod(STEP_PULSER_PWM_PERIOD_VALUE );
        STEP_PULSER_WriteCompare(STEP_PULSER_PWM_COMPARE_VALUE);

        #if (1u == STEP_PULSER_PWM_COMPARE_SWAP)
            STEP_PULSER_SetCompareSwap(1u);
            STEP_PULSER_WriteCompareBuf(STEP_PULSER_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == STEP_PULSER_PWM_COMPARE_SWAP) */

        #if (1u == STEP_PULSER_PWM_PERIOD_SWAP)
            STEP_PULSER_SetPeriodSwap(1u);
            STEP_PULSER_WritePeriodBuf(STEP_PULSER_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == STEP_PULSER_PWM_PERIOD_SWAP) */
    #endif  /* (STEP_PULSER__PWM_SEL == STEP_PULSER_CONFIG) */
    
}


/*******************************************************************************
* Function Name: STEP_PULSER_Enable
********************************************************************************
*
* Summary:
*  Enables the STEP_PULSER.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    STEP_PULSER_BLOCK_CONTROL_REG |= STEP_PULSER_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (STEP_PULSER__PWM_SEL == STEP_PULSER_CONFIG)
        #if (0u == STEP_PULSER_PWM_START_SIGNAL_PRESENT)
            STEP_PULSER_TriggerCommand(STEP_PULSER_MASK, STEP_PULSER_CMD_START);
        #endif /* (0u == STEP_PULSER_PWM_START_SIGNAL_PRESENT) */
    #endif /* (STEP_PULSER__PWM_SEL == STEP_PULSER_CONFIG) */

    #if (STEP_PULSER__TIMER == STEP_PULSER_CONFIG)
        #if (0u == STEP_PULSER_TC_START_SIGNAL_PRESENT)
            STEP_PULSER_TriggerCommand(STEP_PULSER_MASK, STEP_PULSER_CMD_START);
        #endif /* (0u == STEP_PULSER_TC_START_SIGNAL_PRESENT) */
    #endif /* (STEP_PULSER__TIMER == STEP_PULSER_CONFIG) */
    
    #if (STEP_PULSER__QUAD == STEP_PULSER_CONFIG)
        #if (0u != STEP_PULSER_QUAD_AUTO_START)
            STEP_PULSER_TriggerCommand(STEP_PULSER_MASK, STEP_PULSER_CMD_RELOAD);
        #endif /* (0u != STEP_PULSER_QUAD_AUTO_START) */
    #endif  /* (STEP_PULSER__QUAD == STEP_PULSER_CONFIG) */
}


/*******************************************************************************
* Function Name: STEP_PULSER_Start
********************************************************************************
*
* Summary:
*  Initializes the STEP_PULSER with default customizer
*  values when called the first time and enables the STEP_PULSER.
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
*  STEP_PULSER_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time STEP_PULSER_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the STEP_PULSER_Start() routine.
*
*******************************************************************************/
void STEP_PULSER_Start(void)
{
    if (0u == STEP_PULSER_initVar)
    {
        STEP_PULSER_Init();
        STEP_PULSER_initVar = 1u;
    }

    STEP_PULSER_Enable();
}


/*******************************************************************************
* Function Name: STEP_PULSER_Stop
********************************************************************************
*
* Summary:
*  Disables the STEP_PULSER.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_BLOCK_CONTROL_REG &= (uint32)~STEP_PULSER_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the STEP_PULSER. This function is used when
*  configured as a generic STEP_PULSER and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the STEP_PULSER to operate in
*   Values:
*   - STEP_PULSER_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - STEP_PULSER_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - STEP_PULSER_MODE_QUAD - Quadrature decoder
*         - STEP_PULSER_MODE_PWM - PWM
*         - STEP_PULSER_MODE_PWM_DT - PWM with dead time
*         - STEP_PULSER_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_CONTROL_REG &= (uint32)~STEP_PULSER_MODE_MASK;
    STEP_PULSER_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - STEP_PULSER_MODE_X1 - Counts on phi 1 rising
*         - STEP_PULSER_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - STEP_PULSER_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_CONTROL_REG &= (uint32)~STEP_PULSER_QUAD_MODE_MASK;
    STEP_PULSER_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - STEP_PULSER_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - STEP_PULSER_PRESCALE_DIVBY2    - Divide by 2
*         - STEP_PULSER_PRESCALE_DIVBY4    - Divide by 4
*         - STEP_PULSER_PRESCALE_DIVBY8    - Divide by 8
*         - STEP_PULSER_PRESCALE_DIVBY16   - Divide by 16
*         - STEP_PULSER_PRESCALE_DIVBY32   - Divide by 32
*         - STEP_PULSER_PRESCALE_DIVBY64   - Divide by 64
*         - STEP_PULSER_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_CONTROL_REG &= (uint32)~STEP_PULSER_PRESCALER_MASK;
    STEP_PULSER_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the STEP_PULSER runs
*  continuously or stops when terminal count is reached.  By default the
*  STEP_PULSER operates in the continuous mode.
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
void STEP_PULSER_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_CONTROL_REG &= (uint32)~STEP_PULSER_ONESHOT_MASK;
    STEP_PULSER_CONTROL_REG |= ((uint32)((oneShotEnable & STEP_PULSER_1BIT_MASK) <<
                                                               STEP_PULSER_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetPWMMode
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
void STEP_PULSER_SetPWMMode(uint32 modeMask)
{
    STEP_PULSER_TRIG_CONTROL2_REG = (modeMask & STEP_PULSER_6BIT_MASK);
}



/*******************************************************************************
* Function Name: STEP_PULSER_SetPWMSyncKill
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
void STEP_PULSER_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_CONTROL_REG &= (uint32)~STEP_PULSER_PWM_SYNC_KILL_MASK;
    STEP_PULSER_CONTROL_REG |= ((uint32)((syncKillEnable & STEP_PULSER_1BIT_MASK)  <<
                                               STEP_PULSER_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetPWMStopOnKill
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
void STEP_PULSER_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_CONTROL_REG &= (uint32)~STEP_PULSER_PWM_STOP_KILL_MASK;
    STEP_PULSER_CONTROL_REG |= ((uint32)((stopOnKillEnable & STEP_PULSER_1BIT_MASK)  <<
                                                         STEP_PULSER_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetPWMDeadTime
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
void STEP_PULSER_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_CONTROL_REG &= (uint32)~STEP_PULSER_PRESCALER_MASK;
    STEP_PULSER_CONTROL_REG |= ((uint32)((deadTime & STEP_PULSER_8BIT_MASK) <<
                                                          STEP_PULSER_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetPWMInvert
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
*         - STEP_PULSER_INVERT_LINE   - Inverts the line output
*         - STEP_PULSER_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_CONTROL_REG &= (uint32)~STEP_PULSER_INV_OUT_MASK;
    STEP_PULSER_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: STEP_PULSER_WriteCounter
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
void STEP_PULSER_WriteCounter(uint32 count)
{
    STEP_PULSER_COUNTER_REG = (count & STEP_PULSER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: STEP_PULSER_ReadCounter
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
uint32 STEP_PULSER_ReadCounter(void)
{
    return (STEP_PULSER_COUNTER_REG & STEP_PULSER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - STEP_PULSER_COUNT_UP       - Counts up
*     - STEP_PULSER_COUNT_DOWN     - Counts down
*     - STEP_PULSER_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - STEP_PULSER_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_CONTROL_REG &= (uint32)~STEP_PULSER_UPDOWN_MASK;
    STEP_PULSER_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_WritePeriod
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
void STEP_PULSER_WritePeriod(uint32 period)
{
    STEP_PULSER_PERIOD_REG = (period & STEP_PULSER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: STEP_PULSER_ReadPeriod
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
uint32 STEP_PULSER_ReadPeriod(void)
{
    return (STEP_PULSER_PERIOD_REG & STEP_PULSER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetCompareSwap
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
void STEP_PULSER_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_CONTROL_REG &= (uint32)~STEP_PULSER_RELOAD_CC_MASK;
    STEP_PULSER_CONTROL_REG |= (swapEnable & STEP_PULSER_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_WritePeriodBuf
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
void STEP_PULSER_WritePeriodBuf(uint32 periodBuf)
{
    STEP_PULSER_PERIOD_BUF_REG = (periodBuf & STEP_PULSER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: STEP_PULSER_ReadPeriodBuf
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
uint32 STEP_PULSER_ReadPeriodBuf(void)
{
    return (STEP_PULSER_PERIOD_BUF_REG & STEP_PULSER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetPeriodSwap
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
void STEP_PULSER_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_CONTROL_REG &= (uint32)~STEP_PULSER_RELOAD_PERIOD_MASK;
    STEP_PULSER_CONTROL_REG |= ((uint32)((swapEnable & STEP_PULSER_1BIT_MASK) <<
                                                            STEP_PULSER_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_WriteCompare
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
void STEP_PULSER_WriteCompare(uint32 compare)
{
    #if (STEP_PULSER_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (STEP_PULSER_CY_TCPWM_4000) */

    #if (STEP_PULSER_CY_TCPWM_4000)
        currentMode = ((STEP_PULSER_CONTROL_REG & STEP_PULSER_UPDOWN_MASK) >> STEP_PULSER_UPDOWN_SHIFT);

        if (((uint32)STEP_PULSER__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)STEP_PULSER__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (STEP_PULSER_CY_TCPWM_4000) */
    
    STEP_PULSER_COMP_CAP_REG = (compare & STEP_PULSER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: STEP_PULSER_ReadCompare
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
uint32 STEP_PULSER_ReadCompare(void)
{
    #if (STEP_PULSER_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (STEP_PULSER_CY_TCPWM_4000) */

    #if (STEP_PULSER_CY_TCPWM_4000)
        currentMode = ((STEP_PULSER_CONTROL_REG & STEP_PULSER_UPDOWN_MASK) >> STEP_PULSER_UPDOWN_SHIFT);
        
        regVal = STEP_PULSER_COMP_CAP_REG;
        
        if (((uint32)STEP_PULSER__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)STEP_PULSER__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & STEP_PULSER_16BIT_MASK);
    #else
        return (STEP_PULSER_COMP_CAP_REG & STEP_PULSER_16BIT_MASK);
    #endif /* (STEP_PULSER_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: STEP_PULSER_WriteCompareBuf
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
void STEP_PULSER_WriteCompareBuf(uint32 compareBuf)
{
    #if (STEP_PULSER_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (STEP_PULSER_CY_TCPWM_4000) */

    #if (STEP_PULSER_CY_TCPWM_4000)
        currentMode = ((STEP_PULSER_CONTROL_REG & STEP_PULSER_UPDOWN_MASK) >> STEP_PULSER_UPDOWN_SHIFT);

        if (((uint32)STEP_PULSER__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)STEP_PULSER__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (STEP_PULSER_CY_TCPWM_4000) */
    
    STEP_PULSER_COMP_CAP_BUF_REG = (compareBuf & STEP_PULSER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: STEP_PULSER_ReadCompareBuf
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
uint32 STEP_PULSER_ReadCompareBuf(void)
{
    #if (STEP_PULSER_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (STEP_PULSER_CY_TCPWM_4000) */

    #if (STEP_PULSER_CY_TCPWM_4000)
        currentMode = ((STEP_PULSER_CONTROL_REG & STEP_PULSER_UPDOWN_MASK) >> STEP_PULSER_UPDOWN_SHIFT);

        regVal = STEP_PULSER_COMP_CAP_BUF_REG;
        
        if (((uint32)STEP_PULSER__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)STEP_PULSER__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & STEP_PULSER_16BIT_MASK);
    #else
        return (STEP_PULSER_COMP_CAP_BUF_REG & STEP_PULSER_16BIT_MASK);
    #endif /* (STEP_PULSER_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: STEP_PULSER_ReadCapture
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
uint32 STEP_PULSER_ReadCapture(void)
{
    return (STEP_PULSER_COMP_CAP_REG & STEP_PULSER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: STEP_PULSER_ReadCaptureBuf
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
uint32 STEP_PULSER_ReadCaptureBuf(void)
{
    return (STEP_PULSER_COMP_CAP_BUF_REG & STEP_PULSER_16BIT_MASK);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetCaptureMode
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
*     - STEP_PULSER_TRIG_LEVEL     - Level
*     - STEP_PULSER_TRIG_RISING    - Rising edge
*     - STEP_PULSER_TRIG_FALLING   - Falling edge
*     - STEP_PULSER_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_TRIG_CONTROL1_REG &= (uint32)~STEP_PULSER_CAPTURE_MASK;
    STEP_PULSER_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - STEP_PULSER_TRIG_LEVEL     - Level
*     - STEP_PULSER_TRIG_RISING    - Rising edge
*     - STEP_PULSER_TRIG_FALLING   - Falling edge
*     - STEP_PULSER_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_TRIG_CONTROL1_REG &= (uint32)~STEP_PULSER_RELOAD_MASK;
    STEP_PULSER_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << STEP_PULSER_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - STEP_PULSER_TRIG_LEVEL     - Level
*     - STEP_PULSER_TRIG_RISING    - Rising edge
*     - STEP_PULSER_TRIG_FALLING   - Falling edge
*     - STEP_PULSER_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_TRIG_CONTROL1_REG &= (uint32)~STEP_PULSER_START_MASK;
    STEP_PULSER_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << STEP_PULSER_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - STEP_PULSER_TRIG_LEVEL     - Level
*     - STEP_PULSER_TRIG_RISING    - Rising edge
*     - STEP_PULSER_TRIG_FALLING   - Falling edge
*     - STEP_PULSER_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_TRIG_CONTROL1_REG &= (uint32)~STEP_PULSER_STOP_MASK;
    STEP_PULSER_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << STEP_PULSER_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - STEP_PULSER_TRIG_LEVEL     - Level
*     - STEP_PULSER_TRIG_RISING    - Rising edge
*     - STEP_PULSER_TRIG_FALLING   - Falling edge
*     - STEP_PULSER_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_TRIG_CONTROL1_REG &= (uint32)~STEP_PULSER_COUNT_MASK;
    STEP_PULSER_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << STEP_PULSER_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_TriggerCommand
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
*     - STEP_PULSER_CMD_CAPTURE    - Trigger Capture/Switch command
*     - STEP_PULSER_CMD_RELOAD     - Trigger Reload/Index command
*     - STEP_PULSER_CMD_STOP       - Trigger Stop/Kill command
*     - STEP_PULSER_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    STEP_PULSER_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: STEP_PULSER_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the STEP_PULSER.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - STEP_PULSER_STATUS_DOWN    - Set if counting down
*     - STEP_PULSER_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 STEP_PULSER_ReadStatus(void)
{
    return ((STEP_PULSER_STATUS_REG >> STEP_PULSER_RUNNING_STATUS_SHIFT) |
            (STEP_PULSER_STATUS_REG & STEP_PULSER_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - STEP_PULSER_INTR_MASK_TC       - Terminal count mask
*     - STEP_PULSER_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_SetInterruptMode(uint32 interruptMask)
{
    STEP_PULSER_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: STEP_PULSER_GetInterruptSourceMasked
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
*     - STEP_PULSER_INTR_MASK_TC       - Terminal count mask
*     - STEP_PULSER_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 STEP_PULSER_GetInterruptSourceMasked(void)
{
    return (STEP_PULSER_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: STEP_PULSER_GetInterruptSource
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
*     - STEP_PULSER_INTR_MASK_TC       - Terminal count mask
*     - STEP_PULSER_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 STEP_PULSER_GetInterruptSource(void)
{
    return (STEP_PULSER_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: STEP_PULSER_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - STEP_PULSER_INTR_MASK_TC       - Terminal count mask
*     - STEP_PULSER_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_ClearInterrupt(uint32 interruptMask)
{
    STEP_PULSER_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: STEP_PULSER_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - STEP_PULSER_INTR_MASK_TC       - Terminal count mask
*     - STEP_PULSER_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void STEP_PULSER_SetInterrupt(uint32 interruptMask)
{
    STEP_PULSER_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
