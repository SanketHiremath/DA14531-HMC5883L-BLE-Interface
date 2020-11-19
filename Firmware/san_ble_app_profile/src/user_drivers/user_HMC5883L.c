/**
 ****************************************************************************************
 *
 * @file user_HMC5883L.c
 *
 * @brief Functions to interface with the HMC5883L accelerometer.
 *
 * Copyright (c) 2012-2019 Dialog Semiconductor. All rights reserved.
 *
 * This software ("Software") is owned by Dialog Semiconductor.
 *
 * By using this Software you agree that Dialog Semiconductor retains all
 * intellectual property and proprietary rights in and to this Software and any
 * use, reproduction, disclosure or distribution of the Software without express
 * written permission or a license agreement from Dialog Semiconductor is
 * strictly prohibited. This Software is solely for use on or in conjunction
 * with Dialog Semiconductor products.
 *
 * EXCEPT AS OTHERWISE PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, THE
 * SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. EXCEPT AS OTHERWISE
 * PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, IN NO EVENT SHALL
 * DIALOG SEMICONDUCTOR BE LIABLE FOR ANY DIRECT, SPECIAL, INDIRECT, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THE SOFTWARE.
 *
 ****************************************************************************************
 */

#include "user_HMC5883L.h"
#include "gpio.h"
#include "app_easy_timer.h"
#include "i2c.h"
#include  "user_digitalCompass.h"

int16_t sup=0;
 

/**
 ****************************************************************************************
 * @brief HMC5883L sensor initialization function
 * @return void
 ****************************************************************************************
 */
void HMC5883L_init(void){
#ifdef NO_SENSOR
	return;//If the demo is ran without a sensor return immediately
#else
	  i2c_abort_t abort_code; //May be used for error checking
    const uint8_t set_mode_cmd[] = {HMC5883L_REG_MODE, 0x00};      //Set measure mode
//  const uint8_t bw_rate_cmd[] = {HMC5883L_REG_CONFIG_A, 0x18};          //100Hz output
		
  	//Initialize sensor configuration registers
    i2c_master_transmit_buffer_sync(set_mode_cmd, 2, &abort_code, I2C_F_NONE);

#endif //NO_SENSOR
		
}

/**
 ****************************************************************************************
 * @brief Read X-axis magnetometer registers
 * @return X-axis magnetometer raw value
 ****************************************************************************************
 */
int16_t HMC5883L_readXYZ(void){

    i2c_abort_t abort_code;                     //May be used for error checking
    uint8_t reg_addr = HMC5883L_REG_DATAX_LSB;  //Variable to hold the register address
    int8_t xData_MSB, xData_LSB, yData_MSB, yData_LSB, zData_MSB, zData_LSB = 0;
	  uint8_t byte_received[6] = {0};
     

    i2c_master_receive_buffer_sync(byte_received, 6, &abort_code, I2C_F_NONE);
    xData_MSB = (byte_received[0] & 0xFF) << 8;  //Store X MSB
		xData_LSB = (byte_received[1]& 0xFF);
	  X_data = xData_MSB | xData_LSB;
	
    yData_MSB = (byte_received[2] & 0xFF) << 8;  //Store y MSB
		yData_LSB = (byte_received[3]& 0xFF);
	  Y_data = yData_MSB | yData_LSB;	
	
    zData_MSB = (byte_received[4] & 0xFF) << 8;  //Store Z MSB
		zData_LSB = (byte_received[5]& 0xFF);
	  Z_data = zData_MSB | zData_LSB;		
	
    reg_addr = HMC5883L_REG_DATAX_MSB;
    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);	
	  X_data = xData_MSB | xData_LSB;

	return X_data;	
}

/**
 ****************************************************************************************
 * @brief Read X-axis magnetometer registers
 * @return X-axis magnetometer raw value
 ****************************************************************************************
 */
int16_t HMC5883L_read_X(void){

    i2c_abort_t abort_code;                 //May be used for error checking
    uint8_t reg_addr = HMC5883L_REG_DATAX_LSB;  //Variable to hold the register address
    uint8_t byte_received = 0;              //Intermediate storage of the received byte
    
    //Get measurement LSB
    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
	  X_data = byte_received & 0xff;          //Store Y LSB
    
    //Get measurement MSB
    reg_addr = HMC5883L_REG_DATAX_MSB;
    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
   	X_data |= (byte_received & 0xff) << 8;  //Store Y MSB

	return X_data;
}

/**
 ****************************************************************************************
 * @brief Read Y-axis magnetometer registers
 * @return Y-axis magnetometer raw value
 ****************************************************************************************
 */
int16_t HMC5883L_read_Y(void){

    i2c_abort_t abort_code;                 //May be used for error checking
    uint8_t reg_addr = HMC5883L_REG_DATAY_LSB;  //Variable to hold the register address
    uint8_t byte_received = 0;              //Intermediate storage of the received byte
    
    //Get measurement LSB
    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
	  Y_data = byte_received & 0xff;          //Store Y LSB
    
    //Get measurement MSB
    reg_addr = HMC5883L_REG_DATAY_MSB;
    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
   	Y_data |= (byte_received & 0xff) << 8;  //Store Y MSB

	return Y_data;
}

/**
 ****************************************************************************************
 * @brief Read Z-axis magnetometer registers
 * @return Z-axis magnetometer raw value
 ****************************************************************************************
 */
int16_t HMC5883L_read_Z(void){
#ifndef NO_SENSOR
    i2c_abort_t abort_code;                 //May be used for error checking
    uint8_t reg_addr = HMC5883L_REG_DATAZ_LSB;  //Variable to hold the register address
    uint8_t byte_received = 0;              //Intermediate storage of the received byte
    
    //Get measurement LSB
    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
	Z_data = byte_received & 0xff;          //Store Z LSB
    
    //Get measurement MSB
    reg_addr = HMC5883L_REG_DATAZ_MSB;
    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
   	Z_data |= (byte_received & 0xff) << 8;  //Store Z MSB
#else
	//If no sensor is present just increase current data
	Z_data++;
#endif //NO_SENSOR
	return Z_data;
}


