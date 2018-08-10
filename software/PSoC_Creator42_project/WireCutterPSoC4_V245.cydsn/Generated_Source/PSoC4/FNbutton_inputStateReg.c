/*******************************************************************************
* File Name: FNbutton_inputStateReg.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware to read the value of a Status 
*  Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "FNbutton_inputStateReg.h"

#if !defined(FNbutton_inputStateReg_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: FNbutton_inputStateReg_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The current value in the Status Register.
*
*******************************************************************************/
uint8 FNbutton_inputStateReg_Read(void) 
{ 
    return FNbutton_inputStateReg_Status;
}


/*******************************************************************************
* Function Name: FNbutton_inputStateReg_InterruptEnable
********************************************************************************
*
* Summary:
*  Enables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void FNbutton_inputStateReg_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    FNbutton_inputStateReg_Status_Aux_Ctrl |= FNbutton_inputStateReg_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: FNbutton_inputStateReg_InterruptDisable
********************************************************************************
*
* Summary:
*  Disables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void FNbutton_inputStateReg_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    FNbutton_inputStateReg_Status_Aux_Ctrl &= (uint8)(~FNbutton_inputStateReg_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: FNbutton_inputStateReg_WriteMask
********************************************************************************
*
* Summary:
*  Writes the current mask value assigned to the Status Register.
*
* Parameters:
*  mask:  Value to write into the mask register.
*
* Return:
*  None.
*
*******************************************************************************/
void FNbutton_inputStateReg_WriteMask(uint8 mask) 
{
    #if(FNbutton_inputStateReg_INPUTS < 8u)
    	mask &= ((uint8)(1u << FNbutton_inputStateReg_INPUTS) - 1u);
	#endif /* End FNbutton_inputStateReg_INPUTS < 8u */
    FNbutton_inputStateReg_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: FNbutton_inputStateReg_ReadMask
********************************************************************************
*
* Summary:
*  Reads the current interrupt mask assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The value of the interrupt mask of the Status Register.
*
*******************************************************************************/
uint8 FNbutton_inputStateReg_ReadMask(void) 
{
    return FNbutton_inputStateReg_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
