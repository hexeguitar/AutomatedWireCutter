/*******************************************************************************
* File Name: StepPulserINT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "StepPulser.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (StepPulser_RX_INTERRUPT_ENABLED && (StepPulser_RX_ENABLED || StepPulser_HD_ENABLED))
    /*******************************************************************************
    * Function Name: StepPulser_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  StepPulser_rxBuffer - RAM buffer pointer for save received data.
    *  StepPulser_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  StepPulser_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  StepPulser_rxBufferOverflow - software overflow flag. Set to one
    *     when StepPulser_rxBufferWrite index overtakes
    *     StepPulser_rxBufferRead index.
    *  StepPulser_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when StepPulser_rxBufferWrite is equal to
    *    StepPulser_rxBufferRead
    *  StepPulser_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  StepPulser_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(StepPulser_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef StepPulser_RXISR_ENTRY_CALLBACK
        StepPulser_RXISR_EntryCallback();
    #endif /* StepPulser_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START StepPulser_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = StepPulser_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in StepPulser_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (StepPulser_RX_STS_BREAK | 
                            StepPulser_RX_STS_PAR_ERROR |
                            StepPulser_RX_STS_STOP_ERROR | 
                            StepPulser_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                StepPulser_errorStatus |= readStatus & ( StepPulser_RX_STS_BREAK | 
                                                            StepPulser_RX_STS_PAR_ERROR | 
                                                            StepPulser_RX_STS_STOP_ERROR | 
                                                            StepPulser_RX_STS_OVERRUN);
                /* `#START StepPulser_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef StepPulser_RXISR_ERROR_CALLBACK
                StepPulser_RXISR_ERROR_Callback();
            #endif /* StepPulser_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & StepPulser_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = StepPulser_RXDATA_REG;
            #if (StepPulser_RXHW_ADDRESS_ENABLED)
                if(StepPulser_rxAddressMode == (uint8)StepPulser__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & StepPulser_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & StepPulser_RX_STS_ADDR_MATCH) != 0u)
                        {
                            StepPulser_rxAddressDetected = 1u;
                        }
                        else
                        {
                            StepPulser_rxAddressDetected = 0u;
                        }
                    }
                    if(StepPulser_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        StepPulser_rxBuffer[StepPulser_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    StepPulser_rxBuffer[StepPulser_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                StepPulser_rxBuffer[StepPulser_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (StepPulser_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(StepPulser_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        StepPulser_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    StepPulser_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(StepPulser_rxBufferWrite >= StepPulser_RX_BUFFER_SIZE)
                    {
                        StepPulser_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(StepPulser_rxBufferWrite == StepPulser_rxBufferRead)
                    {
                        StepPulser_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (StepPulser_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            StepPulser_RXSTATUS_MASK_REG  &= (uint8)~StepPulser_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(StepPulser_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (StepPulser_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & StepPulser_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START StepPulser_RXISR_END` */

        /* `#END` */

    #ifdef StepPulser_RXISR_EXIT_CALLBACK
        StepPulser_RXISR_ExitCallback();
    #endif /* StepPulser_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (StepPulser_RX_INTERRUPT_ENABLED && (StepPulser_RX_ENABLED || StepPulser_HD_ENABLED)) */


#if (StepPulser_TX_INTERRUPT_ENABLED && StepPulser_TX_ENABLED)
    /*******************************************************************************
    * Function Name: StepPulser_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  StepPulser_txBuffer - RAM buffer pointer for transmit data from.
    *  StepPulser_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  StepPulser_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(StepPulser_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef StepPulser_TXISR_ENTRY_CALLBACK
        StepPulser_TXISR_EntryCallback();
    #endif /* StepPulser_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START StepPulser_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((StepPulser_txBufferRead != StepPulser_txBufferWrite) &&
             ((StepPulser_TXSTATUS_REG & StepPulser_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(StepPulser_txBufferRead >= StepPulser_TX_BUFFER_SIZE)
            {
                StepPulser_txBufferRead = 0u;
            }

            StepPulser_TXDATA_REG = StepPulser_txBuffer[StepPulser_txBufferRead];

            /* Set next pointer */
            StepPulser_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START StepPulser_TXISR_END` */

        /* `#END` */

    #ifdef StepPulser_TXISR_EXIT_CALLBACK
        StepPulser_TXISR_ExitCallback();
    #endif /* StepPulser_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (StepPulser_TX_INTERRUPT_ENABLED && StepPulser_TX_ENABLED) */


/* [] END OF FILE */
