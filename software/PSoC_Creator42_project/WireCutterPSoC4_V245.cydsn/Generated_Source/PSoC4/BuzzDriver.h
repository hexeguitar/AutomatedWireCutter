/*******************************************************************************
* File Name: BuzzDriver.h
* Version 2.30
*
* Description:
*  This header file contains definitions associated with the Shift Register
*  component.
*
* Note: none
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/


#if !defined(CY_SHIFTREG_BuzzDriver_H)
#define CY_SHIFTREG_BuzzDriver_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h"


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define BuzzDriver_FIFO_SIZE          (4u)
#define BuzzDriver_USE_INPUT_FIFO     (0u)
#define BuzzDriver_USE_OUTPUT_FIFO    (0u)
#define BuzzDriver_SR_SIZE            (8u)


/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;

    uint8 saveSrA0Reg;
    uint8 saveSrA1Reg;

    #if(CY_UDB_V0)
        uint8 saveSrIntMask;
    #endif /* (CY_UDB_V0) */

} BuzzDriver_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  BuzzDriver_Start(void)                              ;
void  BuzzDriver_Stop(void)                               ;
void  BuzzDriver_Init(void)                               ;
void  BuzzDriver_Enable(void)                             ;
void  BuzzDriver_RestoreConfig(void)                      ;
void  BuzzDriver_SaveConfig(void)                         ;
void  BuzzDriver_Sleep(void)                              ;
void  BuzzDriver_Wakeup(void)                             ;
void  BuzzDriver_EnableInt(void)                          ;
void  BuzzDriver_DisableInt(void)                         ;
void  BuzzDriver_SetIntMode(uint8 interruptSource)        ;
uint8 BuzzDriver_GetIntStatus(void)                       ;
void  BuzzDriver_WriteRegValue(uint8 shiftData) \
                                                                ;
uint8 BuzzDriver_ReadRegValue(void) ;
uint8    BuzzDriver_GetFIFOStatus(uint8 fifoId)           ;

#if(0u != BuzzDriver_USE_INPUT_FIFO)
    cystatus BuzzDriver_WriteData(uint8 shiftData) \
                                                                ;
#endif /* (0u != BuzzDriver_USE_INPUT_FIFO) */

#if(0u != BuzzDriver_USE_OUTPUT_FIFO)
    uint8 BuzzDriver_ReadData(void) ;
#endif /* (0u != BuzzDriver_USE_OUTPUT_FIFO) */


/**********************************
*   Variable with external linkage
**********************************/

extern uint8 BuzzDriver_initVar;


/***************************************
*           API Constants
***************************************/

#define BuzzDriver_LOAD                   (0x01u)
#define BuzzDriver_STORE                  (0x02u)
#define BuzzDriver_RESET                  (0x04u)

#define BuzzDriver_IN_FIFO                (0x01u)
#define BuzzDriver_OUT_FIFO               (0x02u)

#define BuzzDriver_RET_FIFO_FULL          (0x00u)

/* This define is obsolete */
#define BuzzDriver_RET_FIFO_NOT_EMPTY     (0x01u)

#define BuzzDriver_RET_FIFO_PARTIAL       (0x01u)
#define BuzzDriver_RET_FIFO_EMPTY         (0x02u)
#define BuzzDriver_RET_FIFO_NOT_DEFINED   (0xFEu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define BuzzDriver__LEFT 0
#define BuzzDriver__RIGHT 1



/***************************************
*    Initial Parameter Constants
***************************************/

#define BuzzDriver_SR_MASK    (0xFFu) /* Unsigned is added to parameter */
#define BuzzDriver_INT_SRC    (0u)
#define BuzzDriver_DIRECTION  (1u)


/***************************************
*             Registers
***************************************/

/* Control register */
#define BuzzDriver_SR_CONTROL_REG (* (reg8 *) \
                                           BuzzDriver_bSR_SyncCtl_CtrlReg__CONTROL_REG)
#define BuzzDriver_SR_CONTROL_PTR (  (reg8 *) \
                                           BuzzDriver_bSR_SyncCtl_CtrlReg__CONTROL_REG)

/* Status register */
#define BuzzDriver_SR_STATUS_REG      (* (reg8 *) BuzzDriver_bSR_StsReg__STATUS_REG)
#define BuzzDriver_SR_STATUS_PTR      (  (reg8 *) BuzzDriver_bSR_StsReg__STATUS_REG)

/* Interrupt status register */
#define BuzzDriver_SR_STATUS_MASK_REG (* (reg8 *) BuzzDriver_bSR_StsReg__MASK_REG)
#define BuzzDriver_SR_STATUS_MASK_PTR (  (reg8 *) BuzzDriver_bSR_StsReg__MASK_REG)

/* Aux control register */
#define BuzzDriver_SR_AUX_CONTROL_REG (* (reg8 *) BuzzDriver_bSR_StsReg__STATUS_AUX_CTL_REG)
#define BuzzDriver_SR_AUX_CONTROL_PTR (  (reg8 *) BuzzDriver_bSR_StsReg__STATUS_AUX_CTL_REG)

/* A1 register: only used to implement capture function */
#define BuzzDriver_SHIFT_REG_CAPTURE_PTR    ( (reg8 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__A1_REG )

#if(CY_PSOC3 || CY_PSOC5)
    #define BuzzDriver_IN_FIFO_VAL_LSB_PTR        ( (reg8 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__F0_REG )

    #define BuzzDriver_SHIFT_REG_LSB_PTR          ( (reg8 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__A0_REG )

    #define BuzzDriver_SHIFT_REG_VALUE_LSB_PTR    ( (reg8 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__A1_REG )

    #define BuzzDriver_OUT_FIFO_VAL_LSB_PTR       ( (reg8 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__F1_REG )

#else
    #if(BuzzDriver_SR_SIZE <= 8u) /* 8bit - ShiftReg */
        #define BuzzDriver_IN_FIFO_VAL_LSB_PTR        ( (reg8 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__F0_REG )

        #define BuzzDriver_SHIFT_REG_LSB_PTR          ( (reg8 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__A0_REG )

        #define BuzzDriver_SHIFT_REG_VALUE_LSB_PTR    ( (reg8 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__A1_REG )

        #define BuzzDriver_OUT_FIFO_VAL_LSB_PTR       ( (reg8 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__F1_REG )

    #elif(BuzzDriver_SR_SIZE <= 16u) /* 16bit - ShiftReg */
        #define BuzzDriver_IN_FIFO_VAL_LSB_PTR        ( (reg16 *) \
                                  BuzzDriver_bSR_sC8_BShiftRegDp_u0__16BIT_F0_REG )

        #define BuzzDriver_SHIFT_REG_LSB_PTR          ( (reg16 *) \
                                  BuzzDriver_bSR_sC8_BShiftRegDp_u0__16BIT_A0_REG )

        #define BuzzDriver_SHIFT_REG_VALUE_LSB_PTR    ( (reg16 *) \
                                  BuzzDriver_bSR_sC8_BShiftRegDp_u0__16BIT_A1_REG )

        #define BuzzDriver_OUT_FIFO_VAL_LSB_PTR       ( (reg16 *) \
                                  BuzzDriver_bSR_sC8_BShiftRegDp_u0__16BIT_F1_REG )


    #elif(BuzzDriver_SR_SIZE <= 24u) /* 24bit - ShiftReg */
        #define BuzzDriver_IN_FIFO_VAL_LSB_PTR        ( (reg32 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__F0_REG )

        #define BuzzDriver_SHIFT_REG_LSB_PTR          ( (reg32 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__A0_REG )

        #define BuzzDriver_SHIFT_REG_VALUE_LSB_PTR    ( (reg32 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__A1_REG )

        #define BuzzDriver_OUT_FIFO_VAL_LSB_PTR       ( (reg32 *) \
                                        BuzzDriver_bSR_sC8_BShiftRegDp_u0__F1_REG )

    #else /* 32bit - ShiftReg */
        #define BuzzDriver_IN_FIFO_VAL_LSB_PTR        ( (reg32 *) \
                                  BuzzDriver_bSR_sC8_BShiftRegDp_u0__32BIT_F0_REG )

        #define BuzzDriver_SHIFT_REG_LSB_PTR          ( (reg32 *) \
                                  BuzzDriver_bSR_sC8_BShiftRegDp_u0__32BIT_A0_REG )

        #define BuzzDriver_SHIFT_REG_VALUE_LSB_PTR    ( (reg32 *) \
                                  BuzzDriver_bSR_sC8_BShiftRegDp_u0__32BIT_A1_REG )

        #define BuzzDriver_OUT_FIFO_VAL_LSB_PTR       ( (reg32 *) \
                                  BuzzDriver_bSR_sC8_BShiftRegDp_u0__32BIT_F1_REG )

    #endif  /* (BuzzDriver_SR_SIZE <= 8u) */
#endif      /* (CY_PSOC3 || CY_PSOC5) */


/***************************************
*       Register Constants
***************************************/

#define BuzzDriver_INTERRUPTS_ENABLE      (0x10u)
#define BuzzDriver_LOAD_INT_EN            (0x01u)
#define BuzzDriver_STORE_INT_EN           (0x02u)
#define BuzzDriver_RESET_INT_EN           (0x04u)
#define BuzzDriver_CLK_EN                 (0x01u)

#define BuzzDriver_RESET_INT_EN_MASK      (0xFBu)
#define BuzzDriver_LOAD_INT_EN_MASK       (0xFEu)
#define BuzzDriver_STORE_INT_EN_MASK      (0xFDu)
#define BuzzDriver_INTS_EN_MASK           (0x07u)

#define BuzzDriver_OUT_FIFO_CLR_BIT       (0x02u)

#if(0u != BuzzDriver_USE_INPUT_FIFO)

    #define BuzzDriver_IN_FIFO_MASK       (0x18u)

    #define BuzzDriver_IN_FIFO_FULL       (0x00u)
    #define BuzzDriver_IN_FIFO_EMPTY      (0x01u)
    #define BuzzDriver_IN_FIFO_PARTIAL    (0x02u)
    
    /* This define is obsolete */
    #define BuzzDriver_IN_FIFO_NOT_EMPTY  (0x02u)
    
#endif /* (0u != BuzzDriver_USE_INPUT_FIFO) */

#define BuzzDriver_OUT_FIFO_MASK          (0x60u)

#define BuzzDriver_OUT_FIFO_EMPTY         (0x00u)
#define BuzzDriver_OUT_FIFO_FULL          (0x01u)
#define BuzzDriver_OUT_FIFO_PARTIAL       (0x02u)

/* This define is obsolete */
#define BuzzDriver_OUT_FIFO_NOT_EMPTY     (0x02u)

#define BuzzDriver_IN_FIFO_SHIFT_MASK     (0x03u)
#define BuzzDriver_OUT_FIFO_SHIFT_MASK    (0x05u)

#define BuzzDriver_DISABLED               (0u)
#define BuzzDriver_DEFAULT_A0             (0u)
#define BuzzDriver_DEFAULT_A1             (0u)


/***************************************
*       Macros
***************************************/

#define BuzzDriver_IS_ENABLED         (0u != (BuzzDriver_SR_CONTROL & BuzzDriver_CLK_EN))

#define BuzzDriver_GET_OUT_FIFO_STS   ((BuzzDriver_SR_STATUS & BuzzDriver_OUT_FIFO_MASK) >> \
                                              BuzzDriver_OUT_FIFO_SHIFT_MASK)

#define BuzzDriver_GET_IN_FIFO_STS    ((BuzzDriver_SR_STATUS & BuzzDriver_IN_FIFO_MASK)  >> \
                                              BuzzDriver_IN_FIFO_SHIFT_MASK)


/***************************************
*       Obsolete
***************************************/

/* Following code are OBSOLETE and must not be used 
 * starting from ShiftReg 2.20 
 */

#define BuzzDriver_SR_CONTROL     (BuzzDriver_SR_CONTROL_REG)
#define BuzzDriver_SR_STATUS      (BuzzDriver_SR_STATUS_REG)
#define BuzzDriver_SR_STATUS_MASK (BuzzDriver_SR_STATUS_MASK_REG)
#define BuzzDriver_SR_AUX_CONTROL (BuzzDriver_SR_AUX_CONTROL_REG)

#define BuzzDriver_IN_FIFO_SHFT_MASK  (BuzzDriver_IN_FIFO_SHIFT_MASK)
#define BuzzDriver_OUT_FIFO_SHFT_MASK (BuzzDriver_OUT_FIFO_SHFIT_MASK)

#define BuzzDriver_RET_FIFO_BAD_PARAM (0xFFu)

#endif /* (CY_SHIFTREG_BuzzDriver_H) */


/* [] END OF FILE */
