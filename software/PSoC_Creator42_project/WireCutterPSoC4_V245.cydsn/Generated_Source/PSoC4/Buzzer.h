/*******************************************************************************
* File Name: Buzzer.h  
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

#if !defined(CY_PINS_Buzzer_H) /* Pins Buzzer_H */
#define CY_PINS_Buzzer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Buzzer_aliases.h"


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
} Buzzer_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Buzzer_Read(void);
void    Buzzer_Write(uint8 value);
uint8   Buzzer_ReadDataReg(void);
#if defined(Buzzer__PC) || (CY_PSOC4_4200L) 
    void    Buzzer_SetDriveMode(uint8 mode);
#endif
void    Buzzer_SetInterruptMode(uint16 position, uint16 mode);
uint8   Buzzer_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Buzzer_Sleep(void); 
void Buzzer_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Buzzer__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Buzzer_DRIVE_MODE_BITS        (3)
    #define Buzzer_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Buzzer_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Buzzer_SetDriveMode() function.
         *  @{
         */
        #define Buzzer_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Buzzer_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Buzzer_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Buzzer_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Buzzer_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Buzzer_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Buzzer_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Buzzer_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Buzzer_MASK               Buzzer__MASK
#define Buzzer_SHIFT              Buzzer__SHIFT
#define Buzzer_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Buzzer_SetInterruptMode() function.
     *  @{
     */
        #define Buzzer_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Buzzer_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Buzzer_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Buzzer_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Buzzer__SIO)
    #define Buzzer_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Buzzer__PC) && (CY_PSOC4_4200L)
    #define Buzzer_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Buzzer_USBIO_DISABLE              ((uint32)(~Buzzer_USBIO_ENABLE))
    #define Buzzer_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Buzzer_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Buzzer_USBIO_ENTER_SLEEP          ((uint32)((1u << Buzzer_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Buzzer_USBIO_SUSPEND_DEL_SHIFT)))
    #define Buzzer_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Buzzer_USBIO_SUSPEND_SHIFT)))
    #define Buzzer_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Buzzer_USBIO_SUSPEND_DEL_SHIFT)))
    #define Buzzer_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Buzzer__PC)
    /* Port Configuration */
    #define Buzzer_PC                 (* (reg32 *) Buzzer__PC)
#endif
/* Pin State */
#define Buzzer_PS                     (* (reg32 *) Buzzer__PS)
/* Data Register */
#define Buzzer_DR                     (* (reg32 *) Buzzer__DR)
/* Input Buffer Disable Override */
#define Buzzer_INP_DIS                (* (reg32 *) Buzzer__PC2)

/* Interrupt configuration Registers */
#define Buzzer_INTCFG                 (* (reg32 *) Buzzer__INTCFG)
#define Buzzer_INTSTAT                (* (reg32 *) Buzzer__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Buzzer_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Buzzer__SIO)
    #define Buzzer_SIO_REG            (* (reg32 *) Buzzer__SIO)
#endif /* (Buzzer__SIO_CFG) */

/* USBIO registers */
#if !defined(Buzzer__PC) && (CY_PSOC4_4200L)
    #define Buzzer_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Buzzer_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Buzzer_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Buzzer_DRIVE_MODE_SHIFT       (0x00u)
#define Buzzer_DRIVE_MODE_MASK        (0x07u << Buzzer_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Buzzer_H */


/* [] END OF FILE */
