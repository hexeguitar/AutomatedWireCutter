/*******************************************************************************
* File Name: TMC2130_RST.c  
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
#include "TMC2130_RST.h"

static TMC2130_RST_BACKUP_STRUCT  TMC2130_RST_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: TMC2130_RST_Sleep
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
*  \snippet TMC2130_RST_SUT.c usage_TMC2130_RST_Sleep_Wakeup
*******************************************************************************/
void TMC2130_RST_Sleep(void)
{
    #if defined(TMC2130_RST__PC)
        TMC2130_RST_backup.pcState = TMC2130_RST_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            TMC2130_RST_backup.usbState = TMC2130_RST_CR1_REG;
            TMC2130_RST_USB_POWER_REG |= TMC2130_RST_USBIO_ENTER_SLEEP;
            TMC2130_RST_CR1_REG &= TMC2130_RST_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(TMC2130_RST__SIO)
        TMC2130_RST_backup.sioState = TMC2130_RST_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        TMC2130_RST_SIO_REG &= (uint32)(~TMC2130_RST_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: TMC2130_RST_Wakeup
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
*  Refer to TMC2130_RST_Sleep() for an example usage.
*******************************************************************************/
void TMC2130_RST_Wakeup(void)
{
    #if defined(TMC2130_RST__PC)
        TMC2130_RST_PC = TMC2130_RST_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            TMC2130_RST_USB_POWER_REG &= TMC2130_RST_USBIO_EXIT_SLEEP_PH1;
            TMC2130_RST_CR1_REG = TMC2130_RST_backup.usbState;
            TMC2130_RST_USB_POWER_REG &= TMC2130_RST_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(TMC2130_RST__SIO)
        TMC2130_RST_SIO_REG = TMC2130_RST_backup.sioState;
    #endif
}


/* [] END OF FILE */
