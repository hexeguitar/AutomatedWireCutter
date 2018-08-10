/*******************************************************************************
* File Name: TMC2130_PWR.h  
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

#if !defined(CY_PINS_TMC2130_PWR_ALIASES_H) /* Pins TMC2130_PWR_ALIASES_H */
#define CY_PINS_TMC2130_PWR_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define TMC2130_PWR_0			(TMC2130_PWR__0__PC)
#define TMC2130_PWR_0_PS		(TMC2130_PWR__0__PS)
#define TMC2130_PWR_0_PC		(TMC2130_PWR__0__PC)
#define TMC2130_PWR_0_DR		(TMC2130_PWR__0__DR)
#define TMC2130_PWR_0_SHIFT	(TMC2130_PWR__0__SHIFT)
#define TMC2130_PWR_0_INTR	((uint16)((uint16)0x0003u << (TMC2130_PWR__0__SHIFT*2u)))

#define TMC2130_PWR_INTR_ALL	 ((uint16)(TMC2130_PWR_0_INTR))


#endif /* End Pins TMC2130_PWR_ALIASES_H */


/* [] END OF FILE */
