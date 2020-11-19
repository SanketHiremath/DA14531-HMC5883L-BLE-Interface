/**
 ****************************************************************************************
 *
 * @file user_HMC5883L.h
 *
 * @brief functions for interfacing with the HMC5883L accelerometer over i2c.
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

#ifndef _HMC5883L_
#define _HMC5883L_

#include "gpio.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define HMC5883L_I2C_ADDRESS   0x1E      //This is the sensor address without R/W bit 

//#define NO_SENSOR                     //Uncomment this line when no sensor is connected

#if defined (__DA14531__)

#define HMC5883L_SCL_PORT    GPIO_PORT_0
#define HMC5883L_SCL_PIN     GPIO_PIN_5

#define HMC5883L_SDA_PORT    GPIO_PORT_0
#define HMC5883L_SDA_PIN     GPIO_PIN_8

#else

#define HMC5883L_SDA_PORT    GPIO_PORT_1
#define HMC5883L_SDA_PIN     GPIO_PIN_1
												     
#define HMC5883L_SCL_PORT    GPIO_PORT_1
#define HMC5883L_SCL_PIN     GPIO_PIN_3
												     
#endif

#define NOTIFICATION_DELAY 200 //The time interval between sent notifications

// HMC5883L Registers

#define HMC5883L_REG_CONFIG_A           0x00  
#define HMC5883L_REG_CONFIG_B           0x01 
#define HMC5883L_REG_MODE               0x02 

#define HMC5883L_REG_DATAX_MSB          0x03 //X-Axis Data 0
#define HMC5883L_REG_DATAX_LSB          0x04 //X-Axis Data 1
#define HMC5883L_REG_DATAY_MSB          0x05 //Y-Axis Data 0
#define HMC5883L_REG_DATAY_LSB          0x06 //Y-Axis Data 1
#define HMC5883L_REG_DATAZ_MSB          0x07 //Z-Axis Data 0
#define HMC5883L_REG_DATAZ_LSB          0x08 //Z-Axis Data 1 

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief HMC5883L initialization function.
 * @return void
 ****************************************************************************************
*/
void HMC5883L_init(void);

/**
 ****************************************************************************************
 * @brief Read the data registers of the HMC5883L, reads X to a global variable.
 * @return void
 ****************************************************************************************
*/
int16_t HMC5883L_readXYZ(void);

/**
 ****************************************************************************************
 * @brief Read the data registers of the HMC5883L, reads X to a global variable.
 * @return void
 ****************************************************************************************
*/
int16_t HMC5883L_read_X(void);

/**
 ****************************************************************************************
 * @brief Read the data registers of the HMC5883L, reads Y to a global variable.
 * @return void
 ****************************************************************************************
*/
int16_t HMC5883L_read_Y(void);

/**
 ****************************************************************************************
 * @brief Read the data registers of the HMC5883L, reads Z to a global variable.
 * @return void
 ****************************************************************************************
*/
int16_t HMC5883L_read_Z(void);



#endif
