/*******************************************************************************
* File Name: StepOut.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "StepOut.h"

static StepOut_BACKUP_STRUCT  StepOut_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: StepOut_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet StepOut_SUT.c usage_StepOut_Sleep_Wakeup
*******************************************************************************/
void StepOut_Sleep(void)
{
    #if defined(StepOut__PC)
        StepOut_backup.pcState = StepOut_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            StepOut_backup.usbState = StepOut_CR1_REG;
            StepOut_USB_POWER_REG |= StepOut_USBIO_ENTER_SLEEP;
            StepOut_CR1_REG &= StepOut_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(StepOut__SIO)
        StepOut_backup.sioState = StepOut_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        StepOut_SIO_REG &= (uint32)(~StepOut_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: StepOut_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to StepOut_Sleep() for an example usage.
*******************************************************************************/
void StepOut_Wakeup(void)
{
    #if defined(StepOut__PC)
        StepOut_PC = StepOut_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            StepOut_USB_POWER_REG &= StepOut_USBIO_EXIT_SLEEP_PH1;
            StepOut_CR1_REG = StepOut_backup.usbState;
            StepOut_USB_POWER_REG &= StepOut_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(StepOut__SIO)
        StepOut_SIO_REG = StepOut_backup.sioState;
    #endif
}


/* [] END OF FILE */
