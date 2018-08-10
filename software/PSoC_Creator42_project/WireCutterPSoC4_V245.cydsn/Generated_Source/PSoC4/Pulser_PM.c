/*******************************************************************************
* File Name: Pulser_PM.c
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

#include "Pulser.h"

static Pulser_BACKUP_STRUCT Pulser_backup;


/*******************************************************************************
* Function Name: Pulser_SaveConfig
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
void Pulser_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: Pulser_Sleep
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
void Pulser_Sleep(void)
{
    if(0u != (Pulser_BLOCK_CONTROL_REG & Pulser_MASK))
    {
        Pulser_backup.enableState = 1u;
    }
    else
    {
        Pulser_backup.enableState = 0u;
    }

    Pulser_Stop();
    Pulser_SaveConfig();
}


/*******************************************************************************
* Function Name: Pulser_RestoreConfig
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
void Pulser_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: Pulser_Wakeup
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
void Pulser_Wakeup(void)
{
    Pulser_RestoreConfig();

    if(0u != Pulser_backup.enableState)
    {
        Pulser_Enable();
    }
}


/* [] END OF FILE */
