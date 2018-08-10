/*******************************************************************************
* File Name: Servo_PWR.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Servo_PWR_ALIASES_H) /* Pins Servo_PWR_ALIASES_H */
#define CY_PINS_Servo_PWR_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Servo_PWR_0			(Servo_PWR__0__PC)
#define Servo_PWR_0_PS		(Servo_PWR__0__PS)
#define Servo_PWR_0_PC		(Servo_PWR__0__PC)
#define Servo_PWR_0_DR		(Servo_PWR__0__DR)
#define Servo_PWR_0_SHIFT	(Servo_PWR__0__SHIFT)
#define Servo_PWR_0_INTR	((uint16)((uint16)0x0003u << (Servo_PWR__0__SHIFT*2u)))

#define Servo_PWR_INTR_ALL	 ((uint16)(Servo_PWR_0_INTR))


#endif /* End Pins Servo_PWR_ALIASES_H */


/* [] END OF FILE */
