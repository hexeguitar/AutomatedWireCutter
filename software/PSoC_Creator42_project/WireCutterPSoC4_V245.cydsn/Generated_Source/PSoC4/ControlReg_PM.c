/*******************************************************************************
* File Name: ControlReg_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "ControlReg.h"

/* Check for removal by optimization */
#if !defined(ControlReg_Sync_ctrl_reg__REMOVED)

static ControlReg_BACKUP_STRUCT  ControlReg_backup = {0u};

    
/*******************************************************************************
* Function Name: ControlReg_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ControlReg_SaveConfig(void) 
{
    ControlReg_backup.controlState = ControlReg_Control;
}


/*******************************************************************************
* Function Name: ControlReg_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void ControlReg_RestoreConfig(void) 
{
     ControlReg_Control = ControlReg_backup.controlState;
}


/*******************************************************************************
* Function Name: ControlReg_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ControlReg_Sleep(void) 
{
    ControlReg_SaveConfig();
}


/*******************************************************************************
* Function Name: ControlReg_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ControlReg_Wakeup(void)  
{
    ControlReg_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
