/*******************************************************************************
* File Name: WireSnsTimeout.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the WireSnsTimeout
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

#if !defined(CY_TCPWM_WireSnsTimeout_H)
#define CY_TCPWM_WireSnsTimeout_H


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
} WireSnsTimeout_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  WireSnsTimeout_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define WireSnsTimeout_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define WireSnsTimeout_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define WireSnsTimeout_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define WireSnsTimeout_QUAD_ENCODING_MODES            (0lu)
#define WireSnsTimeout_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define WireSnsTimeout_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define WireSnsTimeout_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define WireSnsTimeout_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define WireSnsTimeout_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define WireSnsTimeout_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define WireSnsTimeout_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define WireSnsTimeout_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define WireSnsTimeout_TC_RUN_MODE                    (1lu)
#define WireSnsTimeout_TC_COUNTER_MODE                (0lu)
#define WireSnsTimeout_TC_COMP_CAP_MODE               (0lu)
#define WireSnsTimeout_TC_PRESCALER                   (0lu)

/* Signal modes */
#define WireSnsTimeout_TC_RELOAD_SIGNAL_MODE          (3lu)
#define WireSnsTimeout_TC_COUNT_SIGNAL_MODE           (3lu)
#define WireSnsTimeout_TC_START_SIGNAL_MODE           (0lu)
#define WireSnsTimeout_TC_STOP_SIGNAL_MODE            (0lu)
#define WireSnsTimeout_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define WireSnsTimeout_TC_RELOAD_SIGNAL_PRESENT       (1lu)
#define WireSnsTimeout_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define WireSnsTimeout_TC_START_SIGNAL_PRESENT        (0lu)
#define WireSnsTimeout_TC_STOP_SIGNAL_PRESENT         (0lu)
#define WireSnsTimeout_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define WireSnsTimeout_TC_INTERRUPT_MASK              (0lu)

/* PWM Mode */
/* Parameters */
#define WireSnsTimeout_PWM_KILL_EVENT                 (0lu)
#define WireSnsTimeout_PWM_STOP_EVENT                 (0lu)
#define WireSnsTimeout_PWM_MODE                       (4lu)
#define WireSnsTimeout_PWM_OUT_N_INVERT               (0lu)
#define WireSnsTimeout_PWM_OUT_INVERT                 (0lu)
#define WireSnsTimeout_PWM_ALIGN                      (0lu)
#define WireSnsTimeout_PWM_RUN_MODE                   (0lu)
#define WireSnsTimeout_PWM_DEAD_TIME_CYCLE            (0lu)
#define WireSnsTimeout_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define WireSnsTimeout_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define WireSnsTimeout_PWM_COUNT_SIGNAL_MODE          (3lu)
#define WireSnsTimeout_PWM_START_SIGNAL_MODE          (0lu)
#define WireSnsTimeout_PWM_STOP_SIGNAL_MODE           (0lu)
#define WireSnsTimeout_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define WireSnsTimeout_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define WireSnsTimeout_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define WireSnsTimeout_PWM_START_SIGNAL_PRESENT       (0lu)
#define WireSnsTimeout_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define WireSnsTimeout_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define WireSnsTimeout_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define WireSnsTimeout_TC_PERIOD_VALUE                (65535lu)
#define WireSnsTimeout_TC_COMPARE_VALUE               (200lu)
#define WireSnsTimeout_TC_COMPARE_BUF_VALUE           (65535lu)
#define WireSnsTimeout_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define WireSnsTimeout_PWM_PERIOD_VALUE               (65535lu)
#define WireSnsTimeout_PWM_PERIOD_BUF_VALUE           (65535lu)
#define WireSnsTimeout_PWM_PERIOD_SWAP                (0lu)
#define WireSnsTimeout_PWM_COMPARE_VALUE              (65535lu)
#define WireSnsTimeout_PWM_COMPARE_BUF_VALUE          (65535lu)
#define WireSnsTimeout_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define WireSnsTimeout__LEFT 0
#define WireSnsTimeout__RIGHT 1
#define WireSnsTimeout__CENTER 2
#define WireSnsTimeout__ASYMMETRIC 3

#define WireSnsTimeout__X1 0
#define WireSnsTimeout__X2 1
#define WireSnsTimeout__X4 2

#define WireSnsTimeout__PWM 4
#define WireSnsTimeout__PWM_DT 5
#define WireSnsTimeout__PWM_PR 6

#define WireSnsTimeout__INVERSE 1
#define WireSnsTimeout__DIRECT 0

#define WireSnsTimeout__CAPTURE 2
#define WireSnsTimeout__COMPARE 0

#define WireSnsTimeout__TRIG_LEVEL 3
#define WireSnsTimeout__TRIG_RISING 0
#define WireSnsTimeout__TRIG_FALLING 1
#define WireSnsTimeout__TRIG_BOTH 2

#define WireSnsTimeout__INTR_MASK_TC 1
#define WireSnsTimeout__INTR_MASK_CC_MATCH 2
#define WireSnsTimeout__INTR_MASK_NONE 0
#define WireSnsTimeout__INTR_MASK_TC_CC 3

#define WireSnsTimeout__UNCONFIG 8
#define WireSnsTimeout__TIMER 1
#define WireSnsTimeout__QUAD 3
#define WireSnsTimeout__PWM_SEL 7

#define WireSnsTimeout__COUNT_UP 0
#define WireSnsTimeout__COUNT_DOWN 1
#define WireSnsTimeout__COUNT_UPDOWN0 2
#define WireSnsTimeout__COUNT_UPDOWN1 3


/* Prescaler */
#define WireSnsTimeout_PRESCALE_DIVBY1                ((uint32)(0u << WireSnsTimeout_PRESCALER_SHIFT))
#define WireSnsTimeout_PRESCALE_DIVBY2                ((uint32)(1u << WireSnsTimeout_PRESCALER_SHIFT))
#define WireSnsTimeout_PRESCALE_DIVBY4                ((uint32)(2u << WireSnsTimeout_PRESCALER_SHIFT))
#define WireSnsTimeout_PRESCALE_DIVBY8                ((uint32)(3u << WireSnsTimeout_PRESCALER_SHIFT))
#define WireSnsTimeout_PRESCALE_DIVBY16               ((uint32)(4u << WireSnsTimeout_PRESCALER_SHIFT))
#define WireSnsTimeout_PRESCALE_DIVBY32               ((uint32)(5u << WireSnsTimeout_PRESCALER_SHIFT))
#define WireSnsTimeout_PRESCALE_DIVBY64               ((uint32)(6u << WireSnsTimeout_PRESCALER_SHIFT))
#define WireSnsTimeout_PRESCALE_DIVBY128              ((uint32)(7u << WireSnsTimeout_PRESCALER_SHIFT))

/* TCPWM set modes */
#define WireSnsTimeout_MODE_TIMER_COMPARE             ((uint32)(WireSnsTimeout__COMPARE         <<  \
                                                                  WireSnsTimeout_MODE_SHIFT))
#define WireSnsTimeout_MODE_TIMER_CAPTURE             ((uint32)(WireSnsTimeout__CAPTURE         <<  \
                                                                  WireSnsTimeout_MODE_SHIFT))
#define WireSnsTimeout_MODE_QUAD                      ((uint32)(WireSnsTimeout__QUAD            <<  \
                                                                  WireSnsTimeout_MODE_SHIFT))
#define WireSnsTimeout_MODE_PWM                       ((uint32)(WireSnsTimeout__PWM             <<  \
                                                                  WireSnsTimeout_MODE_SHIFT))
#define WireSnsTimeout_MODE_PWM_DT                    ((uint32)(WireSnsTimeout__PWM_DT          <<  \
                                                                  WireSnsTimeout_MODE_SHIFT))
#define WireSnsTimeout_MODE_PWM_PR                    ((uint32)(WireSnsTimeout__PWM_PR          <<  \
                                                                  WireSnsTimeout_MODE_SHIFT))

/* Quad Modes */
#define WireSnsTimeout_MODE_X1                        ((uint32)(WireSnsTimeout__X1              <<  \
                                                                  WireSnsTimeout_QUAD_MODE_SHIFT))
#define WireSnsTimeout_MODE_X2                        ((uint32)(WireSnsTimeout__X2              <<  \
                                                                  WireSnsTimeout_QUAD_MODE_SHIFT))
#define WireSnsTimeout_MODE_X4                        ((uint32)(WireSnsTimeout__X4              <<  \
                                                                  WireSnsTimeout_QUAD_MODE_SHIFT))

/* Counter modes */
#define WireSnsTimeout_COUNT_UP                       ((uint32)(WireSnsTimeout__COUNT_UP        <<  \
                                                                  WireSnsTimeout_UPDOWN_SHIFT))
#define WireSnsTimeout_COUNT_DOWN                     ((uint32)(WireSnsTimeout__COUNT_DOWN      <<  \
                                                                  WireSnsTimeout_UPDOWN_SHIFT))
#define WireSnsTimeout_COUNT_UPDOWN0                  ((uint32)(WireSnsTimeout__COUNT_UPDOWN0   <<  \
                                                                  WireSnsTimeout_UPDOWN_SHIFT))
#define WireSnsTimeout_COUNT_UPDOWN1                  ((uint32)(WireSnsTimeout__COUNT_UPDOWN1   <<  \
                                                                  WireSnsTimeout_UPDOWN_SHIFT))

/* PWM output invert */
#define WireSnsTimeout_INVERT_LINE                    ((uint32)(WireSnsTimeout__INVERSE         <<  \
                                                                  WireSnsTimeout_INV_OUT_SHIFT))
#define WireSnsTimeout_INVERT_LINE_N                  ((uint32)(WireSnsTimeout__INVERSE         <<  \
                                                                  WireSnsTimeout_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define WireSnsTimeout_TRIG_RISING                    ((uint32)WireSnsTimeout__TRIG_RISING)
#define WireSnsTimeout_TRIG_FALLING                   ((uint32)WireSnsTimeout__TRIG_FALLING)
#define WireSnsTimeout_TRIG_BOTH                      ((uint32)WireSnsTimeout__TRIG_BOTH)
#define WireSnsTimeout_TRIG_LEVEL                     ((uint32)WireSnsTimeout__TRIG_LEVEL)

/* Interrupt mask */
#define WireSnsTimeout_INTR_MASK_TC                   ((uint32)WireSnsTimeout__INTR_MASK_TC)
#define WireSnsTimeout_INTR_MASK_CC_MATCH             ((uint32)WireSnsTimeout__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define WireSnsTimeout_CC_MATCH_SET                   (0x00u)
#define WireSnsTimeout_CC_MATCH_CLEAR                 (0x01u)
#define WireSnsTimeout_CC_MATCH_INVERT                (0x02u)
#define WireSnsTimeout_CC_MATCH_NO_CHANGE             (0x03u)
#define WireSnsTimeout_OVERLOW_SET                    (0x00u)
#define WireSnsTimeout_OVERLOW_CLEAR                  (0x04u)
#define WireSnsTimeout_OVERLOW_INVERT                 (0x08u)
#define WireSnsTimeout_OVERLOW_NO_CHANGE              (0x0Cu)
#define WireSnsTimeout_UNDERFLOW_SET                  (0x00u)
#define WireSnsTimeout_UNDERFLOW_CLEAR                (0x10u)
#define WireSnsTimeout_UNDERFLOW_INVERT               (0x20u)
#define WireSnsTimeout_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define WireSnsTimeout_PWM_MODE_LEFT                  (WireSnsTimeout_CC_MATCH_CLEAR        |   \
                                                         WireSnsTimeout_OVERLOW_SET           |   \
                                                         WireSnsTimeout_UNDERFLOW_NO_CHANGE)
#define WireSnsTimeout_PWM_MODE_RIGHT                 (WireSnsTimeout_CC_MATCH_SET          |   \
                                                         WireSnsTimeout_OVERLOW_NO_CHANGE     |   \
                                                         WireSnsTimeout_UNDERFLOW_CLEAR)
#define WireSnsTimeout_PWM_MODE_ASYM                  (WireSnsTimeout_CC_MATCH_INVERT       |   \
                                                         WireSnsTimeout_OVERLOW_SET           |   \
                                                         WireSnsTimeout_UNDERFLOW_CLEAR)

#if (WireSnsTimeout_CY_TCPWM_V2)
    #if(WireSnsTimeout_CY_TCPWM_4000)
        #define WireSnsTimeout_PWM_MODE_CENTER                (WireSnsTimeout_CC_MATCH_INVERT       |   \
                                                                 WireSnsTimeout_OVERLOW_NO_CHANGE     |   \
                                                                 WireSnsTimeout_UNDERFLOW_CLEAR)
    #else
        #define WireSnsTimeout_PWM_MODE_CENTER                (WireSnsTimeout_CC_MATCH_INVERT       |   \
                                                                 WireSnsTimeout_OVERLOW_SET           |   \
                                                                 WireSnsTimeout_UNDERFLOW_CLEAR)
    #endif /* (WireSnsTimeout_CY_TCPWM_4000) */
#else
    #define WireSnsTimeout_PWM_MODE_CENTER                (WireSnsTimeout_CC_MATCH_INVERT       |   \
                                                             WireSnsTimeout_OVERLOW_NO_CHANGE     |   \
                                                             WireSnsTimeout_UNDERFLOW_CLEAR)
#endif /* (WireSnsTimeout_CY_TCPWM_NEW) */

/* Command operations without condition */
#define WireSnsTimeout_CMD_CAPTURE                    (0u)
#define WireSnsTimeout_CMD_RELOAD                     (8u)
#define WireSnsTimeout_CMD_STOP                       (16u)
#define WireSnsTimeout_CMD_START                      (24u)

/* Status */
#define WireSnsTimeout_STATUS_DOWN                    (1u)
#define WireSnsTimeout_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   WireSnsTimeout_Init(void);
void   WireSnsTimeout_Enable(void);
void   WireSnsTimeout_Start(void);
void   WireSnsTimeout_Stop(void);

void   WireSnsTimeout_SetMode(uint32 mode);
void   WireSnsTimeout_SetCounterMode(uint32 counterMode);
void   WireSnsTimeout_SetPWMMode(uint32 modeMask);
void   WireSnsTimeout_SetQDMode(uint32 qdMode);

void   WireSnsTimeout_SetPrescaler(uint32 prescaler);
void   WireSnsTimeout_TriggerCommand(uint32 mask, uint32 command);
void   WireSnsTimeout_SetOneShot(uint32 oneShotEnable);
uint32 WireSnsTimeout_ReadStatus(void);

void   WireSnsTimeout_SetPWMSyncKill(uint32 syncKillEnable);
void   WireSnsTimeout_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   WireSnsTimeout_SetPWMDeadTime(uint32 deadTime);
void   WireSnsTimeout_SetPWMInvert(uint32 mask);

void   WireSnsTimeout_SetInterruptMode(uint32 interruptMask);
uint32 WireSnsTimeout_GetInterruptSourceMasked(void);
uint32 WireSnsTimeout_GetInterruptSource(void);
void   WireSnsTimeout_ClearInterrupt(uint32 interruptMask);
void   WireSnsTimeout_SetInterrupt(uint32 interruptMask);

void   WireSnsTimeout_WriteCounter(uint32 count);
uint32 WireSnsTimeout_ReadCounter(void);

uint32 WireSnsTimeout_ReadCapture(void);
uint32 WireSnsTimeout_ReadCaptureBuf(void);

void   WireSnsTimeout_WritePeriod(uint32 period);
uint32 WireSnsTimeout_ReadPeriod(void);
void   WireSnsTimeout_WritePeriodBuf(uint32 periodBuf);
uint32 WireSnsTimeout_ReadPeriodBuf(void);

void   WireSnsTimeout_WriteCompare(uint32 compare);
uint32 WireSnsTimeout_ReadCompare(void);
void   WireSnsTimeout_WriteCompareBuf(uint32 compareBuf);
uint32 WireSnsTimeout_ReadCompareBuf(void);

void   WireSnsTimeout_SetPeriodSwap(uint32 swapEnable);
void   WireSnsTimeout_SetCompareSwap(uint32 swapEnable);

void   WireSnsTimeout_SetCaptureMode(uint32 triggerMode);
void   WireSnsTimeout_SetReloadMode(uint32 triggerMode);
void   WireSnsTimeout_SetStartMode(uint32 triggerMode);
void   WireSnsTimeout_SetStopMode(uint32 triggerMode);
void   WireSnsTimeout_SetCountMode(uint32 triggerMode);

void   WireSnsTimeout_SaveConfig(void);
void   WireSnsTimeout_RestoreConfig(void);
void   WireSnsTimeout_Sleep(void);
void   WireSnsTimeout_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define WireSnsTimeout_BLOCK_CONTROL_REG              (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define WireSnsTimeout_BLOCK_CONTROL_PTR              ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define WireSnsTimeout_COMMAND_REG                    (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define WireSnsTimeout_COMMAND_PTR                    ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define WireSnsTimeout_INTRRUPT_CAUSE_REG             (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define WireSnsTimeout_INTRRUPT_CAUSE_PTR             ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define WireSnsTimeout_CONTROL_REG                    (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__CTRL )
#define WireSnsTimeout_CONTROL_PTR                    ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__CTRL )
#define WireSnsTimeout_STATUS_REG                     (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__STATUS )
#define WireSnsTimeout_STATUS_PTR                     ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__STATUS )
#define WireSnsTimeout_COUNTER_REG                    (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__COUNTER )
#define WireSnsTimeout_COUNTER_PTR                    ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__COUNTER )
#define WireSnsTimeout_COMP_CAP_REG                   (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__CC )
#define WireSnsTimeout_COMP_CAP_PTR                   ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__CC )
#define WireSnsTimeout_COMP_CAP_BUF_REG               (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__CC_BUFF )
#define WireSnsTimeout_COMP_CAP_BUF_PTR               ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__CC_BUFF )
#define WireSnsTimeout_PERIOD_REG                     (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__PERIOD )
#define WireSnsTimeout_PERIOD_PTR                     ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__PERIOD )
#define WireSnsTimeout_PERIOD_BUF_REG                 (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define WireSnsTimeout_PERIOD_BUF_PTR                 ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define WireSnsTimeout_TRIG_CONTROL0_REG              (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define WireSnsTimeout_TRIG_CONTROL0_PTR              ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define WireSnsTimeout_TRIG_CONTROL1_REG              (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define WireSnsTimeout_TRIG_CONTROL1_PTR              ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define WireSnsTimeout_TRIG_CONTROL2_REG              (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define WireSnsTimeout_TRIG_CONTROL2_PTR              ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define WireSnsTimeout_INTERRUPT_REQ_REG              (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__INTR )
#define WireSnsTimeout_INTERRUPT_REQ_PTR              ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__INTR )
#define WireSnsTimeout_INTERRUPT_SET_REG              (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__INTR_SET )
#define WireSnsTimeout_INTERRUPT_SET_PTR              ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__INTR_SET )
#define WireSnsTimeout_INTERRUPT_MASK_REG             (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__INTR_MASK )
#define WireSnsTimeout_INTERRUPT_MASK_PTR             ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__INTR_MASK )
#define WireSnsTimeout_INTERRUPT_MASKED_REG           (*(reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__INTR_MASKED )
#define WireSnsTimeout_INTERRUPT_MASKED_PTR           ( (reg32 *) WireSnsTimeout_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define WireSnsTimeout_MASK                           ((uint32)WireSnsTimeout_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define WireSnsTimeout_RELOAD_CC_SHIFT                (0u)
#define WireSnsTimeout_RELOAD_PERIOD_SHIFT            (1u)
#define WireSnsTimeout_PWM_SYNC_KILL_SHIFT            (2u)
#define WireSnsTimeout_PWM_STOP_KILL_SHIFT            (3u)
#define WireSnsTimeout_PRESCALER_SHIFT                (8u)
#define WireSnsTimeout_UPDOWN_SHIFT                   (16u)
#define WireSnsTimeout_ONESHOT_SHIFT                  (18u)
#define WireSnsTimeout_QUAD_MODE_SHIFT                (20u)
#define WireSnsTimeout_INV_OUT_SHIFT                  (20u)
#define WireSnsTimeout_INV_COMPL_OUT_SHIFT            (21u)
#define WireSnsTimeout_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define WireSnsTimeout_RELOAD_CC_MASK                 ((uint32)(WireSnsTimeout_1BIT_MASK        <<  \
                                                                            WireSnsTimeout_RELOAD_CC_SHIFT))
#define WireSnsTimeout_RELOAD_PERIOD_MASK             ((uint32)(WireSnsTimeout_1BIT_MASK        <<  \
                                                                            WireSnsTimeout_RELOAD_PERIOD_SHIFT))
#define WireSnsTimeout_PWM_SYNC_KILL_MASK             ((uint32)(WireSnsTimeout_1BIT_MASK        <<  \
                                                                            WireSnsTimeout_PWM_SYNC_KILL_SHIFT))
#define WireSnsTimeout_PWM_STOP_KILL_MASK             ((uint32)(WireSnsTimeout_1BIT_MASK        <<  \
                                                                            WireSnsTimeout_PWM_STOP_KILL_SHIFT))
#define WireSnsTimeout_PRESCALER_MASK                 ((uint32)(WireSnsTimeout_8BIT_MASK        <<  \
                                                                            WireSnsTimeout_PRESCALER_SHIFT))
#define WireSnsTimeout_UPDOWN_MASK                    ((uint32)(WireSnsTimeout_2BIT_MASK        <<  \
                                                                            WireSnsTimeout_UPDOWN_SHIFT))
#define WireSnsTimeout_ONESHOT_MASK                   ((uint32)(WireSnsTimeout_1BIT_MASK        <<  \
                                                                            WireSnsTimeout_ONESHOT_SHIFT))
#define WireSnsTimeout_QUAD_MODE_MASK                 ((uint32)(WireSnsTimeout_3BIT_MASK        <<  \
                                                                            WireSnsTimeout_QUAD_MODE_SHIFT))
#define WireSnsTimeout_INV_OUT_MASK                   ((uint32)(WireSnsTimeout_2BIT_MASK        <<  \
                                                                            WireSnsTimeout_INV_OUT_SHIFT))
#define WireSnsTimeout_MODE_MASK                      ((uint32)(WireSnsTimeout_3BIT_MASK        <<  \
                                                                            WireSnsTimeout_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define WireSnsTimeout_CAPTURE_SHIFT                  (0u)
#define WireSnsTimeout_COUNT_SHIFT                    (2u)
#define WireSnsTimeout_RELOAD_SHIFT                   (4u)
#define WireSnsTimeout_STOP_SHIFT                     (6u)
#define WireSnsTimeout_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define WireSnsTimeout_CAPTURE_MASK                   ((uint32)(WireSnsTimeout_2BIT_MASK        <<  \
                                                                  WireSnsTimeout_CAPTURE_SHIFT))
#define WireSnsTimeout_COUNT_MASK                     ((uint32)(WireSnsTimeout_2BIT_MASK        <<  \
                                                                  WireSnsTimeout_COUNT_SHIFT))
#define WireSnsTimeout_RELOAD_MASK                    ((uint32)(WireSnsTimeout_2BIT_MASK        <<  \
                                                                  WireSnsTimeout_RELOAD_SHIFT))
#define WireSnsTimeout_STOP_MASK                      ((uint32)(WireSnsTimeout_2BIT_MASK        <<  \
                                                                  WireSnsTimeout_STOP_SHIFT))
#define WireSnsTimeout_START_MASK                     ((uint32)(WireSnsTimeout_2BIT_MASK        <<  \
                                                                  WireSnsTimeout_START_SHIFT))

/* MASK */
#define WireSnsTimeout_1BIT_MASK                      ((uint32)0x01u)
#define WireSnsTimeout_2BIT_MASK                      ((uint32)0x03u)
#define WireSnsTimeout_3BIT_MASK                      ((uint32)0x07u)
#define WireSnsTimeout_6BIT_MASK                      ((uint32)0x3Fu)
#define WireSnsTimeout_8BIT_MASK                      ((uint32)0xFFu)
#define WireSnsTimeout_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define WireSnsTimeout_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define WireSnsTimeout_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(WireSnsTimeout_QUAD_ENCODING_MODES     << WireSnsTimeout_QUAD_MODE_SHIFT))       |\
         ((uint32)(WireSnsTimeout_CONFIG                  << WireSnsTimeout_MODE_SHIFT)))

#define WireSnsTimeout_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(WireSnsTimeout_PWM_STOP_EVENT          << WireSnsTimeout_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(WireSnsTimeout_PWM_OUT_INVERT          << WireSnsTimeout_INV_OUT_SHIFT))         |\
         ((uint32)(WireSnsTimeout_PWM_OUT_N_INVERT        << WireSnsTimeout_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(WireSnsTimeout_PWM_MODE                << WireSnsTimeout_MODE_SHIFT)))

#define WireSnsTimeout_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(WireSnsTimeout_PWM_RUN_MODE         << WireSnsTimeout_ONESHOT_SHIFT))
            
#define WireSnsTimeout_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(WireSnsTimeout_PWM_ALIGN            << WireSnsTimeout_UPDOWN_SHIFT))

#define WireSnsTimeout_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(WireSnsTimeout_PWM_KILL_EVENT      << WireSnsTimeout_PWM_SYNC_KILL_SHIFT))

#define WireSnsTimeout_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(WireSnsTimeout_PWM_DEAD_TIME_CYCLE  << WireSnsTimeout_PRESCALER_SHIFT))

#define WireSnsTimeout_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(WireSnsTimeout_PWM_PRESCALER        << WireSnsTimeout_PRESCALER_SHIFT))

#define WireSnsTimeout_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(WireSnsTimeout_TC_PRESCALER            << WireSnsTimeout_PRESCALER_SHIFT))       |\
         ((uint32)(WireSnsTimeout_TC_COUNTER_MODE         << WireSnsTimeout_UPDOWN_SHIFT))          |\
         ((uint32)(WireSnsTimeout_TC_RUN_MODE             << WireSnsTimeout_ONESHOT_SHIFT))         |\
         ((uint32)(WireSnsTimeout_TC_COMP_CAP_MODE        << WireSnsTimeout_MODE_SHIFT)))
        
#define WireSnsTimeout_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(WireSnsTimeout_QUAD_PHIA_SIGNAL_MODE   << WireSnsTimeout_COUNT_SHIFT))           |\
         ((uint32)(WireSnsTimeout_QUAD_INDEX_SIGNAL_MODE  << WireSnsTimeout_RELOAD_SHIFT))          |\
         ((uint32)(WireSnsTimeout_QUAD_STOP_SIGNAL_MODE   << WireSnsTimeout_STOP_SHIFT))            |\
         ((uint32)(WireSnsTimeout_QUAD_PHIB_SIGNAL_MODE   << WireSnsTimeout_START_SHIFT)))

#define WireSnsTimeout_PWM_SIGNALS_MODES                                                              \
        (((uint32)(WireSnsTimeout_PWM_SWITCH_SIGNAL_MODE  << WireSnsTimeout_CAPTURE_SHIFT))         |\
         ((uint32)(WireSnsTimeout_PWM_COUNT_SIGNAL_MODE   << WireSnsTimeout_COUNT_SHIFT))           |\
         ((uint32)(WireSnsTimeout_PWM_RELOAD_SIGNAL_MODE  << WireSnsTimeout_RELOAD_SHIFT))          |\
         ((uint32)(WireSnsTimeout_PWM_STOP_SIGNAL_MODE    << WireSnsTimeout_STOP_SHIFT))            |\
         ((uint32)(WireSnsTimeout_PWM_START_SIGNAL_MODE   << WireSnsTimeout_START_SHIFT)))

#define WireSnsTimeout_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(WireSnsTimeout_TC_CAPTURE_SIGNAL_MODE  << WireSnsTimeout_CAPTURE_SHIFT))         |\
         ((uint32)(WireSnsTimeout_TC_COUNT_SIGNAL_MODE    << WireSnsTimeout_COUNT_SHIFT))           |\
         ((uint32)(WireSnsTimeout_TC_RELOAD_SIGNAL_MODE   << WireSnsTimeout_RELOAD_SHIFT))          |\
         ((uint32)(WireSnsTimeout_TC_STOP_SIGNAL_MODE     << WireSnsTimeout_STOP_SHIFT))            |\
         ((uint32)(WireSnsTimeout_TC_START_SIGNAL_MODE    << WireSnsTimeout_START_SHIFT)))
        
#define WireSnsTimeout_TIMER_UPDOWN_CNT_USED                                                          \
                ((WireSnsTimeout__COUNT_UPDOWN0 == WireSnsTimeout_TC_COUNTER_MODE)                  ||\
                 (WireSnsTimeout__COUNT_UPDOWN1 == WireSnsTimeout_TC_COUNTER_MODE))

#define WireSnsTimeout_PWM_UPDOWN_CNT_USED                                                            \
                ((WireSnsTimeout__CENTER == WireSnsTimeout_PWM_ALIGN)                               ||\
                 (WireSnsTimeout__ASYMMETRIC == WireSnsTimeout_PWM_ALIGN))               
        
#define WireSnsTimeout_PWM_PR_INIT_VALUE              (1u)
#define WireSnsTimeout_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_WireSnsTimeout_H */

/* [] END OF FILE */
