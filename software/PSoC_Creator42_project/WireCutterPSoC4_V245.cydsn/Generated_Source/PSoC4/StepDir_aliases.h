/*******************************************************************************
* File Name: StepDir.h  
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

#if !defined(CY_PINS_StepDir_ALIASES_H) /* Pins StepDir_ALIASES_H */
#define CY_PINS_StepDir_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define StepDir_0			(StepDir__0__PC)
#define StepDir_0_PS		(StepDir__0__PS)
#define StepDir_0_PC		(StepDir__0__PC)
#define StepDir_0_DR		(StepDir__0__DR)
#define StepDir_0_SHIFT	(StepDir__0__SHIFT)
#define StepDir_0_INTR	((uint16)((uint16)0x0003u << (StepDir__0__SHIFT*2u)))

#define StepDir_INTR_ALL	 ((uint16)(StepDir_0_INTR))


#endif /* End Pins StepDir_ALIASES_H */


/* [] END OF FILE */
