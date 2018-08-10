/*******************************************************************************
* File Name: BTN_REWIND.h  
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

#if !defined(CY_PINS_BTN_REWIND_ALIASES_H) /* Pins BTN_REWIND_ALIASES_H */
#define CY_PINS_BTN_REWIND_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define BTN_REWIND_0			(BTN_REWIND__0__PC)
#define BTN_REWIND_0_PS		(BTN_REWIND__0__PS)
#define BTN_REWIND_0_PC		(BTN_REWIND__0__PC)
#define BTN_REWIND_0_DR		(BTN_REWIND__0__DR)
#define BTN_REWIND_0_SHIFT	(BTN_REWIND__0__SHIFT)
#define BTN_REWIND_0_INTR	((uint16)((uint16)0x0003u << (BTN_REWIND__0__SHIFT*2u)))

#define BTN_REWIND_INTR_ALL	 ((uint16)(BTN_REWIND_0_INTR))


#endif /* End Pins BTN_REWIND_ALIASES_H */


/* [] END OF FILE */
