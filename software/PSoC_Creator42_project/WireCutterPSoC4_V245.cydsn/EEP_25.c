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
#include <stdbool.h>
#include <project.h>

#include "EEP_25.h"
#include "config.h"


static inline uint32_t SPI_getCurrentSlaveSelect(void);



static inline uint32_t SPI_getCurrentSlaveSelect(void)
{
    return ((SPI_SPI_CTRL_REG & SPI_SPI_CTRL_SLAVE_SELECT_MASK) >> SPI_SPI_CTRL_SLAVE_SELECT_POS);
}

/*****************************************************************************\
 * Function:    EEP25_readStatus
 * Input:       none
 * Returns:     status byte
 * Description: 
 *     Returns the value of the status byte
\*****************************************************************************/
uint8_t EEP25_readStatus(void)
{
    uint8_t statByte;
    
    while ((SPI_SpiUartGetTxBufferSize() > 0) && (SPI_SpiIsBusBusy()!=0)) {};
    SPI_SpiSetActiveSlaveSelect(NOTUSED_SPI_SLAVE_SELECT);  //point the hardware to a non used slave select line
    
    uint8_t databuf[2] = {EEP25_READ_STATUS_REG, 0x00};
    
    EEP_CS_Write(0);
    SPI_SpiUartPutArray(databuf, 2);
    while(SPI_SpiIsBusBusy()!=0){};
    EEP_CS_Write(1);
    SPI_SpiSetActiveSlaveSelect(DEFAULT_SPI_SLAVE_SELECT);  //point the hardware to a default slave select line
    
    statByte = SPI_SpiUartReadRxData();                     //read 1st byte - dummy
    
    return SPI_SpiUartReadRxData();                         // return 2nd byte = status register
}   

/*****************************************************************************\
 * Function:    EEP25_readByte
 * Input:       address
 * Returns:     byte value @ address
 * Description: 
 *     Reads a single byte from the memory
\*****************************************************************************/
uint8_t EEP25_readByte(uint32_t address)
{
    uint8 byteValue;

    while ((SPI_SpiUartGetTxBufferSize() > 0) && (SPI_SpiIsBusBusy()!=0)) {};
    SPI_SpiSetActiveSlaveSelect(NOTUSED_SPI_SLAVE_SELECT);  //point the hardware to a non used slave select line
    
    uint8_t databuf[4];
    databuf[0] = EEP25_READ;
    databuf[1] = (uint8_t)(address>>8);
    databuf[2] = (uint8_t) address;
    databuf[3] = 0x00;
    
    EEP_CS_Write(0);
    SPI_SpiUartPutArray(databuf, 4);
    while(SPI_SpiIsBusBusy()!=0){};
    EEP_CS_Write(1);
    
    byteValue = SPI_SpiUartReadRxData();    // 0
    byteValue = SPI_SpiUartReadRxData();    // 1
    byteValue = SPI_SpiUartReadRxData();    // 2
    byteValue = SPI_SpiUartReadRxData();    // 3    
    
    SPI_SpiSetActiveSlaveSelect(DEFAULT_SPI_SLAVE_SELECT);  //point the hardware to a default slave select line
    
    return byteValue;
}

/*****************************************************************************\
 * Function:    EEP25_readArray
 * Input:       address, pointer to store the data, array length in bytes
 * Returns:     bool success/fail flag
 * Description: 
 *     Reads a n bytes from the memory
\*****************************************************************************/
bool EEP25_readArray(uint32_t address, uint8_t *dataPtr, uint32_t length)
{

    if (length == 0) return false;
    
    while ((SPI_SpiUartGetTxBufferSize() > 0) && (SPI_SpiIsBusBusy()!=0)) {};
    SPI_SpiSetActiveSlaveSelect(NOTUSED_SPI_SLAVE_SELECT);  //point the hardware to a non used slave select line
    
    uint8_t databuf[3];
    databuf[0] = EEP25_READ;
    databuf[1] = (uint8_t)(address>>8);
    databuf[2] = (uint8_t) address;
    
    EEP_CS_Write(0);
    SPI_SpiUartPutArray(databuf, 3);
    while(SPI_SpiIsBusBusy()!=0){};   
    SPI_SpiUartClearRxBuffer();
    
    // now read back the data
    while (length--)
    {
        SPI_SpiUartWriteTxData(0x00);           // write dummy byte
        while(SPI_SpiIsBusBusy()!=0){};  
        *dataPtr++ = SPI_SpiUartReadRxData();   // read the RX byte
    }
    EEP_CS_Write(1);
    SPI_SpiUartClearRxBuffer();
    SPI_SpiSetActiveSlaveSelect(DEFAULT_SPI_SLAVE_SELECT);  //point the hardware to a default slave select line
    
    return true;
}

/*****************************************************************************\
 * Function:    EEP25_writeByte
 * Input:       address, byte value
 * Returns:     bool success/fail flag
 * Description: 
 *     Writes a byte to the eeprom memory
\*****************************************************************************/
bool EEP25_writeByte(uint32_t address, uint8_t data)
{
    bool result = true;
    while ((SPI_SpiUartGetTxBufferSize() > 0) && (SPI_SpiIsBusBusy()!=0)) {};
    SPI_SpiSetActiveSlaveSelect(NOTUSED_SPI_SLAVE_SELECT);  //point the hardware to a non used slave select line
    
    uint8_t databuf[4];
    databuf[0] = EEP25_WRITE;
    databuf[1] = (uint8_t)(address>>8);
    databuf[2] = (uint8_t) address;
    databuf[3] = data;
    // set write enalble latch
    EEP_CS_Write(0);
    SPI_SpiUartWriteTxData(EEP25_SET_WRITE_EN);
    while(SPI_SpiIsBusBusy()!=0){};
    EEP_CS_Write(1);
    
    //CyDelayUs(1); //might be necessary
    
    EEP_CS_Write(0);

    SPI_SpiUartPutArray(databuf, 4);
    while(SPI_SpiIsBusBusy()!=0){};   
    EEP_CS_Write(1);
    
    SPI_SpiUartClearRxBuffer();
    SPI_SpiSetActiveSlaveSelect(DEFAULT_SPI_SLAVE_SELECT);  //point the hardware to a default slave select line   

    return result;
}

/*****************************************************************************\
 * Function:    EEP25_writeArray
 * Input:       address, pointer to an array, length in bytes
 * Returns:     bool success/fail flag
 * Description: 
 *     Writes n bytes to the eeprom memory
\*****************************************************************************/
bool EEP25_writeArray(uint32_t address, uint8_t *dataPtr, uint32_t length)
{
    if (length == 0) return false;
    
    while ((SPI_SpiUartGetTxBufferSize() > 0) && (SPI_SpiIsBusBusy()!=0)) {};
    SPI_SpiSetActiveSlaveSelect(NOTUSED_SPI_SLAVE_SELECT);  //point the hardware to a non used slave select line
    
    uint8_t databuf[3];
    databuf[0] = EEP25_WRITE;
    databuf[1] = (uint8_t)(address>>8);
    databuf[2] = (uint8_t) address;
    
    // set write enalble latch
    EEP_CS_Write(0);
    SPI_SpiUartWriteTxData(EEP25_SET_WRITE_EN);
    while(SPI_SpiIsBusBusy()!=0){};
    EEP_CS_Write(1);
    
    //CyDelayUs(1); //might be necessary
    
    EEP_CS_Write(0);
    SPI_SpiUartPutArray(databuf, 3);
    
    while(length--)
    {
        SPI_SpiUartWriteTxData(*dataPtr++);
    }
    
    while(SPI_SpiIsBusBusy()!=0){}; 
    
    EEP_CS_Write(1);
    
    SPI_SpiUartClearRxBuffer();
    SPI_SpiSetActiveSlaveSelect(DEFAULT_SPI_SLAVE_SELECT);  //point the hardware to a default slave select line   

    return true;
}

/* [] END OF FILE */
