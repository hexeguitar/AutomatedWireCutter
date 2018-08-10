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

#ifndef TMC2130_H
#define TMC2130_H

    #include <cytypes.h>
    #include <stdbool.h>
    
    #define TMC2130_DEFAULT_TOFF            4
    #define TMC2130_DEFAULT_ISCALE_ANALOG   0
    #define TMC2130_ROT_FWD                 1
    #define TMC2130_ROT_BWD                 0
    
    
    
    typedef struct
    {
        uint8_t slaveSelect;            //spi chip select line
        uint8_t status;
        uint8_t coolconf;
        uint8_t pwmconf;
        uint8_t Rsense_mOhn;
        uint32_t Irun_mA;
        uint32_t Ihold_mA;
        void (*SPI_CS_Callback) (uint8_t); //callback to control the SPI CS line
        
    }TMC2130_t;
    
    
    void TMC2130_init(TMC2130_t *device, uint16_t microsteps, uint16_t iRun_mA, uint16_t mult_percent, uint16_t Rsense_mOhm, uint8_t stealthChop );
    int8_t TMC2130_set_device(TMC2130_t *device_ptr);
    void TMC2130_regSPI_CS_Callback(TMC2130_t *device, void(*callback)(uint8_t));
    
    uint8_t TMC2130_read_STAT(void);
    uint8_t TMC2130_read_REG(uint8_t address , uint32_t *data);
    uint8_t TMC2130_write(uint8_t cmd, uint32_t data);
    uint8_t TMC2130_alter_REG(uint8_t address, uint32_t data, uint32_t mask);
    uint8_t TMC2130_checkDiagErrors(void);
    uint8_t TMC2130_clearErrors(void);

    uint8_t TMC2130_set_GCONF(uint8_t bit, uint8_t value);
    uint8_t TMC2130_set_I_scale_analog(uint8_t value);
    uint8_t TMC2130_set_internal_Rsense(uint8_t value);
    uint8_t TMC2130_set_en_pwm_mode(uint8_t value);
    uint8_t TMC2130_set_enc_commutation(uint8_t value);
    uint8_t TMC2130_set_shaft(uint8_t value);
    uint8_t TMC2130_set_diag0_error(uint8_t value);
    uint8_t TMC2130_set_diag0_otpw(uint8_t value);
    uint8_t TMC2130_set_diag0_stall(uint8_t value);
    uint8_t TMC2130_set_diag1_stall(uint8_t value);
    uint8_t TMC2130_set_diag1_index(uint8_t value);
    uint8_t TMC2130_set_diag1_onstate(uint8_t value);
    uint8_t TMC2130_set_diag1_steps_skipped(uint8_t value);
    uint8_t TMC2130_set_diag0_int_pushpull(uint8_t value);
    uint8_t TMC2130_set_diag1_int_pushpull(uint8_t value);
    uint8_t TMC2130_set_small_hysteresis(uint8_t value);
    uint8_t TMC2130_set_stop_enable(uint8_t value);
    uint8_t TMC2130_set_direct_mode(uint8_t value);

    void TMC2130_set_rmscurrent(uint64_t Irun_mA, uint16 multiplier_percent, uint64_t RS_mOhm); 
    uint8_t TMC2130_set_IHOLD_IRUN(uint8_t ihold, uint8_t irun, uint8_t iholddelay);
    uint8_t TMC2130_set_TPOWERDOWN(uint8_t value);
    uint32_t TMC2130_get_TSTEP(void);
    uint8_t TMC2130_set_TPWMTHRS(uint32_t value);
    uint8_t TMC2130_set_TCOOLTHRS(uint32_t value);
    uint8_t TMC2130_set_THIGH(uint32_t value);
    uint8_t TMC2130_set_XDIRECT(uint32_t value);
    uint8_t TMC2130_set_XDIRECT_coils(int16_t coil_a, int16_t coil_b);
    uint32_t TMC2130_get_XDIRECT(void);
    uint8_t TMC2130_set_VDCMIN(int32_t value);

    uint8_t TMC2130_set_MSLUT0(uint32_t value);
    uint8_t TMC2130_set_MSLUT1(uint32_t value);
    uint8_t TMC2130_set_MSLUT2(uint32_t value);
    uint8_t TMC2130_set_MSLUT3(uint32_t value);
    uint8_t TMC2130_set_MSLUT4(uint32_t value);
    uint8_t TMC2130_set_MSLUT5(uint32_t value);
    uint8_t TMC2130_set_MSLUT6(uint32_t value);
    uint8_t TMC2130_set_MSLUT7(uint32_t value);
    uint8_t TMC2130_set_MSLUTSEL(uint32_t value);
    uint8_t TMC2130_set_MSLUTSTART(uint8_t start_sin, uint8_t start_sin90);
    uint16_t TMC2130_get_MSCNT(void);
    int32_t TMC2130_get_MSCURACT(void);

    uint8_t TMC2130_set_CHOPCONF(uint8_t position, uint8_t value);
    uint8_t TMC2130_set_dedge(uint8_t value);
    uint8_t TMC2130_set_diss2g(uint8_t value);
    uint8_t TMC2130_set_intpol(uint8_t value);
    uint8_t TMC2130_set_mres(uint16_t value);
    uint8_t TMC2130_set_sync(uint8_t value);
    uint8_t TMC2130_set_vhighchm(uint8_t value);
    uint8_t TMC2130_set_vhighfs(uint8_t value);
    uint8_t TMC2130_set_vsense(uint8_t value);
    uint8_t TMC2130_get_vsense(void);
    uint8_t TMC2130_set_tbl(uint8_t value);
    uint8_t TMC2130_set_chm(uint8_t value);
    uint8_t TMC2130_set_rndtf(uint8_t value);
    uint8_t TMC2130_set_disfdcc(uint8_t value);
    uint8_t TMC2130_set_fd(uint8_t value);
    uint8_t TMC2130_set_hend(uint8_t value);
    uint8_t TMC2130_set_hstrt(uint8_t value);
    uint8_t TMC2130_set_toff(uint8_t value);

    uint8_t TMC2130_alter_COOLCONF(uint32_t data, uint32_t mask);
    uint8_t TMC2130_set_COOLCONF(uint8_t position, uint8_t value);
    uint8_t TMC2130_set_sfilt(uint8_t value);
    uint8_t TMC2130_set_sgt(uint8_t value);
    uint8_t TMC2130_set_seimin(uint8_t value);
    uint8_t TMC2130_set_sedn(uint8_t value);
    uint8_t TMC2130_set_semax(uint8_t value);
    uint8_t TMC2130_set_seup(uint8_t value);
    uint8_t TMC2130_set_semin(uint8_t value);
    uint32_t TMC2130_get_sg(void);
    
    
    uint8_t TMC2130_set_DCCTRL(uint16_t dc_time, uint16_t dc_sg);

    uint8_t TMC2130_alter_PWMCONF(uint32_t data, uint32_t mask);
    uint8_t TMC2130_set_PWMCONF(uint8_t position, uint8_t value);
    uint8_t TMC2130_set_freewheel(uint8_t value);
    uint8_t TMC2130_set_pwm_symmetric(uint8_t value);
    uint8_t TMC2130_set_pwm_autoscale(uint8_t value);
    uint8_t TMC2130_set_pwm_freq(uint8_t value);
    uint8_t TMC2130_set_PWM_GRAD(uint8_t value);
    uint8_t TMC2130_set_PWM_AMPL(uint8_t value);

    uint8_t TMC2130_set_ENCM_CTRL(uint8_t value);

    uint8_t TMC2130_isReset(void);
    uint8_t TMC2130_isError(void);
    uint8_t TMC2130_isStallguard(void);
    uint8_t TMC2130_isStandstill(void);
    void TMC2130_printDRV_STATUS(void);
    uint32_t  TMC2130_getDRV_STATUS(void);


//    uint32_t _coolconf;
//    uint32_t _pwmconf;
//    uint8_t _csPin;

    
    


#endif

/* [] END OF FILE */
