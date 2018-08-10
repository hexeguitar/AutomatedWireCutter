/*******************************************************************************
* File Name: Beep.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the Beep
*  component.
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

#if !defined(CY_TCPWM_Beep_H)
#define CY_TCPWM_Beep_H


#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/*******************************************************************************
* Internal Type defines
*******************************************************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} Beep_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  Beep_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define Beep_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define Beep_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define Beep_CONFIG                         (7lu)

/* Quad Mode */
/* Parameters */
#define Beep_QUAD_ENCODING_MODES            (0lu)
#define Beep_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define Beep_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define Beep_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define Beep_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define Beep_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define Beep_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define Beep_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define Beep_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define Beep_TC_RUN_MODE                    (0lu)
#define Beep_TC_COUNTER_MODE                (0lu)
#define Beep_TC_COMP_CAP_MODE               (2lu)
#define Beep_TC_PRESCALER                   (0lu)

/* Signal modes */
#define Beep_TC_RELOAD_SIGNAL_MODE          (0lu)
#define Beep_TC_COUNT_SIGNAL_MODE           (3lu)
#define Beep_TC_START_SIGNAL_MODE           (0lu)
#define Beep_TC_STOP_SIGNAL_MODE            (0lu)
#define Beep_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define Beep_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define Beep_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define Beep_TC_START_SIGNAL_PRESENT        (0lu)
#define Beep_TC_STOP_SIGNAL_PRESENT         (0lu)
#define Beep_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define Beep_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define Beep_PWM_KILL_EVENT                 (0lu)
#define Beep_PWM_STOP_EVENT                 (0lu)
#define Beep_PWM_MODE                       (4lu)
#define Beep_PWM_OUT_N_INVERT               (0lu)
#define Beep_PWM_OUT_INVERT                 (0lu)
#define Beep_PWM_ALIGN                      (0lu)
#define Beep_PWM_RUN_MODE                   (0lu)
#define Beep_PWM_DEAD_TIME_CYCLE            (0lu)
#define Beep_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define Beep_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define Beep_PWM_COUNT_SIGNAL_MODE          (3lu)
#define Beep_PWM_START_SIGNAL_MODE          (0lu)
#define Beep_PWM_STOP_SIGNAL_MODE           (0lu)
#define Beep_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define Beep_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define Beep_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define Beep_PWM_START_SIGNAL_PRESENT       (0lu)
#define Beep_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define Beep_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define Beep_PWM_INTERRUPT_MASK             (0lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define Beep_TC_PERIOD_VALUE                (65535lu)
#define Beep_TC_COMPARE_VALUE               (65535lu)
#define Beep_TC_COMPARE_BUF_VALUE           (65535lu)
#define Beep_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define Beep_PWM_PERIOD_VALUE               (65535lu)
#define Beep_PWM_PERIOD_BUF_VALUE           (65535lu)
#define Beep_PWM_PERIOD_SWAP                (0lu)
#define Beep_PWM_COMPARE_VALUE              (32768lu)
#define Beep_PWM_COMPARE_BUF_VALUE          (65535lu)
#define Beep_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define Beep__LEFT 0
#define Beep__RIGHT 1
#define Beep__CENTER 2
#define Beep__ASYMMETRIC 3

#define Beep__X1 0
#define Beep__X2 1
#define Beep__X4 2

#define Beep__PWM 4
#define Beep__PWM_DT 5
#define Beep__PWM_PR 6

#define Beep__INVERSE 1
#define Beep__DIRECT 0

#define Beep__CAPTURE 2
#define Beep__COMPARE 0

#define Beep__TRIG_LEVEL 3
#define Beep__TRIG_RISING 0
#define Beep__TRIG_FALLING 1
#define Beep__TRIG_BOTH 2

#define Beep__INTR_MASK_TC 1
#define Beep__INTR_MASK_CC_MATCH 2
#define Beep__INTR_MASK_NONE 0
#define Beep__INTR_MASK_TC_CC 3

#define Beep__UNCONFIG 8
#define Beep__TIMER 1
#define Beep__QUAD 3
#define Beep__PWM_SEL 7

#define Beep__COUNT_UP 0
#define Beep__COUNT_DOWN 1
#define Beep__COUNT_UPDOWN0 2
#define Beep__COUNT_UPDOWN1 3


/* Prescaler */
#define Beep_PRESCALE_DIVBY1                ((uint32)(0u << Beep_PRESCALER_SHIFT))
#define Beep_PRESCALE_DIVBY2                ((uint32)(1u << Beep_PRESCALER_SHIFT))
#define Beep_PRESCALE_DIVBY4                ((uint32)(2u << Beep_PRESCALER_SHIFT))
#define Beep_PRESCALE_DIVBY8                ((uint32)(3u << Beep_PRESCALER_SHIFT))
#define Beep_PRESCALE_DIVBY16               ((uint32)(4u << Beep_PRESCALER_SHIFT))
#define Beep_PRESCALE_DIVBY32               ((uint32)(5u << Beep_PRESCALER_SHIFT))
#define Beep_PRESCALE_DIVBY64               ((uint32)(6u << Beep_PRESCALER_SHIFT))
#define Beep_PRESCALE_DIVBY128              ((uint32)(7u << Beep_PRESCALER_SHIFT))

/* TCPWM set modes */
#define Beep_MODE_TIMER_COMPARE             ((uint32)(Beep__COMPARE         <<  \
                                                                  Beep_MODE_SHIFT))
#define Beep_MODE_TIMER_CAPTURE             ((uint32)(Beep__CAPTURE         <<  \
                                                                  Beep_MODE_SHIFT))
#define Beep_MODE_QUAD                      ((uint32)(Beep__QUAD            <<  \
                                                                  Beep_MODE_SHIFT))
#define Beep_MODE_PWM                       ((uint32)(Beep__PWM             <<  \
                                                                  Beep_MODE_SHIFT))
#define Beep_MODE_PWM_DT                    ((uint32)(Beep__PWM_DT          <<  \
                                                                  Beep_MODE_SHIFT))
#define Beep_MODE_PWM_PR                    ((uint32)(Beep__PWM_PR          <<  \
                                                                  Beep_MODE_SHIFT))

/* Quad Modes */
#define Beep_MODE_X1                        ((uint32)(Beep__X1              <<  \
                                                                  Beep_QUAD_MODE_SHIFT))
#define Beep_MODE_X2                        ((uint32)(Beep__X2              <<  \
                                                                  Beep_QUAD_MODE_SHIFT))
#define Beep_MODE_X4                        ((uint32)(Beep__X4              <<  \
                                                                  Beep_QUAD_MODE_SHIFT))

/* Counter modes */
#define Beep_COUNT_UP                       ((uint32)(Beep__COUNT_UP        <<  \
                                                                  Beep_UPDOWN_SHIFT))
#define Beep_COUNT_DOWN                     ((uint32)(Beep__COUNT_DOWN      <<  \
                                                                  Beep_UPDOWN_SHIFT))
#define Beep_COUNT_UPDOWN0                  ((uint32)(Beep__COUNT_UPDOWN0   <<  \
                                                                  Beep_UPDOWN_SHIFT))
#define Beep_COUNT_UPDOWN1                  ((uint32)(Beep__COUNT_UPDOWN1   <<  \
                                                                  Beep_UPDOWN_SHIFT))

/* PWM output invert */
#define Beep_INVERT_LINE                    ((uint32)(Beep__INVERSE         <<  \
                                                                  Beep_INV_OUT_SHIFT))
#define Beep_INVERT_LINE_N                  ((uint32)(Beep__INVERSE         <<  \
                                                                  Beep_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define Beep_TRIG_RISING                    ((uint32)Beep__TRIG_RISING)
#define Beep_TRIG_FALLING                   ((uint32)Beep__TRIG_FALLING)
#define Beep_TRIG_BOTH                      ((uint32)Beep__TRIG_BOTH)
#define Beep_TRIG_LEVEL                     ((uint32)Beep__TRIG_LEVEL)

/* Interrupt mask */
#define Beep_INTR_MASK_TC                   ((uint32)Beep__INTR_MASK_TC)
#define Beep_INTR_MASK_CC_MATCH             ((uint32)Beep__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define Beep_CC_MATCH_SET                   (0x00u)
#define Beep_CC_MATCH_CLEAR                 (0x01u)
#define Beep_CC_MATCH_INVERT                (0x02u)
#define Beep_CC_MATCH_NO_CHANGE             (0x03u)
#define Beep_OVERLOW_SET                    (0x00u)
#define Beep_OVERLOW_CLEAR                  (0x04u)
#define Beep_OVERLOW_INVERT                 (0x08u)
#define Beep_OVERLOW_NO_CHANGE              (0x0Cu)
#define Beep_UNDERFLOW_SET                  (0x00u)
#define Beep_UNDERFLOW_CLEAR                (0x10u)
#define Beep_UNDERFLOW_INVERT               (0x20u)
#define Beep_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define Beep_PWM_MODE_LEFT                  (Beep_CC_MATCH_CLEAR        |   \
                                                         Beep_OVERLOW_SET           |   \
                                                         Beep_UNDERFLOW_NO_CHANGE)
#define Beep_PWM_MODE_RIGHT                 (Beep_CC_MATCH_SET          |   \
                                                         Beep_OVERLOW_NO_CHANGE     |   \
                                                         Beep_UNDERFLOW_CLEAR)
#define Beep_PWM_MODE_ASYM                  (Beep_CC_MATCH_INVERT       |   \
                                                         Beep_OVERLOW_SET           |   \
                                                         Beep_UNDERFLOW_CLEAR)

#if (Beep_CY_TCPWM_V2)
    #if(Beep_CY_TCPWM_4000)
        #define Beep_PWM_MODE_CENTER                (Beep_CC_MATCH_INVERT       |   \
                                                                 Beep_OVERLOW_NO_CHANGE     |   \
                                                                 Beep_UNDERFLOW_CLEAR)
    #else
        #define Beep_PWM_MODE_CENTER                (Beep_CC_MATCH_INVERT       |   \
                                                                 Beep_OVERLOW_SET           |   \
                                                                 Beep_UNDERFLOW_CLEAR)
    #endif /* (Beep_CY_TCPWM_4000) */
#else
    #define Beep_PWM_MODE_CENTER                (Beep_CC_MATCH_INVERT       |   \
                                                             Beep_OVERLOW_NO_CHANGE     |   \
                                                             Beep_UNDERFLOW_CLEAR)
#endif /* (Beep_CY_TCPWM_NEW) */

/* Command operations without condition */
#define Beep_CMD_CAPTURE                    (0u)
#define Beep_CMD_RELOAD                     (8u)
#define Beep_CMD_STOP                       (16u)
#define Beep_CMD_START                      (24u)

/* Status */
#define Beep_STATUS_DOWN                    (1u)
#define Beep_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   Beep_Init(void);
void   Beep_Enable(void);
void   Beep_Start(void);
void   Beep_Stop(void);

void   Beep_SetMode(uint32 mode);
void   Beep_SetCounterMode(uint32 counterMode);
void   Beep_SetPWMMode(uint32 modeMask);
void   Beep_SetQDMode(uint32 qdMode);

void   Beep_SetPrescaler(uint32 prescaler);
void   Beep_TriggerCommand(uint32 mask, uint32 command);
void   Beep_SetOneShot(uint32 oneShotEnable);
uint32 Beep_ReadStatus(void);

void   Beep_SetPWMSyncKill(uint32 syncKillEnable);
void   Beep_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   Beep_SetPWMDeadTime(uint32 deadTime);
void   Beep_SetPWMInvert(uint32 mask);

void   Beep_SetInterruptMode(uint32 interruptMask);
uint32 Beep_GetInterruptSourceMasked(void);
uint32 Beep_GetInterruptSource(void);
void   Beep_ClearInterrupt(uint32 interruptMask);
void   Beep_SetInterrupt(uint32 interruptMask);

void   Beep_WriteCounter(uint32 count);
uint32 Beep_ReadCounter(void);

uint32 Beep_ReadCapture(void);
uint32 Beep_ReadCaptureBuf(void);

void   Beep_WritePeriod(uint32 period);
uint32 Beep_ReadPeriod(void);
void   Beep_WritePeriodBuf(uint32 periodBuf);
uint32 Beep_ReadPeriodBuf(void);

void   Beep_WriteCompare(uint32 compare);
uint32 Beep_ReadCompare(void);
void   Beep_WriteCompareBuf(uint32 compareBuf);
uint32 Beep_ReadCompareBuf(void);

void   Beep_SetPeriodSwap(uint32 swapEnable);
void   Beep_SetCompareSwap(uint32 swapEnable);

void   Beep_SetCaptureMode(uint32 triggerMode);
void   Beep_SetReloadMode(uint32 triggerMode);
void   Beep_SetStartMode(uint32 triggerMode);
void   Beep_SetStopMode(uint32 triggerMode);
void   Beep_SetCountMode(uint32 triggerMode);

void   Beep_SaveConfig(void);
void   Beep_RestoreConfig(void);
void   Beep_Sleep(void);
void   Beep_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define Beep_BLOCK_CONTROL_REG              (*(reg32 *) Beep_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define Beep_BLOCK_CONTROL_PTR              ( (reg32 *) Beep_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define Beep_COMMAND_REG                    (*(reg32 *) Beep_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define Beep_COMMAND_PTR                    ( (reg32 *) Beep_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define Beep_INTRRUPT_CAUSE_REG             (*(reg32 *) Beep_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define Beep_INTRRUPT_CAUSE_PTR             ( (reg32 *) Beep_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define Beep_CONTROL_REG                    (*(reg32 *) Beep_cy_m0s8_tcpwm_1__CTRL )
#define Beep_CONTROL_PTR                    ( (reg32 *) Beep_cy_m0s8_tcpwm_1__CTRL )
#define Beep_STATUS_REG                     (*(reg32 *) Beep_cy_m0s8_tcpwm_1__STATUS )
#define Beep_STATUS_PTR                     ( (reg32 *) Beep_cy_m0s8_tcpwm_1__STATUS )
#define Beep_COUNTER_REG                    (*(reg32 *) Beep_cy_m0s8_tcpwm_1__COUNTER )
#define Beep_COUNTER_PTR                    ( (reg32 *) Beep_cy_m0s8_tcpwm_1__COUNTER )
#define Beep_COMP_CAP_REG                   (*(reg32 *) Beep_cy_m0s8_tcpwm_1__CC )
#define Beep_COMP_CAP_PTR                   ( (reg32 *) Beep_cy_m0s8_tcpwm_1__CC )
#define Beep_COMP_CAP_BUF_REG               (*(reg32 *) Beep_cy_m0s8_tcpwm_1__CC_BUFF )
#define Beep_COMP_CAP_BUF_PTR               ( (reg32 *) Beep_cy_m0s8_tcpwm_1__CC_BUFF )
#define Beep_PERIOD_REG                     (*(reg32 *) Beep_cy_m0s8_tcpwm_1__PERIOD )
#define Beep_PERIOD_PTR                     ( (reg32 *) Beep_cy_m0s8_tcpwm_1__PERIOD )
#define Beep_PERIOD_BUF_REG                 (*(reg32 *) Beep_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define Beep_PERIOD_BUF_PTR                 ( (reg32 *) Beep_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define Beep_TRIG_CONTROL0_REG              (*(reg32 *) Beep_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define Beep_TRIG_CONTROL0_PTR              ( (reg32 *) Beep_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define Beep_TRIG_CONTROL1_REG              (*(reg32 *) Beep_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define Beep_TRIG_CONTROL1_PTR              ( (reg32 *) Beep_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define Beep_TRIG_CONTROL2_REG              (*(reg32 *) Beep_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define Beep_TRIG_CONTROL2_PTR              ( (reg32 *) Beep_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define Beep_INTERRUPT_REQ_REG              (*(reg32 *) Beep_cy_m0s8_tcpwm_1__INTR )
#define Beep_INTERRUPT_REQ_PTR              ( (reg32 *) Beep_cy_m0s8_tcpwm_1__INTR )
#define Beep_INTERRUPT_SET_REG              (*(reg32 *) Beep_cy_m0s8_tcpwm_1__INTR_SET )
#define Beep_INTERRUPT_SET_PTR              ( (reg32 *) Beep_cy_m0s8_tcpwm_1__INTR_SET )
#define Beep_INTERRUPT_MASK_REG             (*(reg32 *) Beep_cy_m0s8_tcpwm_1__INTR_MASK )
#define Beep_INTERRUPT_MASK_PTR             ( (reg32 *) Beep_cy_m0s8_tcpwm_1__INTR_MASK )
#define Beep_INTERRUPT_MASKED_REG           (*(reg32 *) Beep_cy_m0s8_tcpwm_1__INTR_MASKED )
#define Beep_INTERRUPT_MASKED_PTR           ( (reg32 *) Beep_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define Beep_MASK                           ((uint32)Beep_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define Beep_RELOAD_CC_SHIFT                (0u)
#define Beep_RELOAD_PERIOD_SHIFT            (1u)
#define Beep_PWM_SYNC_KILL_SHIFT            (2u)
#define Beep_PWM_STOP_KILL_SHIFT            (3u)
#define Beep_PRESCALER_SHIFT                (8u)
#define Beep_UPDOWN_SHIFT                   (16u)
#define Beep_ONESHOT_SHIFT                  (18u)
#define Beep_QUAD_MODE_SHIFT                (20u)
#define Beep_INV_OUT_SHIFT                  (20u)
#define Beep_INV_COMPL_OUT_SHIFT            (21u)
#define Beep_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define Beep_RELOAD_CC_MASK                 ((uint32)(Beep_1BIT_MASK        <<  \
                                                                            Beep_RELOAD_CC_SHIFT))
#define Beep_RELOAD_PERIOD_MASK             ((uint32)(Beep_1BIT_MASK        <<  \
                                                                            Beep_RELOAD_PERIOD_SHIFT))
#define Beep_PWM_SYNC_KILL_MASK             ((uint32)(Beep_1BIT_MASK        <<  \
                                                                            Beep_PWM_SYNC_KILL_SHIFT))
#define Beep_PWM_STOP_KILL_MASK             ((uint32)(Beep_1BIT_MASK        <<  \
                                                                            Beep_PWM_STOP_KILL_SHIFT))
#define Beep_PRESCALER_MASK                 ((uint32)(Beep_8BIT_MASK        <<  \
                                                                            Beep_PRESCALER_SHIFT))
#define Beep_UPDOWN_MASK                    ((uint32)(Beep_2BIT_MASK        <<  \
                                                                            Beep_UPDOWN_SHIFT))
#define Beep_ONESHOT_MASK                   ((uint32)(Beep_1BIT_MASK        <<  \
                                                                            Beep_ONESHOT_SHIFT))
#define Beep_QUAD_MODE_MASK                 ((uint32)(Beep_3BIT_MASK        <<  \
                                                                            Beep_QUAD_MODE_SHIFT))
#define Beep_INV_OUT_MASK                   ((uint32)(Beep_2BIT_MASK        <<  \
                                                                            Beep_INV_OUT_SHIFT))
#define Beep_MODE_MASK                      ((uint32)(Beep_3BIT_MASK        <<  \
                                                                            Beep_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define Beep_CAPTURE_SHIFT                  (0u)
#define Beep_COUNT_SHIFT                    (2u)
#define Beep_RELOAD_SHIFT                   (4u)
#define Beep_STOP_SHIFT                     (6u)
#define Beep_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define Beep_CAPTURE_MASK                   ((uint32)(Beep_2BIT_MASK        <<  \
                                                                  Beep_CAPTURE_SHIFT))
#define Beep_COUNT_MASK                     ((uint32)(Beep_2BIT_MASK        <<  \
                                                                  Beep_COUNT_SHIFT))
#define Beep_RELOAD_MASK                    ((uint32)(Beep_2BIT_MASK        <<  \
                                                                  Beep_RELOAD_SHIFT))
#define Beep_STOP_MASK                      ((uint32)(Beep_2BIT_MASK        <<  \
                                                                  Beep_STOP_SHIFT))
#define Beep_START_MASK                     ((uint32)(Beep_2BIT_MASK        <<  \
                                                                  Beep_START_SHIFT))

/* MASK */
#define Beep_1BIT_MASK                      ((uint32)0x01u)
#define Beep_2BIT_MASK                      ((uint32)0x03u)
#define Beep_3BIT_MASK                      ((uint32)0x07u)
#define Beep_6BIT_MASK                      ((uint32)0x3Fu)
#define Beep_8BIT_MASK                      ((uint32)0xFFu)
#define Beep_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define Beep_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define Beep_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(Beep_QUAD_ENCODING_MODES     << Beep_QUAD_MODE_SHIFT))       |\
         ((uint32)(Beep_CONFIG                  << Beep_MODE_SHIFT)))

#define Beep_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(Beep_PWM_STOP_EVENT          << Beep_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(Beep_PWM_OUT_INVERT          << Beep_INV_OUT_SHIFT))         |\
         ((uint32)(Beep_PWM_OUT_N_INVERT        << Beep_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(Beep_PWM_MODE                << Beep_MODE_SHIFT)))

#define Beep_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(Beep_PWM_RUN_MODE         << Beep_ONESHOT_SHIFT))
            
#define Beep_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(Beep_PWM_ALIGN            << Beep_UPDOWN_SHIFT))

#define Beep_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(Beep_PWM_KILL_EVENT      << Beep_PWM_SYNC_KILL_SHIFT))

#define Beep_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(Beep_PWM_DEAD_TIME_CYCLE  << Beep_PRESCALER_SHIFT))

#define Beep_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(Beep_PWM_PRESCALER        << Beep_PRESCALER_SHIFT))

#define Beep_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(Beep_TC_PRESCALER            << Beep_PRESCALER_SHIFT))       |\
         ((uint32)(Beep_TC_COUNTER_MODE         << Beep_UPDOWN_SHIFT))          |\
         ((uint32)(Beep_TC_RUN_MODE             << Beep_ONESHOT_SHIFT))         |\
         ((uint32)(Beep_TC_COMP_CAP_MODE        << Beep_MODE_SHIFT)))
        
#define Beep_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(Beep_QUAD_PHIA_SIGNAL_MODE   << Beep_COUNT_SHIFT))           |\
         ((uint32)(Beep_QUAD_INDEX_SIGNAL_MODE  << Beep_RELOAD_SHIFT))          |\
         ((uint32)(Beep_QUAD_STOP_SIGNAL_MODE   << Beep_STOP_SHIFT))            |\
         ((uint32)(Beep_QUAD_PHIB_SIGNAL_MODE   << Beep_START_SHIFT)))

#define Beep_PWM_SIGNALS_MODES                                                              \
        (((uint32)(Beep_PWM_SWITCH_SIGNAL_MODE  << Beep_CAPTURE_SHIFT))         |\
         ((uint32)(Beep_PWM_COUNT_SIGNAL_MODE   << Beep_COUNT_SHIFT))           |\
         ((uint32)(Beep_PWM_RELOAD_SIGNAL_MODE  << Beep_RELOAD_SHIFT))          |\
         ((uint32)(Beep_PWM_STOP_SIGNAL_MODE    << Beep_STOP_SHIFT))            |\
         ((uint32)(Beep_PWM_START_SIGNAL_MODE   << Beep_START_SHIFT)))

#define Beep_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(Beep_TC_CAPTURE_SIGNAL_MODE  << Beep_CAPTURE_SHIFT))         |\
         ((uint32)(Beep_TC_COUNT_SIGNAL_MODE    << Beep_COUNT_SHIFT))           |\
         ((uint32)(Beep_TC_RELOAD_SIGNAL_MODE   << Beep_RELOAD_SHIFT))          |\
         ((uint32)(Beep_TC_STOP_SIGNAL_MODE     << Beep_STOP_SHIFT))            |\
         ((uint32)(Beep_TC_START_SIGNAL_MODE    << Beep_START_SHIFT)))
        
#define Beep_TIMER_UPDOWN_CNT_USED                                                          \
                ((Beep__COUNT_UPDOWN0 == Beep_TC_COUNTER_MODE)                  ||\
                 (Beep__COUNT_UPDOWN1 == Beep_TC_COUNTER_MODE))

#define Beep_PWM_UPDOWN_CNT_USED                                                            \
                ((Beep__CENTER == Beep_PWM_ALIGN)                               ||\
                 (Beep__ASYMMETRIC == Beep_PWM_ALIGN))               
        
#define Beep_PWM_PR_INIT_VALUE              (1u)
#define Beep_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_Beep_H */

/* [] END OF FILE */
