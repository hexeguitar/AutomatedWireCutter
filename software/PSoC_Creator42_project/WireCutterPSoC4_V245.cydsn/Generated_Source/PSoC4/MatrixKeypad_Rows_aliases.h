/*******************************************************************************
* File Name: MatrixKeypad_Rows.h  
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

#if !defined(CY_PINS_MatrixKeypad_Rows_ALIASES_H) /* Pins MatrixKeypad_Rows_ALIASES_H */
#define CY_PINS_MatrixKeypad_Rows_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define MatrixKeypad_Rows_0			(MatrixKeypad_Rows__0__PC)
#define MatrixKeypad_Rows_0_PS		(MatrixKeypad_Rows__0__PS)
#define MatrixKeypad_Rows_0_PC		(MatrixKeypad_Rows__0__PC)
#define MatrixKeypad_Rows_0_DR		(MatrixKeypad_Rows__0__DR)
#define MatrixKeypad_Rows_0_SHIFT	(MatrixKeypad_Rows__0__SHIFT)
#define MatrixKeypad_Rows_0_INTR	((uint16)((uint16)0x0003u << (MatrixKeypad_Rows__0__SHIFT*2u)))

#define MatrixKeypad_Rows_1			(MatrixKeypad_Rows__1__PC)
#define MatrixKeypad_Rows_1_PS		(MatrixKeypad_Rows__1__PS)
#define MatrixKeypad_Rows_1_PC		(MatrixKeypad_Rows__1__PC)
#define MatrixKeypad_Rows_1_DR		(MatrixKeypad_Rows__1__DR)
#define MatrixKeypad_Rows_1_SHIFT	(MatrixKeypad_Rows__1__SHIFT)
#define MatrixKeypad_Rows_1_INTR	((uint16)((uint16)0x0003u << (MatrixKeypad_Rows__1__SHIFT*2u)))

#define MatrixKeypad_Rows_2			(MatrixKeypad_Rows__2__PC)
#define MatrixKeypad_Rows_2_PS		(MatrixKeypad_Rows__2__PS)
#define MatrixKeypad_Rows_2_PC		(MatrixKeypad_Rows__2__PC)
#define MatrixKeypad_Rows_2_DR		(MatrixKeypad_Rows__2__DR)
#define MatrixKeypad_Rows_2_SHIFT	(MatrixKeypad_Rows__2__SHIFT)
#define MatrixKeypad_Rows_2_INTR	((uint16)((uint16)0x0003u << (MatrixKeypad_Rows__2__SHIFT*2u)))

#define MatrixKeypad_Rows_3			(MatrixKeypad_Rows__3__PC)
#define MatrixKeypad_Rows_3_PS		(MatrixKeypad_Rows__3__PS)
#define MatrixKeypad_Rows_3_PC		(MatrixKeypad_Rows__3__PC)
#define MatrixKeypad_Rows_3_DR		(MatrixKeypad_Rows__3__DR)
#define MatrixKeypad_Rows_3_SHIFT	(MatrixKeypad_Rows__3__SHIFT)
#define MatrixKeypad_Rows_3_INTR	((uint16)((uint16)0x0003u << (MatrixKeypad_Rows__3__SHIFT*2u)))

#define MatrixKeypad_Rows_INTR_ALL	 ((uint16)(MatrixKeypad_Rows_0_INTR| MatrixKeypad_Rows_1_INTR| MatrixKeypad_Rows_2_INTR| MatrixKeypad_Rows_3_INTR))


#endif /* End Pins MatrixKeypad_Rows_ALIASES_H */


/* [] END OF FILE */
