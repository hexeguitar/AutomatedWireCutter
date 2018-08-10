/*******************************************************************************
* File Name: KeyPad_KbdControl_PM.c
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

#include "KeyPad_KbdControl.h"

/* Check for removal by optimization */
#if !defined(KeyPad_KbdControl_Sync_ctrl_reg__REMOVED)

static KeyPad_KbdControl_BACKUP_STRUCT  KeyPad_KbdControl_backup = {0u};

    
/*******************************************************************************
* Function Name: KeyPad_KbdControl_SaveConfig
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
void KeyPad_KbdControl_SaveConfig(void) 
{
    KeyPad_KbdControl_backup.controlState = KeyPad_KbdControl_Control;
}


/*******************************************************************************
* Function Name: KeyPad_KbdControl_RestoreConfig
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
void KeyPad_KbdControl_RestoreConfig(void) 
{
     KeyPad_KbdControl_Control = KeyPad_KbdControl_backup.controlState;
}


/*******************************************************************************
* Function Name: KeyPad_KbdControl_Sleep
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
void KeyPad_KbdControl_Sleep(void) 
{
    KeyPad_KbdControl_SaveConfig();
}


/*******************************************************************************
* Function Name: KeyPad_KbdControl_Wakeup
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
void KeyPad_KbdControl_Wakeup(void)  
{
    KeyPad_KbdControl_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
