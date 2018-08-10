/*******************************************************************************
* File Name: StepCounter_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "StepCounter.h"

static StepCounter_backupStruct StepCounter_backup;


/*******************************************************************************
* Function Name: StepCounter_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  StepCounter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void StepCounter_SaveConfig(void) 
{
    #if (!StepCounter_UsingFixedFunction)

        StepCounter_backup.CounterUdb = StepCounter_ReadCounter();

        #if(!StepCounter_ControlRegRemoved)
            StepCounter_backup.CounterControlRegister = StepCounter_ReadControlRegister();
        #endif /* (!StepCounter_ControlRegRemoved) */

    #endif /* (!StepCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepCounter_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  StepCounter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void StepCounter_RestoreConfig(void) 
{      
    #if (!StepCounter_UsingFixedFunction)

       StepCounter_WriteCounter(StepCounter_backup.CounterUdb);

        #if(!StepCounter_ControlRegRemoved)
            StepCounter_WriteControlRegister(StepCounter_backup.CounterControlRegister);
        #endif /* (!StepCounter_ControlRegRemoved) */

    #endif /* (!StepCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepCounter_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  StepCounter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void StepCounter_Sleep(void) 
{
    #if(!StepCounter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(StepCounter_CTRL_ENABLE == (StepCounter_CONTROL & StepCounter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            StepCounter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            StepCounter_backup.CounterEnableState = 0u;
        }
    #else
        StepCounter_backup.CounterEnableState = 1u;
        if(StepCounter_backup.CounterEnableState != 0u)
        {
            StepCounter_backup.CounterEnableState = 0u;
        }
    #endif /* (!StepCounter_ControlRegRemoved) */
    
    StepCounter_Stop();
    StepCounter_SaveConfig();
}


/*******************************************************************************
* Function Name: StepCounter_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  StepCounter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void StepCounter_Wakeup(void) 
{
    StepCounter_RestoreConfig();
    #if(!StepCounter_ControlRegRemoved)
        if(StepCounter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            StepCounter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!StepCounter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
