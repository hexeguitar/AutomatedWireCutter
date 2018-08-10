/*******************************************************************************
* File Name: KeyPad_Rows.h  
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

#if !defined(CY_PINS_KeyPad_Rows_ALIASES_H) /* Pins KeyPad_Rows_ALIASES_H */
#define CY_PINS_KeyPad_Rows_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define KeyPad_Rows_0			(KeyPad_Rows__0__PC)
#define KeyPad_Rows_0_PS		(KeyPad_Rows__0__PS)
#define KeyPad_Rows_0_PC		(KeyPad_Rows__0__PC)
#define KeyPad_Rows_0_DR		(KeyPad_Rows__0__DR)
#define KeyPad_Rows_0_SHIFT	(KeyPad_Rows__0__SHIFT)
#define KeyPad_Rows_0_INTR	((uint16)((uint16)0x0003u << (KeyPad_Rows__0__SHIFT*2u)))

#define KeyPad_Rows_1			(KeyPad_Rows__1__PC)
#define KeyPad_Rows_1_PS		(KeyPad_Rows__1__PS)
#define KeyPad_Rows_1_PC		(KeyPad_Rows__1__PC)
#define KeyPad_Rows_1_DR		(KeyPad_Rows__1__DR)
#define KeyPad_Rows_1_SHIFT	(KeyPad_Rows__1__SHIFT)
#define KeyPad_Rows_1_INTR	((uint16)((uint16)0x0003u << (KeyPad_Rows__1__SHIFT*2u)))

#define KeyPad_Rows_2			(KeyPad_Rows__2__PC)
#define KeyPad_Rows_2_PS		(KeyPad_Rows__2__PS)
#define KeyPad_Rows_2_PC		(KeyPad_Rows__2__PC)
#define KeyPad_Rows_2_DR		(KeyPad_Rows__2__DR)
#define KeyPad_Rows_2_SHIFT	(KeyPad_Rows__2__SHIFT)
#define KeyPad_Rows_2_INTR	((uint16)((uint16)0x0003u << (KeyPad_Rows__2__SHIFT*2u)))

#define KeyPad_Rows_3			(KeyPad_Rows__3__PC)
#define KeyPad_Rows_3_PS		(KeyPad_Rows__3__PS)
#define KeyPad_Rows_3_PC		(KeyPad_Rows__3__PC)
#define KeyPad_Rows_3_DR		(KeyPad_Rows__3__DR)
#define KeyPad_Rows_3_SHIFT	(KeyPad_Rows__3__SHIFT)
#define KeyPad_Rows_3_INTR	((uint16)((uint16)0x0003u << (KeyPad_Rows__3__SHIFT*2u)))

#define KeyPad_Rows_INTR_ALL	 ((uint16)(KeyPad_Rows_0_INTR| KeyPad_Rows_1_INTR| KeyPad_Rows_2_INTR| KeyPad_Rows_3_INTR))


#endif /* End Pins KeyPad_Rows_ALIASES_H */


/* [] END OF FILE */
