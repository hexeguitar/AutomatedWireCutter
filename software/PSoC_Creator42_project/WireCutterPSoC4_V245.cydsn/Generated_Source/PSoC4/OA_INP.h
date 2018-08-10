/*******************************************************************************
* File Name: OA_INP.h  
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

#if !defined(CY_PINS_OA_INP_H) /* Pins OA_INP_H */
#define CY_PINS_OA_INP_H

#include "cytypes.h"
#include "cyfitter.h"
#include "OA_INP_aliases.h"


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
} OA_INP_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   OA_INP_Read(void);
void    OA_INP_Write(uint8 value);
uint8   OA_INP_ReadDataReg(void);
#if defined(OA_INP__PC) || (CY_PSOC4_4200L) 
    void    OA_INP_SetDriveMode(uint8 mode);
#endif
void    OA_INP_SetInterruptMode(uint16 position, uint16 mode);
uint8   OA_INP_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void OA_INP_Sleep(void); 
void OA_INP_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(OA_INP__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define OA_INP_DRIVE_MODE_BITS        (3)
    #define OA_INP_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - OA_INP_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the OA_INP_SetDriveMode() function.
         *  @{
         */
        #define OA_INP_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define OA_INP_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define OA_INP_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define OA_INP_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define OA_INP_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define OA_INP_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define OA_INP_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define OA_INP_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define OA_INP_MASK               OA_INP__MASK
#define OA_INP_SHIFT              OA_INP__SHIFT
#define OA_INP_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in OA_INP_SetInterruptMode() function.
     *  @{
     */
        #define OA_INP_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define OA_INP_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define OA_INP_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define OA_INP_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(OA_INP__SIO)
    #define OA_INP_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(OA_INP__PC) && (CY_PSOC4_4200L)
    #define OA_INP_USBIO_ENABLE               ((uint32)0x80000000u)
    #define OA_INP_USBIO_DISABLE              ((uint32)(~OA_INP_USBIO_ENABLE))
    #define OA_INP_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define OA_INP_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define OA_INP_USBIO_ENTER_SLEEP          ((uint32)((1u << OA_INP_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << OA_INP_USBIO_SUSPEND_DEL_SHIFT)))
    #define OA_INP_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << OA_INP_USBIO_SUSPEND_SHIFT)))
    #define OA_INP_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << OA_INP_USBIO_SUSPEND_DEL_SHIFT)))
    #define OA_INP_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(OA_INP__PC)
    /* Port Configuration */
    #define OA_INP_PC                 (* (reg32 *) OA_INP__PC)
#endif
/* Pin State */
#define OA_INP_PS                     (* (reg32 *) OA_INP__PS)
/* Data Register */
#define OA_INP_DR                     (* (reg32 *) OA_INP__DR)
/* Input Buffer Disable Override */
#define OA_INP_INP_DIS                (* (reg32 *) OA_INP__PC2)

/* Interrupt configuration Registers */
#define OA_INP_INTCFG                 (* (reg32 *) OA_INP__INTCFG)
#define OA_INP_INTSTAT                (* (reg32 *) OA_INP__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define OA_INP_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(OA_INP__SIO)
    #define OA_INP_SIO_REG            (* (reg32 *) OA_INP__SIO)
#endif /* (OA_INP__SIO_CFG) */

/* USBIO registers */
#if !defined(OA_INP__PC) && (CY_PSOC4_4200L)
    #define OA_INP_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define OA_INP_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define OA_INP_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define OA_INP_DRIVE_MODE_SHIFT       (0x00u)
#define OA_INP_DRIVE_MODE_MASK        (0x07u << OA_INP_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins OA_INP_H */


/* [] END OF FILE */
