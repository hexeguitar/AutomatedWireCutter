/*
 * Copyright (c) 2018 Piotr Zapart / www.hexeguitar.com
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
 *****************************************************
 *  Trinamic TMC2130 Stepper Motor Controller driver *
 *****************************************************
 * 
 */

#include <stdbool.h>
#include <project.h>
#include "TMC2130.h"
#include "TMC2130_reg.h"
#include "debug.h"
#include "tinyprintf.h"
#include "config.h"

static uint8_t TMC2130_status;
static uint8_t TMC2130_coolconf;
static uint8_t TMC2130_pwmconf;
static uint16_t TMC2130_Rsense_mOhm = 110;


TMC2130_t *currentMotor_ptr;
static volatile uint8_t TMC2130_diagError;


CY_ISR_PROTO(TMC2130_DiagFail_ISR);


/*****************************************************************************\
 * Function:    TMC2130_DiagFail_ISR
 * Input:       none
 * Returns:     none
 * Description: 
 *     TMC2130 DIAG0 and DIAG1 ISR
\*****************************************************************************/
CY_ISR(TMC2130_DiagFail_ISR)
{
    TMC2130_diagError = TMC2130_diag01_Read()&0x03; 
    #ifdef DEBUG_TMC2130
    printf("DIAG=%d\r\n",TMC2130_diagError);
    #endif
    TMC2130_diag01_ClearInterrupt();   
}

/*****************************************************************************\
 * Function:    TMC2130_init
 * Input:       none
 * Returns:     none
 * Description: 
 *     Initialize the TMC210 via SPI
\*****************************************************************************/
void TMC2130_init(TMC2130_t *device, uint16_t microsteps, uint16_t iRun_mA, uint16_t mult_percent, uint16_t Rsense_mOhm, uint8_t stealthChop )
{
    uint32_t temp32;

    TMC2130_set_device(device);                                 //
    TMC2130_set_mres(microsteps);

    TMC2130_set_diag0_int_pushpull(1);
    TMC2130_set_diag1_int_pushpull(1);
    
    TMC2130_set_I_scale_analog(TMC2130_DEFAULT_ISCALE_ANALOG);  //curent set via SPI
    TMC2130_set_rmscurrent(iRun_mA, mult_percent, Rsense_mOhm); //set current values
    TMC2130_set_TPOWERDOWN(127);
    TMC2130_set_en_pwm_mode(stealthChop);                       //Enable stealth chop
    
    TMC2130_set_pwm_autoscale(1);
    TMC2130_set_PWM_GRAD(10);
    TMC2130_set_PWM_AMPL(255);
    
    TMC2130_set_intpol(1);
    
    TMC2130_set_toff(TMC2130_DEFAULT_TOFF);
    TMC2130_set_tbl(2);                                      //Set comparator blank time
    TMC2130_set_hstrt(4);
    TMC2130_set_hend(0);
    
    TMC2130_set_diag0_error(1);                                 //Enable error signalling on DIAG0 output
    
    TMC2130_set_TPWMTHRS(0x000FFFFF);
    TMC2130_set_sfilt(1);
    TMC2130_set_THIGH(0xFFFFF);
    TMC2130_set_TCOOLTHRS(0x01FFF);
    TMC2130_set_sgt(12);
    TMC2130_set_diag1_steps_skipped(1);                         //Enable signalling of skipped steps on DIAG1
    //TMC2130_set_shaft(1);                                       //set the rotation dir

    TMC2130_read_REG(TMC_REG_GSTAT,&temp32);                    //read GSTAT to clear the reset flag
    
    isr_TMC2130_fail_ClearPending();
    isr_TMC2130_fail_StartEx(TMC2130_DiagFail_ISR);
    TMC2130_diagError = 0x00;                               //clear the flag

}

// #########################################################################
/*
    The driver is able to contol multiple motors, but only one at a time.
    This function sets the active motorand sets the step output demux for the chosen motor.
    The Dir signal is shared between available motors.
*/
int8_t TMC2130_set_device(TMC2130_t *device_ptr)
{
    int8_t result = -1; //assume error
    currentMotor_ptr = device_ptr;
    return result;
}
// #########################################################################
uint8_t TMC2130_write(uint8_t cmd, uint32_t data)
{
    uint8_t databuf[5];
    
    //slow down the SPI clock for TMC2130 (max 4MHz with internal oscillator)
    volatile uint16_t SPIclk_divider;   
    Clock_SPI_Disable();
    SPIclk_divider = Clock_SPI_GetDividerRegister();
    Clock_SPI_SetDivider(SPIclk_divider+2);
    Clock_SPI_Enable();
    
    SPI_SpiSetActiveSlaveSelect(currentMotor_ptr->slaveSelect);                          //select the CS output
    
    databuf[0] = cmd|TMC_WRITE;
    databuf[1]=((data>>24UL)&0xFF)&0xFF;
    databuf[2]=((data>>16UL)&0xFF)&0xFF;
    databuf[3]=((data>> 8UL)&0xFF)&0xFF;
    databuf[4]=((data>> 0UL)&0xFF)&0xFF;

    //Wait untill all the data is sent (txbuffer and shift register)
    while ((SPI_SpiUartGetTxBufferSize() > 0) && (SPI_SpiIsBusBusy()!=0)) {};

    SPI_SpiUartPutArray(databuf, 5);
    while(SPI_SpiIsBusBusy()!=0){};
    TMC2130_status = SPI_SpiUartReadRxData();   //read 1st byte
    SPI_SpiUartClearRxBuffer();    
    
    // restore the high speed clock for the display
    Clock_SPI_Disable();
    Clock_SPI_SetDivider(SPIclk_divider);
    Clock_SPI_Enable();
    SPI_SpiSetActiveSlaveSelect(DEFAULT_SPI_SLAVE_SELECT);          //switch back the CS line for the display/default slave
    
    return TMC2130_status;
}
// #########################################################################
// read status
uint8_t TMC2130_read_STAT()
{
    //slow down the SPI clock for TMC2130 (max 4MHz with internal oscillator)
    volatile uint16_t SPIclk_divider;   
    Clock_SPI_Disable();
    SPIclk_divider = Clock_SPI_GetDividerRegister();
    Clock_SPI_SetDivider(SPIclk_divider+2);
    Clock_SPI_Enable();
    
    uint8_t databuf[5] = {0};
    
    //Wait untill all the data is sent (txbuffer and shift register)
    while ((SPI_SpiUartGetTxBufferSize() > 0) && (SPI_SpiIsBusBusy()!=0)) {};
    
    SPI_SpiSetActiveSlaveSelect(currentMotor_ptr->slaveSelect);                          //select the CS output

    SPI_SpiUartPutArray(databuf, 5);
    while(SPI_SpiIsBusBusy()!=0){};
    TMC2130_status = SPI_SpiUartReadRxData();   //read 1st byte
    SPI_SpiUartClearRxBuffer();         //clear the buffer

    
    // restore the high speed clock for the display
    Clock_SPI_Disable();
    Clock_SPI_SetDivider(SPIclk_divider);
    Clock_SPI_Enable();
    SPI_SpiSetActiveSlaveSelect(DEFAULT_SPI_SLAVE_SELECT);
    return TMC2130_status;
}

/*****************************************************************************\
 * Function:    TMC2130_read_REG
 * Input:       address, *data (pointer to u32)
 * Returns:     TMC2130 status byte
 * Description: 
 *     read a register
\*****************************************************************************/
uint8_t TMC2130_read_REG(uint8_t address, uint32_t *data)
{
    //slow down the SPI clock for TMC2130 (max 4MHz with internal oscillator)
    volatile uint16_t SPIclk_divider;   
    Clock_SPI_Disable();
    SPIclk_divider = Clock_SPI_GetDividerRegister();
    Clock_SPI_SetDivider(SPIclk_divider+2);
    Clock_SPI_Enable();
    
    uint8_t databuf[5] = {0};
    
    //set address to read from
    databuf[0] = address&~TMC_WRITE;

    //Wait untill all the data is sent (txbuffer and shift register)
    while ((SPI_SpiUartGetTxBufferSize() > 0) && (SPI_SpiIsBusBusy()!=0)) {};
    SPI_SpiSetActiveSlaveSelect(currentMotor_ptr->slaveSelect);                          //select the CS output
        
    SPI_SpiUartPutArray(databuf, 5);
    while(SPI_SpiIsBusBusy()!=0){};
    TMC2130_status = SPI_SpiUartReadRxData();   //get the status byte
    SPI_SpiUartClearRxBuffer();                 //clear the buffer
    
    //read the data
    databuf[0] = address&~TMC_WRITE;
    while ((SPI_SpiUartGetTxBufferSize() > 0) && (SPI_SpiIsBusBusy()!=0)) {};

    SPI_SpiUartPutArray(databuf, 5);
    while(SPI_SpiIsBusBusy()!=0){};
    TMC2130_status = SPI_SpiUartReadRxData();   // (0) get the status byte
    *data  = SPI_SpiUartReadRxData()&0xFF;      // (1)
    *data <<=8;
    *data |= SPI_SpiUartReadRxData()&0xFF;      // (2)
    *data <<=8;
    *data |= SPI_SpiUartReadRxData()&0xFF;      // (3)
    *data <<=8;
    *data |= SPI_SpiUartReadRxData()&0xFF;      // (4)
    SPI_SpiUartClearRxBuffer();                 //clear the buffer
     
    Clock_SPI_Disable();
    Clock_SPI_SetDivider(SPIclk_divider);
    Clock_SPI_Enable();
    
    SPI_SpiSetActiveSlaveSelect(DEFAULT_SPI_SLAVE_SELECT);          //switch the SPI slave slect back to Display
    return TMC2130_status;
}

/*****************************************************************************\
 * Function:    TMC2130_alter_REG
 * Input:       address, data, mask
 * Returns:     TMC2130 status byte
 * Description: 
 *     alter a register using a bitmask
\*****************************************************************************/
uint8_t TMC2130_alter_REG(uint8_t address, uint32_t data, uint32_t mask)
{
    uint32_t oldData, newData;

    TMC2130_read_REG(address, &oldData);
    newData = (oldData &~ mask) | (data & mask);
    TMC2130_write(address, newData);

    return TMC2130_status;
}

/*****************************************************************************\
 * Function:    TMC2130_set_rmscurrent
 * Input:       Irun_mA
 * Returns:     none
 * Description: 
 *    	
	Requested current = mA = I_rms/1000
	Equation for current:
	I_rms = (CS+1)/32 * V_fs/(R_sense+0.02ohm) * 1/sqrt(2)
	Solve for CS ->
	CS = 32*sqrt(2)*I_rms*(R_sense+0.02)/V_fs - 1
	
    int version (no float calculations)

    temp64 = 4525472ULL * (uint64_t)I_rms_mA * ((uint64_t)R_sense_mOhm+20ULL) / ((uint64_t)V_fs_mV * 10000000ULL)-1

	Example:
	vsense = 0b0 -> V_fs = 0.325V
	mA = 1640mA = I_rms/1000 = 1.64A
	R_sense = 0.10 Ohm
	->
	CS = 32*sqrt(2)*1.64*(0.10+0.02)/0.325 - 1 = 26.4
	CS = 26
\*****************************************************************************/
void TMC2130_set_rmscurrent(uint64_t Irun_mA, uint16 multiplier_percent, uint64_t RS_mOhm) 
{
    uint64_t temp64;
    uint8_t CS;
	TMC2130_Rsense_mOhm = RS_mOhm;
    
    temp64 = (452472ULL * Irun_mA * (RS_mOhm+20ULL) + (3250000000ULL>>1) ) / 3250000000ULL - 1; //optimized for integer calculations
    
	// If Current Scale is too low, turn on high sensitivity R_sense and calculate again
	if (temp64 < 16) 
    {
		TMC2130_set_vsense(1);
        temp64 = (452472ULL * Irun_mA * (RS_mOhm+20ULL) + (1800000000ULL>>1) ) / 1800000000ULL -1;  //recalculate with lower Vsense
        
	} 
    else if(TMC2130_get_vsense()) 
    { // If CS >= 16, turn off high_sense_r if it's currently ON
		TMC2130_set_vsense(0); 
	}
    
    CS = (uint8_t)temp64;
    uint16_t CS_hold = (CS * multiplier_percent + 50)/100;

    
    TMC2130_set_IHOLD_IRUN((uint8_t)CS_hold,CS,8);    
    
    #ifdef DEBUG_TMC2130
    printf("CS_run=%02d CS_hold=%02d\r\n",CS, CS_hold);    
    #endif
    
}
/*****************************************************************************\
 * GCONF
\*****************************************************************************/

// #########################################################################
// set single bits in the GCONF register
uint8_t TMC2130_set_GCONF(uint8_t position, uint8_t value)
{
    TMC2130_alter_REG(TMC_REG_GCONF, (uint32_t)((value)<<position), 0x1UL<<position);

    return TMC2130_status;
}
// #########################################################################
// set single bits or values in the chopconf register (constraining masks are applied if necessary)
uint8_t TMC2130_set_CHOPCONF(uint8_t position, uint8_t value)
{
    TMC2130_alter_REG(TMC_REG_CHOPCONF, (uint32_t)(value)<<position,((uint32_t) TMC_CHOPCONF_MASKS[position])<<position);

    return TMC2130_status;
}
// #########################################################################
// GCONF
/*
    I_scale_analog
    0: Normal operation, use internal reference voltage
    1: Use voltage supplied to AIN as current reference
*/
uint8_t TMC2130_set_I_scale_analog(uint8_t value)
{
  TMC2130_set_GCONF(TMC_GCONF_I_SCALE_ANALOG, value);

  return TMC2130_status;
}
// #########################################################################
/*
    internal_Rsense
    0:  Normal operation
    1:  Internal sense resistors. Use current supplied into
        AIN as reference for internal sense resistor
*/
uint8_t TMC2130_set_internal_Rsense(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_INTERNAL_RSENSE, value);

    return TMC2130_status;
}
// #########################################################################
/*
    en_pwm_mode
    1:  stealthChop voltage PWM mode enabled
        (depending on velocity thresholds). Switch from
        off to on state while in stand still, only.
*/
uint8_t TMC2130_set_en_pwm_mode(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_EN_PWM_MODE, value);

    return TMC2130_status;
}
// #########################################################################
/*
    enc_commutation (Special mode - do not use, leave 0)
    1:  Enable commutation by full step encoder
        (DCIN_CFG5 = ENC_A, DCEN_CFG4 = ENC_B)
*/
uint8_t TMC2130_set_enc_commutation(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_ENC_COMMUTATION, value);

    return TMC2130_status;
}
// #########################################################################
/*
    shaft
    1: Inverse motor direction
*/
uint8_t TMC2130_set_shaft(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_SHAFT, value);

    return TMC2130_status;
}
// #########################################################################
/*
    diag0_error
    1:  Enable DIAG0 active on driver errors:
        Over temperature (ot), short to GND (s2g),
        undervoltage chargepump (uv_cp)
        DIAG0 always shows the reset-status, i.e. is active low
        during reset condition.
*/
uint8_t TMC2130_set_diag0_error(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_DIAG0_ERROR, value);

    return TMC2130_status;
}
// #########################################################################
/*
    diag0_otpw
    1:  Enable DIAG0 active on driver over temperature
        prewarning (otpw)
*/
uint8_t TMC2130_set_diag0_otpw(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_DIAG0_OTPW, value);

    return TMC2130_status;
}
// #########################################################################
/*
    diag0_stall
    1:  Enable DIAG0 active on motor stall (set
        TCOOLTHRS before using this feature)
*/
uint8_t TMC2130_set_diag0_stall(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_DIAG0_STALL, value);

    return TMC2130_status;
}
// #########################################################################
/*
    diag1_stall
    1:  Enable DIAG1 active on motor stall (set
        TCOOLTHRS before using this feature)
*/
uint8_t TMC2130_set_diag1_stall(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_DIAG1_STALL, value);

    return TMC2130_status;
}
// #########################################################################
/*
    diag1_index
    1:  Enable DIAG1 active on index position (microstep
        look up table position 0)
    */
uint8_t TMC2130_set_diag1_index(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_DIAG1_INDEX, value);

    return TMC2130_status;
}
// #########################################################################
/*
    diag1_onstate
    1:  Enable DIAG1 active when chopper is on (for the
        coil which is in the second half of the fullstep)
*/
uint8_t TMC2130_set_diag1_onstate(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_DIAG1_ONSTATE, value);

    return TMC2130_status;
}
// #########################################################################
/*
    diag1_steps_skipped
    1: Enable output toggle when steps are skipped in
    dcStep mode (increment of LOST_STEPS). Do not
    enable in conjunction with other DIAG1 options.
*/
uint8_t TMC2130_set_diag1_steps_skipped(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_DIAG1_STEPS_SKIPPED, value);

    return TMC2130_status;
}
// #########################################################################
/*
    diag0_int_pushpull
    0: DIAG0 is open collector output (active low)
    1: Enable DIAG0 push pull output (active high)
*/
uint8_t TMC2130_set_diag0_int_pushpull(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_DIAG0_INT_PUSHPULL, value);

    return TMC2130_status;
}
// #########################################################################
/*
    diag1_pushpull
    0: DIAG1 is open collector output (active low)
    1: Enable DIAG1 push pull output (active high)
*/
uint8_t TMC2130_set_diag1_int_pushpull(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_DIAG1_INT_PUSHPULL, value);

    return TMC2130_status;
}
// #########################################################################
/*
    small_hysteresis
    0: Hysteresis for step frequency comparison is 1/16
    1: Hysteresis for step frequency comparison is 1/32
*/
uint8_t TMC2130_set_small_hysteresis(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_SMALL_HYSTERESIS, value);

    return TMC2130_status;
}
// #########################################################################
/*
    stop_enable
    0: Normal operation
    1: Emergency stop: DCIN stops the sequencer when
    tied high (no steps become executed by the
    sequencer, motor goes to standstill state).
*/
uint8_t TMC2130_set_stop_enable(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_STOP_ENABLE, value);

    return TMC2130_status;
}
// #########################################################################
/*
    direct_mode
    0: Normal operation
    1: Motor coil currents and polarity directly
    programmed via serial interface: Register XDIRECT
    (0x2D) specifies signed coil A current (bits 8..0)
    and coil B current (bits 24..16). In this mode, the
    current is scaled by IHOLD setting. Velocity based
    current regulation of stealthChop is not available
    in this mode. The automatic stealthChop current
    regulation will work only for low stepper motor
    velocities.
*/
uint8_t TMC2130_set_direct_mode(uint8_t value)
{
    TMC2130_set_GCONF(TMC_GCONF_DIRECT_MODE, value);

    return TMC2130_status;
}

/*****************************************************************************\
 * IHOLD_IRUN
\*****************************************************************************/
/*
    IHOLD
    Standstill current (0=1/32…31=32/32)
    In combination with stealthChop mode, setting
    IHOLD=0 allows to choose freewheeling or coil
    short circuit for motor stand still.

    IRUN
    Motor run current (0=1/32…31=32/32)
    Hint: Choose sense resistors in a way, that normal
    IRUN is 16 to 31 for best microstep performance.

    IHOLDDELAY
    Controls the number of clock cycles for motor
    power down after a motion as soon as standstill is
    detected (stst=1) and TPOWERDOWN has expired.
    The smooth transition avoids a motor jerk upon
    power down.
    0: instant power down
    1..15: Delay per current reduction step in multiple
    of 2^18 clocks
*/

uint8_t TMC2130_set_IHOLD_IRUN(uint8_t ihold, uint8_t irun, uint8_t iholddelay)
{
    uint32_t data;
    // adding ihold
    data = (( (uint32_t)(ihold)&TMC_IHOLD_MASK )<<TMC_IHOLD );
    // adding irun
    data |= (( (uint32_t)(irun)&TMC_IRUN_MASK )<<TMC_IRUN );
    // adding iholddelay
    data |= (( (uint32_t)(iholddelay)&TMC_IHOLDDELAY_MASK )<<TMC_IHOLDDELAY );

    // writing data
    TMC2130_write(TMC_REG_IHOLD_IRUN, data);

    return TMC2130_status;
}

/*****************************************************************************\
 * TPOWERDOWN
\*****************************************************************************/
/*
    TPOWERDOWN sets the delay time after stand still (stst) of the
    motor to motor current power down. Time range is about 0 to
    4 seconds.
    0…((2^8)-1) * 2^18 tCLK
*/
uint8_t TMC2130_set_TPOWERDOWN(uint8_t value)
{
    uint32_t data;

    data = value & TMC_TPOWERDOWN_MASK;
    TMC2130_write(TMC_REG_TPOWERDOWN, data);

    return TMC2130_status;
}

/*****************************************************************************\
 * TSTEP
\*****************************************************************************/
/*
    Actual measured time between two 1/256 microsteps derived
    from the step input frequency in units of 1/fCLK. Measured
    value is (2^20)-1 in case of overflow or stand still.
    All TSTEP related thresholds use a hysteresis of 1/16 of the
    compare value to compensate for jitter in the clock or the step
    frequency. The flag small_hysteresis modifies the hysteresis to
    a smaller value of 1/32.
    (Txxx*15/16)-1 or
    (Txxx*31/32)-1 is used as a second compare value for each
    comparison value.
    This means, that the lower switching velocity equals the
    calculated setting, but the upper switching velocity is higher as
    defined by the hysteresis setting.
    In dcStep mode TSTEP will not show the mean velocity of the
    motor, but the velocities for each microstep, which may not be
    stable and thus does not represent the real motor velocity in
    case it runs slower than the target velocity.
*/
uint32_t TMC2130_get_TSTEP(void)
{
    uint32_t data;

    TMC2130_read_REG(TMC_REG_TPOWERDOWN, &data);
    data &= TMC_TSTEP_MASK;

    return data;
}

/*****************************************************************************\
 * TPWMTHRS
\*****************************************************************************/
/*
    This is the upper velocity for stealthChop voltage PWM mode.
    TSTEP ≥ TPWMTHRS
    - stealthChop PWM mode is enabled, if configured
    - dcStep is disabled

    microstep velocity time reference t for velocities: TSTEP = fCLK / fSTEP
*/
uint8_t TMC2130_set_TPWMTHRS(uint32_t value)
{
    uint32_t data;

    data = value & TMC_TPWMTHRS_MASK;
    TMC2130_write(TMC_REG_TPOWERDOWN, data);

    return TMC2130_status;
}

/*****************************************************************************\
 * TCOOLTHRS
\*****************************************************************************/
/*
    This is the lower threshold velocity for switching on smart
    energy coolStep and stallGuard feature. (unsigned)
    Set this parameter to disable coolStep at low speeds, where it
    cannot work reliably. The stall detection and stallGuard output
    signal becomes enabled when exceeding this velocity. In nondcStep mode, it becomes disabled again once the velocity falls
    below this threshold.
    TCOOLTHRS ≥ TSTEP ≥ THIGH:
    - coolStep is enabled, if configured
    - stealthChop voltage PWM mode is disabled
    TCOOLTHRS ≥ TSTEP
    - Stop on stall and stall output signal is enabled, if
    configured

    microstep velocity time reference t for velocities: TSTEP = fCLK / fSTEP
*/
uint8_t TMC2130_set_TCOOLTHRS(uint32_t value)
{
    uint32_t data;

    data = value & TMC_TCOOLTHRS_MASK;
    TMC2130_write(TMC_REG_TCOOLTHRS, data);

    return TMC2130_status;
}

/*****************************************************************************\
 * THIGH
\*****************************************************************************/
/*
    This velocity setting allows velocity dependent switching into
    a different chopper mode and fullstepping to maximize torque.
    (unsigned)
    The stall detection feature becomes switched off for 2-3
    electrical periods whenever passing THIGH threshold to
    compensate for the effect of switching modes.
    TSTEP ≤ THIGH:
    - coolStep is disabled (motor runs with normal current
    scale)
    - stealthChop voltage PWM mode is disabled
    - If vhighchm is set, the chopper switches to chm=1
    with TFD=0 (constant off time with slow decay, only).
    - chopSync2 is switched off (SYNC=0)
    - If vhighfs is set, the motor operates in fullstep mode
    and the stall detection becomes switched over to
    dcStep stall detection.

    microstep velocity time reference t for velocities: TSTEP = fCLK / fSTEP
*/
uint8_t TMC2130_set_THIGH(uint32_t value)
{
    uint32_t data;

    data = value & TMC_THIGH_MASK;
    TMC2130_write(TMC_REG_THIGH, data);

    return TMC2130_status;
}

/*****************************************************************************\
 * XDIRECT
\*****************************************************************************/
/*
    direct_mode
    0: Normal operation
    1: Directly SPI driven motor current

    Direct mode operation:
    XDIRECT specifies Motor coil currents and
    polarity directly programmed via the serial
    interface. Use signed, two’s complement
    numbers.

    Coil A current (bits 8..0) (signed)
    Coil B current (bits 24..16) (signed)
    Range: +-248 for normal operation, up to +-255
    with stealthChop

    In this mode, the current is scaled by IHOLD
    setting. Velocity based current regulation of
    voltage PWM is not available in this mode. The
    automatic voltage PWM current regulation will
    work only for low stepper motor velocities.
    dcStep is not available in this mode. coolStep
    and stallGuard only can be used, when
    additionally supplying a STEP signal. This will
    also enable automatic current scaling
*/
uint8_t TMC2130_set_XDIRECTcoils(int16_t coil_a, int16_t coil_b)
{
    uint32_t data;

    data = 0x0;
    data |= ( coil_b & TMC_XDIRECT_COIL_B_MASK );
    data = data << TMC_XDIRECT_COIL_B;
    data |= ( coil_a & TMC_XDIRECT_COIL_A_MASK );
    data &= TMC_XDIRECT_MASK;

    TMC2130_write(TMC_REG_XDIRECT, data);

    return TMC2130_status;
}
// #########################################################################
uint8_t TMC2130_set_XDIRECT(uint32_t value)
{
    uint32_t data;

    data = value;// & TMC_XDIRECT_MASK;
    TMC2130_write(TMC_REG_XDIRECT, data);

    return TMC2130_status;
}
// #########################################################################
uint32_t TMC2130_get_XDIRECT()
{
    uint32_t data;

    TMC2130_read_REG(TMC_REG_XDIRECT, &data);
    data &= TMC_XDIRECT_MASK;

    return data;
}

/*****************************************************************************\
 * VDCMIN
\*****************************************************************************/
/*
    The automatic commutation dcStep becomes enabled by the
    external signal DCEN. VDCMIN is used as the minimum step
    velocity when the motor is heavily loaded.
    Hint: Also set DCCTRL parameters in order to operate dcStep.

    time reference t for VDCMIN: t = 2^24 / fCLK
*/
uint8_t TMC2130_set_VDCMIN(int32_t value)
{
    int32_t data;

    data = value & TMC_VDCMIN_MASK;
    TMC2130_write(TMC_REG_VDCMIN, data);

    return TMC2130_status;
}

/*****************************************************************************\
 * MSLUT
\*****************************************************************************/
/*
    Each bit gives the difference between entry x
    and entry x+1 when combined with the corresponding MSLUTSEL W bits:
    0: W= %00: -1
    %01: +0
    %10: +1
    %11: +2
    1: W= %00: +0
    %01: +1
    %10: +2
    %11: +3
    This is the differential coding for the first
    quarter of a wave. Start values for CUR_A and
    CUR_B are stored for MSCNT position 0 in
    START_SIN and START_SIN90.
    ofs31, ofs30, …, ofs01, ofs00
    …
    ofs255, ofs254, …, ofs225, ofs224
*/
uint8_t TMC2130_set_MSLUT0(uint32_t value)
{
    TMC2130_write(TMC_REG_MSLUT0, value);

    return TMC2130_status;
}
// #########################################################################
uint8_t TMC2130_set_MSLUT1(uint32_t value)
{
    TMC2130_write(TMC_REG_MSLUT1, value);

    return TMC2130_status;
}
// #########################################################################
uint8_t TMC2130_set_MSLUT2(uint32_t value)
{
    TMC2130_write(TMC_REG_MSLUT2, value);

    return TMC2130_status;
}
// #########################################################################
uint8_t TMC2130_set_MSLUT3(uint32_t value)
{
    TMC2130_write(TMC_REG_MSLUT3, value);

    return TMC2130_status;
}
// #########################################################################
uint8_t TMC2130_set_MSLUT4(uint32_t value)
{
    TMC2130_write(TMC_REG_MSLUT4, value);

    return TMC2130_status;
}
// #########################################################################
uint8_t TMC2130_set_MSLUT5(uint32_t value)
{
    TMC2130_write(TMC_REG_MSLUT5, value);

    return TMC2130_status;
}
// #########################################################################
uint8_t TMC2130_set_MSLUT6(uint32_t value)
{
    TMC2130_write(TMC_REG_MSLUT6, value);

    return TMC2130_status;
}
// #########################################################################
uint8_t TMC2130_set_MSLUT7(uint32_t value)
{
    TMC2130_write(TMC_REG_MSLUT7, value);

    return TMC2130_status;
}

/*****************************************************************************\
 * MSLUTSEL
\*****************************************************************************/
/*
    This register defines four segments within
    each quarter MSLUT wave. Four 2 bit entries
    determine the meaning of a 0 and a 1 bit in
    the corresponding segment of MSLUT.
*/
uint8_t TMC2130_set_MSLUTSEL(uint32_t value)
{
    TMC2130_write(TMC_REG_MSLUTSEL, value);

    return TMC2130_status;
}

/*****************************************************************************\
 * MSLUTSTART
\*****************************************************************************/
/*
    bit 7… 0: START_SIN
    bit 23… 16: START_SIN90
    START_SIN gives the absolute current at
    microstep table entry 0.
    START_SIN90 gives the absolute current for
    microstep table entry at positions 256.
    Start values are transferred to the microstep
    registers CUR_A and CUR_B, whenever the
    reference position MSCNT=0 is passed.
*/
uint8_t TMC2130_set_MSLUTSTART(uint8_t start_sin, uint8_t start_sin90)
{
    uint32_t data;
    data = ( (uint32_t)(start_sin90) & TMC_MSLUTSTART_START_SIN90_MASK );
    data = data<<TMC_MSLUTSTART_START_SIN90;
    data |= ( (uint32_t)(start_sin) & TMC_MSLUTSTART_START_SIN_MASK );
    data &= TMC_MSLUTSTART_MASK;

    TMC2130_write(TMC_REG_MSLUTSTART, data);

    return TMC2130_status;
}

/*****************************************************************************\
 * MSCNT
\*****************************************************************************/
/*
    Microstep counter. Indicates actual position
    in the microstep table for CUR_A. CUR_B uses
    an offset of 256 (2 phase motor).
    Hint: Move to a position where MSCNT is
    zero before re-initializing MSLUTSTART or
    MSLUT and MSLUTSEL
*/
uint16_t TMC2130_get_MSCNT()
{
    uint32_t data;

    TMC2130_read_REG(TMC_REG_MSCNT, &data);
    data &= TMC_MSCNT_MASK;
    data = (uint16_t)data;

    return data;
}

/*****************************************************************************\
 * MSCURACT
\*****************************************************************************/
/*
    bit 8… 0:   CUR_A (signed):
                Actual microstep current for
                motor phase A as read from
                MSLUT (not scaled by current)

    bit 24… 16: CUR_B (signed):
                Actual microstep current for
                motor phase B as read from
                MSLUT (not scaled by current)
*/
int32_t TMC2130_get_MSCURACT()
{
    uint32_t data;

    TMC2130_read_REG(TMC_REG_MSCURACT, &data);
    data &= TMC_MSCURACT_MASK;

    return data;
}

/*****************************************************************************\
 * CHOPCONF
\*****************************************************************************/
/*
    0: Short to GND protection is on
    1: Short to GND protection is disabled
*/
uint8_t TMC2130_set_diss2g(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_DISS2G, value);

    return TMC2130_status;
}
// #########################################################################
/*
    1:  Enable step impulse at each step edge to reduce step
        frequency requirement.
*/
uint8_t TMC2130_set_dedge(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_DEDGE, value);

    return TMC2130_status;
}
// #########################################################################
/*
    1:  The actual microstep resolution (MRES) becomes
        extrapolated to 256 microsteps for smoothest motor
        operation.
*/
uint8_t TMC2130_set_intpol(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_INTPOL, value);

    return TMC2130_status;
}
// #########################################################################
// setting the microstep resolution
/*
    %0000:Native 256 microstep setting
    %0001 … %1000:
    128, 64, 32, 16, 8, 4, 2, FULLSTEP
    Reduced microstep resolution for STEP/DIR operation.
    The resolution gives the number of microstep entries per
    sine quarter wave.
    The driver automatically uses microstep positions which
    result in a symmetrical wave, when choosing a lower
    microstep resolution.
    step width=2^MRES [microsteps]
*/
uint8_t TMC2130_set_mres(uint16_t value)
{
    uint8_t data = 0;

    switch(value)
    {
        case 1:
                data = 8;
                break;
        case 2:
                data = 7;
                break;
        case 4:
                data = 6;
                break;
        case 8:
                data = 5;
                break;
        case 16:
                data = 4;
                break;
        case 32:
                data = 3;
                break;
        case 64:
                data = 2;
                break;
        case 128:
                data = 1;
                break;
        case 256:
        default:
                data = 0; //native 256 microsteps
                break;
    }

    TMC2130_set_CHOPCONF(TMC_CHOPCONF_MRES, data);

    return TMC2130_status;
}
// #########################################################################
/*
    This register allows synchronization of the chopper for
    both phases of a two phase motor in order to avoid the
    occurrence of a beat, especially at low motor velocities. It
    is automatically switched off above VHIGH.
    %0000: Chopper sync function chopSync off
    %0001 … %1111:
    Synchronization with fSYNC = fCLK/(sync*64)
    Hint: Set TOFF to a low value, so that the chopper cycle is
    ended, before the next sync clock pulse occurs. Set for the
    double desired chopper frequency for chm=0, for the
    desired base chopper frequency for chm=1.
*/
uint8_t TMC2130_set_sync(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_SYNC, value);

    return TMC2130_status;
}
// #########################################################################
/*
    This bit enables switching to chm=1 and fd=0, when VHIGH
    is exceeded. This way, a higher velocity can be achieved.
    Can be combined with vhighfs=1. If set, the TOFF setting
    automatically becomes doubled during high velocity
    operation in order to avoid doubling of the chopper
    frequency.
*/
uint8_t TMC2130_set_vhighchm(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_VHIGHCHM, value);

    return TMC2130_status;
}
// #########################################################################
/*
    This bit enables switching to fullstep, when VHIGH is
    exceeded. Switching takes place only at 45° position.
    The fullstep target current uses the current value from
    the microstep table at the 45° position.
*/
uint8_t TMC2130_set_vhighfs(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_VHIGHFS, value);

    return TMC2130_status;
}
// #########################################################################
/*
    0: Low sensitivity, high sense resistor voltage
    1: High sensitivity, low sense resistor voltage
*/
uint8_t TMC2130_set_vsense(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_VSENSE, value);

    return TMC2130_status;
}

uint8_t TMC2130_get_vsense(void)
{
    uint32_t value;
    TMC2130_read_REG(TMC_REG_CHOPCONF, &value);
    #ifdef DEBUG_TMC2130
    //print debug data    
    #endif
    if (value & (1<<TMC_CHOPCONF_VSENSE)) return 1;
    else return 0;
}

// #########################################################################
/*
    %00 … %11:
    Set comparator blank time to 16, 24, 36 or 54 clocks
    Hint: %01 or %10 is recommended for most applications
*/
uint8_t TMC2130_set_tbl(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_TBL, value);

    return TMC2130_status;
}
// #########################################################################
/*
    0   Standard mode (spreadCycle)
    1   Constant off time with fast decay time.
        Fast decay time is also terminated when the
        negative nominal current is reached. Fast decay is
        after on time.
*/
uint8_t TMC2130_set_chm(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_CHM, value);

    return TMC2130_status;
}
// #########################################################################
/*
    0   Chopper off time is fixed as set by TOFF
    1   Random mode, TOFF is random modulated by
        dNCLK= -12 … +3 clocks
*/
uint8_t TMC2130_set_rndtf(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_RNDTF, value);

    return TMC2130_status;
}
// #########################################################################
/*
    hm=1:
    disfdcc=1 disables current comparator usage for termination of the fast decay cycle
*/
uint8_t TMC2130_set_disfdcc(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_DISFDCC, value);

    return TMC2130_status;
}
// #########################################################################
/*
    chm=1:
    MSB of fast decay time setting TFD
*/
uint8_t TMC2130_set_fd(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_FD, value);

    return TMC2130_status;
}
// #########################################################################
/*
    chm=0 %0000 … %1111:
                    Hysteresis is -3, -2, -1, 0, 1, …, 12
                    (1/512 of this setting adds to current setting)
                    This is the hysteresis value which becomes
                    used for the hysteresis chopper.
    chm=1 %0000 … %1111:
                    Offset is -3, -2, -1, 0, 1, …, 12
                    This is the sine wave offset and 1/512 of the
                    value becomes added to the absolute value
                    of each sine wave entry
*/
uint8_t TMC2130_set_hend(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_HEND, value);

    return TMC2130_status;
}
// #########################################################################
/*
    chm=0 %000 … %111:
                    Add 1, 2, …, 8 to hysteresis low value HEND
                    (1/512 of this setting adds to current setting)
                    Attention: Effective HEND+HSTRT ≤ 16.
                    Hint: Hysteresis decrement is done each 16
                    clocks
    chm=1 Fast decay time setting (MSB: fd3):
                    %0000 … %1111:
                    Fast decay time setting TFD with
                    NCLK= 32*TFD (%0000: slow decay only)
*/
uint8_t TMC2130_set_hstrt(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_HSTRT, value);

    return TMC2130_status;
}
// #########################################################################
/*
    Off time setting controls duration of slow decay phase
    NCLK= 12 + 32*TOFF
    %0000: Driver disable, all bridges off
    %0001: 1 – use only with TBL ≥ 2
    %0010 … %1111: 2 … 15
*/
uint8_t TMC2130_set_toff(uint8_t value)
{
    TMC2130_set_CHOPCONF(TMC_CHOPCONF_TOFF, value);

    return TMC2130_status;
}

/*****************************************************************************\
 * COOLCONF
\*****************************************************************************/
/*
    coolStep smart current control register
    and stallGuard2 configuration
*/
// alter coolconf
uint8_t TMC2130_alter_COOLCONF(uint32_t data, uint32_t mask)
{
    TMC2130_coolconf = ( TMC2130_coolconf & ~mask ) | ( data & mask );
    TMC2130_write( TMC_REG_COOLCONF, TMC2130_coolconf );

    return TMC2130_status;
}
// #########################################################################
// set coolconf
uint8_t TMC2130_set_COOLCONF(uint8_t position, uint8_t value)
{

    TMC2130_alter_COOLCONF( ((uint32_t)(value))<<position, ((uint32_t) TMC_COOLCONF_MASKS[position])<<position);

    return TMC2130_status;
}
// #########################################################################
/*
    0   Standard mode, high time resolution for
        stallGuard2
    1   Filtered mode, stallGuard2 signal updated for each
        four fullsteps (resp. six fullsteps for 3 phase motor)
        only to compensate for motor pole tolerances
*/
uint8_t TMC2130_set_sfilt(uint8_t value)
{
    TMC2130_set_COOLCONF(TMC_COOLCONF_SFILT, value);

    return TMC2130_status;
}
// #########################################################################


/*****************************************************************************\
 * Function:    TMC2130_set_sgt
 * Input:       value
 * Returns:     TMC2130 status byte
 * Description: 
 *    
    This signed value controls stallGuard2 level for stall
    output and sets the optimum measurement range for
    readout. A lower value gives a higher sensitivity. Zero is
    the starting value working with most motors.
    -64 to +63:     A higher value makes stallGuard2 less
                    sensitive and requires more torque to
                    indicate a stall
\*****************************************************************************/
uint8_t TMC2130_set_sgt(uint8_t value)
{
    TMC2130_set_COOLCONF(TMC_COOLCONF_SGT, value);

    return TMC2130_status;
}

/*****************************************************************************\
 * Function:    TMC2130_get_sg
 * Input:       none
 * Returns:     value in SG_RESULT
 * Description: 
 *  Mechanical load measurement:

    The stallGuard2 result gives a means to measure mechanical
    motor load. A higher value means lower mechanical load. A
    value of 0 signals highest load. With optimum SGT setting,
    this is an indicator for a motor stall. The stall detection
    compares SG_RESULT to 0 in order to detect a stall. SG_RESULT
    is used as a base for coolStep operation, by comparing it to a
    programmable upper and a lower limit. It is not applicable in
    stealthChop mode.
    SG_RESULT is ALSO applicable when dcStep is active.
    stallGuard2 works best with microstep operation.

    Temperature measurement:
    In standstill, no stallGuard2 result can be obtained. SG_RESULT
    shows the chopper on-time for motor coil A instead. If the
    motor is moved to a determined microstep position at a
    certain current setting, a comparison of the chopper on-time
    can help to get a rough estimation of motor temperature. As
    the motor heats up, its coil resistance rises and the chopper
    on-time increases.
\*****************************************************************************/
uint32_t TMC2130_get_sg(void)
{
    uint32_t drvStat;
    TMC2130_read_REG(TMC_REG_DRV_STATUS,&drvStat);
    return (drvStat);
}

// #########################################################################
/*
    0: 1/2 of current setting (IRUN)
    1: 1/4 of current setting (IRUN)
*/
uint8_t TMC2130_set_seimin(uint8_t value)
{
    TMC2130_set_COOLCONF(TMC_COOLCONF_SEIMIN, value);

    return TMC2130_status;
}
// #########################################################################
/*
    %00: For each 32 stallGuard2 values decrease by one
    %01: For each 8 stallGuard2 values decrease by one
    %10: For each 2 stallGuard2 values decrease by one
    %11: For each stallGuard2 value decrease by one
*/
uint8_t TMC2130_set_sedn(uint8_t value)
{
    TMC2130_set_COOLCONF(TMC_COOLCONF_SEDN, value);

    return TMC2130_status;
}
// #########################################################################
/*
    If the stallGuard2 result is equal to or above
    (SEMIN+SEMAX+1)*32, the motor current becomes
    decreased to save energy.
    %0000 … %1111: 0 … 15
*/
uint8_t TMC2130_set_semax(uint8_t value)
{
    TMC2130_set_COOLCONF(TMC_COOLCONF_SEMAX, value);

    return TMC2130_status;
}
// #########################################################################
/*
    Current increment steps per measured stallGuard2 value
    %00 … %11: 1, 2, 4, 8
*/
uint8_t TMC2130_set_seup(uint8_t value)
{
    TMC2130_set_COOLCONF(TMC_COOLCONF_SEUP, value);

    return TMC2130_status;
}
// #########################################################################
/*
    If the stallGuard2 result falls below SEMIN*32, the motor
    current becomes increased to reduce motor load angle.
    %0000: smart current control coolStep off
    %0001 … %1111: 1 … 15
*/
uint8_t TMC2130_set_semin(uint8_t value)
{
    TMC2130_set_COOLCONF(TMC_COOLCONF_SEMIN, value);

    return TMC2130_status;
}
/*****************************************************************************\
 * DCCTRL

    The automatic commutation dcStep becomes enabled by the
    external signal DCEN. VDCMIN is used as the minimum step
    velocity when the motor is heavily loaded.
    Hint: Also set DCCTRL parameters in order to operate dcStep.
\*****************************************************************************/
uint8_t TMC2130_set_DCCTRL(uint16_t dc_time, uint16_t dc_sg)
{
  uint32_t data;
  data = ( (uint32_t)(dc_sg) & TMC_DCCTRL_DC_SG_MASK );
  data = data<<TMC_DCCTRL_DC_SG;
  data |= ( (uint32_t)(dc_time) & TMC_DCCTRL_DC_TIME_MASK );
  data &= TMC_DCCTRL_MASK;

  TMC2130_write(TMC_REG_DCCTRL, data);

  return TMC2130_status;
}

/*****************************************************************************\
 * PWMCONF
\*****************************************************************************/
// alter pwmconf
uint8_t TMC2130_alter_PWMCONF(uint32_t data, uint32_t mask)
{
    TMC2130_pwmconf = ( TMC2130_pwmconf & ~mask ) | ( data & mask );
    TMC2130_write( TMC_REG_PWMCONF, TMC2130_pwmconf );

    return TMC2130_status;
}
// #########################################################################
// set pwmconf
uint8_t TMC2130_set_PWMCONF(uint8_t position, uint8_t value)
{
    TMC2130_alter_PWMCONF( (uint32_t)(value)<<position,((uint32_t) TMC_PWMCONF_MASKS[position])<<position );

    return TMC2130_status;
}
// #########################################################################
/*
    Stand still option when motor current setting is zero
    (I_HOLD=0).
    %00: Normal operation
    %01: Freewheeling
    %10: Coil shorted using LS drivers
    %11: Coil shorted using HS drivers
*/
uint8_t TMC2130_set_freewheel(uint8_t value)
{
    TMC2130_set_PWMCONF(TMC_PWMCONF_FREEWHEEL, value);

    return TMC2130_status;
}
// #########################################################################
/*
    0   The PWM value may change within each PWM cycle
        (standard mode)
    1   A symmetric PWM cycle is enforced
*/
uint8_t TMC2130_set_pwm_symmetric(uint8_t value)
{
    TMC2130_set_PWMCONF(TMC_PWMCONF_PWM_SYMMETRIC, value);

    return TMC2130_status;
}
// #########################################################################
/*
    0   User defined PWM amplitude. The current settings
        have no influence.
    1   Enable automatic current control
        Attention: When using a user defined sine wave
        table, the amplitude of this sine wave table should
        not be less than 244. Best results are obtained with
        247 to 252 as peak values.
*/
uint8_t TMC2130_set_pwm_autoscale(uint8_t value)
{
    TMC2130_set_PWMCONF(TMC_PWMCONF_PWM_AUTOSCALE, value);

    return TMC2130_status;
}
// #########################################################################
/*
    %00: fPWM=2/1024 fCLK
    %01: fPWM=2/683 fCLK
    %10: fPWM=2/512 fCLK
    %11: fPWM=2/410 fCLK
*/
uint8_t TMC2130_set_pwm_freq(uint8_t value)
{
    TMC2130_set_PWMCONF(TMC_PWMCONF_PWM_FREQ, value);

    return TMC2130_status;
}
// #########################################################################
/*
    pwm_autoscale=0
                Velocity dependent gradient for PWM
                amplitude:
                PWM_GRAD * 256 / TSTEP
                is added to PWM_AMPL
    pwm_autoscale=1
                User defined maximum PWM amplitude
                change per half wave (1 to 15)
*/
uint8_t TMC2130_set_PWM_GRAD(uint8_t value)
{
    TMC2130_set_PWMCONF(TMC_PWMCONF_PWM_GRAD, value);

    return TMC2130_status;
}
// #########################################################################
/*
    pwm_autoscale=0
            User defined PWM amplitude offset (0-255)
            The resulting amplitude (limited to 0…255)
            is:
            PWM_AMPL + PWM_GRAD * 256 / TSTEP
    pwm_autoscale=1
            User defined maximum PWM amplitude
            when switching back from current chopper
            mode to voltage PWM mode (switch over
            velocity defined by TPWMTHRS). Do not set
            too low values, as the regulation cannot
            measure the current when the actual PWM
            value goes below a setting specific value.
            Settings above 0x40 recommended.
*/
uint8_t TMC2130_set_PWM_AMPL(uint8_t value)
{
    TMC2130_set_PWMCONF(TMC_PWMCONF_PWM_AMPL, value);

    return TMC2130_status;
}

/*****************************************************************************\
 * ENCM_CTRL
\*****************************************************************************/
/*
    Encoder mode configuration for a special
    mode (enc_commutation), not for normal
    use.
    Bit 0:  inv: Invert encoder inputs
    Bit 1:  maxspeed: Ignore Step input. If
            set, the hold current IHOLD
            determines the motor current,
            unless a step source is activated.
            The direction in this mode is determined by
            the shaft bit in GCONF or by the inv bit.
*/
uint8_t TMC2130_set_ENCM_CTRL(uint8_t value)
{
    uint8_t data;

    data = value & TMC_ENCM_CTRL_MASK;
    TMC2130_write(TMC_REG_ENCM_CTRL, data);

    return TMC2130_status;
}
/*****************************************************************************\
 * STATUS
\*****************************************************************************/

// check the reset status
uint8_t TMC2130_isReset(void)
{
    return TMC2130_status&TMC_SPISTATUS_RESET_MASK ? 1 : 0;
}

// check the error status
uint8_t TMC2130_isError(void)
{
    return TMC2130_status&TMC_SPISTATUS_ERROR_MASK ? 1 : 0;
}

// check the stallguard status
uint8_t TMC2130_isStallguard(void)
{
    return TMC2130_status&TMC_SPISTATUS_STALLGUARD_MASK ? 1 : 0;
}

// check the standstill status
uint8_t TMC2130_isStandstill(void)
{
    return TMC2130_status&TMC_SPISTATUS_STANDSTILL_MASK ? 1 : 0;
}


/*****************************************************************************\
 * Function:    TMC2130_checkErrors
 * Input:       none
 * Returns:     TMC2130 error report
 * Description: 
 *      Checks if there has been any error signaled on the DIAG01 outputs
\*****************************************************************************/
uint8_t TMC2130_checkDiagErrors(void)
{    
#ifdef DEBUG_TMC2130    
    if (TMC2130_isReset())   printf("TMC2130 was RESET!\r\n");
    if (TMC2130_isError())   printf("TMC2130 Driver Error detected\r\n");
    if (TMC2130_isStallguard()) printf("TMC2130 STALLGUARD detected!\r\n");
    if (TMC2130_isStandstill()) printf("TMC2130 STANDSTILL detected!\r\n");
    if (TMC2130_diagError & 0x01) printf("TMC2130 diag0 error detected!\r\n");
    if (TMC2130_diagError & 0x02) printf("TMC2130 diag1 error (skipped steps) detected!\r\n");
#endif       
    uint8_t diagErrors = TMC2130_diagError;
    TMC2130_diagError = 0x00;           //clear error flags
    return diagErrors;
}

/*****************************************************************************\
 * Function:    TMC2130_clearErrors
 * Input:       none
 * Returns:     status byte
 * Description: 
 *     Clears the occured errors by 
 *      - reading the DRV_STATUS register
 *      - clearing the error flags by reading the GSTAT register
 *      - resetting toff
\*****************************************************************************/
uint8_t TMC2130_clearErrors(void)
{
    #ifdef DEBUG_TMC2130
    printf("Clearing errors...\r\n");    
    #endif
    uint32_t temp32;
    TMC2130_read_REG(TMC_REG_DRV_STATUS,&temp32);
    TMC2130_read_REG(TMC_REG_GSTAT,&temp32); 
    TMC2130_set_toff(0);
    TMC2130_set_toff(TMC2130_DEFAULT_TOFF);   
    
    TMC2130_diagError = TMC2130_diag01_Read()&0x03;
    return ((TMC2130_read_STAT()&0x0F) | (TMC2130_diagError<<4));
}

void TMC2130_printDRV_STATUS(void)
{
    uint32_t temp32;
    TMC2130_read_REG(TMC_REG_DRV_STATUS,&temp32);
    printf("\r\nstst=%d\r\n",temp32&(1<<31) ? 1:0);
    printf("olb=%d\r\n",temp32&(1<<30) ? 1:0);
    printf("ola=%d\r\n",temp32&(1<<29) ? 1:0);
    printf("s2gb=%d\r\n",temp32&(1<<28) ? 1:0);
    printf("s2ga=%d\r\n",temp32&(1<<27) ? 1:0);
    printf("otpw=%d\r\n",temp32&(1<<26) ? 1:0);
    printf("ot=%d\r\n",temp32&(1<<25) ? 1:0);
    printf("StGrd=%d\r\n",temp32&(1<<24) ? 1:0);

    printf("CS=%d\r\n",(temp32&(0x1F0000UL))>>16);
    printf("fsact=%d\r\n",temp32&(1<<15) ? 1:0);
    printf("SG=%d\r\n",(temp32&(0x01FUL)));
}

uint32_t  TMC2130_getDRV_STATUS(void)
{
    uint32_t temp32;
    TMC2130_read_REG(TMC_REG_DRV_STATUS,&temp32);
    return temp32;
}


/* [] END OF FILE */
