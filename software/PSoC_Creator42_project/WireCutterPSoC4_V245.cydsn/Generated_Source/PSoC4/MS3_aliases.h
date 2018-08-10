/*******************************************************************************
* File Name: MS3.h  
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

#if !defined(CY_PINS_MS3_ALIASES_H) /* Pins MS3_ALIASES_H */
#define CY_PINS_MS3_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define MS3_0			(MS3__0__PC)
#define MS3_0_PS		(MS3__0__PS)
#define MS3_0_PC		(MS3__0__PC)
#define MS3_0_DR		(MS3__0__DR)
#define MS3_0_SHIFT	(MS3__0__SHIFT)
#define MS3_0_INTR	((uint16)((uint16)0x0003u << (MS3__0__SHIFT*2u)))

#define MS3_INTR_ALL	 ((uint16)(MS3_0_INTR))


#endif /* End Pins MS3_ALIASES_H */


/* [] END OF FILE */
