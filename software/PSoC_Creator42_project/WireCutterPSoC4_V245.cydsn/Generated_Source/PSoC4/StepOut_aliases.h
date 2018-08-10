/*******************************************************************************
* File Name: StepOut.h  
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

#if !defined(CY_PINS_StepOut_ALIASES_H) /* Pins StepOut_ALIASES_H */
#define CY_PINS_StepOut_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define StepOut_0			(StepOut__0__PC)
#define StepOut_0_PS		(StepOut__0__PS)
#define StepOut_0_PC		(StepOut__0__PC)
#define StepOut_0_DR		(StepOut__0__DR)
#define StepOut_0_SHIFT	(StepOut__0__SHIFT)
#define StepOut_0_INTR	((uint16)((uint16)0x0003u << (StepOut__0__SHIFT*2u)))

#define StepOut_INTR_ALL	 ((uint16)(StepOut_0_INTR))


#endif /* End Pins StepOut_ALIASES_H */


/* [] END OF FILE */
