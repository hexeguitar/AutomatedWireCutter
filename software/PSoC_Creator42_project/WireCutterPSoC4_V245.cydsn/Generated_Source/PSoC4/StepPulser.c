/*******************************************************************************
* File Name: StepPulser.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the UART component
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "StepPulser.h"
#if (StepPulser_INTERNAL_CLOCK_USED)
    #include "StepPulser_IntClock.h"
#endif /* End StepPulser_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 StepPulser_initVar = 0u;

#if (StepPulser_TX_INTERRUPT_ENABLED && StepPulser_TX_ENABLED)
    volatile uint8 StepPulser_txBuffer[StepPulser_TX_BUFFER_SIZE];
    volatile uint8 StepPulser_txBufferRead = 0u;
    uint8 StepPulser_txBufferWrite = 0u;
#endif /* (StepPulser_TX_INTERRUPT_ENABLED && StepPulser_TX_ENABLED) */

#if (StepPulser_RX_INTERRUPT_ENABLED && (StepPulser_RX_ENABLED || StepPulser_HD_ENABLED))
    uint8 StepPulser_errorStatus = 0u;
    volatile uint8 StepPulser_rxBuffer[StepPulser_RX_BUFFER_SIZE];
    volatile uint8 StepPulser_rxBufferRead  = 0u;
    volatile uint8 StepPulser_rxBufferWrite = 0u;
    volatile uint8 StepPulser_rxBufferLoopDetect = 0u;
    volatile uint8 StepPulser_rxBufferOverflow   = 0u;
    #if (StepPulser_RXHW_ADDRESS_ENABLED)
        volatile uint8 StepPulser_rxAddressMode = StepPulser_RX_ADDRESS_MODE;
        volatile uint8 StepPulser_rxAddressDetected = 0u;
    #endif /* (StepPulser_RXHW_ADDRESS_ENABLED) */
#endif /* (StepPulser_RX_INTERRUPT_ENABLED && (StepPulser_RX_ENABLED || StepPulser_HD_ENABLED)) */


/*******************************************************************************
* Function Name: StepPulser_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  StepPulser_Start() sets the initVar variable, calls the
*  StepPulser_Init() function, and then calls the
*  StepPulser_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The StepPulser_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time StepPulser_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the StepPulser_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void StepPulser_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(StepPulser_initVar == 0u)
    {
        StepPulser_Init();
        StepPulser_initVar = 1u;
    }

    StepPulser_Enable();
}


/*******************************************************************************
* Function Name: StepPulser_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call StepPulser_Init() because
*  the StepPulser_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void StepPulser_Init(void) 
{
    #if(StepPulser_RX_ENABLED || StepPulser_HD_ENABLED)

        #if (StepPulser_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(StepPulser_RX_VECT_NUM, &StepPulser_RXISR);
            CyIntSetPriority(StepPulser_RX_VECT_NUM, StepPulser_RX_PRIOR_NUM);
            StepPulser_errorStatus = 0u;
        #endif /* (StepPulser_RX_INTERRUPT_ENABLED) */

        #if (StepPulser_RXHW_ADDRESS_ENABLED)
            StepPulser_SetRxAddressMode(StepPulser_RX_ADDRESS_MODE);
            StepPulser_SetRxAddress1(StepPulser_RX_HW_ADDRESS1);
            StepPulser_SetRxAddress2(StepPulser_RX_HW_ADDRESS2);
        #endif /* End StepPulser_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        StepPulser_RXBITCTR_PERIOD_REG = StepPulser_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        StepPulser_RXSTATUS_MASK_REG  = StepPulser_INIT_RX_INTERRUPTS_MASK;
    #endif /* End StepPulser_RX_ENABLED || StepPulser_HD_ENABLED*/

    #if(StepPulser_TX_ENABLED)
        #if (StepPulser_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(StepPulser_TX_VECT_NUM, &StepPulser_TXISR);
            CyIntSetPriority(StepPulser_TX_VECT_NUM, StepPulser_TX_PRIOR_NUM);
        #endif /* (StepPulser_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (StepPulser_TXCLKGEN_DP)
            StepPulser_TXBITCLKGEN_CTR_REG = StepPulser_BIT_CENTER;
            StepPulser_TXBITCLKTX_COMPLETE_REG = ((StepPulser_NUMBER_OF_DATA_BITS +
                        StepPulser_NUMBER_OF_START_BIT) * StepPulser_OVER_SAMPLE_COUNT) - 1u;
        #else
            StepPulser_TXBITCTR_PERIOD_REG = ((StepPulser_NUMBER_OF_DATA_BITS +
                        StepPulser_NUMBER_OF_START_BIT) * StepPulser_OVER_SAMPLE_8) - 1u;
        #endif /* End StepPulser_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (StepPulser_TX_INTERRUPT_ENABLED)
            StepPulser_TXSTATUS_MASK_REG = StepPulser_TX_STS_FIFO_EMPTY;
        #else
            StepPulser_TXSTATUS_MASK_REG = StepPulser_INIT_TX_INTERRUPTS_MASK;
        #endif /*End StepPulser_TX_INTERRUPT_ENABLED*/

    #endif /* End StepPulser_TX_ENABLED */

    #if(StepPulser_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        StepPulser_WriteControlRegister( \
            (StepPulser_ReadControlRegister() & (uint8)~StepPulser_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(StepPulser_PARITY_TYPE << StepPulser_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End StepPulser_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: StepPulser_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call StepPulser_Enable() because the StepPulser_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  StepPulser_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void StepPulser_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (StepPulser_RX_ENABLED || StepPulser_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        StepPulser_RXBITCTR_CONTROL_REG |= StepPulser_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        StepPulser_RXSTATUS_ACTL_REG  |= StepPulser_INT_ENABLE;

        #if (StepPulser_RX_INTERRUPT_ENABLED)
            StepPulser_EnableRxInt();

            #if (StepPulser_RXHW_ADDRESS_ENABLED)
                StepPulser_rxAddressDetected = 0u;
            #endif /* (StepPulser_RXHW_ADDRESS_ENABLED) */
        #endif /* (StepPulser_RX_INTERRUPT_ENABLED) */
    #endif /* (StepPulser_RX_ENABLED || StepPulser_HD_ENABLED) */

    #if(StepPulser_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!StepPulser_TXCLKGEN_DP)
            StepPulser_TXBITCTR_CONTROL_REG |= StepPulser_CNTR_ENABLE;
        #endif /* End StepPulser_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        StepPulser_TXSTATUS_ACTL_REG |= StepPulser_INT_ENABLE;
        #if (StepPulser_TX_INTERRUPT_ENABLED)
            StepPulser_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            StepPulser_EnableTxInt();
        #endif /* (StepPulser_TX_INTERRUPT_ENABLED) */
     #endif /* (StepPulser_TX_INTERRUPT_ENABLED) */

    #if (StepPulser_INTERNAL_CLOCK_USED)
        StepPulser_IntClock_Start();  /* Enable the clock */
    #endif /* (StepPulser_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: StepPulser_Stop
********************************************************************************
*
* Summary:
*  Disables the UART operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void StepPulser_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (StepPulser_RX_ENABLED || StepPulser_HD_ENABLED)
        StepPulser_RXBITCTR_CONTROL_REG &= (uint8) ~StepPulser_CNTR_ENABLE;
    #endif /* (StepPulser_RX_ENABLED || StepPulser_HD_ENABLED) */

    #if (StepPulser_TX_ENABLED)
        #if(!StepPulser_TXCLKGEN_DP)
            StepPulser_TXBITCTR_CONTROL_REG &= (uint8) ~StepPulser_CNTR_ENABLE;
        #endif /* (!StepPulser_TXCLKGEN_DP) */
    #endif /* (StepPulser_TX_ENABLED) */

    #if (StepPulser_INTERNAL_CLOCK_USED)
        StepPulser_IntClock_Stop();   /* Disable the clock */
    #endif /* (StepPulser_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (StepPulser_RX_ENABLED || StepPulser_HD_ENABLED)
        StepPulser_RXSTATUS_ACTL_REG  &= (uint8) ~StepPulser_INT_ENABLE;

        #if (StepPulser_RX_INTERRUPT_ENABLED)
            StepPulser_DisableRxInt();
        #endif /* (StepPulser_RX_INTERRUPT_ENABLED) */
    #endif /* (StepPulser_RX_ENABLED || StepPulser_HD_ENABLED) */

    #if (StepPulser_TX_ENABLED)
        StepPulser_TXSTATUS_ACTL_REG &= (uint8) ~StepPulser_INT_ENABLE;

        #if (StepPulser_TX_INTERRUPT_ENABLED)
            StepPulser_DisableTxInt();
        #endif /* (StepPulser_TX_INTERRUPT_ENABLED) */
    #endif /* (StepPulser_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: StepPulser_ReadControlRegister
********************************************************************************
*
* Summary:
*  Returns the current value of the control register.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the control register.
*
*******************************************************************************/
uint8 StepPulser_ReadControlRegister(void) 
{
    #if (StepPulser_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(StepPulser_CONTROL_REG);
    #endif /* (StepPulser_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: StepPulser_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  StepPulser_WriteControlRegister(uint8 control) 
{
    #if (StepPulser_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       StepPulser_CONTROL_REG = control;
    #endif /* (StepPulser_CONTROL_REG_REMOVED) */
}


#if(StepPulser_RX_ENABLED || StepPulser_HD_ENABLED)
    /*******************************************************************************
    * Function Name: StepPulser_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      StepPulser_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      StepPulser_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      StepPulser_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      StepPulser_RX_STS_BREAK            Interrupt on break.
    *      StepPulser_RX_STS_OVERRUN          Interrupt on overrun error.
    *      StepPulser_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      StepPulser_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void StepPulser_SetRxInterruptMode(uint8 intSrc) 
    {
        StepPulser_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: StepPulser_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns the next byte of received data. This function returns data without
    *  checking the status. You must check the status separately.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  StepPulser_rxBuffer - RAM buffer pointer for save received data.
    *  StepPulser_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  StepPulser_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  StepPulser_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 StepPulser_ReadRxData(void) 
    {
        uint8 rxData;

    #if (StepPulser_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        StepPulser_DisableRxInt();

        locRxBufferRead  = StepPulser_rxBufferRead;
        locRxBufferWrite = StepPulser_rxBufferWrite;

        if( (StepPulser_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = StepPulser_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= StepPulser_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            StepPulser_rxBufferRead = locRxBufferRead;

            if(StepPulser_rxBufferLoopDetect != 0u)
            {
                StepPulser_rxBufferLoopDetect = 0u;
                #if ((StepPulser_RX_INTERRUPT_ENABLED) && (StepPulser_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( StepPulser_HD_ENABLED )
                        if((StepPulser_CONTROL_REG & StepPulser_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            StepPulser_RXSTATUS_MASK_REG  |= StepPulser_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        StepPulser_RXSTATUS_MASK_REG  |= StepPulser_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end StepPulser_HD_ENABLED */
                #endif /* ((StepPulser_RX_INTERRUPT_ENABLED) && (StepPulser_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = StepPulser_RXDATA_REG;
        }

        StepPulser_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = StepPulser_RXDATA_REG;

    #endif /* (StepPulser_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: StepPulser_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the receiver status register and the software
    *  buffer overflow status.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Side Effect:
    *  All status register bits are clear-on-read except
    *  StepPulser_RX_STS_FIFO_NOTEMPTY.
    *  StepPulser_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  StepPulser_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   StepPulser_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   StepPulser_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 StepPulser_ReadRxStatus(void) 
    {
        uint8 status;

        status = StepPulser_RXSTATUS_REG & StepPulser_RX_HW_MASK;

    #if (StepPulser_RX_INTERRUPT_ENABLED)
        if(StepPulser_rxBufferOverflow != 0u)
        {
            status |= StepPulser_RX_STS_SOFT_BUFF_OVER;
            StepPulser_rxBufferOverflow = 0u;
        }
    #endif /* (StepPulser_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: StepPulser_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. StepPulser_GetChar() is
    *  designed for ASCII characters and returns a uint8 where 1 to 255 are values
    *  for valid characters and 0 indicates an error occurred or no data is present.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  StepPulser_rxBuffer - RAM buffer pointer for save received data.
    *  StepPulser_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  StepPulser_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  StepPulser_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 StepPulser_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (StepPulser_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        StepPulser_DisableRxInt();

        locRxBufferRead  = StepPulser_rxBufferRead;
        locRxBufferWrite = StepPulser_rxBufferWrite;

        if( (StepPulser_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = StepPulser_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= StepPulser_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            StepPulser_rxBufferRead = locRxBufferRead;

            if(StepPulser_rxBufferLoopDetect != 0u)
            {
                StepPulser_rxBufferLoopDetect = 0u;
                #if( (StepPulser_RX_INTERRUPT_ENABLED) && (StepPulser_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( StepPulser_HD_ENABLED )
                        if((StepPulser_CONTROL_REG & StepPulser_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            StepPulser_RXSTATUS_MASK_REG |= StepPulser_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        StepPulser_RXSTATUS_MASK_REG |= StepPulser_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end StepPulser_HD_ENABLED */
                #endif /* StepPulser_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = StepPulser_RXSTATUS_REG;
            if((rxStatus & StepPulser_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = StepPulser_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (StepPulser_RX_STS_BREAK | StepPulser_RX_STS_PAR_ERROR |
                                StepPulser_RX_STS_STOP_ERROR | StepPulser_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        StepPulser_EnableRxInt();

    #else

        rxStatus =StepPulser_RXSTATUS_REG;
        if((rxStatus & StepPulser_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = StepPulser_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (StepPulser_RX_STS_BREAK | StepPulser_RX_STS_PAR_ERROR |
                            StepPulser_RX_STS_STOP_ERROR | StepPulser_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (StepPulser_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: StepPulser_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, returns received character and error
    *  condition.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains status and LSB contains UART RX data. If the MSB is nonzero,
    *  an error has occurred.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 StepPulser_GetByte(void) 
    {
        
    #if (StepPulser_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        StepPulser_DisableRxInt();
        locErrorStatus = (uint16)StepPulser_errorStatus;
        StepPulser_errorStatus = 0u;
        StepPulser_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | StepPulser_ReadRxData() );
    #else
        return ( ((uint16)StepPulser_ReadRxStatus() << 8u) | StepPulser_ReadRxData() );
    #endif /* StepPulser_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: StepPulser_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received bytes available in the RX buffer.
    *  * RX software buffer is disabled (RX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty RX FIFO or 1 for not empty RX FIFO.
    *  * RX software buffer is enabled: returns the number of bytes available in 
    *    the RX software buffer. Bytes available in the RX FIFO do not take to 
    *    account.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8: Number of bytes in the RX buffer. 
    *    Return value type depends on RX Buffer Size parameter.
    *
    * Global Variables:
    *  StepPulser_rxBufferWrite - used to calculate left bytes.
    *  StepPulser_rxBufferRead - used to calculate left bytes.
    *  StepPulser_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 StepPulser_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (StepPulser_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        StepPulser_DisableRxInt();

        if(StepPulser_rxBufferRead == StepPulser_rxBufferWrite)
        {
            if(StepPulser_rxBufferLoopDetect != 0u)
            {
                size = StepPulser_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(StepPulser_rxBufferRead < StepPulser_rxBufferWrite)
        {
            size = (StepPulser_rxBufferWrite - StepPulser_rxBufferRead);
        }
        else
        {
            size = (StepPulser_RX_BUFFER_SIZE - StepPulser_rxBufferRead) + StepPulser_rxBufferWrite;
        }

        StepPulser_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((StepPulser_RXSTATUS_REG & StepPulser_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (StepPulser_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: StepPulser_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receiver memory buffer and hardware RX FIFO of all received data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  StepPulser_rxBufferWrite - cleared to zero.
    *  StepPulser_rxBufferRead - cleared to zero.
    *  StepPulser_rxBufferLoopDetect - cleared to zero.
    *  StepPulser_rxBufferOverflow - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *
    *******************************************************************************/
    void StepPulser_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        StepPulser_RXDATA_AUX_CTL_REG |= (uint8)  StepPulser_RX_FIFO_CLR;
        StepPulser_RXDATA_AUX_CTL_REG &= (uint8) ~StepPulser_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (StepPulser_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        StepPulser_DisableRxInt();

        StepPulser_rxBufferRead = 0u;
        StepPulser_rxBufferWrite = 0u;
        StepPulser_rxBufferLoopDetect = 0u;
        StepPulser_rxBufferOverflow = 0u;

        StepPulser_EnableRxInt();

    #endif /* (StepPulser_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: StepPulser_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  StepPulser__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  StepPulser__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  StepPulser__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  StepPulser__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  StepPulser__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  StepPulser_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  StepPulser_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void StepPulser_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(StepPulser_RXHW_ADDRESS_ENABLED)
            #if(StepPulser_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* StepPulser_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = StepPulser_CONTROL_REG & (uint8)~StepPulser_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << StepPulser_CTRL_RXADDR_MODE0_SHIFT);
                StepPulser_CONTROL_REG = tmpCtrl;

                #if(StepPulser_RX_INTERRUPT_ENABLED && \
                   (StepPulser_RXBUFFERSIZE > StepPulser_FIFO_LENGTH) )
                    StepPulser_rxAddressMode = addressMode;
                    StepPulser_rxAddressDetected = 0u;
                #endif /* End StepPulser_RXBUFFERSIZE > StepPulser_FIFO_LENGTH*/
            #endif /* End StepPulser_CONTROL_REG_REMOVED */
        #else /* StepPulser_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End StepPulser_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: StepPulser_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Sets the first of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #1 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void StepPulser_SetRxAddress1(uint8 address) 
    {
        StepPulser_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: StepPulser_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the second of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #2 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void StepPulser_SetRxAddress2(uint8 address) 
    {
        StepPulser_RXADDRESS2_REG = address;
    }

#endif  /* StepPulser_RX_ENABLED || StepPulser_HD_ENABLED*/


#if( (StepPulser_TX_ENABLED) || (StepPulser_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: StepPulser_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   StepPulser_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   StepPulser_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   StepPulser_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   StepPulser_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void StepPulser_SetTxInterruptMode(uint8 intSrc) 
    {
        StepPulser_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: StepPulser_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data into the transmit buffer to be sent when the bus is
    *  available without checking the TX status register. You must check status
    *  separately.
    *
    * Parameters:
    *  txDataByte: data byte
    *
    * Return:
    * None.
    *
    * Global Variables:
    *  StepPulser_txBuffer - RAM buffer pointer for save data for transmission
    *  StepPulser_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  StepPulser_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  StepPulser_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void StepPulser_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(StepPulser_initVar != 0u)
        {
        #if (StepPulser_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            StepPulser_DisableTxInt();

            if( (StepPulser_txBufferRead == StepPulser_txBufferWrite) &&
                ((StepPulser_TXSTATUS_REG & StepPulser_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                StepPulser_TXDATA_REG = txDataByte;
            }
            else
            {
                if(StepPulser_txBufferWrite >= StepPulser_TX_BUFFER_SIZE)
                {
                    StepPulser_txBufferWrite = 0u;
                }

                StepPulser_txBuffer[StepPulser_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                StepPulser_txBufferWrite++;
            }

            StepPulser_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            StepPulser_TXDATA_REG = txDataByte;

        #endif /*(StepPulser_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: StepPulser_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status register for the TX portion of the UART.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the TX status register, which is cleared on read.
    *  It is up to the user to handle all bits in this return value accordingly,
    *  even if the bit was not enabled as an interrupt source the event happened
    *  and must be handled accordingly.
    *
    *******************************************************************************/
    uint8 StepPulser_ReadTxStatus(void) 
    {
        return(StepPulser_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: StepPulser_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Puts a byte of data into the transmit buffer to be sent when the bus is
    *  available. This is a blocking API that waits until the TX buffer has room to
    *  hold the data.
    *
    * Parameters:
    *  txDataByte: Byte containing the data to transmit
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  StepPulser_txBuffer - RAM buffer pointer for save data for transmission
    *  StepPulser_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  StepPulser_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  StepPulser_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void StepPulser_PutChar(uint8 txDataByte) 
    {
    #if (StepPulser_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((StepPulser_TX_BUFFER_SIZE > StepPulser_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            StepPulser_DisableTxInt();
        #endif /* (StepPulser_TX_BUFFER_SIZE > StepPulser_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = StepPulser_txBufferWrite;
            locTxBufferRead  = StepPulser_txBufferRead;

        #if ((StepPulser_TX_BUFFER_SIZE > StepPulser_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            StepPulser_EnableTxInt();
        #endif /* (StepPulser_TX_BUFFER_SIZE > StepPulser_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(StepPulser_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((StepPulser_TXSTATUS_REG & StepPulser_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            StepPulser_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= StepPulser_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            StepPulser_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((StepPulser_TX_BUFFER_SIZE > StepPulser_MAX_BYTE_VALUE) && (CY_PSOC3))
            StepPulser_DisableTxInt();
        #endif /* (StepPulser_TX_BUFFER_SIZE > StepPulser_MAX_BYTE_VALUE) && (CY_PSOC3) */

            StepPulser_txBufferWrite = locTxBufferWrite;

        #if ((StepPulser_TX_BUFFER_SIZE > StepPulser_MAX_BYTE_VALUE) && (CY_PSOC3))
            StepPulser_EnableTxInt();
        #endif /* (StepPulser_TX_BUFFER_SIZE > StepPulser_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (StepPulser_TXSTATUS_REG & StepPulser_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                StepPulser_SetPendingTxInt();
            }
        }

    #else

        while((StepPulser_TXSTATUS_REG & StepPulser_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        StepPulser_TXDATA_REG = txDataByte;

    #endif /* StepPulser_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: StepPulser_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a NULL terminated string to the TX buffer for transmission.
    *
    * Parameters:
    *  string[]: Pointer to the null terminated string array residing in RAM or ROM
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  StepPulser_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void StepPulser_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(StepPulser_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                StepPulser_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: StepPulser_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Places N bytes of data from a memory array into the TX buffer for
    *  transmission.
    *
    * Parameters:
    *  string[]: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of bytes to be transmitted. The type depends on TX Buffer
    *             Size parameter.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  StepPulser_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void StepPulser_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(StepPulser_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                StepPulser_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: StepPulser_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Writes a byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) to the transmit buffer.
    *
    * Parameters:
    *  txDataByte: Data byte to transmit before the carriage return and line feed.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  StepPulser_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void StepPulser_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(StepPulser_initVar != 0u)
        {
            StepPulser_PutChar(txDataByte);
            StepPulser_PutChar(0x0Du);
            StepPulser_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: StepPulser_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of bytes in the TX buffer which are waiting to be 
    *  transmitted.
    *  * TX software buffer is disabled (TX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty TX FIFO, 1 for not full TX FIFO or 4 for full TX FIFO.
    *  * TX software buffer is enabled: returns the number of bytes in the TX 
    *    software buffer which are waiting to be transmitted. Bytes available in the
    *    TX FIFO do not count.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Number of bytes used in the TX buffer. Return value type depends on the TX 
    *  Buffer Size parameter.
    *
    * Global Variables:
    *  StepPulser_txBufferWrite - used to calculate left space.
    *  StepPulser_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 StepPulser_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (StepPulser_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        StepPulser_DisableTxInt();

        if(StepPulser_txBufferRead == StepPulser_txBufferWrite)
        {
            size = 0u;
        }
        else if(StepPulser_txBufferRead < StepPulser_txBufferWrite)
        {
            size = (StepPulser_txBufferWrite - StepPulser_txBufferRead);
        }
        else
        {
            size = (StepPulser_TX_BUFFER_SIZE - StepPulser_txBufferRead) +
                    StepPulser_txBufferWrite;
        }

        StepPulser_EnableTxInt();

    #else

        size = StepPulser_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & StepPulser_TX_STS_FIFO_FULL) != 0u)
        {
            size = StepPulser_FIFO_LENGTH;
        }
        else if((size & StepPulser_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (StepPulser_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: StepPulser_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears all data from the TX buffer and hardware TX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  StepPulser_txBufferWrite - cleared to zero.
    *  StepPulser_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Data waiting in the transmit buffer is not sent; a byte that is currently
    *  transmitting finishes transmitting.
    *
    *******************************************************************************/
    void StepPulser_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        StepPulser_TXDATA_AUX_CTL_REG |= (uint8)  StepPulser_TX_FIFO_CLR;
        StepPulser_TXDATA_AUX_CTL_REG &= (uint8) ~StepPulser_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (StepPulser_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        StepPulser_DisableTxInt();

        StepPulser_txBufferRead = 0u;
        StepPulser_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        StepPulser_EnableTxInt();

    #endif /* (StepPulser_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: StepPulser_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   StepPulser_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   StepPulser_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   StepPulser_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   StepPulser_SEND_WAIT_REINIT - Performs both options: 
    *      StepPulser_SEND_BREAK and StepPulser_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  StepPulser_initVar - checked to identify that the component has been
    *     initialized.
    *  txPeriod - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  There are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Function will block CPU until transmission
    *     complete.
    *  2) User may want to use blocking time if UART configured to the low speed
    *     operation
    *     Example for this case:
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to initialize and use the interrupt to
    *     complete break operation.
    *     Example for this case:
    *     Initialize TX interrupt with "TX - On TX Complete" parameter
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     When interrupt appear with StepPulser_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The StepPulser_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void StepPulser_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(StepPulser_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(StepPulser_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == StepPulser_SEND_BREAK) ||
                (retMode == StepPulser_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                StepPulser_WriteControlRegister(StepPulser_ReadControlRegister() |
                                                      StepPulser_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                StepPulser_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = StepPulser_TXSTATUS_REG;
                }
                while((tmpStat & StepPulser_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == StepPulser_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == StepPulser_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = StepPulser_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & StepPulser_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == StepPulser_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == StepPulser_REINIT) ||
                (retMode == StepPulser_SEND_WAIT_REINIT) )
            {
                StepPulser_WriteControlRegister(StepPulser_ReadControlRegister() &
                                              (uint8)~StepPulser_CTRL_HD_SEND_BREAK);
            }

        #else /* StepPulser_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == StepPulser_SEND_BREAK) ||
                (retMode == StepPulser_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (StepPulser_PARITY_TYPE != StepPulser__B_UART__NONE_REVB) || \
                                    (StepPulser_PARITY_TYPE_SW != 0u) )
                    StepPulser_WriteControlRegister(StepPulser_ReadControlRegister() |
                                                          StepPulser_CTRL_HD_SEND_BREAK);
                #endif /* End StepPulser_PARITY_TYPE != StepPulser__B_UART__NONE_REVB  */

                #if(StepPulser_TXCLKGEN_DP)
                    txPeriod = StepPulser_TXBITCLKTX_COMPLETE_REG;
                    StepPulser_TXBITCLKTX_COMPLETE_REG = StepPulser_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = StepPulser_TXBITCTR_PERIOD_REG;
                    StepPulser_TXBITCTR_PERIOD_REG = StepPulser_TXBITCTR_BREAKBITS8X;
                #endif /* End StepPulser_TXCLKGEN_DP */

                /* Send zeros */
                StepPulser_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = StepPulser_TXSTATUS_REG;
                }
                while((tmpStat & StepPulser_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == StepPulser_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == StepPulser_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = StepPulser_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & StepPulser_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == StepPulser_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == StepPulser_REINIT) ||
                (retMode == StepPulser_SEND_WAIT_REINIT) )
            {

            #if(StepPulser_TXCLKGEN_DP)
                StepPulser_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                StepPulser_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End StepPulser_TXCLKGEN_DP */

            #if( (StepPulser_PARITY_TYPE != StepPulser__B_UART__NONE_REVB) || \
                 (StepPulser_PARITY_TYPE_SW != 0u) )
                StepPulser_WriteControlRegister(StepPulser_ReadControlRegister() &
                                                      (uint8) ~StepPulser_CTRL_HD_SEND_BREAK);
            #endif /* End StepPulser_PARITY_TYPE != NONE */
            }
        #endif    /* End StepPulser_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: StepPulser_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       StepPulser_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       StepPulser_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears StepPulser_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void StepPulser_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( StepPulser_CONTROL_REG_REMOVED == 0u )
            StepPulser_WriteControlRegister(StepPulser_ReadControlRegister() |
                                                  StepPulser_CTRL_MARK);
        #endif /* End StepPulser_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( StepPulser_CONTROL_REG_REMOVED == 0u )
            StepPulser_WriteControlRegister(StepPulser_ReadControlRegister() &
                                                  (uint8) ~StepPulser_CTRL_MARK);
        #endif /* End StepPulser_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndStepPulser_TX_ENABLED */

#if(StepPulser_HD_ENABLED)


    /*******************************************************************************
    * Function Name: StepPulser_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the receiver configuration in half duplex mode. After calling this
    *  function, the UART is ready to receive data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the transmitter
    *  configuration.
    *
    *******************************************************************************/
    void StepPulser_LoadRxConfig(void) 
    {
        StepPulser_WriteControlRegister(StepPulser_ReadControlRegister() &
                                                (uint8)~StepPulser_CTRL_HD_SEND);
        StepPulser_RXBITCTR_PERIOD_REG = StepPulser_HD_RXBITCTR_INIT;

    #if (StepPulser_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        StepPulser_SetRxInterruptMode(StepPulser_INIT_RX_INTERRUPTS_MASK);
    #endif /* (StepPulser_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: StepPulser_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the transmitter configuration in half duplex mode. After calling this
    *  function, the UART is ready to transmit data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the receiver configuration.
    *
    *******************************************************************************/
    void StepPulser_LoadTxConfig(void) 
    {
    #if (StepPulser_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        StepPulser_SetRxInterruptMode(0u);
    #endif /* (StepPulser_RX_INTERRUPT_ENABLED) */

        StepPulser_WriteControlRegister(StepPulser_ReadControlRegister() | StepPulser_CTRL_HD_SEND);
        StepPulser_RXBITCTR_PERIOD_REG = StepPulser_HD_TXBITCTR_INIT;
    }

#endif  /* StepPulser_HD_ENABLED */


/* [] END OF FILE */
