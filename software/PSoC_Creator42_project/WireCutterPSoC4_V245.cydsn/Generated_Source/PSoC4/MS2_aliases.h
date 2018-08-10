/*******************************************************************************
* File Name: MS2.h  
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

#if !defined(CY_PINS_MS2_ALIASES_H) /* Pins MS2_ALIASES_H */
#define CY_PINS_MS2_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define MS2_0			(MS2__0__PC)
#define MS2_0_PS		(MS2__0__PS)
#define MS2_0_PC		(MS2__0__PC)
#define MS2_0_DR		(MS2__0__DR)
#define MS2_0_SHIFT	(MS2__0__SHIFT)
#define MS2_0_INTR	((uint16)((uint16)0x0003u << (MS2__0__SHIFT*2u)))

#define MS2_INTR_ALL	 ((uint16)(MS2_0_INTR))


#endif /* End Pins MS2_ALIASES_H */


/* [] END OF FILE */
