/*******************************************************************************
* File Name: Buzzer_out.h  
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

#if !defined(CY_PINS_Buzzer_out_ALIASES_H) /* Pins Buzzer_out_ALIASES_H */
#define CY_PINS_Buzzer_out_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Buzzer_out_0			(Buzzer_out__0__PC)
#define Buzzer_out_0_PS		(Buzzer_out__0__PS)
#define Buzzer_out_0_PC		(Buzzer_out__0__PC)
#define Buzzer_out_0_DR		(Buzzer_out__0__DR)
#define Buzzer_out_0_SHIFT	(Buzzer_out__0__SHIFT)
#define Buzzer_out_0_INTR	((uint16)((uint16)0x0003u << (Buzzer_out__0__SHIFT*2u)))

#define Buzzer_out_INTR_ALL	 ((uint16)(Buzzer_out_0_INTR))


#endif /* End Pins Buzzer_out_ALIASES_H */


/* [] END OF FILE */
