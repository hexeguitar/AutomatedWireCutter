/*******************************************************************************
* File Name: Servo_Ctrl.h  
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

#if !defined(CY_PINS_Servo_Ctrl_H) /* Pins Servo_Ctrl_H */
#define CY_PINS_Servo_Ctrl_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Servo_Ctrl_aliases.h"


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
} Servo_Ctrl_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Servo_Ctrl_Read(void);
void    Servo_Ctrl_Write(uint8 value);
uint8   Servo_Ctrl_ReadDataReg(void);
#if defined(Servo_Ctrl__PC) || (CY_PSOC4_4200L) 
    void    Servo_Ctrl_SetDriveMode(uint8 mode);
#endif
void    Servo_Ctrl_SetInterruptMode(uint16 position, uint16 mode);
uint8   Servo_Ctrl_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Servo_Ctrl_Sleep(void); 
void Servo_Ctrl_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Servo_Ctrl__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Servo_Ctrl_DRIVE_MODE_BITS        (3)
    #define Servo_Ctrl_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Servo_Ctrl_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Servo_Ctrl_SetDriveMode() function.
         *  @{
         */
        #define Servo_Ctrl_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Servo_Ctrl_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Servo_Ctrl_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Servo_Ctrl_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Servo_Ctrl_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Servo_Ctrl_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Servo_Ctrl_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Servo_Ctrl_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Servo_Ctrl_MASK               Servo_Ctrl__MASK
#define Servo_Ctrl_SHIFT              Servo_Ctrl__SHIFT
#define Servo_Ctrl_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Servo_Ctrl_SetInterruptMode() function.
     *  @{
     */
        #define Servo_Ctrl_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Servo_Ctrl_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Servo_Ctrl_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Servo_Ctrl_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Servo_Ctrl__SIO)
    #define Servo_Ctrl_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Servo_Ctrl__PC) && (CY_PSOC4_4200L)
    #define Servo_Ctrl_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Servo_Ctrl_USBIO_DISABLE              ((uint32)(~Servo_Ctrl_USBIO_ENABLE))
    #define Servo_Ctrl_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Servo_Ctrl_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Servo_Ctrl_USBIO_ENTER_SLEEP          ((uint32)((1u << Servo_Ctrl_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Servo_Ctrl_USBIO_SUSPEND_DEL_SHIFT)))
    #define Servo_Ctrl_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Servo_Ctrl_USBIO_SUSPEND_SHIFT)))
    #define Servo_Ctrl_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Servo_Ctrl_USBIO_SUSPEND_DEL_SHIFT)))
    #define Servo_Ctrl_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Servo_Ctrl__PC)
    /* Port Configuration */
    #define Servo_Ctrl_PC                 (* (reg32 *) Servo_Ctrl__PC)
#endif
/* Pin State */
#define Servo_Ctrl_PS                     (* (reg32 *) Servo_Ctrl__PS)
/* Data Register */
#define Servo_Ctrl_DR                     (* (reg32 *) Servo_Ctrl__DR)
/* Input Buffer Disable Override */
#define Servo_Ctrl_INP_DIS                (* (reg32 *) Servo_Ctrl__PC2)

/* Interrupt configuration Registers */
#define Servo_Ctrl_INTCFG                 (* (reg32 *) Servo_Ctrl__INTCFG)
#define Servo_Ctrl_INTSTAT                (* (reg32 *) Servo_Ctrl__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Servo_Ctrl_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Servo_Ctrl__SIO)
    #define Servo_Ctrl_SIO_REG            (* (reg32 *) Servo_Ctrl__SIO)
#endif /* (Servo_Ctrl__SIO_CFG) */

/* USBIO registers */
#if !defined(Servo_Ctrl__PC) && (CY_PSOC4_4200L)
    #define Servo_Ctrl_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Servo_Ctrl_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Servo_Ctrl_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Servo_Ctrl_DRIVE_MODE_SHIFT       (0x00u)
#define Servo_Ctrl_DRIVE_MODE_MASK        (0x07u << Servo_Ctrl_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Servo_Ctrl_H */


/* [] END OF FILE */
