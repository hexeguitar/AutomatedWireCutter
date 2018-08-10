/*******************************************************************************
* File Name: OA_INN.c  
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
#include "OA_INN.h"

static OA_INN_BACKUP_STRUCT  OA_INN_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: OA_INN_Sleep
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
*  \snippet OA_INN_SUT.c usage_OA_INN_Sleep_Wakeup
*******************************************************************************/
void OA_INN_Sleep(void)
{
    #if defined(OA_INN__PC)
        OA_INN_backup.pcState = OA_INN_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            OA_INN_backup.usbState = OA_INN_CR1_REG;
            OA_INN_USB_POWER_REG |= OA_INN_USBIO_ENTER_SLEEP;
            OA_INN_CR1_REG &= OA_INN_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(OA_INN__SIO)
        OA_INN_backup.sioState = OA_INN_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        OA_INN_SIO_REG &= (uint32)(~OA_INN_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: OA_INN_Wakeup
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
*  Refer to OA_INN_Sleep() for an example usage.
*******************************************************************************/
void OA_INN_Wakeup(void)
{
    #if defined(OA_INN__PC)
        OA_INN_PC = OA_INN_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            OA_INN_USB_POWER_REG &= OA_INN_USBIO_EXIT_SLEEP_PH1;
            OA_INN_CR1_REG = OA_INN_backup.usbState;
            OA_INN_USB_POWER_REG &= OA_INN_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(OA_INN__SIO)
        OA_INN_SIO_REG = OA_INN_backup.sioState;
    #endif
}


/* [] END OF FILE */
