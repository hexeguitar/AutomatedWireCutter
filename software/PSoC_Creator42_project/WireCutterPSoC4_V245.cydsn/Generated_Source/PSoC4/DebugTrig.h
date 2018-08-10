/*******************************************************************************
* File Name: DebugTrig.h  
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

#if !defined(CY_PINS_DebugTrig_H) /* Pins DebugTrig_H */
#define CY_PINS_DebugTrig_H

#include "cytypes.h"
#include "cyfitter.h"
#include "DebugTrig_aliases.h"


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
} DebugTrig_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   DebugTrig_Read(void);
void    DebugTrig_Write(uint8 value);
uint8   DebugTrig_ReadDataReg(void);
#if defined(DebugTrig__PC) || (CY_PSOC4_4200L) 
    void    DebugTrig_SetDriveMode(uint8 mode);
#endif
void    DebugTrig_SetInterruptMode(uint16 position, uint16 mode);
uint8   DebugTrig_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void DebugTrig_Sleep(void); 
void DebugTrig_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(DebugTrig__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define DebugTrig_DRIVE_MODE_BITS        (3)
    #define DebugTrig_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DebugTrig_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the DebugTrig_SetDriveMode() function.
         *  @{
         */
        #define DebugTrig_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define DebugTrig_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define DebugTrig_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define DebugTrig_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define DebugTrig_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define DebugTrig_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define DebugTrig_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define DebugTrig_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define DebugTrig_MASK               DebugTrig__MASK
#define DebugTrig_SHIFT              DebugTrig__SHIFT
#define DebugTrig_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DebugTrig_SetInterruptMode() function.
     *  @{
     */
        #define DebugTrig_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define DebugTrig_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define DebugTrig_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define DebugTrig_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(DebugTrig__SIO)
    #define DebugTrig_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(DebugTrig__PC) && (CY_PSOC4_4200L)
    #define DebugTrig_USBIO_ENABLE               ((uint32)0x80000000u)
    #define DebugTrig_USBIO_DISABLE              ((uint32)(~DebugTrig_USBIO_ENABLE))
    #define DebugTrig_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define DebugTrig_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define DebugTrig_USBIO_ENTER_SLEEP          ((uint32)((1u << DebugTrig_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << DebugTrig_USBIO_SUSPEND_DEL_SHIFT)))
    #define DebugTrig_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << DebugTrig_USBIO_SUSPEND_SHIFT)))
    #define DebugTrig_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << DebugTrig_USBIO_SUSPEND_DEL_SHIFT)))
    #define DebugTrig_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(DebugTrig__PC)
    /* Port Configuration */
    #define DebugTrig_PC                 (* (reg32 *) DebugTrig__PC)
#endif
/* Pin State */
#define DebugTrig_PS                     (* (reg32 *) DebugTrig__PS)
/* Data Register */
#define DebugTrig_DR                     (* (reg32 *) DebugTrig__DR)
/* Input Buffer Disable Override */
#define DebugTrig_INP_DIS                (* (reg32 *) DebugTrig__PC2)

/* Interrupt configuration Registers */
#define DebugTrig_INTCFG                 (* (reg32 *) DebugTrig__INTCFG)
#define DebugTrig_INTSTAT                (* (reg32 *) DebugTrig__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define DebugTrig_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(DebugTrig__SIO)
    #define DebugTrig_SIO_REG            (* (reg32 *) DebugTrig__SIO)
#endif /* (DebugTrig__SIO_CFG) */

/* USBIO registers */
#if !defined(DebugTrig__PC) && (CY_PSOC4_4200L)
    #define DebugTrig_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define DebugTrig_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define DebugTrig_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define DebugTrig_DRIVE_MODE_SHIFT       (0x00u)
#define DebugTrig_DRIVE_MODE_MASK        (0x07u << DebugTrig_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins DebugTrig_H */


/* [] END OF FILE */
