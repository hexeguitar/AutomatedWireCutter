/*******************************************************************************
* File Name: ADC12_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC12.h"


/***************************************
* Local data allocation
***************************************/

static ADC12_BACKUP_STRUCT  ADC12_backup =
{
    ADC12_DISABLED,
    0u    
};


/*******************************************************************************
* Function Name: ADC12_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC12_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC12_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC12_RestoreConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC12_Sleep
********************************************************************************
*
* Summary:
*  Stops the ADC operation and saves the configuration registers and component
*  enable state. Should be called just prior to entering sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC12_backup - modified.
*
*******************************************************************************/
void ADC12_Sleep(void)
{
    /* During deepsleep/ hibernate mode keep SARMUX active, i.e. do not open
    *   all switches (disconnect), to be used for ADFT
    */
    ADC12_backup.dftRegVal = ADC12_SAR_DFT_CTRL_REG & (uint32)~ADC12_ADFT_OVERRIDE;
    ADC12_SAR_DFT_CTRL_REG |= ADC12_ADFT_OVERRIDE;
    if((ADC12_SAR_CTRL_REG  & ADC12_ENABLE) != 0u)
    {
        if((ADC12_SAR_SAMPLE_CTRL_REG & ADC12_CONTINUOUS_EN) != 0u)
        {
            ADC12_backup.enableState = ADC12_ENABLED | ADC12_STARTED;
        }
        else
        {
            ADC12_backup.enableState = ADC12_ENABLED;
        }
        ADC12_StopConvert();
        ADC12_Stop();
        
        /* Disable the SAR internal pump before entering the chip low power mode */
        if((ADC12_SAR_CTRL_REG & ADC12_BOOSTPUMP_EN) != 0u)
        {
            ADC12_SAR_CTRL_REG &= (uint32)~ADC12_BOOSTPUMP_EN;
            ADC12_backup.enableState |= ADC12_BOOSTPUMP_ENABLED;
        }
    }
    else
    {
        ADC12_backup.enableState = ADC12_DISABLED;
    }
}


/*******************************************************************************
* Function Name: ADC12_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component enable state and configuration registers.
*  This should be called just after awaking from sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC12_backup - used.
*
*******************************************************************************/
void ADC12_Wakeup(void)
{
    ADC12_SAR_DFT_CTRL_REG = ADC12_backup.dftRegVal;
    if(ADC12_backup.enableState != ADC12_DISABLED)
    {
        /* Enable the SAR internal pump  */
        if((ADC12_backup.enableState & ADC12_BOOSTPUMP_ENABLED) != 0u)
        {
            ADC12_SAR_CTRL_REG |= ADC12_BOOSTPUMP_EN;
        }
        ADC12_Enable();
        if((ADC12_backup.enableState & ADC12_STARTED) != 0u)
        {
            ADC12_StartConvert();
        }
    }
}
/* [] END OF FILE */
