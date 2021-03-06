/*******************************************************************************
* File Name: Servo_PWR.c  
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
#include "Servo_PWR.h"

static Servo_PWR_BACKUP_STRUCT  Servo_PWR_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Servo_PWR_Sleep
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
*  \snippet Servo_PWR_SUT.c usage_Servo_PWR_Sleep_Wakeup
*******************************************************************************/
void Servo_PWR_Sleep(void)
{
    #if defined(Servo_PWR__PC)
        Servo_PWR_backup.pcState = Servo_PWR_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Servo_PWR_backup.usbState = Servo_PWR_CR1_REG;
            Servo_PWR_USB_POWER_REG |= Servo_PWR_USBIO_ENTER_SLEEP;
            Servo_PWR_CR1_REG &= Servo_PWR_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Servo_PWR__SIO)
        Servo_PWR_backup.sioState = Servo_PWR_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Servo_PWR_SIO_REG &= (uint32)(~Servo_PWR_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Servo_PWR_Wakeup
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
*  Refer to Servo_PWR_Sleep() for an example usage.
*******************************************************************************/
void Servo_PWR_Wakeup(void)
{
    #if defined(Servo_PWR__PC)
        Servo_PWR_PC = Servo_PWR_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Servo_PWR_USB_POWER_REG &= Servo_PWR_USBIO_EXIT_SLEEP_PH1;
            Servo_PWR_CR1_REG = Servo_PWR_backup.usbState;
            Servo_PWR_USB_POWER_REG &= Servo_PWR_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Servo_PWR__SIO)
        Servo_PWR_SIO_REG = Servo_PWR_backup.sioState;
    #endif
}


/* [] END OF FILE */
