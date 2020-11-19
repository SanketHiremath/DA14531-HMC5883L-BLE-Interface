/**
 ****************************************************************************************
 *
 * @file user_L3G4200D.c
 *
 * @brief Functions to interface with the L3G4200D accelerometer.
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

#include "user_L3G4200D.h"
#include "gpio.h"
#include "app_easy_timer.h"
#include "i2c.h"
#include  "user_digitalCompass.h"



/**
 ****************************************************************************************
 * @brief L3G4200D sensor initialization function
 * @return void
 ****************************************************************************************
 */
void L3G4200D_init(void){
#ifdef NO_SENSOR
	return;//If the demo is ran without a sensor return immediately
#else
     uint8_t set_mode_cmd[] = {L3G4200D_REG_POWER_CTL1, 0x17};      //Set measure mode
    i2c_abort_t abort_code; //May be used for error checking

	//Initialize sensor configuration registers
    i2c_master_transmit_buffer_sync(set_mode_cmd, 2, &abort_code, I2C_F_NONE);
    set_mode_cmd[0] = L3G4200D_REG_CTR4;
		set_mode_cmd[1] = 0x48; 
    i2c_master_transmit_buffer_sync(set_mode_cmd, 2, &abort_code, I2C_F_NONE);	
    set_mode_cmd[0] = 0x24;
		set_mode_cmd[1] = 0x40;	
 //   i2c_master_transmit_buffer_sync(set_mode_cmd, 2, &abort_code, I2C_F_NONE);		 
//    i2c_master_transmit_buffer_sync(data_format_cmd, 2, &abort_code, I2C_F_NONE);
//    i2c_master_transmit_buffer_sync(bw_rate_cmd, 2, &abort_code, I2C_F_NONE);

#endif //NO_SENSOR
	
}

/**
 ****************************************************************************************
 * @brief Read X-axis acceleration registers
 * @return X-axis acceleration raw value
 ****************************************************************************************
 */
int16_t L3G4200D_read_X(void){
#ifndef NO_SENSOR
    i2c_abort_t abort_code;                     //May be used for error checking
    uint8_t reg_addr = L3G4200D_REG_DATAX_LSB;  //Variable to hold the register address
    uint8_t xData_MSB,xData_LSB = 0;
    uint8_t byte_received = 0;   


    
    //Get measurement x LSB
	  i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
    X_data =  byte_received & 0xff;          //Store X LSB
		
    //Get measurement MSB
    reg_addr = L3G4200D_REG_DATAX_MSB;
    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
    X_data |= (byte_received & 0xff) << 8;  //Store X MSB
	
    //Get measurement y LSB
	  reg_addr = L3G4200D_REG_DATAY_LSB;
	  i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
    Y_data =  byte_received & 0xff;          //Store X LSB
		
    //Get measurement y MSB
    reg_addr = L3G4200D_REG_DATAY_MSB;
    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
    Y_data |= (byte_received & 0xff) << 8;  //Store X MSB	
		
    //Get measurement z LSB
	  reg_addr = L3G4200D_REG_DATAZ_LSB;
	  i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
    Z_data =  byte_received & 0xff;          //Store X LSB
		
    //Get measurement z MSB
    reg_addr = L3G4200D_REG_DATAZ_MSB;
    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
    Z_data |= (byte_received & 0xff) << 8;  //Store X MSB		
#else
	//If no sensor is present just increase current data
	X_data++;
#endif //NO_SENSOR
	  return X_data;
	
}

/**
 ****************************************************************************************
 * @brief Read Y-axis acceleration registers
 * @return Y-axis acceleration raw value
 ****************************************************************************************
 */
int16_t L3G4200D_read_Y(void){

//    i2c_abort_t abort_code;                 //May be used for error checking
//    uint8_t reg_addr = L3G4200D_REG_DATAY_LSB;  //Variable to hold the register address
//    uint8_t byte_received = 0;              //Intermediate storage of the received byte
//    
//    //Get measurement LSB
//    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
//    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
//	  Y_data = byte_received & 0xff;          //Store Y LSB
//    
//    //Get measurement MSB
//    reg_addr = L3G4200D_REG_DATAY_MSB;
//    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
//    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
//   	Y_data |= (byte_received & 0xff) << 8;  //Store Y MSB

	return Y_data;
}

/**
 ****************************************************************************************
 * @brief Read Z-axis acceleration registers
 * @return Z-axis acceleration raw value
 ****************************************************************************************
 */
int16_t L3G4200D_read_Z(void){
//#ifndef NO_SENSOR
//    i2c_abort_t abort_code;                 //May be used for error checking
//    uint8_t reg_addr = L3G4200D_REG_DATAZ_LSB;  //Variable to hold the register address
//    uint8_t byte_received = 0;              //Intermediate storage of the received byte
//    
//    //Get measurement LSB
//    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
//    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
//	Z_data = byte_received & 0xff;          //Store Z LSB
//    
//    //Get measurement MSB
//    reg_addr = L3G4200D_REG_DATAZ_MSB;
//    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
//    i2c_master_receive_buffer_sync(&byte_received, 1, &abort_code, I2C_F_NONE);
//   	Z_data |= (byte_received & 0xff) << 8;  //Store Z MSB
//#else
//	//If no sensor is present just increase current data
//	Z_data++;
//#endif //NO_SENSOR
	return Z_data;
}

/**
 ****************************************************************************************
 * @brief Read all axes acceleration values
 * @param[in] xyz     Pointer to an array which will hold the measurement
 * @return void
 ****************************************************************************************
 */
void L3G4200D_read_XYZ(uint8_t* xyz){
//    i2c_abort_t abort_code;     //May be used for error checking
    static uint8_t previous[6]; //Holds the previous measurement

//    //Setup multiple-byte read from DATAX0 to DATAZ1 registers
//    const uint8_t reg_addr = L3G4200D
//			_REG_DATAY_LSB;
//    i2c_master_transmit_buffer_sync(&reg_addr, 1, &abort_code, I2C_F_NONE);
//    i2c_master_receive_buffer_sync(xyz, 6, &abort_code, I2C_F_NONE);
//    
//    //Process the received register values
//	for(int i = 0; i < 3; i++){
//		int16_t current =   (xyz[i*2+1] & 0xff) << 8 | (xyz[i*2] & 0xff); //Convert the received data to a 16 bit int
//		int16_t prev = (previous[i*2+1] & 0xff) << 8 | (previous[i*2] & 0xff);
//		current = 0.75 * current + 0.25 * prev; //A very simple smoothing algorithm
//		xyz[i*2] = current & 0xff;
//		xyz[i*2+1] = (current >> 8) & 0xff;
//	}
//	
//    //Save the data
	memcpy(previous, xyz, 6);
}
