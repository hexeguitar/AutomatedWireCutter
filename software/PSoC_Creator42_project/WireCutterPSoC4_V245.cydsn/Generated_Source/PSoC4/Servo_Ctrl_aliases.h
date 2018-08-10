/*******************************************************************************
* File Name: Servo_Ctrl.h  
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

#if !defined(CY_PINS_Servo_Ctrl_ALIASES_H) /* Pins Servo_Ctrl_ALIASES_H */
#define CY_PINS_Servo_Ctrl_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Servo_Ctrl_0			(Servo_Ctrl__0__PC)
#define Servo_Ctrl_0_PS		(Servo_Ctrl__0__PS)
#define Servo_Ctrl_0_PC		(Servo_Ctrl__0__PC)
#define Servo_Ctrl_0_DR		(Servo_Ctrl__0__DR)
#define Servo_Ctrl_0_SHIFT	(Servo_Ctrl__0__SHIFT)
#define Servo_Ctrl_0_INTR	((uint16)((uint16)0x0003u << (Servo_Ctrl__0__SHIFT*2u)))

#define Servo_Ctrl_INTR_ALL	 ((uint16)(Servo_Ctrl_0_INTR))


#endif /* End Pins Servo_Ctrl_ALIASES_H */


/* [] END OF FILE */
