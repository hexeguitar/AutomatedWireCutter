/*******************************************************************************
* File Name: TX.h  
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

#if !defined(CY_PINS_TX_H) /* Pins TX_H */
#define CY_PINS_TX_H

#include "cytypes.h"
#include "cyfitter.h"
#include "TX_aliases.h"


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
} TX_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   TX_Read(void);
void    TX_Write(uint8 value);
uint8   TX_ReadDataReg(void);
#if defined(TX__PC) || (CY_PSOC4_4200L) 
    void    TX_SetDriveMode(uint8 mode);
#endif
void    TX_SetInterruptMode(uint16 position, uint16 mode);
uint8   TX_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void TX_Sleep(void); 
void TX_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(TX__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define TX_DRIVE_MODE_BITS        (3)
    #define TX_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - TX_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the TX_SetDriveMode() function.
         *  @{
         */
        #define TX_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define TX_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define TX_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define TX_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define TX_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define TX_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define TX_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define TX_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define TX_MASK               TX__MASK
#define TX_SHIFT              TX__SHIFT
#define TX_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TX_SetInterruptMode() function.
     *  @{
     */
        #define TX_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define TX_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define TX_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define TX_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(TX__SIO)
    #define TX_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(TX__PC) && (CY_PSOC4_4200L)
    #define TX_USBIO_ENABLE               ((uint32)0x80000000u)
    #define TX_USBIO_DISABLE              ((uint32)(~TX_USBIO_ENABLE))
    #define TX_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define TX_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define TX_USBIO_ENTER_SLEEP          ((uint32)((1u << TX_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << TX_USBIO_SUSPEND_DEL_SHIFT)))
    #define TX_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << TX_USBIO_SUSPEND_SHIFT)))
    #define TX_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << TX_USBIO_SUSPEND_DEL_SHIFT)))
    #define TX_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(TX__PC)
    /* Port Configuration */
    #define TX_PC                 (* (reg32 *) TX__PC)
#endif
/* Pin State */
#define TX_PS                     (* (reg32 *) TX__PS)
/* Data Register */
#define TX_DR                     (* (reg32 *) TX__DR)
/* Input Buffer Disable Override */
#define TX_INP_DIS                (* (reg32 *) TX__PC2)

/* Interrupt configuration Registers */
#define TX_INTCFG                 (* (reg32 *) TX__INTCFG)
#define TX_INTSTAT                (* (reg32 *) TX__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define TX_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(TX__SIO)
    #define TX_SIO_REG            (* (reg32 *) TX__SIO)
#endif /* (TX__SIO_CFG) */

/* USBIO registers */
#if !defined(TX__PC) && (CY_PSOC4_4200L)
    #define TX_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define TX_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define TX_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define TX_DRIVE_MODE_SHIFT       (0x00u)
#define TX_DRIVE_MODE_MASK        (0x07u << TX_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins TX_H */


/* [] END OF FILE */
