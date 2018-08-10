/*******************************************************************************
* File Name: BuzzDriver_PM.c
* Version 2.30
*
* Description:
*  This file provides the API source code for sleep mode support for Shift
*  Register component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "BuzzDriver.h"

static BuzzDriver_BACKUP_STRUCT BuzzDriver_backup =
{
    BuzzDriver_DISABLED,

    ((uint8) BuzzDriver_DEFAULT_A0),
    ((uint8) BuzzDriver_DEFAULT_A1),

    #if(CY_UDB_V0)
        ((uint8) BuzzDriver_INT_SRC),
    #endif /* (CY_UDB_V0) */
};


/*******************************************************************************
* Function Name: BuzzDriver_SaveConfig
********************************************************************************
*
* Summary:
*  Saves Shift Register configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void BuzzDriver_SaveConfig(void) 
{
    /* Store working registers A0 and A1 */
    BuzzDriver_backup.saveSrA0Reg   = CY_GET_REG8(BuzzDriver_SHIFT_REG_LSB_PTR);
    BuzzDriver_backup.saveSrA1Reg   = CY_GET_REG8(BuzzDriver_SHIFT_REG_VALUE_LSB_PTR);

    #if(CY_UDB_V0)
        BuzzDriver_backup.saveSrIntMask = BuzzDriver_SR_STATUS_MASK;
    #endif /* (CY_UDB_V0) */
}


/*******************************************************************************
* Function Name: BuzzDriver_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores Shift Register configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void BuzzDriver_RestoreConfig(void) 
{
    /* Restore working registers A0 and A1 */
    CY_SET_REG8(BuzzDriver_SHIFT_REG_LSB_PTR, BuzzDriver_backup.saveSrA0Reg);
    CY_SET_REG8(BuzzDriver_SHIFT_REG_VALUE_LSB_PTR, BuzzDriver_backup.saveSrA1Reg);

    #if(CY_UDB_V0)
        BuzzDriver_SR_STATUS_MASK = ((uint8) BuzzDriver_backup.saveSrIntMask);
    #endif /* (CY_UDB_V0) */
}


/*******************************************************************************
* Function Name: BuzzDriver_Sleep
********************************************************************************
*
* Summary:
*  Prepare the component to enter a Sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void BuzzDriver_Sleep(void) 
{
    BuzzDriver_backup.enableState = ((uint8) BuzzDriver_IS_ENABLED);

    BuzzDriver_Stop();
    BuzzDriver_SaveConfig();
}


/*******************************************************************************
* Function Name: BuzzDriver_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void BuzzDriver_Wakeup(void) 
{
    BuzzDriver_RestoreConfig();

    if(0u != BuzzDriver_backup.enableState)
    {
        BuzzDriver_Enable();
    }
}


/* [] END OF FILE */
