/*******************************************************************************
* File Name: Servo_Cut_PM.c
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

#include "Servo_Cut.h"

static Servo_Cut_BACKUP_STRUCT Servo_Cut_backup;


/*******************************************************************************
* Function Name: Servo_Cut_SaveConfig
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
void Servo_Cut_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: Servo_Cut_Sleep
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
void Servo_Cut_Sleep(void)
{
    if(0u != (Servo_Cut_BLOCK_CONTROL_REG & Servo_Cut_MASK))
    {
        Servo_Cut_backup.enableState = 1u;
    }
    else
    {
        Servo_Cut_backup.enableState = 0u;
    }

    Servo_Cut_Stop();
    Servo_Cut_SaveConfig();
}


/*******************************************************************************
* Function Name: Servo_Cut_RestoreConfig
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
void Servo_Cut_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: Servo_Cut_Wakeup
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
void Servo_Cut_Wakeup(void)
{
    Servo_Cut_RestoreConfig();

    if(0u != Servo_Cut_backup.enableState)
    {
        Servo_Cut_Enable();
    }
}


/* [] END OF FILE */
