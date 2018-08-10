/*******************************************************************************
* File Name: Servo_Cut.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the Servo_Cut
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

#if !defined(CY_TCPWM_Servo_Cut_H)
#define CY_TCPWM_Servo_Cut_H


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
} Servo_Cut_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  Servo_Cut_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define Servo_Cut_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define Servo_Cut_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define Servo_Cut_CONFIG                         (7lu)

/* Quad Mode */
/* Parameters */
#define Servo_Cut_QUAD_ENCODING_MODES            (0lu)
#define Servo_Cut_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define Servo_Cut_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define Servo_Cut_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define Servo_Cut_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define Servo_Cut_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define Servo_Cut_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define Servo_Cut_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define Servo_Cut_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define Servo_Cut_TC_RUN_MODE                    (0lu)
#define Servo_Cut_TC_COUNTER_MODE                (0lu)
#define Servo_Cut_TC_COMP_CAP_MODE               (2lu)
#define Servo_Cut_TC_PRESCALER                   (0lu)

/* Signal modes */
#define Servo_Cut_TC_RELOAD_SIGNAL_MODE          (0lu)
#define Servo_Cut_TC_COUNT_SIGNAL_MODE           (3lu)
#define Servo_Cut_TC_START_SIGNAL_MODE           (0lu)
#define Servo_Cut_TC_STOP_SIGNAL_MODE            (0lu)
#define Servo_Cut_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define Servo_Cut_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define Servo_Cut_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define Servo_Cut_TC_START_SIGNAL_PRESENT        (0lu)
#define Servo_Cut_TC_STOP_SIGNAL_PRESENT         (0lu)
#define Servo_Cut_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define Servo_Cut_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define Servo_Cut_PWM_KILL_EVENT                 (0lu)
#define Servo_Cut_PWM_STOP_EVENT                 (0lu)
#define Servo_Cut_PWM_MODE                       (4lu)
#define Servo_Cut_PWM_OUT_N_INVERT               (0lu)
#define Servo_Cut_PWM_OUT_INVERT                 (0lu)
#define Servo_Cut_PWM_ALIGN                      (0lu)
#define Servo_Cut_PWM_RUN_MODE                   (0lu)
#define Servo_Cut_PWM_DEAD_TIME_CYCLE            (0lu)
#define Servo_Cut_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define Servo_Cut_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define Servo_Cut_PWM_COUNT_SIGNAL_MODE          (3lu)
#define Servo_Cut_PWM_START_SIGNAL_MODE          (0lu)
#define Servo_Cut_PWM_STOP_SIGNAL_MODE           (0lu)
#define Servo_Cut_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define Servo_Cut_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define Servo_Cut_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define Servo_Cut_PWM_START_SIGNAL_PRESENT       (0lu)
#define Servo_Cut_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define Servo_Cut_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define Servo_Cut_PWM_INTERRUPT_MASK             (0lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define Servo_Cut_TC_PERIOD_VALUE                (65535lu)
#define Servo_Cut_TC_COMPARE_VALUE               (65535lu)
#define Servo_Cut_TC_COMPARE_BUF_VALUE           (65535lu)
#define Servo_Cut_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define Servo_Cut_PWM_PERIOD_VALUE               (44000lu)
#define Servo_Cut_PWM_PERIOD_BUF_VALUE           (65535lu)
#define Servo_Cut_PWM_PERIOD_SWAP                (0lu)
#define Servo_Cut_PWM_COMPARE_VALUE              (3300lu)
#define Servo_Cut_PWM_COMPARE_BUF_VALUE          (65535lu)
#define Servo_Cut_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define Servo_Cut__LEFT 0
#define Servo_Cut__RIGHT 1
#define Servo_Cut__CENTER 2
#define Servo_Cut__ASYMMETRIC 3

#define Servo_Cut__X1 0
#define Servo_Cut__X2 1
#define Servo_Cut__X4 2

#define Servo_Cut__PWM 4
#define Servo_Cut__PWM_DT 5
#define Servo_Cut__PWM_PR 6

#define Servo_Cut__INVERSE 1
#define Servo_Cut__DIRECT 0

#define Servo_Cut__CAPTURE 2
#define Servo_Cut__COMPARE 0

#define Servo_Cut__TRIG_LEVEL 3
#define Servo_Cut__TRIG_RISING 0
#define Servo_Cut__TRIG_FALLING 1
#define Servo_Cut__TRIG_BOTH 2

#define Servo_Cut__INTR_MASK_TC 1
#define Servo_Cut__INTR_MASK_CC_MATCH 2
#define Servo_Cut__INTR_MASK_NONE 0
#define Servo_Cut__INTR_MASK_TC_CC 3

#define Servo_Cut__UNCONFIG 8
#define Servo_Cut__TIMER 1
#define Servo_Cut__QUAD 3
#define Servo_Cut__PWM_SEL 7

#define Servo_Cut__COUNT_UP 0
#define Servo_Cut__COUNT_DOWN 1
#define Servo_Cut__COUNT_UPDOWN0 2
#define Servo_Cut__COUNT_UPDOWN1 3


/* Prescaler */
#define Servo_Cut_PRESCALE_DIVBY1                ((uint32)(0u << Servo_Cut_PRESCALER_SHIFT))
#define Servo_Cut_PRESCALE_DIVBY2                ((uint32)(1u << Servo_Cut_PRESCALER_SHIFT))
#define Servo_Cut_PRESCALE_DIVBY4                ((uint32)(2u << Servo_Cut_PRESCALER_SHIFT))
#define Servo_Cut_PRESCALE_DIVBY8                ((uint32)(3u << Servo_Cut_PRESCALER_SHIFT))
#define Servo_Cut_PRESCALE_DIVBY16               ((uint32)(4u << Servo_Cut_PRESCALER_SHIFT))
#define Servo_Cut_PRESCALE_DIVBY32               ((uint32)(5u << Servo_Cut_PRESCALER_SHIFT))
#define Servo_Cut_PRESCALE_DIVBY64               ((uint32)(6u << Servo_Cut_PRESCALER_SHIFT))
#define Servo_Cut_PRESCALE_DIVBY128              ((uint32)(7u << Servo_Cut_PRESCALER_SHIFT))

/* TCPWM set modes */
#define Servo_Cut_MODE_TIMER_COMPARE             ((uint32)(Servo_Cut__COMPARE         <<  \
                                                                  Servo_Cut_MODE_SHIFT))
#define Servo_Cut_MODE_TIMER_CAPTURE             ((uint32)(Servo_Cut__CAPTURE         <<  \
                                                                  Servo_Cut_MODE_SHIFT))
#define Servo_Cut_MODE_QUAD                      ((uint32)(Servo_Cut__QUAD            <<  \
                                                                  Servo_Cut_MODE_SHIFT))
#define Servo_Cut_MODE_PWM                       ((uint32)(Servo_Cut__PWM             <<  \
                                                                  Servo_Cut_MODE_SHIFT))
#define Servo_Cut_MODE_PWM_DT                    ((uint32)(Servo_Cut__PWM_DT          <<  \
                                                                  Servo_Cut_MODE_SHIFT))
#define Servo_Cut_MODE_PWM_PR                    ((uint32)(Servo_Cut__PWM_PR          <<  \
                                                                  Servo_Cut_MODE_SHIFT))

/* Quad Modes */
#define Servo_Cut_MODE_X1                        ((uint32)(Servo_Cut__X1              <<  \
                                                                  Servo_Cut_QUAD_MODE_SHIFT))
#define Servo_Cut_MODE_X2                        ((uint32)(Servo_Cut__X2              <<  \
                                                                  Servo_Cut_QUAD_MODE_SHIFT))
#define Servo_Cut_MODE_X4                        ((uint32)(Servo_Cut__X4              <<  \
                                                                  Servo_Cut_QUAD_MODE_SHIFT))

/* Counter modes */
#define Servo_Cut_COUNT_UP                       ((uint32)(Servo_Cut__COUNT_UP        <<  \
                                                                  Servo_Cut_UPDOWN_SHIFT))
#define Servo_Cut_COUNT_DOWN                     ((uint32)(Servo_Cut__COUNT_DOWN      <<  \
                                                                  Servo_Cut_UPDOWN_SHIFT))
#define Servo_Cut_COUNT_UPDOWN0                  ((uint32)(Servo_Cut__COUNT_UPDOWN0   <<  \
                                                                  Servo_Cut_UPDOWN_SHIFT))
#define Servo_Cut_COUNT_UPDOWN1                  ((uint32)(Servo_Cut__COUNT_UPDOWN1   <<  \
                                                                  Servo_Cut_UPDOWN_SHIFT))

/* PWM output invert */
#define Servo_Cut_INVERT_LINE                    ((uint32)(Servo_Cut__INVERSE         <<  \
                                                                  Servo_Cut_INV_OUT_SHIFT))
#define Servo_Cut_INVERT_LINE_N                  ((uint32)(Servo_Cut__INVERSE         <<  \
                                                                  Servo_Cut_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define Servo_Cut_TRIG_RISING                    ((uint32)Servo_Cut__TRIG_RISING)
#define Servo_Cut_TRIG_FALLING                   ((uint32)Servo_Cut__TRIG_FALLING)
#define Servo_Cut_TRIG_BOTH                      ((uint32)Servo_Cut__TRIG_BOTH)
#define Servo_Cut_TRIG_LEVEL                     ((uint32)Servo_Cut__TRIG_LEVEL)

/* Interrupt mask */
#define Servo_Cut_INTR_MASK_TC                   ((uint32)Servo_Cut__INTR_MASK_TC)
#define Servo_Cut_INTR_MASK_CC_MATCH             ((uint32)Servo_Cut__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define Servo_Cut_CC_MATCH_SET                   (0x00u)
#define Servo_Cut_CC_MATCH_CLEAR                 (0x01u)
#define Servo_Cut_CC_MATCH_INVERT                (0x02u)
#define Servo_Cut_CC_MATCH_NO_CHANGE             (0x03u)
#define Servo_Cut_OVERLOW_SET                    (0x00u)
#define Servo_Cut_OVERLOW_CLEAR                  (0x04u)
#define Servo_Cut_OVERLOW_INVERT                 (0x08u)
#define Servo_Cut_OVERLOW_NO_CHANGE              (0x0Cu)
#define Servo_Cut_UNDERFLOW_SET                  (0x00u)
#define Servo_Cut_UNDERFLOW_CLEAR                (0x10u)
#define Servo_Cut_UNDERFLOW_INVERT               (0x20u)
#define Servo_Cut_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define Servo_Cut_PWM_MODE_LEFT                  (Servo_Cut_CC_MATCH_CLEAR        |   \
                                                         Servo_Cut_OVERLOW_SET           |   \
                                                         Servo_Cut_UNDERFLOW_NO_CHANGE)
#define Servo_Cut_PWM_MODE_RIGHT                 (Servo_Cut_CC_MATCH_SET          |   \
                                                         Servo_Cut_OVERLOW_NO_CHANGE     |   \
                                                         Servo_Cut_UNDERFLOW_CLEAR)
#define Servo_Cut_PWM_MODE_ASYM                  (Servo_Cut_CC_MATCH_INVERT       |   \
                                                         Servo_Cut_OVERLOW_SET           |   \
                                                         Servo_Cut_UNDERFLOW_CLEAR)

#if (Servo_Cut_CY_TCPWM_V2)
    #if(Servo_Cut_CY_TCPWM_4000)
        #define Servo_Cut_PWM_MODE_CENTER                (Servo_Cut_CC_MATCH_INVERT       |   \
                                                                 Servo_Cut_OVERLOW_NO_CHANGE     |   \
                                                                 Servo_Cut_UNDERFLOW_CLEAR)
    #else
        #define Servo_Cut_PWM_MODE_CENTER                (Servo_Cut_CC_MATCH_INVERT       |   \
                                                                 Servo_Cut_OVERLOW_SET           |   \
                                                                 Servo_Cut_UNDERFLOW_CLEAR)
    #endif /* (Servo_Cut_CY_TCPWM_4000) */
#else
    #define Servo_Cut_PWM_MODE_CENTER                (Servo_Cut_CC_MATCH_INVERT       |   \
                                                             Servo_Cut_OVERLOW_NO_CHANGE     |   \
                                                             Servo_Cut_UNDERFLOW_CLEAR)
#endif /* (Servo_Cut_CY_TCPWM_NEW) */

/* Command operations without condition */
#define Servo_Cut_CMD_CAPTURE                    (0u)
#define Servo_Cut_CMD_RELOAD                     (8u)
#define Servo_Cut_CMD_STOP                       (16u)
#define Servo_Cut_CMD_START                      (24u)

/* Status */
#define Servo_Cut_STATUS_DOWN                    (1u)
#define Servo_Cut_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   Servo_Cut_Init(void);
void   Servo_Cut_Enable(void);
void   Servo_Cut_Start(void);
void   Servo_Cut_Stop(void);

void   Servo_Cut_SetMode(uint32 mode);
void   Servo_Cut_SetCounterMode(uint32 counterMode);
void   Servo_Cut_SetPWMMode(uint32 modeMask);
void   Servo_Cut_SetQDMode(uint32 qdMode);

void   Servo_Cut_SetPrescaler(uint32 prescaler);
void   Servo_Cut_TriggerCommand(uint32 mask, uint32 command);
void   Servo_Cut_SetOneShot(uint32 oneShotEnable);
uint32 Servo_Cut_ReadStatus(void);

void   Servo_Cut_SetPWMSyncKill(uint32 syncKillEnable);
void   Servo_Cut_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   Servo_Cut_SetPWMDeadTime(uint32 deadTime);
void   Servo_Cut_SetPWMInvert(uint32 mask);

void   Servo_Cut_SetInterruptMode(uint32 interruptMask);
uint32 Servo_Cut_GetInterruptSourceMasked(void);
uint32 Servo_Cut_GetInterruptSource(void);
void   Servo_Cut_ClearInterrupt(uint32 interruptMask);
void   Servo_Cut_SetInterrupt(uint32 interruptMask);

void   Servo_Cut_WriteCounter(uint32 count);
uint32 Servo_Cut_ReadCounter(void);

uint32 Servo_Cut_ReadCapture(void);
uint32 Servo_Cut_ReadCaptureBuf(void);

void   Servo_Cut_WritePeriod(uint32 period);
uint32 Servo_Cut_ReadPeriod(void);
void   Servo_Cut_WritePeriodBuf(uint32 periodBuf);
uint32 Servo_Cut_ReadPeriodBuf(void);

void   Servo_Cut_WriteCompare(uint32 compare);
uint32 Servo_Cut_ReadCompare(void);
void   Servo_Cut_WriteCompareBuf(uint32 compareBuf);
uint32 Servo_Cut_ReadCompareBuf(void);

void   Servo_Cut_SetPeriodSwap(uint32 swapEnable);
void   Servo_Cut_SetCompareSwap(uint32 swapEnable);

void   Servo_Cut_SetCaptureMode(uint32 triggerMode);
void   Servo_Cut_SetReloadMode(uint32 triggerMode);
void   Servo_Cut_SetStartMode(uint32 triggerMode);
void   Servo_Cut_SetStopMode(uint32 triggerMode);
void   Servo_Cut_SetCountMode(uint32 triggerMode);

void   Servo_Cut_SaveConfig(void);
void   Servo_Cut_RestoreConfig(void);
void   Servo_Cut_Sleep(void);
void   Servo_Cut_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define Servo_Cut_BLOCK_CONTROL_REG              (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define Servo_Cut_BLOCK_CONTROL_PTR              ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define Servo_Cut_COMMAND_REG                    (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define Servo_Cut_COMMAND_PTR                    ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define Servo_Cut_INTRRUPT_CAUSE_REG             (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define Servo_Cut_INTRRUPT_CAUSE_PTR             ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define Servo_Cut_CONTROL_REG                    (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__CTRL )
#define Servo_Cut_CONTROL_PTR                    ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__CTRL )
#define Servo_Cut_STATUS_REG                     (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__STATUS )
#define Servo_Cut_STATUS_PTR                     ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__STATUS )
#define Servo_Cut_COUNTER_REG                    (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__COUNTER )
#define Servo_Cut_COUNTER_PTR                    ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__COUNTER )
#define Servo_Cut_COMP_CAP_REG                   (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__CC )
#define Servo_Cut_COMP_CAP_PTR                   ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__CC )
#define Servo_Cut_COMP_CAP_BUF_REG               (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__CC_BUFF )
#define Servo_Cut_COMP_CAP_BUF_PTR               ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__CC_BUFF )
#define Servo_Cut_PERIOD_REG                     (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__PERIOD )
#define Servo_Cut_PERIOD_PTR                     ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__PERIOD )
#define Servo_Cut_PERIOD_BUF_REG                 (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define Servo_Cut_PERIOD_BUF_PTR                 ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define Servo_Cut_TRIG_CONTROL0_REG              (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define Servo_Cut_TRIG_CONTROL0_PTR              ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define Servo_Cut_TRIG_CONTROL1_REG              (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define Servo_Cut_TRIG_CONTROL1_PTR              ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define Servo_Cut_TRIG_CONTROL2_REG              (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define Servo_Cut_TRIG_CONTROL2_PTR              ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define Servo_Cut_INTERRUPT_REQ_REG              (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__INTR )
#define Servo_Cut_INTERRUPT_REQ_PTR              ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__INTR )
#define Servo_Cut_INTERRUPT_SET_REG              (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__INTR_SET )
#define Servo_Cut_INTERRUPT_SET_PTR              ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__INTR_SET )
#define Servo_Cut_INTERRUPT_MASK_REG             (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__INTR_MASK )
#define Servo_Cut_INTERRUPT_MASK_PTR             ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__INTR_MASK )
#define Servo_Cut_INTERRUPT_MASKED_REG           (*(reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__INTR_MASKED )
#define Servo_Cut_INTERRUPT_MASKED_PTR           ( (reg32 *) Servo_Cut_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define Servo_Cut_MASK                           ((uint32)Servo_Cut_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define Servo_Cut_RELOAD_CC_SHIFT                (0u)
#define Servo_Cut_RELOAD_PERIOD_SHIFT            (1u)
#define Servo_Cut_PWM_SYNC_KILL_SHIFT            (2u)
#define Servo_Cut_PWM_STOP_KILL_SHIFT            (3u)
#define Servo_Cut_PRESCALER_SHIFT                (8u)
#define Servo_Cut_UPDOWN_SHIFT                   (16u)
#define Servo_Cut_ONESHOT_SHIFT                  (18u)
#define Servo_Cut_QUAD_MODE_SHIFT                (20u)
#define Servo_Cut_INV_OUT_SHIFT                  (20u)
#define Servo_Cut_INV_COMPL_OUT_SHIFT            (21u)
#define Servo_Cut_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define Servo_Cut_RELOAD_CC_MASK                 ((uint32)(Servo_Cut_1BIT_MASK        <<  \
                                                                            Servo_Cut_RELOAD_CC_SHIFT))
#define Servo_Cut_RELOAD_PERIOD_MASK             ((uint32)(Servo_Cut_1BIT_MASK        <<  \
                                                                            Servo_Cut_RELOAD_PERIOD_SHIFT))
#define Servo_Cut_PWM_SYNC_KILL_MASK             ((uint32)(Servo_Cut_1BIT_MASK        <<  \
                                                                            Servo_Cut_PWM_SYNC_KILL_SHIFT))
#define Servo_Cut_PWM_STOP_KILL_MASK             ((uint32)(Servo_Cut_1BIT_MASK        <<  \
                                                                            Servo_Cut_PWM_STOP_KILL_SHIFT))
#define Servo_Cut_PRESCALER_MASK                 ((uint32)(Servo_Cut_8BIT_MASK        <<  \
                                                                            Servo_Cut_PRESCALER_SHIFT))
#define Servo_Cut_UPDOWN_MASK                    ((uint32)(Servo_Cut_2BIT_MASK        <<  \
                                                                            Servo_Cut_UPDOWN_SHIFT))
#define Servo_Cut_ONESHOT_MASK                   ((uint32)(Servo_Cut_1BIT_MASK        <<  \
                                                                            Servo_Cut_ONESHOT_SHIFT))
#define Servo_Cut_QUAD_MODE_MASK                 ((uint32)(Servo_Cut_3BIT_MASK        <<  \
                                                                            Servo_Cut_QUAD_MODE_SHIFT))
#define Servo_Cut_INV_OUT_MASK                   ((uint32)(Servo_Cut_2BIT_MASK        <<  \
                                                                            Servo_Cut_INV_OUT_SHIFT))
#define Servo_Cut_MODE_MASK                      ((uint32)(Servo_Cut_3BIT_MASK        <<  \
                                                                            Servo_Cut_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define Servo_Cut_CAPTURE_SHIFT                  (0u)
#define Servo_Cut_COUNT_SHIFT                    (2u)
#define Servo_Cut_RELOAD_SHIFT                   (4u)
#define Servo_Cut_STOP_SHIFT                     (6u)
#define Servo_Cut_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define Servo_Cut_CAPTURE_MASK                   ((uint32)(Servo_Cut_2BIT_MASK        <<  \
                                                                  Servo_Cut_CAPTURE_SHIFT))
#define Servo_Cut_COUNT_MASK                     ((uint32)(Servo_Cut_2BIT_MASK        <<  \
                                                                  Servo_Cut_COUNT_SHIFT))
#define Servo_Cut_RELOAD_MASK                    ((uint32)(Servo_Cut_2BIT_MASK        <<  \
                                                                  Servo_Cut_RELOAD_SHIFT))
#define Servo_Cut_STOP_MASK                      ((uint32)(Servo_Cut_2BIT_MASK        <<  \
                                                                  Servo_Cut_STOP_SHIFT))
#define Servo_Cut_START_MASK                     ((uint32)(Servo_Cut_2BIT_MASK        <<  \
                                                                  Servo_Cut_START_SHIFT))

/* MASK */
#define Servo_Cut_1BIT_MASK                      ((uint32)0x01u)
#define Servo_Cut_2BIT_MASK                      ((uint32)0x03u)
#define Servo_Cut_3BIT_MASK                      ((uint32)0x07u)
#define Servo_Cut_6BIT_MASK                      ((uint32)0x3Fu)
#define Servo_Cut_8BIT_MASK                      ((uint32)0xFFu)
#define Servo_Cut_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define Servo_Cut_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define Servo_Cut_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(Servo_Cut_QUAD_ENCODING_MODES     << Servo_Cut_QUAD_MODE_SHIFT))       |\
         ((uint32)(Servo_Cut_CONFIG                  << Servo_Cut_MODE_SHIFT)))

#define Servo_Cut_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(Servo_Cut_PWM_STOP_EVENT          << Servo_Cut_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(Servo_Cut_PWM_OUT_INVERT          << Servo_Cut_INV_OUT_SHIFT))         |\
         ((uint32)(Servo_Cut_PWM_OUT_N_INVERT        << Servo_Cut_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(Servo_Cut_PWM_MODE                << Servo_Cut_MODE_SHIFT)))

#define Servo_Cut_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(Servo_Cut_PWM_RUN_MODE         << Servo_Cut_ONESHOT_SHIFT))
            
#define Servo_Cut_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(Servo_Cut_PWM_ALIGN            << Servo_Cut_UPDOWN_SHIFT))

#define Servo_Cut_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(Servo_Cut_PWM_KILL_EVENT      << Servo_Cut_PWM_SYNC_KILL_SHIFT))

#define Servo_Cut_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(Servo_Cut_PWM_DEAD_TIME_CYCLE  << Servo_Cut_PRESCALER_SHIFT))

#define Servo_Cut_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(Servo_Cut_PWM_PRESCALER        << Servo_Cut_PRESCALER_SHIFT))

#define Servo_Cut_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(Servo_Cut_TC_PRESCALER            << Servo_Cut_PRESCALER_SHIFT))       |\
         ((uint32)(Servo_Cut_TC_COUNTER_MODE         << Servo_Cut_UPDOWN_SHIFT))          |\
         ((uint32)(Servo_Cut_TC_RUN_MODE             << Servo_Cut_ONESHOT_SHIFT))         |\
         ((uint32)(Servo_Cut_TC_COMP_CAP_MODE        << Servo_Cut_MODE_SHIFT)))
        
#define Servo_Cut_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(Servo_Cut_QUAD_PHIA_SIGNAL_MODE   << Servo_Cut_COUNT_SHIFT))           |\
         ((uint32)(Servo_Cut_QUAD_INDEX_SIGNAL_MODE  << Servo_Cut_RELOAD_SHIFT))          |\
         ((uint32)(Servo_Cut_QUAD_STOP_SIGNAL_MODE   << Servo_Cut_STOP_SHIFT))            |\
         ((uint32)(Servo_Cut_QUAD_PHIB_SIGNAL_MODE   << Servo_Cut_START_SHIFT)))

#define Servo_Cut_PWM_SIGNALS_MODES                                                              \
        (((uint32)(Servo_Cut_PWM_SWITCH_SIGNAL_MODE  << Servo_Cut_CAPTURE_SHIFT))         |\
         ((uint32)(Servo_Cut_PWM_COUNT_SIGNAL_MODE   << Servo_Cut_COUNT_SHIFT))           |\
         ((uint32)(Servo_Cut_PWM_RELOAD_SIGNAL_MODE  << Servo_Cut_RELOAD_SHIFT))          |\
         ((uint32)(Servo_Cut_PWM_STOP_SIGNAL_MODE    << Servo_Cut_STOP_SHIFT))            |\
         ((uint32)(Servo_Cut_PWM_START_SIGNAL_MODE   << Servo_Cut_START_SHIFT)))

#define Servo_Cut_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(Servo_Cut_TC_CAPTURE_SIGNAL_MODE  << Servo_Cut_CAPTURE_SHIFT))         |\
         ((uint32)(Servo_Cut_TC_COUNT_SIGNAL_MODE    << Servo_Cut_COUNT_SHIFT))           |\
         ((uint32)(Servo_Cut_TC_RELOAD_SIGNAL_MODE   << Servo_Cut_RELOAD_SHIFT))          |\
         ((uint32)(Servo_Cut_TC_STOP_SIGNAL_MODE     << Servo_Cut_STOP_SHIFT))            |\
         ((uint32)(Servo_Cut_TC_START_SIGNAL_MODE    << Servo_Cut_START_SHIFT)))
        
#define Servo_Cut_TIMER_UPDOWN_CNT_USED                                                          \
                ((Servo_Cut__COUNT_UPDOWN0 == Servo_Cut_TC_COUNTER_MODE)                  ||\
                 (Servo_Cut__COUNT_UPDOWN1 == Servo_Cut_TC_COUNTER_MODE))

#define Servo_Cut_PWM_UPDOWN_CNT_USED                                                            \
                ((Servo_Cut__CENTER == Servo_Cut_PWM_ALIGN)                               ||\
                 (Servo_Cut__ASYMMETRIC == Servo_Cut_PWM_ALIGN))               
        
#define Servo_Cut_PWM_PR_INIT_VALUE              (1u)
#define Servo_Cut_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_Servo_Cut_H */

/* [] END OF FILE */
