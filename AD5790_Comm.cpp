 /***************************************************************************//** 
  *   @file   Communication.c 
  *   @brief  Implementation of Communication Driver. 
  *   @author DBogdan (dragos.bogdan@analog.com) 
 ******************************************************************************** 
  * Copyright 2012(c) Analog Devices, Inc. 
  * 
  * All rights reserved. 
  * 
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted provided that the following conditions are met: 
  *  - Redistributions of source code must retain the above copyright 
  *    notice, this list of conditions and the following disclaimer. 
  *  - Redistributions in binary form must reproduce the above copyright   
  *    notice, this list of conditions and the following disclaimer in 
  *    the documentation and/or other materials provided with the 
  *    distribution. 
  *  - Neither the name of Analog Devices, Inc. nor the names of its 
  *    contributors may be used to endorse or promote products derived 
  *    from this software without specific prior written permission. 
  *  - The use of this software may or may not infringe the patent rights 
  *    of one or more patent holders.  This license does not release you 
  *    from the requirement that you obtain separate licenses from these 
  *    patent holders to use this software. 
  *  - Use of the software either in source or binary form, must be run 
  *    on or directly connected to an Analog Devices Inc. component. 
  * 
  * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR 
  * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT, 
  * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
  * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT, 
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR 
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
  * 
 ******************************************************************************** 
  *   SVN Revision: 812 
 *******************************************************************************/ 
 
 
 /******************************************************************************/ 
 /* Include Files                                                              */ 
 /******************************************************************************/ 
 #include <SPI.h> 
 #include <stdint.h> 
 #include "AD5790_Comm.h" 
 
AD5790_Comm::AD5790_Comm(uint8_t device_num){
     device_number = device_num;
}

 
 /***************************************************************************//** 
  * @brief Initializes the SPI communication peripheral. 
  * 
  * @param lsbFirst - Transfer format (0 or 1). 
  *                   Example: 0x0 - MSB first. 
  *                            0x1 - LSB first. 
  * @param clockFreq - SPI clock frequency (Hz). 
  *                    Example: 1000 - SPI clock frequency is 1 kHz. 
  * @param clockPol - SPI clock polarity (0 or 1). 
  *                   Example: 0x0 - Idle state for clock is a low level; active 
  *                                  state is a high level; 
  *                        0x1 - Idle state for clock is a high level; active 
  *                                  state is a low level. 
  * @param clockEdg - SPI clock edge (0 or 1). 
  *                   Example: 0x0 - Serial output data changes on transition 
  *                                  from idle clock state to active clock state; 
  *                            0x1 - Serial output data changes on transition 
  *                                  from active clock state to idle clock state. 
  * 
  * @return status - Result of the initialization procedure. 
  *                  Example: 1 - if initialization was successful; 
  *                           0 - if initialization was unsuccessful. 
 *******************************************************************************/ 
 uint8_t AD5790_Comm::AD57XX_COMM_Init(uint8_t lsbFirst, 
                        uint32_t clockFreq, 
                        uint8_t clockPol, 
                        uint8_t clockEdg) 
 { 
     // Add your code here.
     SPI.begin(); 
     AD57XX_spisettings = SPISettings( 
         clockFreq, 
         lsbFirst ? LSBFIRST : MSBFIRST, 
         clockPol ? ( clockEdg ? SPI_MODE1 : SPI_MODE0) : (clockEdg ? SPI_MODE3 : SPI_MODE2) 
         ); 
    // SPI.beginTransaction(AD57XX_spisettings); 
 
     #ifdef DEBUGG
     SerialUSB.print("SPI: "); 
     SerialUSB.print(clockFreq); 
     SerialUSB.print(" "); 
     SerialUSB.print(lsbFirst ? LSBFIRST : MSBFIRST); 
     SerialUSB.print(" "); 
     SerialUSB.println(clockPol ? ( clockEdg ? SPI_MODE1 : SPI_MODE0) : (clockEdg ? SPI_MODE3 : SPI_MODE2)); 
     #endif // DEBUGG
     return 1; 
 } 
 
 
 /***************************************************************************//** 
  * @brief Reads data from SPI. 
  * 
  * @param slaveDeviceId - The ID of the selected slave device. 
  * @param data - Data represents the write buffer as an input parameter and the 
  *               read buffer as an output parameter. 
  * @param bytesNumber - Number of bytes to read. 
  * 
  * @return Number of read bytes. 
 *******************************************************************************/ 
 uint8_t AD5790_Comm::AD57XX_Read(uint8_t slaveDeviceId, 
                        uint8_t* data, 
                        uint8_t bytesNumber) 
 { 
    
     AD57XX_SYNC_LOW();
     AD57XX_MISO_EN_HIGH();
     SPI.beginTransaction(AD57XX_spisettings); 
     for (uint8_t i = 0; i < bytesNumber; ++i){ 
         data[i] = SPI.transfer(data[i]); 
     } 
     
    
     
     SPI.endTransaction(); 
     AD57XX_SYNC_HIGH();
     AD57XX_MISO_EN_LOW(); //disable miso output again
     return bytesNumber; 
 } 
 
 
 /***************************************************************************//** 
  * @brief Writes data to SPI. 
  * 
  * @param slaveDeviceId - The ID of the selected slave device. 
  * @param data - Data represents the write buffer. 
  * @param bytesNumber - Number of bytes to write. 
  * 
  * @return Number of written bytes. 
 *******************************************************************************/ 
 uint8_t AD5790_Comm::AD57XX_Write(uint8_t slaveDeviceId, 
                         uint8_t* data, 
                         uint8_t bytesNumber) 
 { 
     AD57XX_SYNC_LOW();
     SPI.beginTransaction(AD57XX_spisettings); 
     for (uint8_t i = 0; i < bytesNumber; ++i){ 
         data[i] = SPI.transfer(data[i]); 
     } 
     SPI.endTransaction(); 
     AD57XX_SYNC_HIGH();
     return bytesNumber; 
 } 

  /* LDAC */
void AD5790_Comm::AD57XX_LDAC_OUT(){
  switch(device_number){
    case 0:
      PIOB->PIO_PER |= (1<<AD57XX_LDAC_PIN2); //override peripheral on this pin
      PIOB->PIO_OER |= (1<<AD57XX_LDAC_PIN2); //set ddr of pin to output
      break;
    case 1:
      PIOC->PIO_PER |= (1<<AD57XX_LDAC_PIN1); //override peripheral on this pin
      PIOC->PIO_OER |= (1<<AD57XX_LDAC_PIN1); //set ddr of pin to output
      break;
    default:
      PIOB->PIO_PER |= (1<<AD57XX_LDAC_PIN2); //override peripheral on this pin
      PIOB->PIO_OER |= (1<<AD57XX_LDAC_PIN2); //set ddr of pin to output
      break;
  }
}

void AD5790_Comm::AD57XX_LDAC_HIGH(){
  //SerialUSB.println("AD57XX_LDAC_HIGH called");
  //SerialUSB.println(device_number);
  switch(device_number){
    case 0:
      PIOB->PIO_SODR |= (1<<AD57XX_LDAC_PIN2);
      break;
    case 1:
      PIOC->PIO_SODR |= (1<<AD57XX_LDAC_PIN1);
      break;
    default:
      PIOB->PIO_SODR |= (1<<AD57XX_LDAC_PIN2);
      break;
  }
    //digitalWrite(AD57XX_LDAC_PIN, LOW);
}

void AD5790_Comm::AD57XX_LDAC_LOW(){

    //SerialUSB.println("AD57XX_LDAC_LOW called");
    //SerialUSB.println(device_number);
    switch(device_number){
    case 0:
      PIOB->PIO_CODR |= (1<<AD57XX_LDAC_PIN2);
      break;
    case 1:
      PIOC->PIO_CODR |= (1<<AD57XX_LDAC_PIN1);
      break;
    default:
      PIOB->PIO_CODR |= (1<<AD57XX_LDAC_PIN2);
      break;
    }
    //digitalWrite(AD57XX_LDAC_PIN, HIGH);
}

/* CLR */
void AD5790_Comm::AD57XX_CLR_OUT(){
  switch(device_number){
    case 0:
      PIOB->PIO_PER |= (1<<AD57XX_CLR_PIN2); //override peripheral on this pin
      PIOB->PIO_OER |= (1<<AD57XX_CLR_PIN2); //set ddr of pin to output
      break;
    case 1:
      PIOC->PIO_PER |= (1<<AD57XX_CLR_PIN1); //override peripheral on this pin
      PIOC->PIO_OER |= (1<<AD57XX_CLR_PIN1); //set ddr of pin to output
      break;
    default:
      PIOB->PIO_PER |= (1<<AD57XX_CLR_PIN2); //override peripheral on this pin
      PIOB->PIO_OER |= (1<<AD57XX_CLR_PIN2); //set ddr of pin to output
      break;
  }
    //pinMode(AD57XX_CLR_PIN, OUTPUT);
}

void AD5790_Comm::AD57XX_CLR_HIGH(){
  switch(device_number){
    case 0:
      PIOB->PIO_SODR |= (1<<AD57XX_CLR_PIN2);
      break;
    case 1:
      PIOC->PIO_SODR |= (1<<AD57XX_CLR_PIN1);
      break;
    default:
      PIOB->PIO_SODR |= (1<<AD57XX_CLR_PIN2);
      break;
  }
    //digitalWrite(AD57XX_CLR_PIN, LOW);
}

void AD5790_Comm::AD57XX_CLR_LOW(){
  switch(device_number){
    case 0:
      PIOB->PIO_CODR |= (1<<AD57XX_CLR_PIN2);
      break;
    case 1:
      PIOC->PIO_CODR |= (1<<AD57XX_CLR_PIN1);
      break;
    default:
      PIOB->PIO_CODR |= (1<<AD57XX_CLR_PIN2);
      break;
    }
    //digitalWrite(AD57XX_CLR_PIN, HIGH);
}
/* RESET */
void AD5790_Comm::AD57XX_RESET_OUT(){
    switch(device_number){
    case 0:
      PIOB->PIO_PER |= (1<<AD57XX_RESET_PIN2); //override peripheral on this pin
      PIOB->PIO_OER |= (1<<AD57XX_RESET_PIN2); //set ddr of pin to output
      break;
    case 1:
      PIOC->PIO_PER |= (1<<AD57XX_RESET_PIN1); //override peripheral on this pin
      PIOC->PIO_OER |= (1<<AD57XX_RESET_PIN1); //set ddr of pin to output
      break;
    default:
      PIOB->PIO_PER |= (1<<AD57XX_RESET_PIN2); //override peripheral on this pin
      PIOB->PIO_OER |= (1<<AD57XX_RESET_PIN2); //set ddr of pin to output
      break;
  }
}

void AD5790_Comm::AD57XX_RESET_HIGH(){
  switch(device_number){
    case 0:
      PIOB->PIO_SODR |= (1<<AD57XX_RESET_PIN2);
      break;
    case 1:
      PIOC->PIO_SODR |= (1<<AD57XX_RESET_PIN1);
      break;
    default:
      PIOB->PIO_SODR |= (1<<AD57XX_RESET_PIN2);
      break;
  }
    //digitalWrite(AD57XX_RESET_PIN, LOW);
}

void AD5790_Comm::AD57XX_RESET_LOW(){
  switch(device_number){
    case 0:
      PIOB->PIO_CODR |= (1<<AD57XX_RESET_PIN2);
      break;
    case 1:
      PIOC->PIO_CODR |= (1<<AD57XX_RESET_PIN1);
      break;
    default:
      PIOB->PIO_CODR |= (1<<AD57XX_RESET_PIN2);
      break;
    }
    //digitalWrite(AD57XX_RESET_PIN, HIGH);
}

/* SYNC */
void AD5790_Comm::AD57XX_SYNC_OUT(){
    switch(device_number){
    case 0:
      PIOB->PIO_PER |= (1<<AD57XX_SYNC_PIN2); //override peripheral on this pin
      PIOB->PIO_OER |= (1<<AD57XX_SYNC_PIN2); //set ddr of pin to output
      break;
    case 1:
      PIOC->PIO_PER |= (1<<AD57XX_SYNC_PIN1); //override peripheral on this pin
      PIOC->PIO_OER |= (1<<AD57XX_SYNC_PIN1); //set ddr of pin to output
      break;
    default:
      PIOB->PIO_PER |= (1<<AD57XX_SYNC_PIN2); //override peripheral on this pin
      PIOB->PIO_OER |= (1<<AD57XX_SYNC_PIN2); //set ddr of pin to output
      break;
  }
}

void AD5790_Comm::AD57XX_SYNC_HIGH(){
  switch(device_number){
    case 0:
      PIOB->PIO_SODR |= (1<<AD57XX_SYNC_PIN2);
      break;
    case 1:
      PIOC->PIO_SODR |= (1<<AD57XX_SYNC_PIN1);
      break;
    default:
      PIOB->PIO_SODR |= (1<<AD57XX_SYNC_PIN2);
      break;
  }
    //digitalWrite(AD57XX_RESET_PIN, LOW);
}

void AD5790_Comm::AD57XX_SYNC_LOW(){
  switch(device_number){
    case 0:
      PIOB->PIO_CODR |= (1<<AD57XX_SYNC_PIN2);
      break;
    case 1:
      PIOC->PIO_CODR |= (1<<AD57XX_SYNC_PIN1);
      break;
    default:
      PIOB->PIO_CODR |= (1<<AD57XX_SYNC_PIN2);
      break;
    }
    //digitalWrite(AD57XX_RESET_PIN, HIGH);
}

void AD5790_Comm::AD57XX_MISO_EN_OUT(){
  switch(device_number){
    case 0:
      PIOC->PIO_PER |= (1<<AD57XX_MISO_ENABLE_PIN1); //override peripheral on this pin
      PIOC->PIO_OER |= (1<<AD57XX_MISO_ENABLE_PIN1); //set ddr of pin to output
      break;
    case 1:
      PIOC->PIO_PER |= (1<<AD57XX_MISO_ENABLE_PIN2); //override peripheral on this pin
      PIOC->PIO_OER |= (1<<AD57XX_MISO_ENABLE_PIN2); //set ddr of pin to output
      break;
    default:
      PIOC->PIO_PER |= (1<<AD57XX_MISO_ENABLE_PIN1); //override peripheral on this pin
      PIOC->PIO_OER |= (1<<AD57XX_MISO_ENABLE_PIN1); //set ddr of pin to output
      break;
  }
}
void AD5790_Comm::AD57XX_MISO_EN_LOW(){
  switch(device_number){
    case 0:
      PIOC->PIO_CODR |= (1<<AD57XX_MISO_ENABLE_PIN1);
      break;
    case 1:
      PIOC->PIO_CODR |= (1<<AD57XX_MISO_ENABLE_PIN2);
      break;
    default:
      PIOC->PIO_CODR |= (1<<AD57XX_MISO_ENABLE_PIN1);
      break;
    }
   
}
void AD5790_Comm::AD57XX_MISO_EN_HIGH(){
  switch(device_number){
    case 0:
      PIOC->PIO_SODR |= (1<<AD57XX_MISO_ENABLE_PIN1);
      break;
    case 1:
      PIOC->PIO_SODR |= (1<<AD57XX_MISO_ENABLE_PIN2);
      break;
    default:
      PIOC->PIO_SODR |= (1<<AD57XX_MISO_ENABLE_PIN1);
      break;
  }
   
}
