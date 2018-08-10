/*******************************************************************************
* File Name: WireSnsTimeout_PM.c
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

#include "WireSnsTimeout.h"

static WireSnsTimeout_BACKUP_STRUCT WireSnsTimeout_backup;


/*******************************************************************************
* Function Name: WireSnsTimeout_SaveConfig
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
void WireSnsTimeout_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: WireSnsTimeout_Sleep
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
void WireSnsTimeout_Sleep(void)
{
    if(0u != (WireSnsTimeout_BLOCK_CONTROL_REG & WireSnsTimeout_MASK))
    {
        WireSnsTimeout_backup.enableState = 1u;
    }
    else
    {
        WireSnsTimeout_backup.enableState = 0u;
    }

    WireSnsTimeout_Stop();
    WireSnsTimeout_SaveConfig();
}


/*******************************************************************************
* Function Name: WireSnsTimeout_RestoreConfig
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
void WireSnsTimeout_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: WireSnsTimeout_Wakeup
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
void WireSnsTimeout_Wakeup(void)
{
    WireSnsTimeout_RestoreConfig();

    if(0u != WireSnsTimeout_backup.enableState)
    {
        WireSnsTimeout_Enable();
    }
}


/* [] END OF FILE */
