/*******************************************************************************
* File Name: DebugTrig.h  
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

#if !defined(CY_PINS_DebugTrig_ALIASES_H) /* Pins DebugTrig_ALIASES_H */
#define CY_PINS_DebugTrig_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define DebugTrig_0			(DebugTrig__0__PC)
#define DebugTrig_0_PS		(DebugTrig__0__PS)
#define DebugTrig_0_PC		(DebugTrig__0__PC)
#define DebugTrig_0_DR		(DebugTrig__0__DR)
#define DebugTrig_0_SHIFT	(DebugTrig__0__SHIFT)
#define DebugTrig_0_INTR	((uint16)((uint16)0x0003u << (DebugTrig__0__SHIFT*2u)))

#define DebugTrig_INTR_ALL	 ((uint16)(DebugTrig_0_INTR))


#endif /* End Pins DebugTrig_ALIASES_H */


/* [] END OF FILE */
