/*******************************************************************************
* File Name: BTN_STOP.c  
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
#include "BTN_STOP.h"

static BTN_STOP_BACKUP_STRUCT  BTN_STOP_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: BTN_STOP_Sleep
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
*  \snippet BTN_STOP_SUT.c usage_BTN_STOP_Sleep_Wakeup
*******************************************************************************/
void BTN_STOP_Sleep(void)
{
    #if defined(BTN_STOP__PC)
        BTN_STOP_backup.pcState = BTN_STOP_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            BTN_STOP_backup.usbState = BTN_STOP_CR1_REG;
            BTN_STOP_USB_POWER_REG |= BTN_STOP_USBIO_ENTER_SLEEP;
            BTN_STOP_CR1_REG &= BTN_STOP_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(BTN_STOP__SIO)
        BTN_STOP_backup.sioState = BTN_STOP_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        BTN_STOP_SIO_REG &= (uint32)(~BTN_STOP_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: BTN_STOP_Wakeup
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
*  Refer to BTN_STOP_Sleep() for an example usage.
*******************************************************************************/
void BTN_STOP_Wakeup(void)
{
    #if defined(BTN_STOP__PC)
        BTN_STOP_PC = BTN_STOP_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            BTN_STOP_USB_POWER_REG &= BTN_STOP_USBIO_EXIT_SLEEP_PH1;
            BTN_STOP_CR1_REG = BTN_STOP_backup.usbState;
            BTN_STOP_USB_POWER_REG &= BTN_STOP_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(BTN_STOP__SIO)
        BTN_STOP_SIO_REG = BTN_STOP_backup.sioState;
    #endif
}


/* [] END OF FILE */
