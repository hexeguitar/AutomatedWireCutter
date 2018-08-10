/*******************************************************************************
* File Name: Pulser.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the Pulser
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

#if !defined(CY_TCPWM_Pulser_H)
#define CY_TCPWM_Pulser_H


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
} Pulser_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  Pulser_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define Pulser_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define Pulser_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define Pulser_CONFIG                         (7lu)

/* Quad Mode */
/* Parameters */
#define Pulser_QUAD_ENCODING_MODES            (0lu)
#define Pulser_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define Pulser_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define Pulser_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define Pulser_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define Pulser_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define Pulser_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define Pulser_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define Pulser_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define Pulser_TC_RUN_MODE                    (1lu)
#define Pulser_TC_COUNTER_MODE                (0lu)
#define Pulser_TC_COMP_CAP_MODE               (0lu)
#define Pulser_TC_PRESCALER                   (0lu)

/* Signal modes */
#define Pulser_TC_RELOAD_SIGNAL_MODE          (0lu)
#define Pulser_TC_COUNT_SIGNAL_MODE           (3lu)
#define Pulser_TC_START_SIGNAL_MODE           (0lu)
#define Pulser_TC_STOP_SIGNAL_MODE            (0lu)
#define Pulser_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define Pulser_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define Pulser_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define Pulser_TC_START_SIGNAL_PRESENT        (0lu)
#define Pulser_TC_STOP_SIGNAL_PRESENT         (1lu)
#define Pulser_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define Pulser_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define Pulser_PWM_KILL_EVENT                 (0lu)
#define Pulser_PWM_STOP_EVENT                 (1lu)
#define Pulser_PWM_MODE                       (4lu)
#define Pulser_PWM_OUT_N_INVERT               (0lu)
#define Pulser_PWM_OUT_INVERT                 (0lu)
#define Pulser_PWM_ALIGN                      (0lu)
#define Pulser_PWM_RUN_MODE                   (0lu)
#define Pulser_PWM_DEAD_TIME_CYCLE            (0lu)
#define Pulser_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define Pulser_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define Pulser_PWM_COUNT_SIGNAL_MODE          (3lu)
#define Pulser_PWM_START_SIGNAL_MODE          (0lu)
#define Pulser_PWM_STOP_SIGNAL_MODE           (1lu)
#define Pulser_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define Pulser_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define Pulser_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define Pulser_PWM_START_SIGNAL_PRESENT       (0lu)
#define Pulser_PWM_STOP_SIGNAL_PRESENT        (1lu)
#define Pulser_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define Pulser_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define Pulser_TC_PERIOD_VALUE                (100lu)
#define Pulser_TC_COMPARE_VALUE               (50lu)
#define Pulser_TC_COMPARE_BUF_VALUE           (65535lu)
#define Pulser_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define Pulser_PWM_PERIOD_VALUE               (1000lu)
#define Pulser_PWM_PERIOD_BUF_VALUE           (65535lu)
#define Pulser_PWM_PERIOD_SWAP                (0lu)
#define Pulser_PWM_COMPARE_VALUE              (500lu)
#define Pulser_PWM_COMPARE_BUF_VALUE          (65535lu)
#define Pulser_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define Pulser__LEFT 0
#define Pulser__RIGHT 1
#define Pulser__CENTER 2
#define Pulser__ASYMMETRIC 3

#define Pulser__X1 0
#define Pulser__X2 1
#define Pulser__X4 2

#define Pulser__PWM 4
#define Pulser__PWM_DT 5
#define Pulser__PWM_PR 6

#define Pulser__INVERSE 1
#define Pulser__DIRECT 0

#define Pulser__CAPTURE 2
#define Pulser__COMPARE 0

#define Pulser__TRIG_LEVEL 3
#define Pulser__TRIG_RISING 0
#define Pulser__TRIG_FALLING 1
#define Pulser__TRIG_BOTH 2

#define Pulser__INTR_MASK_TC 1
#define Pulser__INTR_MASK_CC_MATCH 2
#define Pulser__INTR_MASK_NONE 0
#define Pulser__INTR_MASK_TC_CC 3

#define Pulser__UNCONFIG 8
#define Pulser__TIMER 1
#define Pulser__QUAD 3
#define Pulser__PWM_SEL 7

#define Pulser__COUNT_UP 0
#define Pulser__COUNT_DOWN 1
#define Pulser__COUNT_UPDOWN0 2
#define Pulser__COUNT_UPDOWN1 3


/* Prescaler */
#define Pulser_PRESCALE_DIVBY1                ((uint32)(0u << Pulser_PRESCALER_SHIFT))
#define Pulser_PRESCALE_DIVBY2                ((uint32)(1u << Pulser_PRESCALER_SHIFT))
#define Pulser_PRESCALE_DIVBY4                ((uint32)(2u << Pulser_PRESCALER_SHIFT))
#define Pulser_PRESCALE_DIVBY8                ((uint32)(3u << Pulser_PRESCALER_SHIFT))
#define Pulser_PRESCALE_DIVBY16               ((uint32)(4u << Pulser_PRESCALER_SHIFT))
#define Pulser_PRESCALE_DIVBY32               ((uint32)(5u << Pulser_PRESCALER_SHIFT))
#define Pulser_PRESCALE_DIVBY64               ((uint32)(6u << Pulser_PRESCALER_SHIFT))
#define Pulser_PRESCALE_DIVBY128              ((uint32)(7u << Pulser_PRESCALER_SHIFT))

/* TCPWM set modes */
#define Pulser_MODE_TIMER_COMPARE             ((uint32)(Pulser__COMPARE         <<  \
                                                                  Pulser_MODE_SHIFT))
#define Pulser_MODE_TIMER_CAPTURE             ((uint32)(Pulser__CAPTURE         <<  \
                                                                  Pulser_MODE_SHIFT))
#define Pulser_MODE_QUAD                      ((uint32)(Pulser__QUAD            <<  \
                                                                  Pulser_MODE_SHIFT))
#define Pulser_MODE_PWM                       ((uint32)(Pulser__PWM             <<  \
                                                                  Pulser_MODE_SHIFT))
#define Pulser_MODE_PWM_DT                    ((uint32)(Pulser__PWM_DT          <<  \
                                                                  Pulser_MODE_SHIFT))
#define Pulser_MODE_PWM_PR                    ((uint32)(Pulser__PWM_PR          <<  \
                                                                  Pulser_MODE_SHIFT))

/* Quad Modes */
#define Pulser_MODE_X1                        ((uint32)(Pulser__X1              <<  \
                                                                  Pulser_QUAD_MODE_SHIFT))
#define Pulser_MODE_X2                        ((uint32)(Pulser__X2              <<  \
                                                                  Pulser_QUAD_MODE_SHIFT))
#define Pulser_MODE_X4                        ((uint32)(Pulser__X4              <<  \
                                                                  Pulser_QUAD_MODE_SHIFT))

/* Counter modes */
#define Pulser_COUNT_UP                       ((uint32)(Pulser__COUNT_UP        <<  \
                                                                  Pulser_UPDOWN_SHIFT))
#define Pulser_COUNT_DOWN                     ((uint32)(Pulser__COUNT_DOWN      <<  \
                                                                  Pulser_UPDOWN_SHIFT))
#define Pulser_COUNT_UPDOWN0                  ((uint32)(Pulser__COUNT_UPDOWN0   <<  \
                                                                  Pulser_UPDOWN_SHIFT))
#define Pulser_COUNT_UPDOWN1                  ((uint32)(Pulser__COUNT_UPDOWN1   <<  \
                                                                  Pulser_UPDOWN_SHIFT))

/* PWM output invert */
#define Pulser_INVERT_LINE                    ((uint32)(Pulser__INVERSE         <<  \
                                                                  Pulser_INV_OUT_SHIFT))
#define Pulser_INVERT_LINE_N                  ((uint32)(Pulser__INVERSE         <<  \
                                                                  Pulser_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define Pulser_TRIG_RISING                    ((uint32)Pulser__TRIG_RISING)
#define Pulser_TRIG_FALLING                   ((uint32)Pulser__TRIG_FALLING)
#define Pulser_TRIG_BOTH                      ((uint32)Pulser__TRIG_BOTH)
#define Pulser_TRIG_LEVEL                     ((uint32)Pulser__TRIG_LEVEL)

/* Interrupt mask */
#define Pulser_INTR_MASK_TC                   ((uint32)Pulser__INTR_MASK_TC)
#define Pulser_INTR_MASK_CC_MATCH             ((uint32)Pulser__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define Pulser_CC_MATCH_SET                   (0x00u)
#define Pulser_CC_MATCH_CLEAR                 (0x01u)
#define Pulser_CC_MATCH_INVERT                (0x02u)
#define Pulser_CC_MATCH_NO_CHANGE             (0x03u)
#define Pulser_OVERLOW_SET                    (0x00u)
#define Pulser_OVERLOW_CLEAR                  (0x04u)
#define Pulser_OVERLOW_INVERT                 (0x08u)
#define Pulser_OVERLOW_NO_CHANGE              (0x0Cu)
#define Pulser_UNDERFLOW_SET                  (0x00u)
#define Pulser_UNDERFLOW_CLEAR                (0x10u)
#define Pulser_UNDERFLOW_INVERT               (0x20u)
#define Pulser_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define Pulser_PWM_MODE_LEFT                  (Pulser_CC_MATCH_CLEAR        |   \
                                                         Pulser_OVERLOW_SET           |   \
                                                         Pulser_UNDERFLOW_NO_CHANGE)
#define Pulser_PWM_MODE_RIGHT                 (Pulser_CC_MATCH_SET          |   \
                                                         Pulser_OVERLOW_NO_CHANGE     |   \
                                                         Pulser_UNDERFLOW_CLEAR)
#define Pulser_PWM_MODE_ASYM                  (Pulser_CC_MATCH_INVERT       |   \
                                                         Pulser_OVERLOW_SET           |   \
                                                         Pulser_UNDERFLOW_CLEAR)

#if (Pulser_CY_TCPWM_V2)
    #if(Pulser_CY_TCPWM_4000)
        #define Pulser_PWM_MODE_CENTER                (Pulser_CC_MATCH_INVERT       |   \
                                                                 Pulser_OVERLOW_NO_CHANGE     |   \
                                                                 Pulser_UNDERFLOW_CLEAR)
    #else
        #define Pulser_PWM_MODE_CENTER                (Pulser_CC_MATCH_INVERT       |   \
                                                                 Pulser_OVERLOW_SET           |   \
                                                                 Pulser_UNDERFLOW_CLEAR)
    #endif /* (Pulser_CY_TCPWM_4000) */
#else
    #define Pulser_PWM_MODE_CENTER                (Pulser_CC_MATCH_INVERT       |   \
                                                             Pulser_OVERLOW_NO_CHANGE     |   \
                                                             Pulser_UNDERFLOW_CLEAR)
#endif /* (Pulser_CY_TCPWM_NEW) */

/* Command operations without condition */
#define Pulser_CMD_CAPTURE                    (0u)
#define Pulser_CMD_RELOAD                     (8u)
#define Pulser_CMD_STOP                       (16u)
#define Pulser_CMD_START                      (24u)

/* Status */
#define Pulser_STATUS_DOWN                    (1u)
#define Pulser_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   Pulser_Init(void);
void   Pulser_Enable(void);
void   Pulser_Start(void);
void   Pulser_Stop(void);

void   Pulser_SetMode(uint32 mode);
void   Pulser_SetCounterMode(uint32 counterMode);
void   Pulser_SetPWMMode(uint32 modeMask);
void   Pulser_SetQDMode(uint32 qdMode);

void   Pulser_SetPrescaler(uint32 prescaler);
void   Pulser_TriggerCommand(uint32 mask, uint32 command);
void   Pulser_SetOneShot(uint32 oneShotEnable);
uint32 Pulser_ReadStatus(void);

void   Pulser_SetPWMSyncKill(uint32 syncKillEnable);
void   Pulser_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   Pulser_SetPWMDeadTime(uint32 deadTime);
void   Pulser_SetPWMInvert(uint32 mask);

void   Pulser_SetInterruptMode(uint32 interruptMask);
uint32 Pulser_GetInterruptSourceMasked(void);
uint32 Pulser_GetInterruptSource(void);
void   Pulser_ClearInterrupt(uint32 interruptMask);
void   Pulser_SetInterrupt(uint32 interruptMask);

void   Pulser_WriteCounter(uint32 count);
uint32 Pulser_ReadCounter(void);

uint32 Pulser_ReadCapture(void);
uint32 Pulser_ReadCaptureBuf(void);

void   Pulser_WritePeriod(uint32 period);
uint32 Pulser_ReadPeriod(void);
void   Pulser_WritePeriodBuf(uint32 periodBuf);
uint32 Pulser_ReadPeriodBuf(void);

void   Pulser_WriteCompare(uint32 compare);
uint32 Pulser_ReadCompare(void);
void   Pulser_WriteCompareBuf(uint32 compareBuf);
uint32 Pulser_ReadCompareBuf(void);

void   Pulser_SetPeriodSwap(uint32 swapEnable);
void   Pulser_SetCompareSwap(uint32 swapEnable);

void   Pulser_SetCaptureMode(uint32 triggerMode);
void   Pulser_SetReloadMode(uint32 triggerMode);
void   Pulser_SetStartMode(uint32 triggerMode);
void   Pulser_SetStopMode(uint32 triggerMode);
void   Pulser_SetCountMode(uint32 triggerMode);

void   Pulser_SaveConfig(void);
void   Pulser_RestoreConfig(void);
void   Pulser_Sleep(void);
void   Pulser_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define Pulser_BLOCK_CONTROL_REG              (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define Pulser_BLOCK_CONTROL_PTR              ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define Pulser_COMMAND_REG                    (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define Pulser_COMMAND_PTR                    ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define Pulser_INTRRUPT_CAUSE_REG             (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define Pulser_INTRRUPT_CAUSE_PTR             ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define Pulser_CONTROL_REG                    (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__CTRL )
#define Pulser_CONTROL_PTR                    ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__CTRL )
#define Pulser_STATUS_REG                     (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__STATUS )
#define Pulser_STATUS_PTR                     ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__STATUS )
#define Pulser_COUNTER_REG                    (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__COUNTER )
#define Pulser_COUNTER_PTR                    ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__COUNTER )
#define Pulser_COMP_CAP_REG                   (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__CC )
#define Pulser_COMP_CAP_PTR                   ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__CC )
#define Pulser_COMP_CAP_BUF_REG               (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__CC_BUFF )
#define Pulser_COMP_CAP_BUF_PTR               ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__CC_BUFF )
#define Pulser_PERIOD_REG                     (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__PERIOD )
#define Pulser_PERIOD_PTR                     ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__PERIOD )
#define Pulser_PERIOD_BUF_REG                 (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define Pulser_PERIOD_BUF_PTR                 ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define Pulser_TRIG_CONTROL0_REG              (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define Pulser_TRIG_CONTROL0_PTR              ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define Pulser_TRIG_CONTROL1_REG              (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define Pulser_TRIG_CONTROL1_PTR              ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define Pulser_TRIG_CONTROL2_REG              (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define Pulser_TRIG_CONTROL2_PTR              ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define Pulser_INTERRUPT_REQ_REG              (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__INTR )
#define Pulser_INTERRUPT_REQ_PTR              ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__INTR )
#define Pulser_INTERRUPT_SET_REG              (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__INTR_SET )
#define Pulser_INTERRUPT_SET_PTR              ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__INTR_SET )
#define Pulser_INTERRUPT_MASK_REG             (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__INTR_MASK )
#define Pulser_INTERRUPT_MASK_PTR             ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__INTR_MASK )
#define Pulser_INTERRUPT_MASKED_REG           (*(reg32 *) Pulser_cy_m0s8_tcpwm_1__INTR_MASKED )
#define Pulser_INTERRUPT_MASKED_PTR           ( (reg32 *) Pulser_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define Pulser_MASK                           ((uint32)Pulser_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define Pulser_RELOAD_CC_SHIFT                (0u)
#define Pulser_RELOAD_PERIOD_SHIFT            (1u)
#define Pulser_PWM_SYNC_KILL_SHIFT            (2u)
#define Pulser_PWM_STOP_KILL_SHIFT            (3u)
#define Pulser_PRESCALER_SHIFT                (8u)
#define Pulser_UPDOWN_SHIFT                   (16u)
#define Pulser_ONESHOT_SHIFT                  (18u)
#define Pulser_QUAD_MODE_SHIFT                (20u)
#define Pulser_INV_OUT_SHIFT                  (20u)
#define Pulser_INV_COMPL_OUT_SHIFT            (21u)
#define Pulser_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define Pulser_RELOAD_CC_MASK                 ((uint32)(Pulser_1BIT_MASK        <<  \
                                                                            Pulser_RELOAD_CC_SHIFT))
#define Pulser_RELOAD_PERIOD_MASK             ((uint32)(Pulser_1BIT_MASK        <<  \
                                                                            Pulser_RELOAD_PERIOD_SHIFT))
#define Pulser_PWM_SYNC_KILL_MASK             ((uint32)(Pulser_1BIT_MASK        <<  \
                                                                            Pulser_PWM_SYNC_KILL_SHIFT))
#define Pulser_PWM_STOP_KILL_MASK             ((uint32)(Pulser_1BIT_MASK        <<  \
                                                                            Pulser_PWM_STOP_KILL_SHIFT))
#define Pulser_PRESCALER_MASK                 ((uint32)(Pulser_8BIT_MASK        <<  \
                                                                            Pulser_PRESCALER_SHIFT))
#define Pulser_UPDOWN_MASK                    ((uint32)(Pulser_2BIT_MASK        <<  \
                                                                            Pulser_UPDOWN_SHIFT))
#define Pulser_ONESHOT_MASK                   ((uint32)(Pulser_1BIT_MASK        <<  \
                                                                            Pulser_ONESHOT_SHIFT))
#define Pulser_QUAD_MODE_MASK                 ((uint32)(Pulser_3BIT_MASK        <<  \
                                                                            Pulser_QUAD_MODE_SHIFT))
#define Pulser_INV_OUT_MASK                   ((uint32)(Pulser_2BIT_MASK        <<  \
                                                                            Pulser_INV_OUT_SHIFT))
#define Pulser_MODE_MASK                      ((uint32)(Pulser_3BIT_MASK        <<  \
                                                                            Pulser_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define Pulser_CAPTURE_SHIFT                  (0u)
#define Pulser_COUNT_SHIFT                    (2u)
#define Pulser_RELOAD_SHIFT                   (4u)
#define Pulser_STOP_SHIFT                     (6u)
#define Pulser_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define Pulser_CAPTURE_MASK                   ((uint32)(Pulser_2BIT_MASK        <<  \
                                                                  Pulser_CAPTURE_SHIFT))
#define Pulser_COUNT_MASK                     ((uint32)(Pulser_2BIT_MASK        <<  \
                                                                  Pulser_COUNT_SHIFT))
#define Pulser_RELOAD_MASK                    ((uint32)(Pulser_2BIT_MASK        <<  \
                                                                  Pulser_RELOAD_SHIFT))
#define Pulser_STOP_MASK                      ((uint32)(Pulser_2BIT_MASK        <<  \
                                                                  Pulser_STOP_SHIFT))
#define Pulser_START_MASK                     ((uint32)(Pulser_2BIT_MASK        <<  \
                                                                  Pulser_START_SHIFT))

/* MASK */
#define Pulser_1BIT_MASK                      ((uint32)0x01u)
#define Pulser_2BIT_MASK                      ((uint32)0x03u)
#define Pulser_3BIT_MASK                      ((uint32)0x07u)
#define Pulser_6BIT_MASK                      ((uint32)0x3Fu)
#define Pulser_8BIT_MASK                      ((uint32)0xFFu)
#define Pulser_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define Pulser_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define Pulser_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(Pulser_QUAD_ENCODING_MODES     << Pulser_QUAD_MODE_SHIFT))       |\
         ((uint32)(Pulser_CONFIG                  << Pulser_MODE_SHIFT)))

#define Pulser_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(Pulser_PWM_STOP_EVENT          << Pulser_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(Pulser_PWM_OUT_INVERT          << Pulser_INV_OUT_SHIFT))         |\
         ((uint32)(Pulser_PWM_OUT_N_INVERT        << Pulser_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(Pulser_PWM_MODE                << Pulser_MODE_SHIFT)))

#define Pulser_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(Pulser_PWM_RUN_MODE         << Pulser_ONESHOT_SHIFT))
            
#define Pulser_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(Pulser_PWM_ALIGN            << Pulser_UPDOWN_SHIFT))

#define Pulser_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(Pulser_PWM_KILL_EVENT      << Pulser_PWM_SYNC_KILL_SHIFT))

#define Pulser_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(Pulser_PWM_DEAD_TIME_CYCLE  << Pulser_PRESCALER_SHIFT))

#define Pulser_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(Pulser_PWM_PRESCALER        << Pulser_PRESCALER_SHIFT))

#define Pulser_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(Pulser_TC_PRESCALER            << Pulser_PRESCALER_SHIFT))       |\
         ((uint32)(Pulser_TC_COUNTER_MODE         << Pulser_UPDOWN_SHIFT))          |\
         ((uint32)(Pulser_TC_RUN_MODE             << Pulser_ONESHOT_SHIFT))         |\
         ((uint32)(Pulser_TC_COMP_CAP_MODE        << Pulser_MODE_SHIFT)))
        
#define Pulser_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(Pulser_QUAD_PHIA_SIGNAL_MODE   << Pulser_COUNT_SHIFT))           |\
         ((uint32)(Pulser_QUAD_INDEX_SIGNAL_MODE  << Pulser_RELOAD_SHIFT))          |\
         ((uint32)(Pulser_QUAD_STOP_SIGNAL_MODE   << Pulser_STOP_SHIFT))            |\
         ((uint32)(Pulser_QUAD_PHIB_SIGNAL_MODE   << Pulser_START_SHIFT)))

#define Pulser_PWM_SIGNALS_MODES                                                              \
        (((uint32)(Pulser_PWM_SWITCH_SIGNAL_MODE  << Pulser_CAPTURE_SHIFT))         |\
         ((uint32)(Pulser_PWM_COUNT_SIGNAL_MODE   << Pulser_COUNT_SHIFT))           |\
         ((uint32)(Pulser_PWM_RELOAD_SIGNAL_MODE  << Pulser_RELOAD_SHIFT))          |\
         ((uint32)(Pulser_PWM_STOP_SIGNAL_MODE    << Pulser_STOP_SHIFT))            |\
         ((uint32)(Pulser_PWM_START_SIGNAL_MODE   << Pulser_START_SHIFT)))

#define Pulser_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(Pulser_TC_CAPTURE_SIGNAL_MODE  << Pulser_CAPTURE_SHIFT))         |\
         ((uint32)(Pulser_TC_COUNT_SIGNAL_MODE    << Pulser_COUNT_SHIFT))           |\
         ((uint32)(Pulser_TC_RELOAD_SIGNAL_MODE   << Pulser_RELOAD_SHIFT))          |\
         ((uint32)(Pulser_TC_STOP_SIGNAL_MODE     << Pulser_STOP_SHIFT))            |\
         ((uint32)(Pulser_TC_START_SIGNAL_MODE    << Pulser_START_SHIFT)))
        
#define Pulser_TIMER_UPDOWN_CNT_USED                                                          \
                ((Pulser__COUNT_UPDOWN0 == Pulser_TC_COUNTER_MODE)                  ||\
                 (Pulser__COUNT_UPDOWN1 == Pulser_TC_COUNTER_MODE))

#define Pulser_PWM_UPDOWN_CNT_USED                                                            \
                ((Pulser__CENTER == Pulser_PWM_ALIGN)                               ||\
                 (Pulser__ASYMMETRIC == Pulser_PWM_ALIGN))               
        
#define Pulser_PWM_PR_INIT_VALUE              (1u)
#define Pulser_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_Pulser_H */

/* [] END OF FILE */
