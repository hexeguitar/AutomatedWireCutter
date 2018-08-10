/*******************************************************************************
* File Name: MatrixKeypad_Columns.h  
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

#if !defined(CY_PINS_MatrixKeypad_Columns_ALIASES_H) /* Pins MatrixKeypad_Columns_ALIASES_H */
#define CY_PINS_MatrixKeypad_Columns_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define MatrixKeypad_Columns_0			(MatrixKeypad_Columns__0__PC)
#define MatrixKeypad_Columns_0_PS		(MatrixKeypad_Columns__0__PS)
#define MatrixKeypad_Columns_0_PC		(MatrixKeypad_Columns__0__PC)
#define MatrixKeypad_Columns_0_DR		(MatrixKeypad_Columns__0__DR)
#define MatrixKeypad_Columns_0_SHIFT	(MatrixKeypad_Columns__0__SHIFT)
#define MatrixKeypad_Columns_0_INTR	((uint16)((uint16)0x0003u << (MatrixKeypad_Columns__0__SHIFT*2u)))

#define MatrixKeypad_Columns_1			(MatrixKeypad_Columns__1__PC)
#define MatrixKeypad_Columns_1_PS		(MatrixKeypad_Columns__1__PS)
#define MatrixKeypad_Columns_1_PC		(MatrixKeypad_Columns__1__PC)
#define MatrixKeypad_Columns_1_DR		(MatrixKeypad_Columns__1__DR)
#define MatrixKeypad_Columns_1_SHIFT	(MatrixKeypad_Columns__1__SHIFT)
#define MatrixKeypad_Columns_1_INTR	((uint16)((uint16)0x0003u << (MatrixKeypad_Columns__1__SHIFT*2u)))

#define MatrixKeypad_Columns_2			(MatrixKeypad_Columns__2__PC)
#define MatrixKeypad_Columns_2_PS		(MatrixKeypad_Columns__2__PS)
#define MatrixKeypad_Columns_2_PC		(MatrixKeypad_Columns__2__PC)
#define MatrixKeypad_Columns_2_DR		(MatrixKeypad_Columns__2__DR)
#define MatrixKeypad_Columns_2_SHIFT	(MatrixKeypad_Columns__2__SHIFT)
#define MatrixKeypad_Columns_2_INTR	((uint16)((uint16)0x0003u << (MatrixKeypad_Columns__2__SHIFT*2u)))

#define MatrixKeypad_Columns_3			(MatrixKeypad_Columns__3__PC)
#define MatrixKeypad_Columns_3_PS		(MatrixKeypad_Columns__3__PS)
#define MatrixKeypad_Columns_3_PC		(MatrixKeypad_Columns__3__PC)
#define MatrixKeypad_Columns_3_DR		(MatrixKeypad_Columns__3__DR)
#define MatrixKeypad_Columns_3_SHIFT	(MatrixKeypad_Columns__3__SHIFT)
#define MatrixKeypad_Columns_3_INTR	((uint16)((uint16)0x0003u << (MatrixKeypad_Columns__3__SHIFT*2u)))

#define MatrixKeypad_Columns_INTR_ALL	 ((uint16)(MatrixKeypad_Columns_0_INTR| MatrixKeypad_Columns_1_INTR| MatrixKeypad_Columns_2_INTR| MatrixKeypad_Columns_3_INTR))


#endif /* End Pins MatrixKeypad_Columns_ALIASES_H */


/* [] END OF FILE */
