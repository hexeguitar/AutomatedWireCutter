/*******************************************************************************
* File Name: DAC.h  
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

#if !defined(CY_PINS_DAC_ALIASES_H) /* Pins DAC_ALIASES_H */
#define CY_PINS_DAC_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define DAC_0			(DAC__0__PC)
#define DAC_0_PS		(DAC__0__PS)
#define DAC_0_PC		(DAC__0__PC)
#define DAC_0_DR		(DAC__0__DR)
#define DAC_0_SHIFT	(DAC__0__SHIFT)
#define DAC_0_INTR	((uint16)((uint16)0x0003u << (DAC__0__SHIFT*2u)))

#define DAC_INTR_ALL	 ((uint16)(DAC_0_INTR))


#endif /* End Pins DAC_ALIASES_H */


/* [] END OF FILE */
