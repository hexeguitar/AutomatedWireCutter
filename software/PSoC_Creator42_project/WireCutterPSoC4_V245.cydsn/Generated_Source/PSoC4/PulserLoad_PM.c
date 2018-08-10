/*******************************************************************************
* File Name: PulserLoad_PM.c
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

#include "PulserLoad.h"

/* Check for removal by optimization */
#if !defined(PulserLoad_Sync_ctrl_reg__REMOVED)

static PulserLoad_BACKUP_STRUCT  PulserLoad_backup = {0u};

    
/*******************************************************************************
* Function Name: PulserLoad_SaveConfig
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
void PulserLoad_SaveConfig(void) 
{
    PulserLoad_backup.controlState = PulserLoad_Control;
}


/*******************************************************************************
* Function Name: PulserLoad_RestoreConfig
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
void PulserLoad_RestoreConfig(void) 
{
     PulserLoad_Control = PulserLoad_backup.controlState;
}


/*******************************************************************************
* Function Name: PulserLoad_Sleep
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
void PulserLoad_Sleep(void) 
{
    PulserLoad_SaveConfig();
}


/*******************************************************************************
* Function Name: PulserLoad_Wakeup
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
void PulserLoad_Wakeup(void)  
{
    PulserLoad_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
