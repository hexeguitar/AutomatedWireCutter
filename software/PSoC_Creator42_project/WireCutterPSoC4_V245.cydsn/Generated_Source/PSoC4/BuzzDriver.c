/*******************************************************************************
* File Name: BuzzDriver.c
* Version 2.30
*
* Description:
*  This file provides the API source code for the Shift Register component.
*
* Note: none
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "BuzzDriver.h"

uint8 BuzzDriver_initVar = 0u;


/*******************************************************************************
* Function Name: BuzzDriver_Start
********************************************************************************
*
* Summary:
*  Starts the Shift Register.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  BuzzDriver_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void BuzzDriver_Start(void) 
{
    if(0u == BuzzDriver_initVar)
    {
        BuzzDriver_Init();
        BuzzDriver_initVar = 1u; /* Component initialized */
    }

    BuzzDriver_Enable();
}


/*******************************************************************************
* Function Name: BuzzDriver_Enable
********************************************************************************
*
* Summary:
*  Enables the Shift Register.
*
* Parameters:
*  void.
*
* Return:
*  void.
*
*******************************************************************************/
void BuzzDriver_Enable(void) 
{
    /* Changing address in Datapath Control Store
       from NOP to component state machine commands space */
    BuzzDriver_SR_CONTROL |= BuzzDriver_CLK_EN;

    BuzzDriver_EnableInt();
}


/*******************************************************************************
* Function Name: BuzzDriver_Init
********************************************************************************
*
* Summary:
*  Initializes Tx and/or Rx interrupt sources with initial values.
*
* Parameters:
*  void.
*
* Return:
*  void.
*
*******************************************************************************/
void BuzzDriver_Init(void) 
{
    BuzzDriver_SetIntMode(BuzzDriver_INT_SRC);
}


/*******************************************************************************
* Function Name: BuzzDriver_Stop
********************************************************************************
*
* Summary:
*  Disables the Shift Register
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void BuzzDriver_Stop(void) 
{
    /*changing Datapath Control Store address to NOP space*/
    BuzzDriver_SR_CONTROL &= ((uint8) ~BuzzDriver_CLK_EN);
    BuzzDriver_DisableInt();
}


/*******************************************************************************
* Function Name: BuzzDriver_EnableInt
********************************************************************************
*
* Summary:
*  Enables the Shift Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void BuzzDriver_EnableInt(void) 
{
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();
    BuzzDriver_SR_AUX_CONTROL |= BuzzDriver_INTERRUPTS_ENABLE;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: BuzzDriver_DisableInt
********************************************************************************
*
* Summary:
*  Disables the Shift Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void BuzzDriver_DisableInt(void) 
{
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();
    BuzzDriver_SR_AUX_CONTROL &= ((uint8) ~BuzzDriver_INTERRUPTS_ENABLE);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: BuzzDriver_GetFIFOStatus
********************************************************************************
*
* Summary:
*  Returns current status of input or output FIFO.
*
* Parameters:
*  fifoId.
*
* Return:
*  FIFO status.
*
*******************************************************************************/
uint8 BuzzDriver_GetFIFOStatus(uint8 fifoId) 
{
    uint8 result;

    result = BuzzDriver_RET_FIFO_NOT_DEFINED;

    #if(0u != BuzzDriver_USE_INPUT_FIFO)
        if(BuzzDriver_IN_FIFO == fifoId)
        {
            switch(BuzzDriver_GET_IN_FIFO_STS)
            {
                case BuzzDriver_IN_FIFO_FULL :
                    result = BuzzDriver_RET_FIFO_FULL;
                    break;

                case BuzzDriver_IN_FIFO_EMPTY :
                    result = BuzzDriver_RET_FIFO_EMPTY;
                    break;

                case BuzzDriver_IN_FIFO_PARTIAL:
                    result = BuzzDriver_RET_FIFO_PARTIAL;
                    break;
                    
                default:
                    /* Initial result value, while 
                       IN_FIFO_EMPTY case is false 
                     */
                    result = BuzzDriver_RET_FIFO_EMPTY;
                    break;
            }   
        }
    #endif /* (0u != BuzzDriver_USE_INPUT_FIFO) */

    if(BuzzDriver_OUT_FIFO == fifoId)
    {
        switch(BuzzDriver_GET_OUT_FIFO_STS)
        {
            case BuzzDriver_OUT_FIFO_FULL :
                result = BuzzDriver_RET_FIFO_FULL;
                break;

            case BuzzDriver_OUT_FIFO_EMPTY :
                result = BuzzDriver_RET_FIFO_EMPTY;
                break;

            case BuzzDriver_OUT_FIFO_PARTIAL :
                result = BuzzDriver_RET_FIFO_PARTIAL;
                break;

            default:
                /* Initial result value, while 
                   OUT_FIFO_FULL case is false 
                 */
                result = BuzzDriver_RET_FIFO_FULL;
                break;
        }
    }

    return(result);
}


/*******************************************************************************
* Function Name: BuzzDriver_SetIntMode
********************************************************************************
*
* Summary:
*  Sets the Interrupt Source for the Shift Register interrupt. Multiple
*  sources may be ORed together
*
* Parameters:
*  interruptSource: Byte containing the constant for the selected interrupt
*  source/s.
*
* Return:
*  None.
*
*******************************************************************************/
void BuzzDriver_SetIntMode(uint8 interruptSource) 
{
    BuzzDriver_SR_STATUS_MASK &= ((uint8) ~BuzzDriver_INTS_EN_MASK);          /* Clear existing int */
    BuzzDriver_SR_STATUS_MASK |= (interruptSource & BuzzDriver_INTS_EN_MASK); /* Set int */
}


/*******************************************************************************
* Function Name: BuzzDriver_GetIntStatus
********************************************************************************
*
* Summary:
*  Gets the Shift Register Interrupt status.
*
* Parameters:
*  None.
*
* Return:
*  Byte containing the constant for the selected interrupt source/s.
*
*******************************************************************************/
uint8 BuzzDriver_GetIntStatus(void) 
{
    return(BuzzDriver_SR_STATUS & BuzzDriver_INTS_EN_MASK);
}


/*******************************************************************************
* Function Name: BuzzDriver_WriteRegValue
********************************************************************************
*
* Summary:
*  Send state directly to shift register
*
* Parameters:
*  shiftData: containing shift register state.
*
* Return:
*  None.
*
*******************************************************************************/
void BuzzDriver_WriteRegValue(uint8 shiftData)
                                                                     
{
    CY_SET_REG8(BuzzDriver_SHIFT_REG_LSB_PTR, shiftData);
}


#if(0u != BuzzDriver_USE_INPUT_FIFO)
    /*******************************************************************************
    * Function Name: BuzzDriver_WriteData
    ********************************************************************************
    *
    * Summary:
    *  Send state to FIFO for later transfer to shift register based on the Load
    *  input
    *
    * Parameters:
    *  shiftData: containing shift register state.
    *
    * Return:
    *  Indicates: successful execution of function
    *  when FIFO is empty; and error when FIFO is full.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    cystatus BuzzDriver_WriteData(uint8 shiftData)
                                                                         
    {
        cystatus result;

        result = CYRET_INVALID_STATE;

        /* Writes data into the input FIFO if it is not FULL */
        if(BuzzDriver_RET_FIFO_FULL != (BuzzDriver_GetFIFOStatus(BuzzDriver_IN_FIFO)))
        {
            CY_SET_REG8(BuzzDriver_IN_FIFO_VAL_LSB_PTR, shiftData);
            result = CYRET_SUCCESS;
        }

        return(result);
    }
#endif /* (0u != BuzzDriver_USE_INPUT_FIFO) */


#if(0u != BuzzDriver_USE_OUTPUT_FIFO)
    /*******************************************************************************
    * Function Name: BuzzDriver_ReadData
    ********************************************************************************
    *
    * Summary:
    *  Returns state in FIFO due to Store input.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Shift Register state
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 BuzzDriver_ReadData(void) 
    {
        return(CY_GET_REG8(BuzzDriver_OUT_FIFO_VAL_LSB_PTR));
    }
#endif /* (0u != BuzzDriver_USE_OUTPUT_FIFO) */


/*******************************************************************************
* Function Name: BuzzDriver_ReadRegValue
********************************************************************************
*
* Summary:
*  Directly returns current state in shift register, not data in FIFO due
*  to Store input.
*
* Parameters:
*  None.
*
* Return:
*  Shift Register state. Clears output FIFO.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 BuzzDriver_ReadRegValue(void) 
{
    uint8 result;

    /* Clear FIFO before software capture */
    while(BuzzDriver_RET_FIFO_EMPTY != BuzzDriver_GetFIFOStatus(BuzzDriver_OUT_FIFO))
    {
        (void) CY_GET_REG8(BuzzDriver_OUT_FIFO_VAL_LSB_PTR);
    }

    /* Read of 8 bits from A1 causes capture to output FIFO */
    (void) CY_GET_REG8(BuzzDriver_SHIFT_REG_CAPTURE_PTR);

    /* Read output FIFO */
    result  = CY_GET_REG8(BuzzDriver_OUT_FIFO_VAL_LSB_PTR);
    
    #if(0u != (BuzzDriver_SR_SIZE % 8u))
        result &= ((uint8) BuzzDriver_SR_MASK);
    #endif /* (0u != (BuzzDriver_SR_SIZE % 8u)) */
    
    return(result);
}


/* [] END OF FILE */
