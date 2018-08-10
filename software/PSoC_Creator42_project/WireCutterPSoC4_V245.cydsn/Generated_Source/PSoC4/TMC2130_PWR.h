/*******************************************************************************
* File Name: TMC2130_PWR.h  
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

#if !defined(CY_PINS_TMC2130_PWR_H) /* Pins TMC2130_PWR_H */
#define CY_PINS_TMC2130_PWR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "TMC2130_PWR_aliases.h"


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
} TMC2130_PWR_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   TMC2130_PWR_Read(void);
void    TMC2130_PWR_Write(uint8 value);
uint8   TMC2130_PWR_ReadDataReg(void);
#if defined(TMC2130_PWR__PC) || (CY_PSOC4_4200L) 
    void    TMC2130_PWR_SetDriveMode(uint8 mode);
#endif
void    TMC2130_PWR_SetInterruptMode(uint16 position, uint16 mode);
uint8   TMC2130_PWR_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void TMC2130_PWR_Sleep(void); 
void TMC2130_PWR_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(TMC2130_PWR__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define TMC2130_PWR_DRIVE_MODE_BITS        (3)
    #define TMC2130_PWR_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - TMC2130_PWR_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the TMC2130_PWR_SetDriveMode() function.
         *  @{
         */
        #define TMC2130_PWR_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define TMC2130_PWR_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define TMC2130_PWR_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define TMC2130_PWR_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define TMC2130_PWR_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define TMC2130_PWR_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define TMC2130_PWR_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define TMC2130_PWR_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define TMC2130_PWR_MASK               TMC2130_PWR__MASK
#define TMC2130_PWR_SHIFT              TMC2130_PWR__SHIFT
#define TMC2130_PWR_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TMC2130_PWR_SetInterruptMode() function.
     *  @{
     */
        #define TMC2130_PWR_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define TMC2130_PWR_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define TMC2130_PWR_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define TMC2130_PWR_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(TMC2130_PWR__SIO)
    #define TMC2130_PWR_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(TMC2130_PWR__PC) && (CY_PSOC4_4200L)
    #define TMC2130_PWR_USBIO_ENABLE               ((uint32)0x80000000u)
    #define TMC2130_PWR_USBIO_DISABLE              ((uint32)(~TMC2130_PWR_USBIO_ENABLE))
    #define TMC2130_PWR_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define TMC2130_PWR_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define TMC2130_PWR_USBIO_ENTER_SLEEP          ((uint32)((1u << TMC2130_PWR_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << TMC2130_PWR_USBIO_SUSPEND_DEL_SHIFT)))
    #define TMC2130_PWR_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << TMC2130_PWR_USBIO_SUSPEND_SHIFT)))
    #define TMC2130_PWR_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << TMC2130_PWR_USBIO_SUSPEND_DEL_SHIFT)))
    #define TMC2130_PWR_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(TMC2130_PWR__PC)
    /* Port Configuration */
    #define TMC2130_PWR_PC                 (* (reg32 *) TMC2130_PWR__PC)
#endif
/* Pin State */
#define TMC2130_PWR_PS                     (* (reg32 *) TMC2130_PWR__PS)
/* Data Register */
#define TMC2130_PWR_DR                     (* (reg32 *) TMC2130_PWR__DR)
/* Input Buffer Disable Override */
#define TMC2130_PWR_INP_DIS                (* (reg32 *) TMC2130_PWR__PC2)

/* Interrupt configuration Registers */
#define TMC2130_PWR_INTCFG                 (* (reg32 *) TMC2130_PWR__INTCFG)
#define TMC2130_PWR_INTSTAT                (* (reg32 *) TMC2130_PWR__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define TMC2130_PWR_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(TMC2130_PWR__SIO)
    #define TMC2130_PWR_SIO_REG            (* (reg32 *) TMC2130_PWR__SIO)
#endif /* (TMC2130_PWR__SIO_CFG) */

/* USBIO registers */
#if !defined(TMC2130_PWR__PC) && (CY_PSOC4_4200L)
    #define TMC2130_PWR_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define TMC2130_PWR_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define TMC2130_PWR_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define TMC2130_PWR_DRIVE_MODE_SHIFT       (0x00u)
#define TMC2130_PWR_DRIVE_MODE_MASK        (0x07u << TMC2130_PWR_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins TMC2130_PWR_H */


/* [] END OF FILE */
