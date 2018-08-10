/*******************************************************************************
* File Name: KeyPad_Columns.h  
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

#if !defined(CY_PINS_KeyPad_Columns_ALIASES_H) /* Pins KeyPad_Columns_ALIASES_H */
#define CY_PINS_KeyPad_Columns_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define KeyPad_Columns_0			(KeyPad_Columns__0__PC)
#define KeyPad_Columns_0_PS		(KeyPad_Columns__0__PS)
#define KeyPad_Columns_0_PC		(KeyPad_Columns__0__PC)
#define KeyPad_Columns_0_DR		(KeyPad_Columns__0__DR)
#define KeyPad_Columns_0_SHIFT	(KeyPad_Columns__0__SHIFT)
#define KeyPad_Columns_0_INTR	((uint16)((uint16)0x0003u << (KeyPad_Columns__0__SHIFT*2u)))

#define KeyPad_Columns_1			(KeyPad_Columns__1__PC)
#define KeyPad_Columns_1_PS		(KeyPad_Columns__1__PS)
#define KeyPad_Columns_1_PC		(KeyPad_Columns__1__PC)
#define KeyPad_Columns_1_DR		(KeyPad_Columns__1__DR)
#define KeyPad_Columns_1_SHIFT	(KeyPad_Columns__1__SHIFT)
#define KeyPad_Columns_1_INTR	((uint16)((uint16)0x0003u << (KeyPad_Columns__1__SHIFT*2u)))

#define KeyPad_Columns_2			(KeyPad_Columns__2__PC)
#define KeyPad_Columns_2_PS		(KeyPad_Columns__2__PS)
#define KeyPad_Columns_2_PC		(KeyPad_Columns__2__PC)
#define KeyPad_Columns_2_DR		(KeyPad_Columns__2__DR)
#define KeyPad_Columns_2_SHIFT	(KeyPad_Columns__2__SHIFT)
#define KeyPad_Columns_2_INTR	((uint16)((uint16)0x0003u << (KeyPad_Columns__2__SHIFT*2u)))

#define KeyPad_Columns_3			(KeyPad_Columns__3__PC)
#define KeyPad_Columns_3_PS		(KeyPad_Columns__3__PS)
#define KeyPad_Columns_3_PC		(KeyPad_Columns__3__PC)
#define KeyPad_Columns_3_DR		(KeyPad_Columns__3__DR)
#define KeyPad_Columns_3_SHIFT	(KeyPad_Columns__3__SHIFT)
#define KeyPad_Columns_3_INTR	((uint16)((uint16)0x0003u << (KeyPad_Columns__3__SHIFT*2u)))

#define KeyPad_Columns_INTR_ALL	 ((uint16)(KeyPad_Columns_0_INTR| KeyPad_Columns_1_INTR| KeyPad_Columns_2_INTR| KeyPad_Columns_3_INTR))


#endif /* End Pins KeyPad_Columns_ALIASES_H */


/* [] END OF FILE */
