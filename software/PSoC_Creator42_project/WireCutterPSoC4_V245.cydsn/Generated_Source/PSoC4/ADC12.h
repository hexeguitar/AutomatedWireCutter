/*******************************************************************************
* File Name: ADC12.h
* Version 2.50
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Sequencing Successive Approximation ADC Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_ADC_SAR_SEQ_ADC12_H)
#define CY_ADC_SAR_SEQ_ADC12_H

#include "cytypes.h"
#include "CyLib.h"


/***************************************
*      Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint32 dftRegVal;
} ADC12_BACKUP_STRUCT;


/**************************************
*    Enumerated Types and Parameters
**************************************/

/*  Clock Source setting constants */
#define ADC12__EXTERNAL 0
#define ADC12__INTERNAL 1

/*  Sample Mode setting constants */
#define ADC12__FREERUNNING 0
#define ADC12__HARDWARESOC 1

/*  Reference type setting constants */
#define ADC12__VDDA_2 0
#define ADC12__VDDA 1
#define ADC12__INTERNAL1024 2
#define ADC12__INTERNAL1024BYPASSED 3
#define ADC12__INTERNALVREF 4
#define ADC12__INTERNALVREFBYPASSED 5
#define ADC12__VDDA_2BYPASSED 6
#define ADC12__EXTERNALVREF 7

/* Input buffer gain setting constants */
#define ADC12__DISABLED 0
#define ADC12__ONE 1
#define ADC12__TWO 2
#define ADC12__FOUR 3
#define ADC12__EIGHT 4
#define ADC12__SIXTEEN 5

/* Negative input setting sonstants in single ended mode */
#define ADC12__VSS 0
#define ADC12__VREF 1
#define ADC12__OTHER 2

/* Compare mode setting constants:
*    Mode0 - Disable
*    Mode1 - Result < Low_Limit
*    Mode2 - Low_Limit <= Result < High_Limit
*    Mode3 - High_Limit <= Result
*    Mode4 - (Result < Low_Limit) or (High_Limit <= Result)
*/
#define ADC12__MODE0 0
#define ADC12__MODE1 1
#define ADC12__MODE2 2
#define ADC12__MODE3 3

#define ADC12__RES8 0
#define ADC12__RES10 1

#define ADC12__RIGHT 0
#define ADC12__LEFT 1

#define ADC12__FSIGNED 1
#define ADC12__FUNSIGNED 0

#define ADC12__ACCUMULATE 0
#define ADC12__FIXEDRESOLUTION 1



/***************************************
*   Conditional Compilation Parameters
****************************************/ 

#define ADC12_CY_SAR_IP_VER0               	(0u)
#define ADC12_CY_SAR_IP_VER1               	(1u)

#if (CY_PSOC4_4100 || CY_PSOC4_4200)
    #define ADC12_CY_SAR_IP_VER              (ADC12_CY_SAR_IP_VER0)
#else /* Other devices */
    #define ADC12_CY_SAR_IP_VER              (ADC12_CY_SAR_IP_VER1)
#endif  /* (CY_PSOC4_4100 || CY_PSOC4_4200) */


/***************************************
*    Initial Parameter Constants
***************************************/
#define ADC12_DEFAULT_SAMPLE_MODE_SEL        (0u)
#define ADC12_DEFAULT_VREF_SEL               (1u)
#define ADC12_DEFAULT_NEG_INPUT_SEL          (0u)
#define ADC12_DEFAULT_ALT_RESOLUTION_SEL     (0u)
#define ADC12_DEFAULT_JUSTIFICATION_SEL      (0u)
#define ADC12_DEFAULT_DIFF_RESULT_FORMAT_SEL (1u)
#define ADC12_DEFAULT_SE_RESULT_FORMAT_SEL   (1u)
#define ADC12_DEFAULT_CLOCK_SOURCE           (1u)
#define ADC12_DEFAULT_VREF_MV_VALUE          (5000)
#define ADC12_DEFAULT_BUFFER_GAIN            (0u)
#define ADC12_DEFAULT_AVG_SAMPLES_NUM        (5u)
#define ADC12_DEFAULT_AVG_SAMPLES_DIV        (5u < 4u) ? (int16)(0x100u >> (7u - 5u)) : (int16)(0x100u >> 4u)
#define ADC12_DEFAULT_AVG_MODE               (1u)
#define ADC12_MAX_RESOLUTION                 (12u)
#define ADC12_DEFAULT_LOW_LIMIT              (0u)
#define ADC12_DEFAULT_HIGH_LIMIT             (2047u)
#define ADC12_DEFAULT_COMPARE_MODE           (0u)
#define ADC12_DEFAULT_ACLKS_NUM              (8u)
#define ADC12_DEFAULT_BCLKS_NUM              (4u)
#define ADC12_DEFAULT_CCLKS_NUM              (4u)
#define ADC12_DEFAULT_DCLKS_NUM              (4u)
#define ADC12_TOTAL_CHANNELS_NUM             (1u)
#define ADC12_SEQUENCED_CHANNELS_NUM         (1u)
#define ADC12_DEFAULT_EN_CHANNELS            (1u)
#define ADC12_NOMINAL_CLOCK_FREQ             (7333333)
#define ADC12_INJ_CHANNEL_ENABLED            (0u)
#define ADC12_IRQ_REMOVE                     (0u)

/* Determines whether the configuration contains external negative input. */
#define ADC12_SINGLE_PRESENT                 (0u)
#define ADC12_CHANNELS_MODE                  (0u)
#define ADC12_MAX_CHANNELS_EN_MASK           (0xffffu >> (16u - ADC12_SEQUENCED_CHANNELS_NUM))


/***************************************
*        Function Prototypes
***************************************/

void ADC12_Start(void);
void ADC12_Stop(void);
void ADC12_Init(void);
void ADC12_Enable(void);
void ADC12_StartConvert(void);
void ADC12_StopConvert(void);
uint32 ADC12_IsEndConversion(uint32 retMode);
int16 ADC12_GetResult16(uint32 chan);
void ADC12_SetChanMask(uint32 mask);
void ADC12_SetLowLimit(uint32 lowLimit);
void ADC12_SetHighLimit(uint32 highLimit);
void ADC12_SetLimitMask(uint32 mask);
void ADC12_SetSatMask(uint32 mask);
void ADC12_SetOffset(uint32 chan, int16 offset);
void ADC12_SetGain(uint32 chan, int32 adcGain);
#if(ADC12_INJ_CHANNEL_ENABLED)
    void ADC12_EnableInjection(void);
#endif /* ADC12_INJ_CHANNEL_ENABLED */
#if(ADC12_DEFAULT_JUSTIFICATION_SEL == ADC12__RIGHT)
    int16 ADC12_CountsTo_mVolts(uint32 chan, int16 adcCounts);
    int32 ADC12_CountsTo_uVolts(uint32 chan, int16 adcCounts);
    float32 ADC12_CountsTo_Volts(uint32 chan, int16 adcCounts);
#endif /* End ADC12_DEFAULT_JUSTIFICATION_SEL == ADC12__RIGHT */
void ADC12_Sleep(void);
void ADC12_Wakeup(void);
void ADC12_SaveConfig(void);
void ADC12_RestoreConfig(void);

CY_ISR_PROTO( ADC12_ISR );


/**************************************
*           API Constants
**************************************/
/* Constants for Sleep mode states */
#define ADC12_DISABLED                   (0x00u)
#define ADC12_ENABLED                    (0x01u)
#define ADC12_STARTED                    (0x02u)
#define ADC12_BOOSTPUMP_ENABLED          (0x04u)

/*   Constants for IsEndConversion() "retMode" parameter  */
#define ADC12_RETURN_STATUS              (0x01u)
#define ADC12_WAIT_FOR_RESULT            (0x02u)
#define ADC12_RETURN_STATUS_INJ          (0x04u)
#define ADC12_WAIT_FOR_RESULT_INJ        (0x08u)

#define ADC12_MAX_FREQUENCY              (18000000)       /*18Mhz*/

#define ADC12_RESOLUTION_12              (12u)
#define ADC12_RESOLUTION_10              (10u)
#define ADC12_RESOLUTION_8               (8u)

#define ADC12_10US_DELAY                 (10u)

#define ADC12_10V_COUNTS                 (10.0F)
#define ADC12_10MV_COUNTS                (10000)
#define ADC12_10UV_COUNTS                (10000000L)


/***************************************
* Global variables external identifier
***************************************/

extern uint8 ADC12_initVar;
extern volatile int16 ADC12_offset[ADC12_TOTAL_CHANNELS_NUM];
extern volatile int32 ADC12_countsPer10Volt[ADC12_TOTAL_CHANNELS_NUM];


/***************************************
*              Registers
***************************************/

#define ADC12_SAR_CTRL_REG                (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CTRL )
#define ADC12_SAR_CTRL_PTR                ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CTRL )

#define ADC12_SAR_SAMPLE_CTRL_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_SAMPLE_CTRL )
#define ADC12_SAR_SAMPLE_CTRL_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_SAMPLE_CTRL )

#define ADC12_SAR_SAMPLE_TIME01_REG       (*(reg32 *) ADC12_cy_psoc4_sar__SAR_SAMPLE_TIME01 )
#define ADC12_SAR_SAMPLE_TIME01_PTR       ( (reg32 *) ADC12_cy_psoc4_sar__SAR_SAMPLE_TIME01 )

#define ADC12_SAR_SAMPLE_TIME23_REG       (*(reg32 *) ADC12_cy_psoc4_sar__SAR_SAMPLE_TIME23 )
#define ADC12_SAR_SAMPLE_TIME23_PTR       ( (reg32 *) ADC12_cy_psoc4_sar__SAR_SAMPLE_TIME23 )

#define ADC12_SAR_RANGE_THRES_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_RANGE_THRES )
#define ADC12_SAR_RANGE_THRES_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_RANGE_THRES )

#define ADC12_SAR_RANGE_COND_REG          (*(reg32 *) ADC12_cy_psoc4_sar__SAR_RANGE_COND )
#define ADC12_SAR_RANGE_COND_PTR          ( (reg32 *) ADC12_cy_psoc4_sar__SAR_RANGE_COND )

#define ADC12_SAR_CHAN_EN_REG             (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_EN )
#define ADC12_SAR_CHAN_EN_PTR             ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_EN )

#define ADC12_SAR_START_CTRL_REG          (*(reg32 *) ADC12_cy_psoc4_sar__SAR_START_CTRL )
#define ADC12_SAR_START_CTRL_PTR          ( (reg32 *) ADC12_cy_psoc4_sar__SAR_START_CTRL )

#define ADC12_SAR_DFT_CTRL_REG            (*(reg32 *) ADC12_cy_psoc4_sar__SAR_DFT_CTRL )
#define ADC12_SAR_DFT_CTRL_PTR            ( (reg32 *) ADC12_cy_psoc4_sar__SAR_DFT_CTRL )

#define ADC12_SAR_CHAN_CONFIG_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_CONFIG00 )
#define ADC12_SAR_CHAN_CONFIG_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_CONFIG00 )
#define ADC12_SAR_CHAN_CONFIG_IND         ADC12_cy_psoc4_sar__SAR_CHAN_CONFIG00

#define ADC12_SAR_CHAN_WORK_REG           (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_WORK00 )
#define ADC12_SAR_CHAN_WORK_PTR           ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_WORK00 )

#define ADC12_SAR_CHAN_RESULT_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define ADC12_SAR_CHAN_RESULT_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define ADC12_SAR_CHAN_RESULT_IND         ADC12_cy_psoc4_sar__SAR_CHAN_RESULT00

#define ADC12_SAR_CHAN0_RESULT_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define ADC12_SAR_CHAN0_RESULT_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT00 )

#define ADC12_SAR_CHAN1_RESULT_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT01 )
#define ADC12_SAR_CHAN1_RESULT_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT01 )

#define ADC12_SAR_CHAN2_RESULT_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT02 )
#define ADC12_SAR_CHAN2_RESULT_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT02 )

#define ADC12_SAR_CHAN3_RESULT_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT03 )
#define ADC12_SAR_CHAN3_RESULT_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT03 )

#define ADC12_SAR_CHAN4_RESULT_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT04 )
#define ADC12_SAR_CHAN4_RESULT_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT04 )

#define ADC12_SAR_CHAN5_RESULT_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT05 )
#define ADC12_SAR_CHAN5_RESULT_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT05 )

#define ADC12_SAR_CHAN6_RESULT_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT06 )
#define ADC12_SAR_CHAN6_RESULT_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT06 )

#define ADC12_SAR_CHAN7_RESULT_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT07 )
#define ADC12_SAR_CHAN7_RESULT_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT07 )

#if(ADC12_CY_SAR_IP_VER != ADC12_CY_SAR_IP_VER0)
    #define ADC12_SAR_CHAN8_RESULT_REG     (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT08 )
    #define ADC12_SAR_CHAN8_RESULT_PTR     ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT08 )

    #define ADC12_SAR_CHAN9_RESULT_REG     (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT09 )
    #define ADC12_SAR_CHAN9_RESULT_PTR     ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT09 )

    #define ADC12_SAR_CHAN10_RESULT_REG    (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT10 )
    #define ADC12_SAR_CHAN10_RESULT_PTR    ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT10 )

    #define ADC12_SAR_CHAN11_RESULT_REG    (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT11 )
    #define ADC12_SAR_CHAN11_RESULT_PTR    ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT11 )

    #define ADC12_SAR_CHAN12_RESULT_REG    (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT12 )
    #define ADC12_SAR_CHAN12_RESULT_PTR    ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT12 )

    #define ADC12_SAR_CHAN13_RESULT_REG    (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT13 )
    #define ADC12_SAR_CHAN13_RESULT_PTR    ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT13 )

    #define ADC12_SAR_CHAN14_RESULT_REG    (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT14 )
    #define ADC12_SAR_CHAN14_RESULT_PTR    ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT14 )

    #define ADC12_SAR_CHAN15_RESULT_REG    (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT15 )
    #define ADC12_SAR_CHAN15_RESULT_PTR    ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT15 )
#endif /* (ADC12_CY_SAR_IP_VER != ADC12_CY_SAR_IP_VER0) */

#define ADC12_SAR_CHAN_WORK_VALID_REG     (*(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_WORK_VALID)
#define ADC12_SAR_CHAN_WORK_VALID_PTR     ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_WORK_VALID)

#define ADC12_SAR_CHAN_RESULT_VALID_REG  ( *(reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT_VALID )
#define ADC12_SAR_CHAN_RESULT_VALID_PTR  ( (reg32 *) ADC12_cy_psoc4_sar__SAR_CHAN_RESULT_VALID )

#define ADC12_SAR_STATUS_REG              (*(reg32 *) ADC12_cy_psoc4_sar__SAR_STATUS )
#define ADC12_SAR_STATUS_PTR              ( (reg32 *) ADC12_cy_psoc4_sar__SAR_STATUS )

#define ADC12_SAR_AVG_START_REG           (*(reg32 *) ADC12_cy_psoc4_sar__SAR_AVG_STAT )
#define ADC12_SAR_AVG_START_PTR           ( (reg32 *) ADC12_cy_psoc4_sar__SAR_AVG_STAT )

#define ADC12_SAR_INTR_REG                (*(reg32 *) ADC12_cy_psoc4_sar__SAR_INTR )
#define ADC12_SAR_INTR_PTR                ( (reg32 *) ADC12_cy_psoc4_sar__SAR_INTR )

#define ADC12_SAR_INTR_SET_REG            (*(reg32 *) ADC12_cy_psoc4_sar__SAR_INTR_SET )
#define ADC12_SAR_INTR_SET_PTR            ( (reg32 *) ADC12_cy_psoc4_sar__SAR_INTR_SET )

#define ADC12_SAR_INTR_MASK_REG           (*(reg32 *) ADC12_cy_psoc4_sar__SAR_INTR_MASK )
#define ADC12_SAR_INTR_MASK_PTR           ( (reg32 *) ADC12_cy_psoc4_sar__SAR_INTR_MASK )

#define ADC12_SAR_INTR_MASKED_REG         (*(reg32 *) ADC12_cy_psoc4_sar__SAR_INTR_MASKED )
#define ADC12_SAR_INTR_MASKED_PTR         ( (reg32 *) ADC12_cy_psoc4_sar__SAR_INTR_MASKED )

#define ADC12_SAR_SATURATE_INTR_REG       (*(reg32 *) ADC12_cy_psoc4_sar__SAR_SATURATE_INTR )
#define ADC12_SAR_SATURATE_INTR_PTR       ( (reg32 *) ADC12_cy_psoc4_sar__SAR_SATURATE_INTR )

#define ADC12_SAR_SATURATE_INTR_SET_REG   (*(reg32 *) ADC12_cy_psoc4_sar__SAR_SATURATE_INTR_SET )
#define ADC12_SAR_SATURATE_INTR_SET_PTR   ( (reg32 *) ADC12_cy_psoc4_sar__SAR_SATURATE_INTR_SET )

#define ADC12_SAR_SATURATE_INTR_MASK_REG (*(reg32 *) ADC12_cy_psoc4_sar__SAR_SATURATE_INTR_MASK )
#define ADC12_SAR_SATURATE_INTR_MASK_PTR ( (reg32 *) ADC12_cy_psoc4_sar__SAR_SATURATE_INTR_MASK )

#define ADC12_SAR_SATURATE_INTR_MASKED_REG \
                                                 (*(reg32 *) ADC12_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED )
#define ADC12_SAR_SATURATE_INTR_MASKED_PTR \
                                                 ( (reg32 *) ADC12_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED )

#define ADC12_SAR_RANGE_INTR_REG          (*(reg32 *) ADC12_cy_psoc4_sar__SAR_RANGE_INTR )
#define ADC12_SAR_RANGE_INTR_PTR          ( (reg32 *) ADC12_cy_psoc4_sar__SAR_RANGE_INTR )

#define ADC12_SAR_RANGE_INTR_SET_REG      (*(reg32 *) ADC12_cy_psoc4_sar__SAR_RANGE_INTR_SET )
#define ADC12_SAR_RANGE_INTR_SET_PTR      ( (reg32 *) ADC12_cy_psoc4_sar__SAR_RANGE_INTR_SET )

#define ADC12_SAR_RANGE_INTR_MASK_REG     (*(reg32 *) ADC12_cy_psoc4_sar__SAR_RANGE_INTR_MASK )
#define ADC12_SAR_RANGE_INTR_MASK_PTR     ( (reg32 *) ADC12_cy_psoc4_sar__SAR_RANGE_INTR_MASK )

#define ADC12_SAR_RANGE_INTR_MASKED_REG   (*(reg32 *) ADC12_cy_psoc4_sar__SAR_RANGE_INTR_MASKED )
#define ADC12_SAR_RANGE_INTR_MASKED_PTR   ( (reg32 *) ADC12_cy_psoc4_sar__SAR_RANGE_INTR_MASKED )

#define ADC12_SAR_INTR_CAUSE_REG          (*(reg32 *) ADC12_cy_psoc4_sar__SAR_INTR_CAUSE )
#define ADC12_SAR_INTR_CAUSE_PTR          ( (reg32 *) ADC12_cy_psoc4_sar__SAR_INTR_CAUSE )

#if(ADC12_INJ_CHANNEL_ENABLED)
    #define ADC12_SAR_INJ_CHAN_CONFIG_REG \
                                                 (*(reg32 *) ADC12_cy_psoc4_sarmux_8__SAR_INJ_CHAN_CONFIG )
    #define ADC12_SAR_INJ_CHAN_CONFIG_PTR    \
                                                 ( (reg32 *) ADC12_cy_psoc4_sarmux_8__SAR_INJ_CHAN_CONFIG )

    #define ADC12_SAR_INJ_RESULT_REG    (*(reg32 *) ADC12_cy_psoc4_sarmux_8__SAR_INJ_RESULT )
    #define ADC12_SAR_INJ_RESULT_PTR    ( (reg32 *) ADC12_cy_psoc4_sarmux_8__SAR_INJ_RESULT )
#endif /* ADC12_INJ_CHANNEL_ENABLED */
    
#define ADC12_MUX_SWITCH0_REG           (*(reg32 *)  ADC12_cy_psoc4_sar__SAR_MUX_SWITCH0 )
#define ADC12_MUX_SWITCH0_PTR           ( (reg32 *)  ADC12_cy_psoc4_sar__SAR_MUX_SWITCH0 )

#define ADC12_MUX_SWITCH_HW_CTRL_REG    (*(reg32 *)  ADC12_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL )
#define ADC12_MUX_SWITCH_HW_CTRL_PTR    ( (reg32 *)  ADC12_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL )

#define ADC12_PUMP_CTRL_REG             (*(reg32 *)  ADC12_cy_psoc4_sar__SAR_PUMP_CTRL )
#define ADC12_PUMP_CTRL_PTR             ( (reg32 *)  ADC12_cy_psoc4_sar__SAR_PUMP_CTRL )

#define ADC12_ANA_TRIM_REG              (*(reg32 *)  ADC12_cy_psoc4_sar__SAR_ANA_TRIM )
#define ADC12_ANA_TRIM_PTR              ( (reg32 *)  ADC12_cy_psoc4_sar__SAR_ANA_TRIM )

#define ADC12_WOUNDING_REG              (*(reg32 *)  ADC12_cy_psoc4_sar__SAR_WOUNDING )
#define ADC12_WOUNDING_PTR              ( (reg32 *)  ADC12_cy_psoc4_sar__SAR_WOUNDING )


/**************************************
*       Register Constants
**************************************/
#define ADC12_INTC_NUMBER                (ADC12_IRQ__INTC_NUMBER)
#define ADC12_INTC_PRIOR_NUMBER          (ADC12_IRQ__INTC_PRIOR_NUM)

/* defines for CTRL register */
#define ADC12_VREF_INTERNAL1024          (0x00000040Lu)
#define ADC12_VREF_EXTERNAL              (0x00000050Lu)
#define ADC12_VREF_VDDA_2                (0x00000060Lu)
#define ADC12_VREF_VDDA                  (0x00000070Lu)
#define ADC12_VREF_INTERNAL1024BYPASSED  (0x000000C0Lu)
#define ADC12_VREF_VDDA_2BYPASSED        (0x000000E0Lu)
#define ADC12_VREF_INTERNALVREF          (0x00000040Lu)
#define ADC12_VREF_INTERNALVREFBYPASSED  (0x000000C0Lu)

#define ADC12_NEG_VSSA_KELVIN            (0x00000000Lu)
#define ADC12_NEG_VSSA                   (0x00000200Lu)
#define ADC12_NEG_VREF                   (0x00000E00Lu)
#if(ADC12_TOTAL_CHANNELS_NUM > 1u)
    #define ADC12_NEG_OTHER              (uint16)((uint16)ADC12_cy_psoc4_sarmux_8__VNEG0 << 9u)
#else
    #define ADC12_NEG_OTHER              (0)
#endif /* ADC12_TOTAL_CHANNELS_NUM > 1u */

#define ADC12_SAR_HW_CTRL_NEGVREF        (0x00002000Lu)

#define ADC12_BOOSTPUMP_EN               (0x00100000Lu)

#define ADC12_NORMAL_PWR                 (0x00000000Lu)
#define ADC12_HALF_PWR                   (0x01000000Lu)
#define ADC12_MORE_PWR                   (0x02000000Lu)
#define ADC12_QUARTER_PWR                (0x03000000Lu)
#define ADC12_DEEPSLEEP_ON               (0x08000000Lu)

#define ADC12_DSI_SYNC_CONFIG            (0x10000000Lu)
#define ADC12_DSI_MODE                   (0x20000000Lu)
#define ADC12_SWITCH_DISABLE             (0x40000000Lu)
#define ADC12_ENABLE                     (0x80000000Lu)

/* defines for STATUS register */
#define ADC12_STATUS_BUSY                (0x80000000Lu)

/* defines for SAMPLE_CTRL register */
#define ADC12_ALT_RESOLUTION_10BIT       (0x00000001Lu)
#define ADC12_ALT_RESOLUTION_8BIT        (0x00000000Lu)

#define ADC12_DATA_ALIGN_LEFT            (0x00000002Lu)
#define ADC12_DATA_ALIGN_RIGHT           (0x00000000Lu)

#define ADC12_SE_SIGNED_RESULT           (0x00000004Lu)
#define ADC12_SE_UNSIGNED_RESULT         (0x00000000Lu)

#define ADC12_DIFF_SIGNED_RESULT         (0x00000008Lu)
#define ADC12_DIFF_UNSIGNED_RESULT       (0x00000000Lu)

#define ADC12_AVG_CNT_OFFSET             (4u)
#define ADC12_AVG_CNT_MASK               (0x00000070Lu)
#define ADC12_AVG_SHIFT                  (0x00000080Lu)

#define ADC12_CONTINUOUS_EN              (0x00010000Lu)
#define ADC12_DSI_TRIGGER_EN             (0x00020000Lu)
#define ADC12_DSI_TRIGGER_LEVEL          (0x00040000Lu)
#define ADC12_DSI_SYNC_TRIGGER           (0x00080000Lu)
#define ADC12_EOS_DSI_OUT_EN             (0x80000000Lu)

/* defines for SAMPLE_TIME01 / SAMPLE_TIME23 registers */
#define ADC12_SAMPLE_TIME13_OFFSET       (16u)
#define ADC12_SAMPLE_TIME02_MASK         (0x000003FFLu)
#define ADC12_SAMPLE_TIME13_MASK         (0x03FF0000Lu)

/* defines for RANGE_THRES registers */
#define ADC12_RANGE_HIGH_OFFSET          (16u)
#define ADC12_RANGE_HIGH_MASK            (0xFFFF0000Lu)
#define ADC12_RANGE_LOW_MASK             (0x0000FFFFLu)

/* defines for RANGE_COND register */
/* Compare mode setting constants:
*    BELOW   - Result < Low_Limit
*    INSIDE  - Low_Limit <= Result < High_Limit
*    ABOVE   - High_Limit <= Result
*    OUTSIDE - (Result < Low_Limit) or (High_Limit <= Result)
*/
#define ADC12_CMP_MODE_BELOW             (0x00000000Lu)
#define ADC12_CMP_MODE_INSIDE            (0x40000000Lu)
#define ADC12_CMP_MODE_ABOVE             (0x80000000Lu)
#define ADC12_CMP_MODE_OUTSIDE           (0xC0000000Lu)
#define ADC12_CMP_OFFSET                 (30u)

/* defines for _START_CTRL register */
#define ADC12_FW_TRIGGER                 (0x00000001Lu)

/* defines for DFT_CTRL register */
#define ADC12_DLY_INC                    (0x00000001Lu)
#define ADC12_HIZ                        (0x00000002Lu)
#define ADC12_DFT_INC_MASK               (0x000F0000Lu)
#define ADC12_DFT_OUTC_MASK              (0x00700000Lu)
#define ADC12_SEL_CSEL_DFT_MASK          (0x0F000000Lu)

/* configuration for clock speed > 9 Mhz based on
* characterization results
*/
#define ADC12_SEL_CSEL_DFT_CHAR          (0x03000000Lu)
#define ADC12_EN_CSEL_DFT                (0x10000000Lu)
#define ADC12_DCEN                       (0x20000000Lu)
#define ADC12_ADFT_OVERRIDE              (0x80000000Lu)

/* defines for CHAN_CONFIG / DIE_CHAN_CONFIG register
*  and channelsConfig parameter
*/
#define ADC12_SARMUX_VIRT_SELECT         (0x00000070Lu)
#define ADC12_DIFFERENTIAL_EN            (0x00000100Lu)
#define ADC12_ALT_RESOLUTION_ON          (0x00000200Lu)
#define ADC12_AVERAGING_EN               (0x00000400Lu)

#define ADC12_SAMPLE_TIME_SEL_SHIFT      (12u)
#define ADC12_SAMPLE_TIME_SEL_MASK       (0x00003000Lu)

#define ADC12_CHANNEL_CONFIG_MASK        (0x00003700Lu)

/* for CHAN_CONFIG only */
#define ADC12_DSI_OUT_EN                 (0x80000000Lu)

/* for INJ_CHAN_CONFIG only */
#define ADC12_INJ_TAILGATING             (0x40000000Lu)
#define ADC12_INJ_CHAN_EN                (0x80000000Lu)

/* defines for CHAN_WORK register */
#define ADC12_SAR_WRK_MAX_12BIT          (0x00001000Lu)
#define ADC12_SAR_WRK_MAX_10BIT          (0x00000400Lu)
#define ADC12_SAR_WRK_MAX_8BIT           (0x00000100Lu)

/* defines for CHAN_RESULT register */
#define ADC12_RESULT_MASK                (0x0000FFFFLu)
#define ADC12_SATURATE_INTR_MIR          (0x20000000Lu)
#define ADC12_RANGE_INTR_MIR             (0x40000000Lu)
#define ADC12_CHAN_RESULT_VALID_MIR      (0x80000000Lu)

/* defines for INTR_MASK register */
#define ADC12_EOS_MASK                   (0x00000001Lu)
#define ADC12_OVERFLOW_MASK              (0x00000002Lu)
#define ADC12_FW_COLLISION_MASK          (0x00000004Lu)
#define ADC12_DSI_COLLISION_MASK         (0x00000008Lu)
#define ADC12_INJ_EOC_MASK               (0x00000010Lu)
#define ADC12_INJ_SATURATE_MASK          (0x00000020Lu)
#define ADC12_INJ_RANGE_MASK             (0x00000040Lu)
#define ADC12_INJ_COLLISION_MASK         (0x00000080Lu)

/* defines for INJ_RESULT register */
#define ADC12_INJ_COLLISION_INTR_MIR     (0x10000000Lu)
#define ADC12_INJ_SATURATE_INTR_MIR      (0x20000000Lu)
#define ADC12_INJ_RANGE_INTR_MIR         (0x40000000Lu)
#define ADC12_INJ_EOC_INTR_MIR           (0x80000000Lu)

/* defines for MUX_SWITCH0 register */
#define ADC12_MUX_FW_VSSA_VMINUS         (0x00010000Lu)

/* defines for PUMP_CTRL register */
#define ADC12_PUMP_CTRL_ENABLED          (0x80000000Lu)

/* additional defines for channelsConfig parameter */
#define ADC12_IS_SATURATE_EN_MASK        (0x00000001Lu)
#define ADC12_IS_RANGE_CTRL_EN_MASK      (0x00000002Lu)

/* defines for WOUNDING register */
#define ADC12_WOUNDING_12BIT             (0x00000000Lu)
#define ADC12_WOUNDING_10BIT             (0x00000001Lu)
#define ADC12_WOUNDING_8BIT              (0x00000002Lu)

/* Trim value based on characterization */
#define ADC12_TRIM_COEF                  (2u)

#if(ADC12_MAX_RESOLUTION == ADC12_RESOLUTION_10)
    #define ADC12_ALT_WOUNDING           ADC12_WOUNDING_10BIT
#else
    #define ADC12_ALT_WOUNDING           ADC12_WOUNDING_8BIT
#endif /* ADC12_MAX_RESOLUTION == ADC12_RESOLUTION_10 */

#if(ADC12_DEFAULT_VREF_SEL == ADC12__VDDA_2)
    #define ADC12_DEFAULT_VREF_SOURCE    ADC12_VREF_VDDA_2
#elif(ADC12_DEFAULT_VREF_SEL == ADC12__VDDA)
    #define ADC12_DEFAULT_VREF_SOURCE    ADC12_VREF_VDDA
#elif(ADC12_DEFAULT_VREF_SEL == ADC12__INTERNAL1024)
    #define ADC12_DEFAULT_VREF_SOURCE    ADC12_VREF_INTERNAL1024
#elif(ADC12_DEFAULT_VREF_SEL == ADC12__INTERNAL1024BYPASSED)
    #define ADC12_DEFAULT_VREF_SOURCE    ADC12_VREF_INTERNAL1024BYPASSED
#elif(ADC12_DEFAULT_VREF_SEL == ADC12__INTERNALVREF)
    #define ADC12_DEFAULT_VREF_SOURCE    ADC12_VREF_INTERNALVREF
#elif(ADC12_DEFAULT_VREF_SEL == ADC12__INTERNALVREFBYPASSED)
    #define ADC12_DEFAULT_VREF_SOURCE    ADC12_VREF_INTERNALVREFBYPASSED
#elif(ADC12_DEFAULT_VREF_SEL == ADC12__VDDA_2BYPASSED)
    #define ADC12_DEFAULT_VREF_SOURCE    ADC12_VREF_VDDA_2BYPASSED
#else
    #define ADC12_DEFAULT_VREF_SOURCE    ADC12_VREF_EXTERNAL
#endif /* ADC12_DEFAULT_VREF_SEL == ADC12__VDDA_2 */

#if(ADC12_DEFAULT_NEG_INPUT_SEL == ADC12__VSS)
    /* Connect NEG input of SARADC to VSSA close to the SARADC for single channel mode */
    #if(ADC12_TOTAL_CHANNELS_NUM == 1u)
        #define ADC12_DEFAULT_SE_NEG_INPUT    ADC12_NEG_VSSA
    #else
        #define ADC12_DEFAULT_SE_NEG_INPUT    ADC12_NEG_VSSA_KELVIN
    #endif /* (ADC12_TOTAL_CHANNELS_NUM == 1u) */
    /* Do not connect VSSA to VMINUS when one channel in differential mode used */
    #if((ADC12_TOTAL_CHANNELS_NUM == 1u) && (ADC12_CHANNELS_MODE != 0u))
        #define ADC12_DEFAULT_MUX_SWITCH0     0u
    #else    /* miltiple channels or one single channel */
        #define ADC12_DEFAULT_MUX_SWITCH0     ADC12_MUX_FW_VSSA_VMINUS
    #endif /* (ADC12_TOTAL_CHANNELS_NUM == 1u) */
#elif(ADC12_DEFAULT_NEG_INPUT_SEL == ADC12__VREF)
    /* Do not connect VNEG to VREF when one channel in differential mode used */
    #if((ADC12_TOTAL_CHANNELS_NUM == 1u) && (ADC12_CHANNELS_MODE != 0u))
        #define ADC12_DEFAULT_SE_NEG_INPUT    0u
    #else    /* miltiple channels or one single channel */
        #define ADC12_DEFAULT_SE_NEG_INPUT    ADC12_NEG_VREF
    #endif /* (ADC12_TOTAL_CHANNELS_NUM == 1u) */
    #define ADC12_DEFAULT_MUX_SWITCH0     0u
#elif (ADC12_SINGLE_PRESENT != 0u)
    #define ADC12_DEFAULT_SE_NEG_INPUT    ADC12_NEG_OTHER
    #define ADC12_DEFAULT_MUX_SWITCH0     0u
#else
    #define ADC12_DEFAULT_SE_NEG_INPUT    0u
    #define ADC12_DEFAULT_MUX_SWITCH0     0u
#endif /* ADC12_DEFAULT_NEG_INPUT_SEL == ADC12__VREF */

/* If the SAR is configured for multiple channels, always set SAR_HW_CTRL_NEGVREF to 1 */
#if(ADC12_TOTAL_CHANNELS_NUM == 1u)
    #define ADC12_DEFAULT_HW_CTRL_NEGVREF 0u
#else
    #define ADC12_DEFAULT_HW_CTRL_NEGVREF ADC12_SAR_HW_CTRL_NEGVREF
#endif /* (ADC12_TOTAL_CHANNELS_NUM == 1u) */


#if(ADC12_DEFAULT_ALT_RESOLUTION_SEL == ADC12__RES8)
    #define ADC12_DEFAULT_ALT_RESOLUTION     (ADC12_ALT_RESOLUTION_8BIT)
    #define ADC12_DEFAULT_MAX_WRK_ALT        (ADC12_SAR_WRK_MAX_8BIT)
#else
    #define ADC12_DEFAULT_ALT_RESOLUTION     (ADC12_ALT_RESOLUTION_10BIT)
    #define ADC12_DEFAULT_MAX_WRK_ALT        (ADC12_SAR_WRK_MAX_10BIT)
#endif /* End ADC12_DEFAULT_ALT_RESOLUTION_SEL == ADC12__RES8 */

#if(ADC12_DEFAULT_JUSTIFICATION_SEL == ADC12__RIGHT)
    #define ADC12_DEFAULT_JUSTIFICATION  ADC12_DATA_ALIGN_RIGHT
#else
    #define ADC12_DEFAULT_JUSTIFICATION  ADC12_DATA_ALIGN_LEFT
#endif /* ADC12_DEFAULT_JUSTIFICATION_SEL == ADC12__RIGHT */

#if(ADC12_DEFAULT_DIFF_RESULT_FORMAT_SEL == ADC12__FSIGNED)
    #define ADC12_DEFAULT_DIFF_RESULT_FORMAT ADC12_DIFF_SIGNED_RESULT
#else
    #define ADC12_DEFAULT_DIFF_RESULT_FORMAT ADC12_DIFF_UNSIGNED_RESULT
#endif /* ADC12_DEFAULT_DIFF_RESULT_FORMAT_SEL == ADC12__FSIGNED */

#if(ADC12_DEFAULT_SE_RESULT_FORMAT_SEL == ADC12__FSIGNED)
    #define ADC12_DEFAULT_SE_RESULT_FORMAT ADC12_SE_SIGNED_RESULT
#else
    #define ADC12_DEFAULT_SE_RESULT_FORMAT ADC12_SE_UNSIGNED_RESULT
#endif /* ADC12_DEFAULT_SE_RESULT_FORMAT_SEL == ADC12__FSIGNED */

#if(ADC12_DEFAULT_SAMPLE_MODE_SEL == ADC12__FREERUNNING)
    #define ADC12_DSI_TRIGGER        0u
#else /* Firmware trigger */
    #define ADC12_DSI_TRIGGER        (ADC12_DSI_TRIGGER_EN | ADC12_DSI_SYNC_TRIGGER)
#endif /* End ADC12_DEFAULT_SAMPLE_MODE == ADC12__FREERUNNING */

#if(ADC12_INJ_CHANNEL_ENABLED)
    #define ADC12_SAR_INTR_MASK      (ADC12_EOS_MASK | ADC12_INJ_EOC_MASK)
#else
    #define ADC12_SAR_INTR_MASK      (ADC12_EOS_MASK)
#endif /* ADC12_INJ_CHANNEL_ENABLED*/

#if(ADC12_DEFAULT_AVG_MODE == ADC12__FIXEDRESOLUTION)
    #define ADC12_AVG_SHIFT_MODE     ADC12_AVG_SHIFT
#else
    #define ADC12_AVG_SHIFT_MODE     0u
#endif /* End ADC12_DEFAULT_AVG_MODE */

#define ADC12_COMPARE_MODE           (uint32)((uint32)(ADC12_DEFAULT_COMPARE_MODE) \
                                                << ADC12_CMP_OFFSET)

#if(ADC12_TOTAL_CHANNELS_NUM > 1u)
    #define ADC12_DEFAULT_SWITCH_CONF    0u
#else /* Disable SAR sequencer from enabling routing switches in single channel mode */
    #define ADC12_DEFAULT_SWITCH_CONF    ADC12_SWITCH_DISABLE
#endif /* End ADC12_TOTAL_CHANNELS_NUM > 1 */

#define ADC12_DEFAULT_POWER \
       ((ADC12_NOMINAL_CLOCK_FREQ > (ADC12_MAX_FREQUENCY / 4)) ? ADC12_NORMAL_PWR : \
       ((ADC12_NOMINAL_CLOCK_FREQ > (ADC12_MAX_FREQUENCY / 8)) ? ADC12_HALF_PWR : \
                                                                                       ADC12_QUARTER_PWR))

#define ADC12_DEFAULT_CTRL_REG_CFG       (ADC12_DEFAULT_VREF_SOURCE \
                                                   | ADC12_DEFAULT_SE_NEG_INPUT \
                                                   | ADC12_DEFAULT_HW_CTRL_NEGVREF \
                                                   | ADC12_DEFAULT_POWER \
                                                   | ADC12_DSI_SYNC_CONFIG \
                                                   | ADC12_DEFAULT_SWITCH_CONF)

#define ADC12_DEFAULT_SAMPLE_CTRL_REG_CFG (ADC12_DEFAULT_DIFF_RESULT_FORMAT \
                                                    | ADC12_DEFAULT_SE_RESULT_FORMAT \
                                                    | ADC12_DEFAULT_JUSTIFICATION \
                                                    | ADC12_DEFAULT_ALT_RESOLUTION \
                                           | (uint8)(ADC12_DEFAULT_AVG_SAMPLES_NUM \
                                                   << ADC12_AVG_CNT_OFFSET) \
                                                    | ADC12_AVG_SHIFT_MODE \
                                                    | ADC12_DSI_TRIGGER \
                                                    | ADC12_EOS_DSI_OUT_EN)

#define ADC12_DEFAULT_RANGE_THRES_REG_CFG (ADC12_DEFAULT_LOW_LIMIT \
            | (uint32)((uint32)ADC12_DEFAULT_HIGH_LIMIT << ADC12_RANGE_HIGH_OFFSET))

#define ADC12_DEFAULT_SAMPLE_TIME01_REG_CFG (ADC12_DEFAULT_ACLKS_NUM \
            | (uint32)((uint32)ADC12_DEFAULT_BCLKS_NUM << ADC12_SAMPLE_TIME13_OFFSET))

#define ADC12_DEFAULT_SAMPLE_TIME23_REG_CFG (ADC12_DEFAULT_CCLKS_NUM \
            | (uint32)((uint32)ADC12_DEFAULT_DCLKS_NUM << ADC12_SAMPLE_TIME13_OFFSET))


#endif /* End CY_ADC_SAR_SEQ_ADC12_H */


/* [] END OF FILE */
