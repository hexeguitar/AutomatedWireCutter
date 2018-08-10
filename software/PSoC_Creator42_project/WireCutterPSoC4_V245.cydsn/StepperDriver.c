/*
 * Copyright (c) 2018 by Piotr Zapart / www.hexeguitar.com
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list 
 * of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this 
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 *  
 * Neither the name of the Piotr Zapart or HEXE Guitar Electronics nor the names of its 
 * contributors may be used to endorse or promote products derived from this software 
 * without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 * 
 */
#include <project.h>
#include "StepperDriver.h"
#include "tinyprintf.h"
#ifdef USE_TMC2130
    #include "TMC2130.h"
#endif
#include "A4988.h"
#include "config.h"
#include "debug.h"

const uint8_t fullPulseByte = 0x55;
const uint8_t partialPulseBytes[4]=
{
    0x00,   //1 puse 
    0x01,   //2 pulses
    0x05,   //3 pulses
    0x15    //4 pulses
};

#ifdef USE_TMC2130
TMC2130_t Stepper0;
#endif
// ### Global variables ###

Stepper_drvType_t Stepper_driverType = STPDRV_A4988;    //assume TMC2130 is not present

static volatile uint32_t _fullBytesToSend = 0;
static volatile uint8_t _lastByteToSend = 0;
uint16_t Stepper_defaultClockDivider;
uint8_t Stepper_defaultClockDividerFrac;

static volatile uint32_t Stepper_fullBytesJob;
static volatile uint32_t Stepper_clkDiv;
volatile int16_t Stepper_driverTemp = 0; 

static volatile uint8_t Stepper_pauseFlag = 0; 

StepperStatus_t Stepper0_Status;

uint32_t Stepper_usteps = 0;

// ### Function declarations ###
CY_ISR_PROTO(PulserFifoNotEmpty_ISR);

void Stepper_sendPulses(void);



// ### Function definitions ###
// #############################################################################

CY_ISR (PulserFifoNotEmpty_ISR)
{
    static uint32_t Stepper_clkDiv_old = 0;
    #ifdef STEPPER_USE_RAMP
        // ramp up
        if (_fullBytesToSend > Stepper_fullBytesJob-STEPPER_RAMP_TIME)
        {
            if (Stepper_clkDiv > STEPPER_CRUISE_SPEED)  Stepper_clkDiv -=STEPPER_SPEED_DELTA;
            else                                        Stepper_clkDiv = STEPPER_CRUISE_SPEED;         
        }
        // ramp down
        if (_fullBytesToSend <= STEPPER_RAMP_TIME && Stepper_clkDiv < STEPPER_SLOW_SPEED)
        {
            Stepper_clkDiv +=STEPPER_SPEED_DELTA;
        }

        if (Stepper_clkDiv != Stepper_clkDiv_old)
        {
            Clock_Stepper_SetDivider(Stepper_clkDiv);
            Stepper_clkDiv_old = Stepper_clkDiv;
        }
    #endif
    if (Stepper_pauseFlag)
    {
        Stepper_pauseFlag = 0;
        StepPulser_DisableTxInt();
    }
    else
    {
        if (_fullBytesToSend)                            //if there are sill full pulse-bytes to send
        {
            StepPulser_PutChar(fullPulseByte);
            _fullBytesToSend--;
        }
        else
        {
            if (Stepper0_Status == STPDRV_RUNNING)  
            {
                if (_lastByteToSend)  StepPulser_PutChar(partialPulseBytes[_lastByteToSend-1]);
                Stepper0_Status = STPDRV_IDLE;     //job done
            }
            else    StepPulser_DisableTxInt();          //disable the interrupt
        }

    }
}

/*****************************************************************************\
 * Function:    Stepper_init
 * Input:       none
 * Returns:     none
 * Description: 
 *     Steps the motor
\*****************************************************************************/
void Stepper_init(void)
{    
    #ifdef STEPPER_USE_RAMP
    Clock_Stepper_SetDivider(STEPPER_SLOW_SPEED);
    #else
    Clock_Stepper_SetDivider(STEPPER_CRUISE_SPEED);    
    #endif
    
    // in order to HE reset the TMC130 the SPI has to be disabled and the pins driven with 0
    // to avoid powering the chip via input protection diodes.    
    SPI_Stop();         
    SPI_sclk_m_Write(0);
    SPI_miso_m_Write(0);
    SPI_mosi_m_Write(0);
    SPI_ss1_m_Write(0);        
    TMC2130_PWR_Write(1);   //power off
    CyDelay(100);
    TMC2130_PWR_Write(0);   //power on    
    CyDelay(100);    
    SPI_Start();            // restart the SPI
    TMC2130_set_device(&Stepper0);                                  //set the device to 0
    Stepper0.slaveSelect = TMC2130_SPI_SLAVE_SELECT;                //set the chip select line
    
    // detect the TMC2130 presence by reading DRV_STATUS, it the read returns 0xFFFFFFFF - assume the TMC is not present
    if (TMC2130_getDRV_STATUS() != 0xFFFFFFFF) 
    {
        Stepper_driverType = STPDRV_TMC2130;    
        Stepper_usteps = MICROSTEPS;    
        
        TMC2130_init(   &Stepper0,  //pointer to the motor struct
                        MICROSTEPS, //microsteps
                        1000,       //Irun = 1.0A
                        20,         //Ihold = 0% Irun
                        110,        //Rsense = 0R110
                        0);         //Stealth mode
    }
    else    // A4988 microstepping control - max value = 16, update the global value if required
    {
        Stepper_usteps = A4988_setUsteps(MICROSTEPS);
    }
    
    StepDir_Write(STEPPER_ROT_FWD);
    
    StepPulser_Start();
    StepPulser_DisableTxInt();                      //will be enabled in "startStepping" function
    isr_StepPulserFifoNotEmpty_ClearPending();
    isr_StepPulserFifoNotEmpty_StartEx(PulserFifoNotEmpty_ISR);

    Stepper0_Status = STPDRV_IDLE;  
}

/*****************************************************************************\
 * Function:    Stepper_getDriverType
 * Input:       none
 * Returns:     Stepper_driverType
 * Description: 
 *      returns the current detected (TMC2130) or default assumed stepper
 *      driver type
\*****************************************************************************/
Stepper_drvType_t Stepper_getDriverType(void)
{
    return Stepper_driverType;
}

/*****************************************************************************\
 * Function:    Stepper_getUsteps
 * Input:       none
 * Returns:     Current usteps value
 * Description: 
\*****************************************************************************/
uint8_t Stepper_getUsteps(void)
{
    return Stepper_usteps;
}

/*****************************************************************************\
 * Function:    Stepper_reset
 * Input:       none
 * Returns:     none
 * Description: 
 *     Performs a hardware reset of the stepper driver by toggling the power
\*****************************************************************************/
void Stepper_reset(void)
{
    TMC2130_PWR_Write(1);   //power off   
    CyDelay(10);
    TMC2130_PWR_Write(1);   //power on
    CyDelay(10);
}

/*****************************************************************************\
 * Function:    Stepper_runSteps
 * Input:       number of steps
 * Returns:     status
 * Description: 
 *     Steps the motor
\*****************************************************************************/
void Stepper_runSteps(uint32_t steps)
{
    if (Stepper0_Status==STPDRV_RUNNING) return;

    _fullBytesToSend = steps / 5;
    _lastByteToSend = steps % 5;
    Stepper_fullBytesJob = _fullBytesToSend;

    Stepper_sendPulses(); 
}

/*****************************************************************************\
 * Function:    Stepper_runMM
 * Input:       length in mm
 * Returns:     success=0, fail=1
 * Description: 
 *     pulls desired length of wire
\*****************************************************************************/
uint8 Stepper_runMM(uint32_t length_mm, uint32_t stepsPerMm, Stepper_dir_t dir)
{
    uint8 result = 1;
    
    if (Stepper0_Status==STPDRV_RUNNING) return result;
    
    StepDir_Write(dir);
    
    uint32_t steps = length_mm * stepsPerMm;
    
    #ifdef DEBUG_STEPPER
    printf("\r\nRun:%dmm\r\n",length_mm);    
    printf("Steps:%d\r\n",steps);    
    #endif  
    
    _fullBytesToSend = steps / 5;
    _lastByteToSend = steps % 5;
    Stepper_fullBytesJob = _fullBytesToSend;
    
    #ifdef STEPPER_USE_RAMP
        Clock_Stepper_SetDivider(STEPPER_SLOW_SPEED);
        Stepper_clkDiv = STEPPER_SLOW_SPEED;
    #else
        Clock_Stepper_SetDivider(STEPPER_CRUISE_SPEED);    
    #endif

    Stepper_sendPulses();
    
    result  = 0;
    return result;
}

/*****************************************************************************\
 * Function:    Stepper_sendPulses
 * Input:       none
 * Returns:     none
 * Description: 
 *     Starts sending out pulses according to the values set by runMM 
 *     or runSteps functions.
 *     If the # of bytes to send is <= 4, the HW fifo is used (will not trigger
 *     the FifoNotFull Int, hence the bytes are sent manually.
 *     If it's more, HW fifo is filled up + one byte in SW fifo, triggering 
 *     FifoNotFull Int. From there the ISR takes over.
\*****************************************************************************/
void Stepper_sendPulses(void)
{
    uint8_t i; 
    
    StepPulser_EnableTxInt();
     
    if (_fullBytesToSend <= 4)                           //using hardware fifo only
    {
        while (_fullBytesToSend)
        {
            StepPulser_PutChar(fullPulseByte);
            _fullBytesToSend--;
        }
        if (_lastByteToSend)  StepPulser_PutChar(partialPulseBytes[_lastByteToSend-1]);
    }
    else
    {
        Stepper0_Status = STPDRV_RUNNING;               //the 6th byte will go into software buffer and fire 
        for (i=0; i<5;i++)                              //the FIFO not empty interrupt
        {                                               //from there the interrupt driven routine takes over
            StepPulser_PutChar(fullPulseByte);
            _fullBytesToSend--;
        }       
    } 
}

/*****************************************************************************\
 * Function:    Stepper_abortRun
 * Input:       none
 * Returns:     none
 * Description: 
 *     Stop the ongoing run and clear the not yet sent pulses
\*****************************************************************************/
void Stepper_abortRun(void)
{
    StepPulser_DisableTxInt();
    StepPulser_ClearTxBuffer();
    _fullBytesToSend = 0;
    _lastByteToSend = 0;
    Stepper0_Status = STPDRV_IDLE;
}

/*****************************************************************************\
 * Function:    Stepper_setPause
 * Input:       pause on = 1, off = 0
 * Returns:     none
 * Description: 
 *     Pause/resume the ongoing run
 *     Once paused, the UART FIFO will empty itself, including the 4 byte HW one.
 *     In order to resume the job we need to full it up again and send the bytes
 *     to trigger the FifoNotFull Int again.
\*****************************************************************************/
void Stepper_setPause(uint8_t value)
{   
    if (value)  Stepper_pauseFlag = 1;
    else        
    {
        Stepper_sendPulses(); 
    }
}

// #############################################################################
StepperStatus_t Stepper_getStatus(void)
{
    return Stepper0_Status;
}
// #############################################################################
void printStepperStatus(void)
{
    if (Stepper_driverType == STPDRV_TMC2130)
    {
        uint8_t status = TMC2130_read_STAT();
        printf("standstill=%d\r\n",status&(1<<3)?1:0);
        printf("sg2=%d\r\n",status&(1<<2)?1:0);
        printf("driver_error=%d\r\n",status&(1<<1)?1:0);
        printf("reset_flag=%d\r\n",status&(1<<0)?1:0);
        printf("diag0=%d\r\n",status&(1<<4)?1:0);
        printf("diag1=%d\r\n",status&(1<<5)?1:0);
    }
}
// #############################################################################

/*****************************************************************************\
 * Function:    Stepper_checkErrors
 * Input:       none
 * Returns:     status byte
 * Description: 
 *      Checks if any of the DIAG01 outputs have been triggered
 *      For A4988 driver returns 0 
\*****************************************************************************/
uint8 Stepper_checkErrors(void)
{
    uint8 status = 0;
    if (Stepper_driverType == STPDRV_TMC2130)
    {
        status = TMC2130_checkDiagErrors(); 
    }
    //no error checking for A4988 driver
    
    return status;
}

/*****************************************************************************\
 * Function:    Stepper_clearErrors
 * Input:       none
 * Returns:     status byte
 * Description: 
 *     Clears detected stepper driver errors
\*****************************************************************************/
uint8_t Stepper_clearErrors(void)
{
    uint8 status = 0;
    if (Stepper_driverType == STPDRV_TMC2130)
    {
        status = TMC2130_clearErrors();    
    }
    //no error checking for A4988 driver 
    
    return status;
}

/*****************************************************************************\
 * Function:    Stepper_printErrors
 * Input:       none
 * Returns:     none
 * Description: 
 *     Prints error status via UART
\*****************************************************************************/
bool Stepper_printErrors(void)
{
    bool result = false;
    if (Stepper_driverType == STPDRV_TMC2130)
    {
        result = true;
        TMC2130_printDRV_STATUS();
    }
    return result;
}
/*****************************************************************************\
 * Function:    Stepper_ErrorStatus
 * Input:       none
 * Returns:     Error status word
 * Description: 
 *      Function returns the TMC2130 DRV_Status word
\*****************************************************************************/
uint32_t Stepper_getErrorStatus(void)
{
    uint32_t errorStatus = 0;
    if (Stepper_driverType == STPDRV_TMC2130)
    {
        errorStatus =   TMC2130_getDRV_STATUS();
    }                    
    return errorStatus;
}

/*****************************************************************************\
 * Function:    Stepper_getLoad
 * Input:       none
 * Returns:     SG_RESULT value
 * Description: 
 *     Returns the TMC2130 SG_RESULT register value
\*****************************************************************************/
uint32_t Stepper_getLoad(void)
{
    uint32_t result = 0;
    if (Stepper_driverType == STPDRV_TMC2130)
    {
        result = TMC2130_get_sg();
    }
    return result;
}



/* [] END OF FILE */
