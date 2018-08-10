/*******************************************************************************
* File Name: MatrixKeypad_Rows.h  
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

#if !defined(CY_PINS_MatrixKeypad_Rows_H) /* Pins MatrixKeypad_Rows_H */
#define CY_PINS_MatrixKeypad_Rows_H

#include "cytypes.h"
#include "cyfitter.h"
#include "MatrixKeypad_Rows_aliases.h"


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
} MatrixKeypad_Rows_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   MatrixKeypad_Rows_Read(void);
void    MatrixKeypad_Rows_Write(uint8 value);
uint8   MatrixKeypad_Rows_ReadDataReg(void);
#if defined(MatrixKeypad_Rows__PC) || (CY_PSOC4_4200L) 
    void    MatrixKeypad_Rows_SetDriveMode(uint8 mode);
#endif
void    MatrixKeypad_Rows_SetInterruptMode(uint16 position, uint16 mode);
uint8   MatrixKeypad_Rows_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void MatrixKeypad_Rows_Sleep(void); 
void MatrixKeypad_Rows_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(MatrixKeypad_Rows__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define MatrixKeypad_Rows_DRIVE_MODE_BITS        (3)
    #define MatrixKeypad_Rows_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - MatrixKeypad_Rows_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the MatrixKeypad_Rows_SetDriveMode() function.
         *  @{
         */
        #define MatrixKeypad_Rows_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define MatrixKeypad_Rows_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define MatrixKeypad_Rows_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define MatrixKeypad_Rows_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define MatrixKeypad_Rows_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define MatrixKeypad_Rows_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define MatrixKeypad_Rows_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define MatrixKeypad_Rows_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define MatrixKeypad_Rows_MASK               MatrixKeypad_Rows__MASK
#define MatrixKeypad_Rows_SHIFT              MatrixKeypad_Rows__SHIFT
#define MatrixKeypad_Rows_WIDTH              4u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MatrixKeypad_Rows_SetInterruptMode() function.
     *  @{
     */
        #define MatrixKeypad_Rows_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define MatrixKeypad_Rows_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define MatrixKeypad_Rows_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define MatrixKeypad_Rows_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(MatrixKeypad_Rows__SIO)
    #define MatrixKeypad_Rows_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(MatrixKeypad_Rows__PC) && (CY_PSOC4_4200L)
    #define MatrixKeypad_Rows_USBIO_ENABLE               ((uint32)0x80000000u)
    #define MatrixKeypad_Rows_USBIO_DISABLE              ((uint32)(~MatrixKeypad_Rows_USBIO_ENABLE))
    #define MatrixKeypad_Rows_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define MatrixKeypad_Rows_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define MatrixKeypad_Rows_USBIO_ENTER_SLEEP          ((uint32)((1u << MatrixKeypad_Rows_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << MatrixKeypad_Rows_USBIO_SUSPEND_DEL_SHIFT)))
    #define MatrixKeypad_Rows_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << MatrixKeypad_Rows_USBIO_SUSPEND_SHIFT)))
    #define MatrixKeypad_Rows_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << MatrixKeypad_Rows_USBIO_SUSPEND_DEL_SHIFT)))
    #define MatrixKeypad_Rows_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(MatrixKeypad_Rows__PC)
    /* Port Configuration */
    #define MatrixKeypad_Rows_PC                 (* (reg32 *) MatrixKeypad_Rows__PC)
#endif
/* Pin State */
#define MatrixKeypad_Rows_PS                     (* (reg32 *) MatrixKeypad_Rows__PS)
/* Data Register */
#define MatrixKeypad_Rows_DR                     (* (reg32 *) MatrixKeypad_Rows__DR)
/* Input Buffer Disable Override */
#define MatrixKeypad_Rows_INP_DIS                (* (reg32 *) MatrixKeypad_Rows__PC2)

/* Interrupt configuration Registers */
#define MatrixKeypad_Rows_INTCFG                 (* (reg32 *) MatrixKeypad_Rows__INTCFG)
#define MatrixKeypad_Rows_INTSTAT                (* (reg32 *) MatrixKeypad_Rows__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define MatrixKeypad_Rows_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(MatrixKeypad_Rows__SIO)
    #define MatrixKeypad_Rows_SIO_REG            (* (reg32 *) MatrixKeypad_Rows__SIO)
#endif /* (MatrixKeypad_Rows__SIO_CFG) */

/* USBIO registers */
#if !defined(MatrixKeypad_Rows__PC) && (CY_PSOC4_4200L)
    #define MatrixKeypad_Rows_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define MatrixKeypad_Rows_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define MatrixKeypad_Rows_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define MatrixKeypad_Rows_DRIVE_MODE_SHIFT       (0x00u)
#define MatrixKeypad_Rows_DRIVE_MODE_MASK        (0x07u << MatrixKeypad_Rows_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins MatrixKeypad_Rows_H */


/* [] END OF FILE */
