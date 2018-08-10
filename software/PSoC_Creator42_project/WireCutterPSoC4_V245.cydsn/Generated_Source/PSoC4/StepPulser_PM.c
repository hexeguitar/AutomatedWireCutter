/*******************************************************************************
* File Name: StepPulser_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "StepPulser.h"


/***************************************
* Local data allocation
***************************************/

static StepPulser_BACKUP_STRUCT  StepPulser_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: StepPulser_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the StepPulser_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  StepPulser_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void StepPulser_SaveConfig(void)
{
    #if(StepPulser_CONTROL_REG_REMOVED == 0u)
        StepPulser_backup.cr = StepPulser_CONTROL_REG;
    #endif /* End StepPulser_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: StepPulser_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  StepPulser_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling StepPulser_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void StepPulser_RestoreConfig(void)
{
    #if(StepPulser_CONTROL_REG_REMOVED == 0u)
        StepPulser_CONTROL_REG = StepPulser_backup.cr;
    #endif /* End StepPulser_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: StepPulser_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The StepPulser_Sleep() API saves the current component state. Then it
*  calls the StepPulser_Stop() function and calls 
*  StepPulser_SaveConfig() to save the hardware configuration.
*  Call the StepPulser_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  StepPulser_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void StepPulser_Sleep(void)
{
    #if(StepPulser_RX_ENABLED || StepPulser_HD_ENABLED)
        if((StepPulser_RXSTATUS_ACTL_REG  & StepPulser_INT_ENABLE) != 0u)
        {
            StepPulser_backup.enableState = 1u;
        }
        else
        {
            StepPulser_backup.enableState = 0u;
        }
    #else
        if((StepPulser_TXSTATUS_ACTL_REG  & StepPulser_INT_ENABLE) !=0u)
        {
            StepPulser_backup.enableState = 1u;
        }
        else
        {
            StepPulser_backup.enableState = 0u;
        }
    #endif /* End StepPulser_RX_ENABLED || StepPulser_HD_ENABLED*/

    StepPulser_Stop();
    StepPulser_SaveConfig();
}


/*******************************************************************************
* Function Name: StepPulser_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  StepPulser_Sleep() was called. The StepPulser_Wakeup() function
*  calls the StepPulser_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  StepPulser_Sleep() function was called, the StepPulser_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  StepPulser_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void StepPulser_Wakeup(void)
{
    StepPulser_RestoreConfig();
    #if( (StepPulser_RX_ENABLED) || (StepPulser_HD_ENABLED) )
        StepPulser_ClearRxBuffer();
    #endif /* End (StepPulser_RX_ENABLED) || (StepPulser_HD_ENABLED) */
    #if(StepPulser_TX_ENABLED || StepPulser_HD_ENABLED)
        StepPulser_ClearTxBuffer();
    #endif /* End StepPulser_TX_ENABLED || StepPulser_HD_ENABLED */

    if(StepPulser_backup.enableState != 0u)
    {
        StepPulser_Enable();
    }
}


/* [] END OF FILE */
