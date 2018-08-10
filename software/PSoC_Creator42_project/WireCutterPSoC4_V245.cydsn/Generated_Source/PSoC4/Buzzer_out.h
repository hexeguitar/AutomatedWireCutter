/*******************************************************************************
* File Name: Buzzer_out.h  
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

#if !defined(CY_PINS_Buzzer_out_H) /* Pins Buzzer_out_H */
#define CY_PINS_Buzzer_out_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Buzzer_out_aliases.h"


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
} Buzzer_out_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Buzzer_out_Read(void);
void    Buzzer_out_Write(uint8 value);
uint8   Buzzer_out_ReadDataReg(void);
#if defined(Buzzer_out__PC) || (CY_PSOC4_4200L) 
    void    Buzzer_out_SetDriveMode(uint8 mode);
#endif
void    Buzzer_out_SetInterruptMode(uint16 position, uint16 mode);
uint8   Buzzer_out_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Buzzer_out_Sleep(void); 
void Buzzer_out_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Buzzer_out__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Buzzer_out_DRIVE_MODE_BITS        (3)
    #define Buzzer_out_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Buzzer_out_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Buzzer_out_SetDriveMode() function.
         *  @{
         */
        #define Buzzer_out_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Buzzer_out_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Buzzer_out_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Buzzer_out_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Buzzer_out_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Buzzer_out_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Buzzer_out_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Buzzer_out_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Buzzer_out_MASK               Buzzer_out__MASK
#define Buzzer_out_SHIFT              Buzzer_out__SHIFT
#define Buzzer_out_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Buzzer_out_SetInterruptMode() function.
     *  @{
     */
        #define Buzzer_out_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Buzzer_out_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Buzzer_out_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Buzzer_out_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Buzzer_out__SIO)
    #define Buzzer_out_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Buzzer_out__PC) && (CY_PSOC4_4200L)
    #define Buzzer_out_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Buzzer_out_USBIO_DISABLE              ((uint32)(~Buzzer_out_USBIO_ENABLE))
    #define Buzzer_out_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Buzzer_out_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Buzzer_out_USBIO_ENTER_SLEEP          ((uint32)((1u << Buzzer_out_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Buzzer_out_USBIO_SUSPEND_DEL_SHIFT)))
    #define Buzzer_out_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Buzzer_out_USBIO_SUSPEND_SHIFT)))
    #define Buzzer_out_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Buzzer_out_USBIO_SUSPEND_DEL_SHIFT)))
    #define Buzzer_out_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Buzzer_out__PC)
    /* Port Configuration */
    #define Buzzer_out_PC                 (* (reg32 *) Buzzer_out__PC)
#endif
/* Pin State */
#define Buzzer_out_PS                     (* (reg32 *) Buzzer_out__PS)
/* Data Register */
#define Buzzer_out_DR                     (* (reg32 *) Buzzer_out__DR)
/* Input Buffer Disable Override */
#define Buzzer_out_INP_DIS                (* (reg32 *) Buzzer_out__PC2)

/* Interrupt configuration Registers */
#define Buzzer_out_INTCFG                 (* (reg32 *) Buzzer_out__INTCFG)
#define Buzzer_out_INTSTAT                (* (reg32 *) Buzzer_out__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Buzzer_out_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Buzzer_out__SIO)
    #define Buzzer_out_SIO_REG            (* (reg32 *) Buzzer_out__SIO)
#endif /* (Buzzer_out__SIO_CFG) */

/* USBIO registers */
#if !defined(Buzzer_out__PC) && (CY_PSOC4_4200L)
    #define Buzzer_out_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Buzzer_out_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Buzzer_out_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Buzzer_out_DRIVE_MODE_SHIFT       (0x00u)
#define Buzzer_out_DRIVE_MODE_MASK        (0x07u << Buzzer_out_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Buzzer_out_H */


/* [] END OF FILE */
