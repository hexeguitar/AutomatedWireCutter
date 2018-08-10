/*******************************************************************************
* File Name: BTN_FUNC.c  
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
#include "BTN_FUNC.h"

static BTN_FUNC_BACKUP_STRUCT  BTN_FUNC_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: BTN_FUNC_Sleep
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
*  \snippet BTN_FUNC_SUT.c usage_BTN_FUNC_Sleep_Wakeup
*******************************************************************************/
void BTN_FUNC_Sleep(void)
{
    #if defined(BTN_FUNC__PC)
        BTN_FUNC_backup.pcState = BTN_FUNC_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            BTN_FUNC_backup.usbState = BTN_FUNC_CR1_REG;
            BTN_FUNC_USB_POWER_REG |= BTN_FUNC_USBIO_ENTER_SLEEP;
            BTN_FUNC_CR1_REG &= BTN_FUNC_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(BTN_FUNC__SIO)
        BTN_FUNC_backup.sioState = BTN_FUNC_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        BTN_FUNC_SIO_REG &= (uint32)(~BTN_FUNC_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: BTN_FUNC_Wakeup
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
*  Refer to BTN_FUNC_Sleep() for an example usage.
*******************************************************************************/
void BTN_FUNC_Wakeup(void)
{
    #if defined(BTN_FUNC__PC)
        BTN_FUNC_PC = BTN_FUNC_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            BTN_FUNC_USB_POWER_REG &= BTN_FUNC_USBIO_EXIT_SLEEP_PH1;
            BTN_FUNC_CR1_REG = BTN_FUNC_backup.usbState;
            BTN_FUNC_USB_POWER_REG &= BTN_FUNC_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(BTN_FUNC__SIO)
        BTN_FUNC_SIO_REG = BTN_FUNC_backup.sioState;
    #endif
}


/* [] END OF FILE */
