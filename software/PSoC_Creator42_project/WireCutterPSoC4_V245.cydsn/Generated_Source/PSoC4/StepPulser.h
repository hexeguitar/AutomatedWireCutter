/*******************************************************************************
* File Name: StepPulser.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_StepPulser_H)
#define CY_UART_StepPulser_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define StepPulser_RX_ENABLED                     (0u)
#define StepPulser_TX_ENABLED                     (1u)
#define StepPulser_HD_ENABLED                     (0u)
#define StepPulser_RX_INTERRUPT_ENABLED           (0u)
#define StepPulser_TX_INTERRUPT_ENABLED           (1u)
#define StepPulser_INTERNAL_CLOCK_USED            (0u)
#define StepPulser_RXHW_ADDRESS_ENABLED           (0u)
#define StepPulser_OVER_SAMPLE_COUNT              (8u)
#define StepPulser_PARITY_TYPE                    (0u)
#define StepPulser_PARITY_TYPE_SW                 (0u)
#define StepPulser_BREAK_DETECT                   (0u)
#define StepPulser_BREAK_BITS_TX                  (13u)
#define StepPulser_BREAK_BITS_RX                  (13u)
#define StepPulser_TXCLKGEN_DP                    (1u)
#define StepPulser_USE23POLLING                   (1u)
#define StepPulser_FLOW_CONTROL                   (0u)
#define StepPulser_CLK_FREQ                       (0u)
#define StepPulser_TX_BUFFER_SIZE                 (8u)
#define StepPulser_RX_BUFFER_SIZE                 (4u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(StepPulser_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define StepPulser_CONTROL_REG_REMOVED            (0u)
#else
    #define StepPulser_CONTROL_REG_REMOVED            (1u)
#endif /* End StepPulser_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct StepPulser_backupStruct_
{
    uint8 enableState;

    #if(StepPulser_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End StepPulser_CONTROL_REG_REMOVED */

} StepPulser_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void StepPulser_Start(void) ;
void StepPulser_Stop(void) ;
uint8 StepPulser_ReadControlRegister(void) ;
void StepPulser_WriteControlRegister(uint8 control) ;

void StepPulser_Init(void) ;
void StepPulser_Enable(void) ;
void StepPulser_SaveConfig(void) ;
void StepPulser_RestoreConfig(void) ;
void StepPulser_Sleep(void) ;
void StepPulser_Wakeup(void) ;

/* Only if RX is enabled */
#if( (StepPulser_RX_ENABLED) || (StepPulser_HD_ENABLED) )

    #if (StepPulser_RX_INTERRUPT_ENABLED)
        #define StepPulser_EnableRxInt()  CyIntEnable (StepPulser_RX_VECT_NUM)
        #define StepPulser_DisableRxInt() CyIntDisable(StepPulser_RX_VECT_NUM)
        CY_ISR_PROTO(StepPulser_RXISR);
    #endif /* StepPulser_RX_INTERRUPT_ENABLED */

    void StepPulser_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void StepPulser_SetRxAddress1(uint8 address) ;
    void StepPulser_SetRxAddress2(uint8 address) ;

    void  StepPulser_SetRxInterruptMode(uint8 intSrc) ;
    uint8 StepPulser_ReadRxData(void) ;
    uint8 StepPulser_ReadRxStatus(void) ;
    uint8 StepPulser_GetChar(void) ;
    uint16 StepPulser_GetByte(void) ;
    uint8 StepPulser_GetRxBufferSize(void)
                                                            ;
    void StepPulser_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define StepPulser_GetRxInterruptSource   StepPulser_ReadRxStatus

#endif /* End (StepPulser_RX_ENABLED) || (StepPulser_HD_ENABLED) */

/* Only if TX is enabled */
#if(StepPulser_TX_ENABLED || StepPulser_HD_ENABLED)

    #if(StepPulser_TX_INTERRUPT_ENABLED)
        #define StepPulser_EnableTxInt()  CyIntEnable (StepPulser_TX_VECT_NUM)
        #define StepPulser_DisableTxInt() CyIntDisable(StepPulser_TX_VECT_NUM)
        #define StepPulser_SetPendingTxInt() CyIntSetPending(StepPulser_TX_VECT_NUM)
        #define StepPulser_ClearPendingTxInt() CyIntClearPending(StepPulser_TX_VECT_NUM)
        CY_ISR_PROTO(StepPulser_TXISR);
    #endif /* StepPulser_TX_INTERRUPT_ENABLED */

    void StepPulser_SetTxInterruptMode(uint8 intSrc) ;
    void StepPulser_WriteTxData(uint8 txDataByte) ;
    uint8 StepPulser_ReadTxStatus(void) ;
    void StepPulser_PutChar(uint8 txDataByte) ;
    void StepPulser_PutString(const char8 string[]) ;
    void StepPulser_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void StepPulser_PutCRLF(uint8 txDataByte) ;
    void StepPulser_ClearTxBuffer(void) ;
    void StepPulser_SetTxAddressMode(uint8 addressMode) ;
    void StepPulser_SendBreak(uint8 retMode) ;
    uint8 StepPulser_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define StepPulser_PutStringConst         StepPulser_PutString
    #define StepPulser_PutArrayConst          StepPulser_PutArray
    #define StepPulser_GetTxInterruptSource   StepPulser_ReadTxStatus

#endif /* End StepPulser_TX_ENABLED || StepPulser_HD_ENABLED */

#if(StepPulser_HD_ENABLED)
    void StepPulser_LoadRxConfig(void) ;
    void StepPulser_LoadTxConfig(void) ;
#endif /* End StepPulser_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_StepPulser) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    StepPulser_CyBtldrCommStart(void) CYSMALL ;
    void    StepPulser_CyBtldrCommStop(void) CYSMALL ;
    void    StepPulser_CyBtldrCommReset(void) CYSMALL ;
    cystatus StepPulser_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus StepPulser_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_StepPulser)
        #define CyBtldrCommStart    StepPulser_CyBtldrCommStart
        #define CyBtldrCommStop     StepPulser_CyBtldrCommStop
        #define CyBtldrCommReset    StepPulser_CyBtldrCommReset
        #define CyBtldrCommWrite    StepPulser_CyBtldrCommWrite
        #define CyBtldrCommRead     StepPulser_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_StepPulser) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define StepPulser_BYTE2BYTE_TIME_OUT (25u)
    #define StepPulser_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define StepPulser_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define StepPulser_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define StepPulser_SET_SPACE      (0x00u)
#define StepPulser_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (StepPulser_TX_ENABLED) || (StepPulser_HD_ENABLED) )
    #if(StepPulser_TX_INTERRUPT_ENABLED)
        #define StepPulser_TX_VECT_NUM            (uint8)StepPulser_TXInternalInterrupt__INTC_NUMBER
        #define StepPulser_TX_PRIOR_NUM           (uint8)StepPulser_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* StepPulser_TX_INTERRUPT_ENABLED */

    #define StepPulser_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define StepPulser_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define StepPulser_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(StepPulser_TX_ENABLED)
        #define StepPulser_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (StepPulser_HD_ENABLED) */
        #define StepPulser_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (StepPulser_TX_ENABLED) */

    #define StepPulser_TX_STS_COMPLETE            (uint8)(0x01u << StepPulser_TX_STS_COMPLETE_SHIFT)
    #define StepPulser_TX_STS_FIFO_EMPTY          (uint8)(0x01u << StepPulser_TX_STS_FIFO_EMPTY_SHIFT)
    #define StepPulser_TX_STS_FIFO_FULL           (uint8)(0x01u << StepPulser_TX_STS_FIFO_FULL_SHIFT)
    #define StepPulser_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << StepPulser_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (StepPulser_TX_ENABLED) || (StepPulser_HD_ENABLED)*/

#if( (StepPulser_RX_ENABLED) || (StepPulser_HD_ENABLED) )
    #if(StepPulser_RX_INTERRUPT_ENABLED)
        #define StepPulser_RX_VECT_NUM            (uint8)StepPulser_RXInternalInterrupt__INTC_NUMBER
        #define StepPulser_RX_PRIOR_NUM           (uint8)StepPulser_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* StepPulser_RX_INTERRUPT_ENABLED */
    #define StepPulser_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define StepPulser_RX_STS_BREAK_SHIFT             (0x01u)
    #define StepPulser_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define StepPulser_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define StepPulser_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define StepPulser_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define StepPulser_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define StepPulser_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define StepPulser_RX_STS_MRKSPC           (uint8)(0x01u << StepPulser_RX_STS_MRKSPC_SHIFT)
    #define StepPulser_RX_STS_BREAK            (uint8)(0x01u << StepPulser_RX_STS_BREAK_SHIFT)
    #define StepPulser_RX_STS_PAR_ERROR        (uint8)(0x01u << StepPulser_RX_STS_PAR_ERROR_SHIFT)
    #define StepPulser_RX_STS_STOP_ERROR       (uint8)(0x01u << StepPulser_RX_STS_STOP_ERROR_SHIFT)
    #define StepPulser_RX_STS_OVERRUN          (uint8)(0x01u << StepPulser_RX_STS_OVERRUN_SHIFT)
    #define StepPulser_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << StepPulser_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define StepPulser_RX_STS_ADDR_MATCH       (uint8)(0x01u << StepPulser_RX_STS_ADDR_MATCH_SHIFT)
    #define StepPulser_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << StepPulser_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define StepPulser_RX_HW_MASK                     (0x7Fu)
#endif /* End (StepPulser_RX_ENABLED) || (StepPulser_HD_ENABLED) */

/* Control Register definitions */
#define StepPulser_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define StepPulser_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define StepPulser_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define StepPulser_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define StepPulser_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define StepPulser_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define StepPulser_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define StepPulser_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define StepPulser_CTRL_HD_SEND               (uint8)(0x01u << StepPulser_CTRL_HD_SEND_SHIFT)
#define StepPulser_CTRL_HD_SEND_BREAK         (uint8)(0x01u << StepPulser_CTRL_HD_SEND_BREAK_SHIFT)
#define StepPulser_CTRL_MARK                  (uint8)(0x01u << StepPulser_CTRL_MARK_SHIFT)
#define StepPulser_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << StepPulser_CTRL_PARITY_TYPE0_SHIFT)
#define StepPulser_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << StepPulser_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define StepPulser_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define StepPulser_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define StepPulser_SEND_BREAK                         (0x00u)
#define StepPulser_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define StepPulser_REINIT                             (0x02u)
#define StepPulser_SEND_WAIT_REINIT                   (0x03u)

#define StepPulser_OVER_SAMPLE_8                      (8u)
#define StepPulser_OVER_SAMPLE_16                     (16u)

#define StepPulser_BIT_CENTER                         (StepPulser_OVER_SAMPLE_COUNT - 2u)

#define StepPulser_FIFO_LENGTH                        (4u)
#define StepPulser_NUMBER_OF_START_BIT                (1u)
#define StepPulser_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define StepPulser_TXBITCTR_BREAKBITS8X   ((StepPulser_BREAK_BITS_TX * StepPulser_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define StepPulser_TXBITCTR_BREAKBITS ((StepPulser_BREAK_BITS_TX * StepPulser_OVER_SAMPLE_COUNT) - 1u)

#define StepPulser_HALF_BIT_COUNT   \
                            (((StepPulser_OVER_SAMPLE_COUNT / 2u) + (StepPulser_USE23POLLING * 1u)) - 2u)
#if (StepPulser_OVER_SAMPLE_COUNT == StepPulser_OVER_SAMPLE_8)
    #define StepPulser_HD_TXBITCTR_INIT   (((StepPulser_BREAK_BITS_TX + \
                            StepPulser_NUMBER_OF_START_BIT) * StepPulser_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define StepPulser_RXBITCTR_INIT  ((((StepPulser_BREAK_BITS_RX + StepPulser_NUMBER_OF_START_BIT) \
                            * StepPulser_OVER_SAMPLE_COUNT) + StepPulser_HALF_BIT_COUNT) - 1u)

#else /* StepPulser_OVER_SAMPLE_COUNT == StepPulser_OVER_SAMPLE_16 */
    #define StepPulser_HD_TXBITCTR_INIT   ((8u * StepPulser_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define StepPulser_RXBITCTR_INIT      (((7u * StepPulser_OVER_SAMPLE_COUNT) - 1u) + \
                                                      StepPulser_HALF_BIT_COUNT)
#endif /* End StepPulser_OVER_SAMPLE_COUNT */

#define StepPulser_HD_RXBITCTR_INIT                   StepPulser_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 StepPulser_initVar;
#if (StepPulser_TX_INTERRUPT_ENABLED && StepPulser_TX_ENABLED)
    extern volatile uint8 StepPulser_txBuffer[StepPulser_TX_BUFFER_SIZE];
    extern volatile uint8 StepPulser_txBufferRead;
    extern uint8 StepPulser_txBufferWrite;
#endif /* (StepPulser_TX_INTERRUPT_ENABLED && StepPulser_TX_ENABLED) */
#if (StepPulser_RX_INTERRUPT_ENABLED && (StepPulser_RX_ENABLED || StepPulser_HD_ENABLED))
    extern uint8 StepPulser_errorStatus;
    extern volatile uint8 StepPulser_rxBuffer[StepPulser_RX_BUFFER_SIZE];
    extern volatile uint8 StepPulser_rxBufferRead;
    extern volatile uint8 StepPulser_rxBufferWrite;
    extern volatile uint8 StepPulser_rxBufferLoopDetect;
    extern volatile uint8 StepPulser_rxBufferOverflow;
    #if (StepPulser_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 StepPulser_rxAddressMode;
        extern volatile uint8 StepPulser_rxAddressDetected;
    #endif /* (StepPulser_RXHW_ADDRESS_ENABLED) */
#endif /* (StepPulser_RX_INTERRUPT_ENABLED && (StepPulser_RX_ENABLED || StepPulser_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define StepPulser__B_UART__AM_SW_BYTE_BYTE 1
#define StepPulser__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define StepPulser__B_UART__AM_HW_BYTE_BY_BYTE 3
#define StepPulser__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define StepPulser__B_UART__AM_NONE 0

#define StepPulser__B_UART__NONE_REVB 0
#define StepPulser__B_UART__EVEN_REVB 1
#define StepPulser__B_UART__ODD_REVB 2
#define StepPulser__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define StepPulser_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define StepPulser_NUMBER_OF_STOP_BITS    (1u)

#if (StepPulser_RXHW_ADDRESS_ENABLED)
    #define StepPulser_RX_ADDRESS_MODE    (0u)
    #define StepPulser_RX_HW_ADDRESS1     (0u)
    #define StepPulser_RX_HW_ADDRESS2     (0u)
#endif /* (StepPulser_RXHW_ADDRESS_ENABLED) */

#define StepPulser_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((0 << StepPulser_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << StepPulser_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << StepPulser_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << StepPulser_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << StepPulser_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << StepPulser_RX_STS_BREAK_SHIFT) \
                                        | (0 << StepPulser_RX_STS_OVERRUN_SHIFT))

#define StepPulser_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << StepPulser_TX_STS_COMPLETE_SHIFT) \
                                        | (1 << StepPulser_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << StepPulser_TX_STS_FIFO_FULL_SHIFT) \
                                        | (1 << StepPulser_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef StepPulser_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define StepPulser_CONTROL_REG \
                            (* (reg8 *) StepPulser_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define StepPulser_CONTROL_PTR \
                            (  (reg8 *) StepPulser_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End StepPulser_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(StepPulser_TX_ENABLED)
    #define StepPulser_TXDATA_REG          (* (reg8 *) StepPulser_BUART_sTX_TxShifter_u0__F0_REG)
    #define StepPulser_TXDATA_PTR          (  (reg8 *) StepPulser_BUART_sTX_TxShifter_u0__F0_REG)
    #define StepPulser_TXDATA_AUX_CTL_REG  (* (reg8 *) StepPulser_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define StepPulser_TXDATA_AUX_CTL_PTR  (  (reg8 *) StepPulser_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define StepPulser_TXSTATUS_REG        (* (reg8 *) StepPulser_BUART_sTX_TxSts__STATUS_REG)
    #define StepPulser_TXSTATUS_PTR        (  (reg8 *) StepPulser_BUART_sTX_TxSts__STATUS_REG)
    #define StepPulser_TXSTATUS_MASK_REG   (* (reg8 *) StepPulser_BUART_sTX_TxSts__MASK_REG)
    #define StepPulser_TXSTATUS_MASK_PTR   (  (reg8 *) StepPulser_BUART_sTX_TxSts__MASK_REG)
    #define StepPulser_TXSTATUS_ACTL_REG   (* (reg8 *) StepPulser_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define StepPulser_TXSTATUS_ACTL_PTR   (  (reg8 *) StepPulser_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(StepPulser_TXCLKGEN_DP)
        #define StepPulser_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) StepPulser_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define StepPulser_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) StepPulser_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define StepPulser_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) StepPulser_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define StepPulser_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) StepPulser_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define StepPulser_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) StepPulser_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define StepPulser_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) StepPulser_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define StepPulser_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) StepPulser_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define StepPulser_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) StepPulser_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define StepPulser_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) StepPulser_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define StepPulser_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) StepPulser_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* StepPulser_TXCLKGEN_DP */

#endif /* End StepPulser_TX_ENABLED */

#if(StepPulser_HD_ENABLED)

    #define StepPulser_TXDATA_REG             (* (reg8 *) StepPulser_BUART_sRX_RxShifter_u0__F1_REG )
    #define StepPulser_TXDATA_PTR             (  (reg8 *) StepPulser_BUART_sRX_RxShifter_u0__F1_REG )
    #define StepPulser_TXDATA_AUX_CTL_REG     (* (reg8 *) StepPulser_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define StepPulser_TXDATA_AUX_CTL_PTR     (  (reg8 *) StepPulser_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define StepPulser_TXSTATUS_REG           (* (reg8 *) StepPulser_BUART_sRX_RxSts__STATUS_REG )
    #define StepPulser_TXSTATUS_PTR           (  (reg8 *) StepPulser_BUART_sRX_RxSts__STATUS_REG )
    #define StepPulser_TXSTATUS_MASK_REG      (* (reg8 *) StepPulser_BUART_sRX_RxSts__MASK_REG )
    #define StepPulser_TXSTATUS_MASK_PTR      (  (reg8 *) StepPulser_BUART_sRX_RxSts__MASK_REG )
    #define StepPulser_TXSTATUS_ACTL_REG      (* (reg8 *) StepPulser_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define StepPulser_TXSTATUS_ACTL_PTR      (  (reg8 *) StepPulser_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End StepPulser_HD_ENABLED */

#if( (StepPulser_RX_ENABLED) || (StepPulser_HD_ENABLED) )
    #define StepPulser_RXDATA_REG             (* (reg8 *) StepPulser_BUART_sRX_RxShifter_u0__F0_REG )
    #define StepPulser_RXDATA_PTR             (  (reg8 *) StepPulser_BUART_sRX_RxShifter_u0__F0_REG )
    #define StepPulser_RXADDRESS1_REG         (* (reg8 *) StepPulser_BUART_sRX_RxShifter_u0__D0_REG )
    #define StepPulser_RXADDRESS1_PTR         (  (reg8 *) StepPulser_BUART_sRX_RxShifter_u0__D0_REG )
    #define StepPulser_RXADDRESS2_REG         (* (reg8 *) StepPulser_BUART_sRX_RxShifter_u0__D1_REG )
    #define StepPulser_RXADDRESS2_PTR         (  (reg8 *) StepPulser_BUART_sRX_RxShifter_u0__D1_REG )
    #define StepPulser_RXDATA_AUX_CTL_REG     (* (reg8 *) StepPulser_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define StepPulser_RXBITCTR_PERIOD_REG    (* (reg8 *) StepPulser_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define StepPulser_RXBITCTR_PERIOD_PTR    (  (reg8 *) StepPulser_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define StepPulser_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) StepPulser_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define StepPulser_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) StepPulser_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define StepPulser_RXBITCTR_COUNTER_REG   (* (reg8 *) StepPulser_BUART_sRX_RxBitCounter__COUNT_REG )
    #define StepPulser_RXBITCTR_COUNTER_PTR   (  (reg8 *) StepPulser_BUART_sRX_RxBitCounter__COUNT_REG )

    #define StepPulser_RXSTATUS_REG           (* (reg8 *) StepPulser_BUART_sRX_RxSts__STATUS_REG )
    #define StepPulser_RXSTATUS_PTR           (  (reg8 *) StepPulser_BUART_sRX_RxSts__STATUS_REG )
    #define StepPulser_RXSTATUS_MASK_REG      (* (reg8 *) StepPulser_BUART_sRX_RxSts__MASK_REG )
    #define StepPulser_RXSTATUS_MASK_PTR      (  (reg8 *) StepPulser_BUART_sRX_RxSts__MASK_REG )
    #define StepPulser_RXSTATUS_ACTL_REG      (* (reg8 *) StepPulser_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define StepPulser_RXSTATUS_ACTL_PTR      (  (reg8 *) StepPulser_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (StepPulser_RX_ENABLED) || (StepPulser_HD_ENABLED) */

#if(StepPulser_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define StepPulser_INTCLOCK_CLKEN_REG     (* (reg8 *) StepPulser_IntClock__PM_ACT_CFG)
    #define StepPulser_INTCLOCK_CLKEN_PTR     (  (reg8 *) StepPulser_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define StepPulser_INTCLOCK_CLKEN_MASK    StepPulser_IntClock__PM_ACT_MSK
#endif /* End StepPulser_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(StepPulser_TX_ENABLED)
    #define StepPulser_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End StepPulser_TX_ENABLED */

#if(StepPulser_HD_ENABLED)
    #define StepPulser_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End StepPulser_HD_ENABLED */

#if( (StepPulser_RX_ENABLED) || (StepPulser_HD_ENABLED) )
    #define StepPulser_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (StepPulser_RX_ENABLED) || (StepPulser_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define StepPulser_WAIT_1_MS      StepPulser_BL_CHK_DELAY_MS   

#define StepPulser_TXBUFFERSIZE   StepPulser_TX_BUFFER_SIZE
#define StepPulser_RXBUFFERSIZE   StepPulser_RX_BUFFER_SIZE

#if (StepPulser_RXHW_ADDRESS_ENABLED)
    #define StepPulser_RXADDRESSMODE  StepPulser_RX_ADDRESS_MODE
    #define StepPulser_RXHWADDRESS1   StepPulser_RX_HW_ADDRESS1
    #define StepPulser_RXHWADDRESS2   StepPulser_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define StepPulser_RXAddressMode  StepPulser_RXADDRESSMODE
#endif /* (StepPulser_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define StepPulser_initvar                    StepPulser_initVar

#define StepPulser_RX_Enabled                 StepPulser_RX_ENABLED
#define StepPulser_TX_Enabled                 StepPulser_TX_ENABLED
#define StepPulser_HD_Enabled                 StepPulser_HD_ENABLED
#define StepPulser_RX_IntInterruptEnabled     StepPulser_RX_INTERRUPT_ENABLED
#define StepPulser_TX_IntInterruptEnabled     StepPulser_TX_INTERRUPT_ENABLED
#define StepPulser_InternalClockUsed          StepPulser_INTERNAL_CLOCK_USED
#define StepPulser_RXHW_Address_Enabled       StepPulser_RXHW_ADDRESS_ENABLED
#define StepPulser_OverSampleCount            StepPulser_OVER_SAMPLE_COUNT
#define StepPulser_ParityType                 StepPulser_PARITY_TYPE

#if( StepPulser_TX_ENABLED && (StepPulser_TXBUFFERSIZE > StepPulser_FIFO_LENGTH))
    #define StepPulser_TXBUFFER               StepPulser_txBuffer
    #define StepPulser_TXBUFFERREAD           StepPulser_txBufferRead
    #define StepPulser_TXBUFFERWRITE          StepPulser_txBufferWrite
#endif /* End StepPulser_TX_ENABLED */
#if( ( StepPulser_RX_ENABLED || StepPulser_HD_ENABLED ) && \
     (StepPulser_RXBUFFERSIZE > StepPulser_FIFO_LENGTH) )
    #define StepPulser_RXBUFFER               StepPulser_rxBuffer
    #define StepPulser_RXBUFFERREAD           StepPulser_rxBufferRead
    #define StepPulser_RXBUFFERWRITE          StepPulser_rxBufferWrite
    #define StepPulser_RXBUFFERLOOPDETECT     StepPulser_rxBufferLoopDetect
    #define StepPulser_RXBUFFER_OVERFLOW      StepPulser_rxBufferOverflow
#endif /* End StepPulser_RX_ENABLED */

#ifdef StepPulser_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define StepPulser_CONTROL                StepPulser_CONTROL_REG
#endif /* End StepPulser_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(StepPulser_TX_ENABLED)
    #define StepPulser_TXDATA                 StepPulser_TXDATA_REG
    #define StepPulser_TXSTATUS               StepPulser_TXSTATUS_REG
    #define StepPulser_TXSTATUS_MASK          StepPulser_TXSTATUS_MASK_REG
    #define StepPulser_TXSTATUS_ACTL          StepPulser_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(StepPulser_TXCLKGEN_DP)
        #define StepPulser_TXBITCLKGEN_CTR        StepPulser_TXBITCLKGEN_CTR_REG
        #define StepPulser_TXBITCLKTX_COMPLETE    StepPulser_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define StepPulser_TXBITCTR_PERIOD        StepPulser_TXBITCTR_PERIOD_REG
        #define StepPulser_TXBITCTR_CONTROL       StepPulser_TXBITCTR_CONTROL_REG
        #define StepPulser_TXBITCTR_COUNTER       StepPulser_TXBITCTR_COUNTER_REG
    #endif /* StepPulser_TXCLKGEN_DP */
#endif /* End StepPulser_TX_ENABLED */

#if(StepPulser_HD_ENABLED)
    #define StepPulser_TXDATA                 StepPulser_TXDATA_REG
    #define StepPulser_TXSTATUS               StepPulser_TXSTATUS_REG
    #define StepPulser_TXSTATUS_MASK          StepPulser_TXSTATUS_MASK_REG
    #define StepPulser_TXSTATUS_ACTL          StepPulser_TXSTATUS_ACTL_REG
#endif /* End StepPulser_HD_ENABLED */

#if( (StepPulser_RX_ENABLED) || (StepPulser_HD_ENABLED) )
    #define StepPulser_RXDATA                 StepPulser_RXDATA_REG
    #define StepPulser_RXADDRESS1             StepPulser_RXADDRESS1_REG
    #define StepPulser_RXADDRESS2             StepPulser_RXADDRESS2_REG
    #define StepPulser_RXBITCTR_PERIOD        StepPulser_RXBITCTR_PERIOD_REG
    #define StepPulser_RXBITCTR_CONTROL       StepPulser_RXBITCTR_CONTROL_REG
    #define StepPulser_RXBITCTR_COUNTER       StepPulser_RXBITCTR_COUNTER_REG
    #define StepPulser_RXSTATUS               StepPulser_RXSTATUS_REG
    #define StepPulser_RXSTATUS_MASK          StepPulser_RXSTATUS_MASK_REG
    #define StepPulser_RXSTATUS_ACTL          StepPulser_RXSTATUS_ACTL_REG
#endif /* End  (StepPulser_RX_ENABLED) || (StepPulser_HD_ENABLED) */

#if(StepPulser_INTERNAL_CLOCK_USED)
    #define StepPulser_INTCLOCK_CLKEN         StepPulser_INTCLOCK_CLKEN_REG
#endif /* End StepPulser_INTERNAL_CLOCK_USED */

#define StepPulser_WAIT_FOR_COMLETE_REINIT    StepPulser_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_StepPulser_H */


/* [] END OF FILE */
