/*******************************************************************************
* File Name: MatrixKeypad_Columns.c  
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
#include "MatrixKeypad_Columns.h"

static MatrixKeypad_Columns_BACKUP_STRUCT  MatrixKeypad_Columns_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: MatrixKeypad_Columns_Sleep
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
*  \snippet MatrixKeypad_Columns_SUT.c usage_MatrixKeypad_Columns_Sleep_Wakeup
*******************************************************************************/
void MatrixKeypad_Columns_Sleep(void)
{
    #if defined(MatrixKeypad_Columns__PC)
        MatrixKeypad_Columns_backup.pcState = MatrixKeypad_Columns_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            MatrixKeypad_Columns_backup.usbState = MatrixKeypad_Columns_CR1_REG;
            MatrixKeypad_Columns_USB_POWER_REG |= MatrixKeypad_Columns_USBIO_ENTER_SLEEP;
            MatrixKeypad_Columns_CR1_REG &= MatrixKeypad_Columns_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(MatrixKeypad_Columns__SIO)
        MatrixKeypad_Columns_backup.sioState = MatrixKeypad_Columns_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        MatrixKeypad_Columns_SIO_REG &= (uint32)(~MatrixKeypad_Columns_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: MatrixKeypad_Columns_Wakeup
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
*  Refer to MatrixKeypad_Columns_Sleep() for an example usage.
*******************************************************************************/
void MatrixKeypad_Columns_Wakeup(void)
{
    #if defined(MatrixKeypad_Columns__PC)
        MatrixKeypad_Columns_PC = MatrixKeypad_Columns_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            MatrixKeypad_Columns_USB_POWER_REG &= MatrixKeypad_Columns_USBIO_EXIT_SLEEP_PH1;
            MatrixKeypad_Columns_CR1_REG = MatrixKeypad_Columns_backup.usbState;
            MatrixKeypad_Columns_USB_POWER_REG &= MatrixKeypad_Columns_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(MatrixKeypad_Columns__SIO)
        MatrixKeypad_Columns_SIO_REG = MatrixKeypad_Columns_backup.sioState;
    #endif
}


/* [] END OF FILE */
