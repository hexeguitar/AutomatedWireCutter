/*******************************************************************************
* File Name: BtnLongPressDetector.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the BtnLongPressDetector
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

#if !defined(CY_TCPWM_BtnLongPressDetector_H)
#define CY_TCPWM_BtnLongPressDetector_H


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
} BtnLongPressDetector_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  BtnLongPressDetector_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define BtnLongPressDetector_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define BtnLongPressDetector_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define BtnLongPressDetector_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define BtnLongPressDetector_QUAD_ENCODING_MODES            (0lu)
#define BtnLongPressDetector_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define BtnLongPressDetector_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define BtnLongPressDetector_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define BtnLongPressDetector_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define BtnLongPressDetector_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define BtnLongPressDetector_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define BtnLongPressDetector_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define BtnLongPressDetector_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define BtnLongPressDetector_TC_RUN_MODE                    (1lu)
#define BtnLongPressDetector_TC_COUNTER_MODE                (0lu)
#define BtnLongPressDetector_TC_COMP_CAP_MODE               (0lu)
#define BtnLongPressDetector_TC_PRESCALER                   (0lu)

/* Signal modes */
#define BtnLongPressDetector_TC_RELOAD_SIGNAL_MODE          (3lu)
#define BtnLongPressDetector_TC_COUNT_SIGNAL_MODE           (3lu)
#define BtnLongPressDetector_TC_START_SIGNAL_MODE           (0lu)
#define BtnLongPressDetector_TC_STOP_SIGNAL_MODE            (0lu)
#define BtnLongPressDetector_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define BtnLongPressDetector_TC_RELOAD_SIGNAL_PRESENT       (1lu)
#define BtnLongPressDetector_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define BtnLongPressDetector_TC_START_SIGNAL_PRESENT        (0lu)
#define BtnLongPressDetector_TC_STOP_SIGNAL_PRESENT         (0lu)
#define BtnLongPressDetector_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define BtnLongPressDetector_TC_INTERRUPT_MASK              (2lu)

/* PWM Mode */
/* Parameters */
#define BtnLongPressDetector_PWM_KILL_EVENT                 (0lu)
#define BtnLongPressDetector_PWM_STOP_EVENT                 (0lu)
#define BtnLongPressDetector_PWM_MODE                       (4lu)
#define BtnLongPressDetector_PWM_OUT_N_INVERT               (0lu)
#define BtnLongPressDetector_PWM_OUT_INVERT                 (0lu)
#define BtnLongPressDetector_PWM_ALIGN                      (0lu)
#define BtnLongPressDetector_PWM_RUN_MODE                   (0lu)
#define BtnLongPressDetector_PWM_DEAD_TIME_CYCLE            (0lu)
#define BtnLongPressDetector_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define BtnLongPressDetector_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define BtnLongPressDetector_PWM_COUNT_SIGNAL_MODE          (3lu)
#define BtnLongPressDetector_PWM_START_SIGNAL_MODE          (0lu)
#define BtnLongPressDetector_PWM_STOP_SIGNAL_MODE           (0lu)
#define BtnLongPressDetector_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define BtnLongPressDetector_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define BtnLongPressDetector_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define BtnLongPressDetector_PWM_START_SIGNAL_PRESENT       (0lu)
#define BtnLongPressDetector_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define BtnLongPressDetector_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define BtnLongPressDetector_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define BtnLongPressDetector_TC_PERIOD_VALUE                (65535lu)
#define BtnLongPressDetector_TC_COMPARE_VALUE               (50lu)
#define BtnLongPressDetector_TC_COMPARE_BUF_VALUE           (65535lu)
#define BtnLongPressDetector_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define BtnLongPressDetector_PWM_PERIOD_VALUE               (65535lu)
#define BtnLongPressDetector_PWM_PERIOD_BUF_VALUE           (65535lu)
#define BtnLongPressDetector_PWM_PERIOD_SWAP                (0lu)
#define BtnLongPressDetector_PWM_COMPARE_VALUE              (65535lu)
#define BtnLongPressDetector_PWM_COMPARE_BUF_VALUE          (65535lu)
#define BtnLongPressDetector_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define BtnLongPressDetector__LEFT 0
#define BtnLongPressDetector__RIGHT 1
#define BtnLongPressDetector__CENTER 2
#define BtnLongPressDetector__ASYMMETRIC 3

#define BtnLongPressDetector__X1 0
#define BtnLongPressDetector__X2 1
#define BtnLongPressDetector__X4 2

#define BtnLongPressDetector__PWM 4
#define BtnLongPressDetector__PWM_DT 5
#define BtnLongPressDetector__PWM_PR 6

#define BtnLongPressDetector__INVERSE 1
#define BtnLongPressDetector__DIRECT 0

#define BtnLongPressDetector__CAPTURE 2
#define BtnLongPressDetector__COMPARE 0

#define BtnLongPressDetector__TRIG_LEVEL 3
#define BtnLongPressDetector__TRIG_RISING 0
#define BtnLongPressDetector__TRIG_FALLING 1
#define BtnLongPressDetector__TRIG_BOTH 2

#define BtnLongPressDetector__INTR_MASK_TC 1
#define BtnLongPressDetector__INTR_MASK_CC_MATCH 2
#define BtnLongPressDetector__INTR_MASK_NONE 0
#define BtnLongPressDetector__INTR_MASK_TC_CC 3

#define BtnLongPressDetector__UNCONFIG 8
#define BtnLongPressDetector__TIMER 1
#define BtnLongPressDetector__QUAD 3
#define BtnLongPressDetector__PWM_SEL 7

#define BtnLongPressDetector__COUNT_UP 0
#define BtnLongPressDetector__COUNT_DOWN 1
#define BtnLongPressDetector__COUNT_UPDOWN0 2
#define BtnLongPressDetector__COUNT_UPDOWN1 3


/* Prescaler */
#define BtnLongPressDetector_PRESCALE_DIVBY1                ((uint32)(0u << BtnLongPressDetector_PRESCALER_SHIFT))
#define BtnLongPressDetector_PRESCALE_DIVBY2                ((uint32)(1u << BtnLongPressDetector_PRESCALER_SHIFT))
#define BtnLongPressDetector_PRESCALE_DIVBY4                ((uint32)(2u << BtnLongPressDetector_PRESCALER_SHIFT))
#define BtnLongPressDetector_PRESCALE_DIVBY8                ((uint32)(3u << BtnLongPressDetector_PRESCALER_SHIFT))
#define BtnLongPressDetector_PRESCALE_DIVBY16               ((uint32)(4u << BtnLongPressDetector_PRESCALER_SHIFT))
#define BtnLongPressDetector_PRESCALE_DIVBY32               ((uint32)(5u << BtnLongPressDetector_PRESCALER_SHIFT))
#define BtnLongPressDetector_PRESCALE_DIVBY64               ((uint32)(6u << BtnLongPressDetector_PRESCALER_SHIFT))
#define BtnLongPressDetector_PRESCALE_DIVBY128              ((uint32)(7u << BtnLongPressDetector_PRESCALER_SHIFT))

/* TCPWM set modes */
#define BtnLongPressDetector_MODE_TIMER_COMPARE             ((uint32)(BtnLongPressDetector__COMPARE         <<  \
                                                                  BtnLongPressDetector_MODE_SHIFT))
#define BtnLongPressDetector_MODE_TIMER_CAPTURE             ((uint32)(BtnLongPressDetector__CAPTURE         <<  \
                                                                  BtnLongPressDetector_MODE_SHIFT))
#define BtnLongPressDetector_MODE_QUAD                      ((uint32)(BtnLongPressDetector__QUAD            <<  \
                                                                  BtnLongPressDetector_MODE_SHIFT))
#define BtnLongPressDetector_MODE_PWM                       ((uint32)(BtnLongPressDetector__PWM             <<  \
                                                                  BtnLongPressDetector_MODE_SHIFT))
#define BtnLongPressDetector_MODE_PWM_DT                    ((uint32)(BtnLongPressDetector__PWM_DT          <<  \
                                                                  BtnLongPressDetector_MODE_SHIFT))
#define BtnLongPressDetector_MODE_PWM_PR                    ((uint32)(BtnLongPressDetector__PWM_PR          <<  \
                                                                  BtnLongPressDetector_MODE_SHIFT))

/* Quad Modes */
#define BtnLongPressDetector_MODE_X1                        ((uint32)(BtnLongPressDetector__X1              <<  \
                                                                  BtnLongPressDetector_QUAD_MODE_SHIFT))
#define BtnLongPressDetector_MODE_X2                        ((uint32)(BtnLongPressDetector__X2              <<  \
                                                                  BtnLongPressDetector_QUAD_MODE_SHIFT))
#define BtnLongPressDetector_MODE_X4                        ((uint32)(BtnLongPressDetector__X4              <<  \
                                                                  BtnLongPressDetector_QUAD_MODE_SHIFT))

/* Counter modes */
#define BtnLongPressDetector_COUNT_UP                       ((uint32)(BtnLongPressDetector__COUNT_UP        <<  \
                                                                  BtnLongPressDetector_UPDOWN_SHIFT))
#define BtnLongPressDetector_COUNT_DOWN                     ((uint32)(BtnLongPressDetector__COUNT_DOWN      <<  \
                                                                  BtnLongPressDetector_UPDOWN_SHIFT))
#define BtnLongPressDetector_COUNT_UPDOWN0                  ((uint32)(BtnLongPressDetector__COUNT_UPDOWN0   <<  \
                                                                  BtnLongPressDetector_UPDOWN_SHIFT))
#define BtnLongPressDetector_COUNT_UPDOWN1                  ((uint32)(BtnLongPressDetector__COUNT_UPDOWN1   <<  \
                                                                  BtnLongPressDetector_UPDOWN_SHIFT))

/* PWM output invert */
#define BtnLongPressDetector_INVERT_LINE                    ((uint32)(BtnLongPressDetector__INVERSE         <<  \
                                                                  BtnLongPressDetector_INV_OUT_SHIFT))
#define BtnLongPressDetector_INVERT_LINE_N                  ((uint32)(BtnLongPressDetector__INVERSE         <<  \
                                                                  BtnLongPressDetector_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define BtnLongPressDetector_TRIG_RISING                    ((uint32)BtnLongPressDetector__TRIG_RISING)
#define BtnLongPressDetector_TRIG_FALLING                   ((uint32)BtnLongPressDetector__TRIG_FALLING)
#define BtnLongPressDetector_TRIG_BOTH                      ((uint32)BtnLongPressDetector__TRIG_BOTH)
#define BtnLongPressDetector_TRIG_LEVEL                     ((uint32)BtnLongPressDetector__TRIG_LEVEL)

/* Interrupt mask */
#define BtnLongPressDetector_INTR_MASK_TC                   ((uint32)BtnLongPressDetector__INTR_MASK_TC)
#define BtnLongPressDetector_INTR_MASK_CC_MATCH             ((uint32)BtnLongPressDetector__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define BtnLongPressDetector_CC_MATCH_SET                   (0x00u)
#define BtnLongPressDetector_CC_MATCH_CLEAR                 (0x01u)
#define BtnLongPressDetector_CC_MATCH_INVERT                (0x02u)
#define BtnLongPressDetector_CC_MATCH_NO_CHANGE             (0x03u)
#define BtnLongPressDetector_OVERLOW_SET                    (0x00u)
#define BtnLongPressDetector_OVERLOW_CLEAR                  (0x04u)
#define BtnLongPressDetector_OVERLOW_INVERT                 (0x08u)
#define BtnLongPressDetector_OVERLOW_NO_CHANGE              (0x0Cu)
#define BtnLongPressDetector_UNDERFLOW_SET                  (0x00u)
#define BtnLongPressDetector_UNDERFLOW_CLEAR                (0x10u)
#define BtnLongPressDetector_UNDERFLOW_INVERT               (0x20u)
#define BtnLongPressDetector_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define BtnLongPressDetector_PWM_MODE_LEFT                  (BtnLongPressDetector_CC_MATCH_CLEAR        |   \
                                                         BtnLongPressDetector_OVERLOW_SET           |   \
                                                         BtnLongPressDetector_UNDERFLOW_NO_CHANGE)
#define BtnLongPressDetector_PWM_MODE_RIGHT                 (BtnLongPressDetector_CC_MATCH_SET          |   \
                                                         BtnLongPressDetector_OVERLOW_NO_CHANGE     |   \
                                                         BtnLongPressDetector_UNDERFLOW_CLEAR)
#define BtnLongPressDetector_PWM_MODE_ASYM                  (BtnLongPressDetector_CC_MATCH_INVERT       |   \
                                                         BtnLongPressDetector_OVERLOW_SET           |   \
                                                         BtnLongPressDetector_UNDERFLOW_CLEAR)

#if (BtnLongPressDetector_CY_TCPWM_V2)
    #if(BtnLongPressDetector_CY_TCPWM_4000)
        #define BtnLongPressDetector_PWM_MODE_CENTER                (BtnLongPressDetector_CC_MATCH_INVERT       |   \
                                                                 BtnLongPressDetector_OVERLOW_NO_CHANGE     |   \
                                                                 BtnLongPressDetector_UNDERFLOW_CLEAR)
    #else
        #define BtnLongPressDetector_PWM_MODE_CENTER                (BtnLongPressDetector_CC_MATCH_INVERT       |   \
                                                                 BtnLongPressDetector_OVERLOW_SET           |   \
                                                                 BtnLongPressDetector_UNDERFLOW_CLEAR)
    #endif /* (BtnLongPressDetector_CY_TCPWM_4000) */
#else
    #define BtnLongPressDetector_PWM_MODE_CENTER                (BtnLongPressDetector_CC_MATCH_INVERT       |   \
                                                             BtnLongPressDetector_OVERLOW_NO_CHANGE     |   \
                                                             BtnLongPressDetector_UNDERFLOW_CLEAR)
#endif /* (BtnLongPressDetector_CY_TCPWM_NEW) */

/* Command operations without condition */
#define BtnLongPressDetector_CMD_CAPTURE                    (0u)
#define BtnLongPressDetector_CMD_RELOAD                     (8u)
#define BtnLongPressDetector_CMD_STOP                       (16u)
#define BtnLongPressDetector_CMD_START                      (24u)

/* Status */
#define BtnLongPressDetector_STATUS_DOWN                    (1u)
#define BtnLongPressDetector_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   BtnLongPressDetector_Init(void);
void   BtnLongPressDetector_Enable(void);
void   BtnLongPressDetector_Start(void);
void   BtnLongPressDetector_Stop(void);

void   BtnLongPressDetector_SetMode(uint32 mode);
void   BtnLongPressDetector_SetCounterMode(uint32 counterMode);
void   BtnLongPressDetector_SetPWMMode(uint32 modeMask);
void   BtnLongPressDetector_SetQDMode(uint32 qdMode);

void   BtnLongPressDetector_SetPrescaler(uint32 prescaler);
void   BtnLongPressDetector_TriggerCommand(uint32 mask, uint32 command);
void   BtnLongPressDetector_SetOneShot(uint32 oneShotEnable);
uint32 BtnLongPressDetector_ReadStatus(void);

void   BtnLongPressDetector_SetPWMSyncKill(uint32 syncKillEnable);
void   BtnLongPressDetector_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   BtnLongPressDetector_SetPWMDeadTime(uint32 deadTime);
void   BtnLongPressDetector_SetPWMInvert(uint32 mask);

void   BtnLongPressDetector_SetInterruptMode(uint32 interruptMask);
uint32 BtnLongPressDetector_GetInterruptSourceMasked(void);
uint32 BtnLongPressDetector_GetInterruptSource(void);
void   BtnLongPressDetector_ClearInterrupt(uint32 interruptMask);
void   BtnLongPressDetector_SetInterrupt(uint32 interruptMask);

void   BtnLongPressDetector_WriteCounter(uint32 count);
uint32 BtnLongPressDetector_ReadCounter(void);

uint32 BtnLongPressDetector_ReadCapture(void);
uint32 BtnLongPressDetector_ReadCaptureBuf(void);

void   BtnLongPressDetector_WritePeriod(uint32 period);
uint32 BtnLongPressDetector_ReadPeriod(void);
void   BtnLongPressDetector_WritePeriodBuf(uint32 periodBuf);
uint32 BtnLongPressDetector_ReadPeriodBuf(void);

void   BtnLongPressDetector_WriteCompare(uint32 compare);
uint32 BtnLongPressDetector_ReadCompare(void);
void   BtnLongPressDetector_WriteCompareBuf(uint32 compareBuf);
uint32 BtnLongPressDetector_ReadCompareBuf(void);

void   BtnLongPressDetector_SetPeriodSwap(uint32 swapEnable);
void   BtnLongPressDetector_SetCompareSwap(uint32 swapEnable);

void   BtnLongPressDetector_SetCaptureMode(uint32 triggerMode);
void   BtnLongPressDetector_SetReloadMode(uint32 triggerMode);
void   BtnLongPressDetector_SetStartMode(uint32 triggerMode);
void   BtnLongPressDetector_SetStopMode(uint32 triggerMode);
void   BtnLongPressDetector_SetCountMode(uint32 triggerMode);

void   BtnLongPressDetector_SaveConfig(void);
void   BtnLongPressDetector_RestoreConfig(void);
void   BtnLongPressDetector_Sleep(void);
void   BtnLongPressDetector_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define BtnLongPressDetector_BLOCK_CONTROL_REG              (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define BtnLongPressDetector_BLOCK_CONTROL_PTR              ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define BtnLongPressDetector_COMMAND_REG                    (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define BtnLongPressDetector_COMMAND_PTR                    ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define BtnLongPressDetector_INTRRUPT_CAUSE_REG             (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define BtnLongPressDetector_INTRRUPT_CAUSE_PTR             ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define BtnLongPressDetector_CONTROL_REG                    (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__CTRL )
#define BtnLongPressDetector_CONTROL_PTR                    ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__CTRL )
#define BtnLongPressDetector_STATUS_REG                     (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__STATUS )
#define BtnLongPressDetector_STATUS_PTR                     ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__STATUS )
#define BtnLongPressDetector_COUNTER_REG                    (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__COUNTER )
#define BtnLongPressDetector_COUNTER_PTR                    ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__COUNTER )
#define BtnLongPressDetector_COMP_CAP_REG                   (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__CC )
#define BtnLongPressDetector_COMP_CAP_PTR                   ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__CC )
#define BtnLongPressDetector_COMP_CAP_BUF_REG               (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__CC_BUFF )
#define BtnLongPressDetector_COMP_CAP_BUF_PTR               ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__CC_BUFF )
#define BtnLongPressDetector_PERIOD_REG                     (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__PERIOD )
#define BtnLongPressDetector_PERIOD_PTR                     ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__PERIOD )
#define BtnLongPressDetector_PERIOD_BUF_REG                 (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define BtnLongPressDetector_PERIOD_BUF_PTR                 ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define BtnLongPressDetector_TRIG_CONTROL0_REG              (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define BtnLongPressDetector_TRIG_CONTROL0_PTR              ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define BtnLongPressDetector_TRIG_CONTROL1_REG              (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define BtnLongPressDetector_TRIG_CONTROL1_PTR              ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define BtnLongPressDetector_TRIG_CONTROL2_REG              (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define BtnLongPressDetector_TRIG_CONTROL2_PTR              ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define BtnLongPressDetector_INTERRUPT_REQ_REG              (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__INTR )
#define BtnLongPressDetector_INTERRUPT_REQ_PTR              ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__INTR )
#define BtnLongPressDetector_INTERRUPT_SET_REG              (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__INTR_SET )
#define BtnLongPressDetector_INTERRUPT_SET_PTR              ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__INTR_SET )
#define BtnLongPressDetector_INTERRUPT_MASK_REG             (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__INTR_MASK )
#define BtnLongPressDetector_INTERRUPT_MASK_PTR             ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__INTR_MASK )
#define BtnLongPressDetector_INTERRUPT_MASKED_REG           (*(reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__INTR_MASKED )
#define BtnLongPressDetector_INTERRUPT_MASKED_PTR           ( (reg32 *) BtnLongPressDetector_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define BtnLongPressDetector_MASK                           ((uint32)BtnLongPressDetector_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define BtnLongPressDetector_RELOAD_CC_SHIFT                (0u)
#define BtnLongPressDetector_RELOAD_PERIOD_SHIFT            (1u)
#define BtnLongPressDetector_PWM_SYNC_KILL_SHIFT            (2u)
#define BtnLongPressDetector_PWM_STOP_KILL_SHIFT            (3u)
#define BtnLongPressDetector_PRESCALER_SHIFT                (8u)
#define BtnLongPressDetector_UPDOWN_SHIFT                   (16u)
#define BtnLongPressDetector_ONESHOT_SHIFT                  (18u)
#define BtnLongPressDetector_QUAD_MODE_SHIFT                (20u)
#define BtnLongPressDetector_INV_OUT_SHIFT                  (20u)
#define BtnLongPressDetector_INV_COMPL_OUT_SHIFT            (21u)
#define BtnLongPressDetector_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define BtnLongPressDetector_RELOAD_CC_MASK                 ((uint32)(BtnLongPressDetector_1BIT_MASK        <<  \
                                                                            BtnLongPressDetector_RELOAD_CC_SHIFT))
#define BtnLongPressDetector_RELOAD_PERIOD_MASK             ((uint32)(BtnLongPressDetector_1BIT_MASK        <<  \
                                                                            BtnLongPressDetector_RELOAD_PERIOD_SHIFT))
#define BtnLongPressDetector_PWM_SYNC_KILL_MASK             ((uint32)(BtnLongPressDetector_1BIT_MASK        <<  \
                                                                            BtnLongPressDetector_PWM_SYNC_KILL_SHIFT))
#define BtnLongPressDetector_PWM_STOP_KILL_MASK             ((uint32)(BtnLongPressDetector_1BIT_MASK        <<  \
                                                                            BtnLongPressDetector_PWM_STOP_KILL_SHIFT))
#define BtnLongPressDetector_PRESCALER_MASK                 ((uint32)(BtnLongPressDetector_8BIT_MASK        <<  \
                                                                            BtnLongPressDetector_PRESCALER_SHIFT))
#define BtnLongPressDetector_UPDOWN_MASK                    ((uint32)(BtnLongPressDetector_2BIT_MASK        <<  \
                                                                            BtnLongPressDetector_UPDOWN_SHIFT))
#define BtnLongPressDetector_ONESHOT_MASK                   ((uint32)(BtnLongPressDetector_1BIT_MASK        <<  \
                                                                            BtnLongPressDetector_ONESHOT_SHIFT))
#define BtnLongPressDetector_QUAD_MODE_MASK                 ((uint32)(BtnLongPressDetector_3BIT_MASK        <<  \
                                                                            BtnLongPressDetector_QUAD_MODE_SHIFT))
#define BtnLongPressDetector_INV_OUT_MASK                   ((uint32)(BtnLongPressDetector_2BIT_MASK        <<  \
                                                                            BtnLongPressDetector_INV_OUT_SHIFT))
#define BtnLongPressDetector_MODE_MASK                      ((uint32)(BtnLongPressDetector_3BIT_MASK        <<  \
                                                                            BtnLongPressDetector_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define BtnLongPressDetector_CAPTURE_SHIFT                  (0u)
#define BtnLongPressDetector_COUNT_SHIFT                    (2u)
#define BtnLongPressDetector_RELOAD_SHIFT                   (4u)
#define BtnLongPressDetector_STOP_SHIFT                     (6u)
#define BtnLongPressDetector_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define BtnLongPressDetector_CAPTURE_MASK                   ((uint32)(BtnLongPressDetector_2BIT_MASK        <<  \
                                                                  BtnLongPressDetector_CAPTURE_SHIFT))
#define BtnLongPressDetector_COUNT_MASK                     ((uint32)(BtnLongPressDetector_2BIT_MASK        <<  \
                                                                  BtnLongPressDetector_COUNT_SHIFT))
#define BtnLongPressDetector_RELOAD_MASK                    ((uint32)(BtnLongPressDetector_2BIT_MASK        <<  \
                                                                  BtnLongPressDetector_RELOAD_SHIFT))
#define BtnLongPressDetector_STOP_MASK                      ((uint32)(BtnLongPressDetector_2BIT_MASK        <<  \
                                                                  BtnLongPressDetector_STOP_SHIFT))
#define BtnLongPressDetector_START_MASK                     ((uint32)(BtnLongPressDetector_2BIT_MASK        <<  \
                                                                  BtnLongPressDetector_START_SHIFT))

/* MASK */
#define BtnLongPressDetector_1BIT_MASK                      ((uint32)0x01u)
#define BtnLongPressDetector_2BIT_MASK                      ((uint32)0x03u)
#define BtnLongPressDetector_3BIT_MASK                      ((uint32)0x07u)
#define BtnLongPressDetector_6BIT_MASK                      ((uint32)0x3Fu)
#define BtnLongPressDetector_8BIT_MASK                      ((uint32)0xFFu)
#define BtnLongPressDetector_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define BtnLongPressDetector_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define BtnLongPressDetector_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(BtnLongPressDetector_QUAD_ENCODING_MODES     << BtnLongPressDetector_QUAD_MODE_SHIFT))       |\
         ((uint32)(BtnLongPressDetector_CONFIG                  << BtnLongPressDetector_MODE_SHIFT)))

#define BtnLongPressDetector_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(BtnLongPressDetector_PWM_STOP_EVENT          << BtnLongPressDetector_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(BtnLongPressDetector_PWM_OUT_INVERT          << BtnLongPressDetector_INV_OUT_SHIFT))         |\
         ((uint32)(BtnLongPressDetector_PWM_OUT_N_INVERT        << BtnLongPressDetector_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(BtnLongPressDetector_PWM_MODE                << BtnLongPressDetector_MODE_SHIFT)))

#define BtnLongPressDetector_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(BtnLongPressDetector_PWM_RUN_MODE         << BtnLongPressDetector_ONESHOT_SHIFT))
            
#define BtnLongPressDetector_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(BtnLongPressDetector_PWM_ALIGN            << BtnLongPressDetector_UPDOWN_SHIFT))

#define BtnLongPressDetector_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(BtnLongPressDetector_PWM_KILL_EVENT      << BtnLongPressDetector_PWM_SYNC_KILL_SHIFT))

#define BtnLongPressDetector_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(BtnLongPressDetector_PWM_DEAD_TIME_CYCLE  << BtnLongPressDetector_PRESCALER_SHIFT))

#define BtnLongPressDetector_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(BtnLongPressDetector_PWM_PRESCALER        << BtnLongPressDetector_PRESCALER_SHIFT))

#define BtnLongPressDetector_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(BtnLongPressDetector_TC_PRESCALER            << BtnLongPressDetector_PRESCALER_SHIFT))       |\
         ((uint32)(BtnLongPressDetector_TC_COUNTER_MODE         << BtnLongPressDetector_UPDOWN_SHIFT))          |\
         ((uint32)(BtnLongPressDetector_TC_RUN_MODE             << BtnLongPressDetector_ONESHOT_SHIFT))         |\
         ((uint32)(BtnLongPressDetector_TC_COMP_CAP_MODE        << BtnLongPressDetector_MODE_SHIFT)))
        
#define BtnLongPressDetector_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(BtnLongPressDetector_QUAD_PHIA_SIGNAL_MODE   << BtnLongPressDetector_COUNT_SHIFT))           |\
         ((uint32)(BtnLongPressDetector_QUAD_INDEX_SIGNAL_MODE  << BtnLongPressDetector_RELOAD_SHIFT))          |\
         ((uint32)(BtnLongPressDetector_QUAD_STOP_SIGNAL_MODE   << BtnLongPressDetector_STOP_SHIFT))            |\
         ((uint32)(BtnLongPressDetector_QUAD_PHIB_SIGNAL_MODE   << BtnLongPressDetector_START_SHIFT)))

#define BtnLongPressDetector_PWM_SIGNALS_MODES                                                              \
        (((uint32)(BtnLongPressDetector_PWM_SWITCH_SIGNAL_MODE  << BtnLongPressDetector_CAPTURE_SHIFT))         |\
         ((uint32)(BtnLongPressDetector_PWM_COUNT_SIGNAL_MODE   << BtnLongPressDetector_COUNT_SHIFT))           |\
         ((uint32)(BtnLongPressDetector_PWM_RELOAD_SIGNAL_MODE  << BtnLongPressDetector_RELOAD_SHIFT))          |\
         ((uint32)(BtnLongPressDetector_PWM_STOP_SIGNAL_MODE    << BtnLongPressDetector_STOP_SHIFT))            |\
         ((uint32)(BtnLongPressDetector_PWM_START_SIGNAL_MODE   << BtnLongPressDetector_START_SHIFT)))

#define BtnLongPressDetector_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(BtnLongPressDetector_TC_CAPTURE_SIGNAL_MODE  << BtnLongPressDetector_CAPTURE_SHIFT))         |\
         ((uint32)(BtnLongPressDetector_TC_COUNT_SIGNAL_MODE    << BtnLongPressDetector_COUNT_SHIFT))           |\
         ((uint32)(BtnLongPressDetector_TC_RELOAD_SIGNAL_MODE   << BtnLongPressDetector_RELOAD_SHIFT))          |\
         ((uint32)(BtnLongPressDetector_TC_STOP_SIGNAL_MODE     << BtnLongPressDetector_STOP_SHIFT))            |\
         ((uint32)(BtnLongPressDetector_TC_START_SIGNAL_MODE    << BtnLongPressDetector_START_SHIFT)))
        
#define BtnLongPressDetector_TIMER_UPDOWN_CNT_USED                                                          \
                ((BtnLongPressDetector__COUNT_UPDOWN0 == BtnLongPressDetector_TC_COUNTER_MODE)                  ||\
                 (BtnLongPressDetector__COUNT_UPDOWN1 == BtnLongPressDetector_TC_COUNTER_MODE))

#define BtnLongPressDetector_PWM_UPDOWN_CNT_USED                                                            \
                ((BtnLongPressDetector__CENTER == BtnLongPressDetector_PWM_ALIGN)                               ||\
                 (BtnLongPressDetector__ASYMMETRIC == BtnLongPressDetector_PWM_ALIGN))               
        
#define BtnLongPressDetector_PWM_PR_INIT_VALUE              (1u)
#define BtnLongPressDetector_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_BtnLongPressDetector_H */

/* [] END OF FILE */
