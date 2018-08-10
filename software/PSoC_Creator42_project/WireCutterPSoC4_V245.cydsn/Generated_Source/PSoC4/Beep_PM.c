/*******************************************************************************
* File Name: Beep_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
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

#include "Beep.h"

static Beep_BACKUP_STRUCT Beep_backup;


/*******************************************************************************
* Function Name: Beep_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Beep_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: Beep_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Beep_Sleep(void)
{
    if(0u != (Beep_BLOCK_CONTROL_REG & Beep_MASK))
    {
        Beep_backup.enableState = 1u;
    }
    else
    {
        Beep_backup.enableState = 0u;
    }

    Beep_Stop();
    Beep_SaveConfig();
}


/*******************************************************************************
* Function Name: Beep_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Beep_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: Beep_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Beep_Wakeup(void)
{
    Beep_RestoreConfig();

    if(0u != Beep_backup.enableState)
    {
        Beep_Enable();
    }
}


/* [] END OF FILE */
