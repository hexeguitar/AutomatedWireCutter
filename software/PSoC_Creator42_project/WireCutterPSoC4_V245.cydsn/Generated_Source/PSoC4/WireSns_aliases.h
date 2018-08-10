/*******************************************************************************
* File Name: WireSns.h  
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

#if !defined(CY_PINS_WireSns_ALIASES_H) /* Pins WireSns_ALIASES_H */
#define CY_PINS_WireSns_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define WireSns_0			(WireSns__0__PC)
#define WireSns_0_PS		(WireSns__0__PS)
#define WireSns_0_PC		(WireSns__0__PC)
#define WireSns_0_DR		(WireSns__0__DR)
#define WireSns_0_SHIFT	(WireSns__0__SHIFT)
#define WireSns_0_INTR	((uint16)((uint16)0x0003u << (WireSns__0__SHIFT*2u)))

#define WireSns_INTR_ALL	 ((uint16)(WireSns_0_INTR))


#endif /* End Pins WireSns_ALIASES_H */


/* [] END OF FILE */
