/*******************************************************************************
* File Name: BtnLongPressDetector_PM.c
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

#include "BtnLongPressDetector.h"

static BtnLongPressDetector_BACKUP_STRUCT BtnLongPressDetector_backup;


/*******************************************************************************
* Function Name: BtnLongPressDetector_SaveConfig
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
void BtnLongPressDetector_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: BtnLongPressDetector_Sleep
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
void BtnLongPressDetector_Sleep(void)
{
    if(0u != (BtnLongPressDetector_BLOCK_CONTROL_REG & BtnLongPressDetector_MASK))
    {
        BtnLongPressDetector_backup.enableState = 1u;
    }
    else
    {
        BtnLongPressDetector_backup.enableState = 0u;
    }

    BtnLongPressDetector_Stop();
    BtnLongPressDetector_SaveConfig();
}


/*******************************************************************************
* Function Name: BtnLongPressDetector_RestoreConfig
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
void BtnLongPressDetector_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: BtnLongPressDetector_Wakeup
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
void BtnLongPressDetector_Wakeup(void)
{
    BtnLongPressDetector_RestoreConfig();

    if(0u != BtnLongPressDetector_backup.enableState)
    {
        BtnLongPressDetector_Enable();
    }
}


/* [] END OF FILE */
