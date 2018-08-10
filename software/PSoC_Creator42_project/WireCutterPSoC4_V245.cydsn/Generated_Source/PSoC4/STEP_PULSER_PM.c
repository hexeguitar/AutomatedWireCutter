/*******************************************************************************
* File Name: STEP_PULSER_PM.c
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

#include "STEP_PULSER.h"

static STEP_PULSER_BACKUP_STRUCT STEP_PULSER_backup;


/*******************************************************************************
* Function Name: STEP_PULSER_SaveConfig
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
void STEP_PULSER_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: STEP_PULSER_Sleep
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
void STEP_PULSER_Sleep(void)
{
    if(0u != (STEP_PULSER_BLOCK_CONTROL_REG & STEP_PULSER_MASK))
    {
        STEP_PULSER_backup.enableState = 1u;
    }
    else
    {
        STEP_PULSER_backup.enableState = 0u;
    }

    STEP_PULSER_Stop();
    STEP_PULSER_SaveConfig();
}


/*******************************************************************************
* Function Name: STEP_PULSER_RestoreConfig
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
void STEP_PULSER_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: STEP_PULSER_Wakeup
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
void STEP_PULSER_Wakeup(void)
{
    STEP_PULSER_RestoreConfig();

    if(0u != STEP_PULSER_backup.enableState)
    {
        STEP_PULSER_Enable();
    }
}


/* [] END OF FILE */
