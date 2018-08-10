/*******************************************************************************
* File Name: StepDir.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_StepDir_H) /* Pins StepDir_H */
#define CY_PINS_StepDir_H

#include "cytypes.h"
#include "cyfitter.h"
#include "StepDir_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} StepDir_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   StepDir_Read(void);
void    StepDir_Write(uint8 value);
uint8   StepDir_ReadDataReg(void);
#if defined(StepDir__PC) || (CY_PSOC4_4200L) 
    void    StepDir_SetDriveMode(uint8 mode);
#endif
void    StepDir_SetInterruptMode(uint16 position, uint16 mode);
uint8   StepDir_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void StepDir_Sleep(void); 
void StepDir_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(StepDir__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define StepDir_DRIVE_MODE_BITS        (3)
    #define StepDir_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - StepDir_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the StepDir_SetDriveMode() function.
         *  @{
         */
        #define StepDir_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define StepDir_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define StepDir_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define StepDir_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define StepDir_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define StepDir_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define StepDir_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define StepDir_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define StepDir_MASK               StepDir__MASK
#define StepDir_SHIFT              StepDir__SHIFT
#define StepDir_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in StepDir_SetInterruptMode() function.
     *  @{
     */
        #define StepDir_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define StepDir_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define StepDir_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define StepDir_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(StepDir__SIO)
    #define StepDir_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(StepDir__PC) && (CY_PSOC4_4200L)
    #define StepDir_USBIO_ENABLE               ((uint32)0x80000000u)
    #define StepDir_USBIO_DISABLE              ((uint32)(~StepDir_USBIO_ENABLE))
    #define StepDir_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define StepDir_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define StepDir_USBIO_ENTER_SLEEP          ((uint32)((1u << StepDir_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << StepDir_USBIO_SUSPEND_DEL_SHIFT)))
    #define StepDir_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << StepDir_USBIO_SUSPEND_SHIFT)))
    #define StepDir_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << StepDir_USBIO_SUSPEND_DEL_SHIFT)))
    #define StepDir_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(StepDir__PC)
    /* Port Configuration */
    #define StepDir_PC                 (* (reg32 *) StepDir__PC)
#endif
/* Pin State */
#define StepDir_PS                     (* (reg32 *) StepDir__PS)
/* Data Register */
#define StepDir_DR                     (* (reg32 *) StepDir__DR)
/* Input Buffer Disable Override */
#define StepDir_INP_DIS                (* (reg32 *) StepDir__PC2)

/* Interrupt configuration Registers */
#define StepDir_INTCFG                 (* (reg32 *) StepDir__INTCFG)
#define StepDir_INTSTAT                (* (reg32 *) StepDir__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define StepDir_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(StepDir__SIO)
    #define StepDir_SIO_REG            (* (reg32 *) StepDir__SIO)
#endif /* (StepDir__SIO_CFG) */

/* USBIO registers */
#if !defined(StepDir__PC) && (CY_PSOC4_4200L)
    #define StepDir_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define StepDir_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define StepDir_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define StepDir_DRIVE_MODE_SHIFT       (0x00u)
#define StepDir_DRIVE_MODE_MASK        (0x07u << StepDir_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins StepDir_H */


/* [] END OF FILE */
