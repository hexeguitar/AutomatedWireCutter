/*******************************************************************************
* File Name: KeypadCLK.h  
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

#if !defined(CY_PINS_KeypadCLK_H) /* Pins KeypadCLK_H */
#define CY_PINS_KeypadCLK_H

#include "cytypes.h"
#include "cyfitter.h"
#include "KeypadCLK_aliases.h"


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
} KeypadCLK_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   KeypadCLK_Read(void);
void    KeypadCLK_Write(uint8 value);
uint8   KeypadCLK_ReadDataReg(void);
#if defined(KeypadCLK__PC) || (CY_PSOC4_4200L) 
    void    KeypadCLK_SetDriveMode(uint8 mode);
#endif
void    KeypadCLK_SetInterruptMode(uint16 position, uint16 mode);
uint8   KeypadCLK_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void KeypadCLK_Sleep(void); 
void KeypadCLK_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(KeypadCLK__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define KeypadCLK_DRIVE_MODE_BITS        (3)
    #define KeypadCLK_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - KeypadCLK_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the KeypadCLK_SetDriveMode() function.
         *  @{
         */
        #define KeypadCLK_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define KeypadCLK_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define KeypadCLK_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define KeypadCLK_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define KeypadCLK_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define KeypadCLK_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define KeypadCLK_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define KeypadCLK_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define KeypadCLK_MASK               KeypadCLK__MASK
#define KeypadCLK_SHIFT              KeypadCLK__SHIFT
#define KeypadCLK_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in KeypadCLK_SetInterruptMode() function.
     *  @{
     */
        #define KeypadCLK_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define KeypadCLK_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define KeypadCLK_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define KeypadCLK_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(KeypadCLK__SIO)
    #define KeypadCLK_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(KeypadCLK__PC) && (CY_PSOC4_4200L)
    #define KeypadCLK_USBIO_ENABLE               ((uint32)0x80000000u)
    #define KeypadCLK_USBIO_DISABLE              ((uint32)(~KeypadCLK_USBIO_ENABLE))
    #define KeypadCLK_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define KeypadCLK_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define KeypadCLK_USBIO_ENTER_SLEEP          ((uint32)((1u << KeypadCLK_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << KeypadCLK_USBIO_SUSPEND_DEL_SHIFT)))
    #define KeypadCLK_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << KeypadCLK_USBIO_SUSPEND_SHIFT)))
    #define KeypadCLK_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << KeypadCLK_USBIO_SUSPEND_DEL_SHIFT)))
    #define KeypadCLK_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(KeypadCLK__PC)
    /* Port Configuration */
    #define KeypadCLK_PC                 (* (reg32 *) KeypadCLK__PC)
#endif
/* Pin State */
#define KeypadCLK_PS                     (* (reg32 *) KeypadCLK__PS)
/* Data Register */
#define KeypadCLK_DR                     (* (reg32 *) KeypadCLK__DR)
/* Input Buffer Disable Override */
#define KeypadCLK_INP_DIS                (* (reg32 *) KeypadCLK__PC2)

/* Interrupt configuration Registers */
#define KeypadCLK_INTCFG                 (* (reg32 *) KeypadCLK__INTCFG)
#define KeypadCLK_INTSTAT                (* (reg32 *) KeypadCLK__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define KeypadCLK_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(KeypadCLK__SIO)
    #define KeypadCLK_SIO_REG            (* (reg32 *) KeypadCLK__SIO)
#endif /* (KeypadCLK__SIO_CFG) */

/* USBIO registers */
#if !defined(KeypadCLK__PC) && (CY_PSOC4_4200L)
    #define KeypadCLK_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define KeypadCLK_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define KeypadCLK_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define KeypadCLK_DRIVE_MODE_SHIFT       (0x00u)
#define KeypadCLK_DRIVE_MODE_MASK        (0x07u << KeypadCLK_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins KeypadCLK_H */


/* [] END OF FILE */
