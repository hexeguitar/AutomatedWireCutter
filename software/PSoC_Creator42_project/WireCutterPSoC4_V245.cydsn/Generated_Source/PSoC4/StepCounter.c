/*******************************************************************************
* File Name: StepCounter.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "StepCounter.h"

uint8 StepCounter_initVar = 0u;


/*******************************************************************************
* Function Name: StepCounter_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void StepCounter_Init(void) 
{
        #if (!StepCounter_UsingFixedFunction && !StepCounter_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!StepCounter_UsingFixedFunction && !StepCounter_ControlRegRemoved) */
        
        #if(!StepCounter_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 StepCounter_interruptState;
        #endif /* (!StepCounter_UsingFixedFunction) */
        
        #if (StepCounter_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            StepCounter_CONTROL &= StepCounter_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                StepCounter_CONTROL2 &= ((uint8)(~StepCounter_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                StepCounter_CONTROL3 &= ((uint8)(~StepCounter_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (StepCounter_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                StepCounter_CONTROL |= StepCounter_ONESHOT;
            #endif /* (StepCounter_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            StepCounter_CONTROL2 |= StepCounter_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            StepCounter_RT1 &= ((uint8)(~StepCounter_RT1_MASK));
            StepCounter_RT1 |= StepCounter_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            StepCounter_RT1 &= ((uint8)(~StepCounter_SYNCDSI_MASK));
            StepCounter_RT1 |= StepCounter_SYNCDSI_EN;

        #else
            #if(!StepCounter_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = StepCounter_CONTROL & ((uint8)(~StepCounter_CTRL_CMPMODE_MASK));
            StepCounter_CONTROL = ctrl | StepCounter_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = StepCounter_CONTROL & ((uint8)(~StepCounter_CTRL_CAPMODE_MASK));
            
            #if( 0 != StepCounter_CAPTURE_MODE_CONF)
                StepCounter_CONTROL = ctrl | StepCounter_DEFAULT_CAPTURE_MODE;
            #else
                StepCounter_CONTROL = ctrl;
            #endif /* 0 != StepCounter_CAPTURE_MODE */ 
            
            #endif /* (!StepCounter_ControlRegRemoved) */
        #endif /* (StepCounter_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!StepCounter_UsingFixedFunction)
            StepCounter_ClearFIFO();
        #endif /* (!StepCounter_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        StepCounter_WritePeriod(StepCounter_INIT_PERIOD_VALUE);
        #if (!(StepCounter_UsingFixedFunction && (CY_PSOC5A)))
            StepCounter_WriteCounter(StepCounter_INIT_COUNTER_VALUE);
        #endif /* (!(StepCounter_UsingFixedFunction && (CY_PSOC5A))) */
        StepCounter_SetInterruptMode(StepCounter_INIT_INTERRUPTS_MASK);
        
        #if (!StepCounter_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)StepCounter_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            StepCounter_WriteCompare(StepCounter_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            StepCounter_interruptState = CyEnterCriticalSection();
            
            StepCounter_STATUS_AUX_CTRL |= StepCounter_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(StepCounter_interruptState);
            
        #endif /* (!StepCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepCounter_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void StepCounter_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (StepCounter_UsingFixedFunction)
        StepCounter_GLOBAL_ENABLE |= StepCounter_BLOCK_EN_MASK;
        StepCounter_GLOBAL_STBY_ENABLE |= StepCounter_BLOCK_STBY_EN_MASK;
    #endif /* (StepCounter_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!StepCounter_ControlRegRemoved || StepCounter_UsingFixedFunction)
        StepCounter_CONTROL |= StepCounter_CTRL_ENABLE;                
    #endif /* (!StepCounter_ControlRegRemoved || StepCounter_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: StepCounter_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  StepCounter_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void StepCounter_Start(void) 
{
    if(StepCounter_initVar == 0u)
    {
        StepCounter_Init();
        
        StepCounter_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    StepCounter_Enable();        
}


/*******************************************************************************
* Function Name: StepCounter_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void StepCounter_Stop(void) 
{
    /* Disable Counter */
    #if(!StepCounter_ControlRegRemoved || StepCounter_UsingFixedFunction)
        StepCounter_CONTROL &= ((uint8)(~StepCounter_CTRL_ENABLE));        
    #endif /* (!StepCounter_ControlRegRemoved || StepCounter_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (StepCounter_UsingFixedFunction)
        StepCounter_GLOBAL_ENABLE &= ((uint8)(~StepCounter_BLOCK_EN_MASK));
        StepCounter_GLOBAL_STBY_ENABLE &= ((uint8)(~StepCounter_BLOCK_STBY_EN_MASK));
    #endif /* (StepCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepCounter_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void StepCounter_SetInterruptMode(uint8 interruptsMask) 
{
    StepCounter_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: StepCounter_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   StepCounter_ReadStatusRegister(void) 
{
    return StepCounter_STATUS;
}


#if(!StepCounter_ControlRegRemoved)
/*******************************************************************************
* Function Name: StepCounter_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   StepCounter_ReadControlRegister(void) 
{
    return StepCounter_CONTROL;
}


/*******************************************************************************
* Function Name: StepCounter_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    StepCounter_WriteControlRegister(uint8 control) 
{
    StepCounter_CONTROL = control;
}

#endif  /* (!StepCounter_ControlRegRemoved) */


#if (!(StepCounter_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: StepCounter_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void StepCounter_WriteCounter(uint16 counter) \
                                   
{
    #if(StepCounter_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (StepCounter_GLOBAL_ENABLE & StepCounter_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        StepCounter_GLOBAL_ENABLE |= StepCounter_BLOCK_EN_MASK;
        CY_SET_REG16(StepCounter_COUNTER_LSB_PTR, (uint16)counter);
        StepCounter_GLOBAL_ENABLE &= ((uint8)(~StepCounter_BLOCK_EN_MASK));
    #else
        CY_SET_REG16(StepCounter_COUNTER_LSB_PTR, counter);
    #endif /* (StepCounter_UsingFixedFunction) */
}
#endif /* (!(StepCounter_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: StepCounter_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) The present value of the counter.
*
*******************************************************************************/
uint16 StepCounter_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(StepCounter_UsingFixedFunction)
		(void)CY_GET_REG16(StepCounter_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(StepCounter_COUNTER_LSB_PTR_8BIT);
	#endif/* (StepCounter_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(StepCounter_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(StepCounter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(StepCounter_STATICCOUNT_LSB_PTR));
    #endif /* (StepCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepCounter_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint16) Present Capture value.
*
*******************************************************************************/
uint16 StepCounter_ReadCapture(void) 
{
    #if(StepCounter_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(StepCounter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(StepCounter_STATICCOUNT_LSB_PTR));
    #endif /* (StepCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepCounter_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint16) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void StepCounter_WritePeriod(uint16 period) 
{
    #if(StepCounter_UsingFixedFunction)
        CY_SET_REG16(StepCounter_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG16(StepCounter_PERIOD_LSB_PTR, period);
    #endif /* (StepCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepCounter_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) Present period value.
*
*******************************************************************************/
uint16 StepCounter_ReadPeriod(void) 
{
    #if(StepCounter_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(StepCounter_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(StepCounter_PERIOD_LSB_PTR));
    #endif /* (StepCounter_UsingFixedFunction) */
}


#if (!StepCounter_UsingFixedFunction)
/*******************************************************************************
* Function Name: StepCounter_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void StepCounter_WriteCompare(uint16 compare) \
                                   
{
    #if(StepCounter_UsingFixedFunction)
        CY_SET_REG16(StepCounter_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG16(StepCounter_COMPARE_LSB_PTR, compare);
    #endif /* (StepCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepCounter_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint16) Present compare value.
*
*******************************************************************************/
uint16 StepCounter_ReadCompare(void) 
{
    return (CY_GET_REG16(StepCounter_COMPARE_LSB_PTR));
}


#if (StepCounter_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: StepCounter_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void StepCounter_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    StepCounter_CONTROL &= ((uint8)(~StepCounter_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    StepCounter_CONTROL |= compareMode;
}
#endif  /* (StepCounter_COMPARE_MODE_SOFTWARE) */


#if (StepCounter_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: StepCounter_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void StepCounter_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    StepCounter_CONTROL &= ((uint8)(~StepCounter_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    StepCounter_CONTROL |= ((uint8)((uint8)captureMode << StepCounter_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (StepCounter_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: StepCounter_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void StepCounter_ClearFIFO(void) 
{

    while(0u != (StepCounter_ReadStatusRegister() & StepCounter_STATUS_FIFONEMP))
    {
        (void)StepCounter_ReadCapture();
    }

}
#endif  /* (!StepCounter_UsingFixedFunction) */


/* [] END OF FILE */

