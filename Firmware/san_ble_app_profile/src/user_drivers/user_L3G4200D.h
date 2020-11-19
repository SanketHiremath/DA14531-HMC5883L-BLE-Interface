/**
 ****************************************************************************************
 *
 * @file user_L3G4200D.h
 *
 * @brief functions for interfacing with the L3G4200D accelerometer over i2c.
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

#ifndef _L3G4200D_
#define _L3G4200D_

#include "gpio.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define L3G4200D_I2C_ADDRESS   0x1E      //This is the sensor address without R/W bit 

//#define NO_SENSOR                     //Uncomment this line when no sensor is connected

#if defined (__DA14531__)

#define L3G4200D_SCL_PORT    GPIO_PORT_0
#define L3G4200D_SCL_PIN     GPIO_PIN_5

#define L3G4200D_SDA_PORT    GPIO_PORT_0
#define L3G4200D_SDA_PIN     GPIO_PIN_8

#else

#define L3G4200D_SDA_PORT    GPIO_PORT_1
#define L3G4200D_SDA_PIN     GPIO_PIN_1
												     
#define L3G4200D_SCL_PORT    GPIO_PORT_1
#define L3G4200D_SCL_PIN     GPIO_PIN_3
												     
#endif




#define L3G4200D_REG_POWER_CTL1       0x20 //Power-saving features control 
#define L3G4200D_REG_CTR4            0x23 //Data format control 
//#define L3G4200D_REG_BW_RATE         0x2C //Data rate and power mode control

#define L3G4200D_REG_DATAX_MSB          0x29 //X-Axis Data 0
#define L3G4200D_REG_DATAX_LSB          0x28 //X-Axis Data 1
#define L3G4200D_REG_DATAY_MSB          0x2B //Y-Axis Data 0
#define L3G4200D_REG_DATAY_LSB          0x2A //Y-Axis Data 1
#define L3G4200D_REG_DATAZ_MSB          0x2D //Z-Axis Data 0
#define L3G4200D_REG_DATAZ_LSB          0x2C //Z-Axis Data 1 

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief L3G4200D initialization function.
 * @return void
 ****************************************************************************************
*/
void L3G4200D_init(void);

/**
 ****************************************************************************************
 * @brief Read the data registers of the L3G4200D, reads X to a global variable.
 * @return void
 ****************************************************************************************
*/
int16_t L3G4200D_read_X(void);

/**
 ****************************************************************************************
 * @brief Read the data registers of the L3G4200D, reads Y to a global variable.
 * @return void
 ****************************************************************************************
*/
int16_t L3G4200D_read_Y(void);

/**
 ****************************************************************************************
 * @brief Read the data registers of the L3G4200D, reads Z to a global variable.
 * @return void
 ****************************************************************************************
*/
int16_t L3G4200D_read_Z(void);

void L3G4200D_read_XYZ(uint8_t* xyz);

#endif
