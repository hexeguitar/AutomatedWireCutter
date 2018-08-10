/*******************************************************************************
* File Name: ADC0.h  
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

#if !defined(CY_PINS_ADC0_ALIASES_H) /* Pins ADC0_ALIASES_H */
#define CY_PINS_ADC0_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define ADC0_0			(ADC0__0__PC)
#define ADC0_0_PS		(ADC0__0__PS)
#define ADC0_0_PC		(ADC0__0__PC)
#define ADC0_0_DR		(ADC0__0__DR)
#define ADC0_0_SHIFT	(ADC0__0__SHIFT)
#define ADC0_0_INTR	((uint16)((uint16)0x0003u << (ADC0__0__SHIFT*2u)))

#define ADC0_INTR_ALL	 ((uint16)(ADC0_0_INTR))


#endif /* End Pins ADC0_ALIASES_H */


/* [] END OF FILE */
