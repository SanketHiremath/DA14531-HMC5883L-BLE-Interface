/**
 ****************************************************************************************
 *
 * @file user_periph_setup.c
 *
 * @brief Peripherals setup and initialization.
 *
 * Copyright (C) 2015-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "user_periph_setup.h"
#include "datasheet.h"
#include "system_library.h"
#include "rwip_config.h"
#include "gpio.h"
#include "uart.h"
#include "syscntl.h"
#include "i2c.h"
#include "user_HMC5883L.h"


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

#if DEVELOPMENT_DEBUG

void GPIO_reservations(void)
{
/*
* Globally reserved GPIOs reservation
*/

/*
* Application specific GPIOs reservation. Used only in Development mode (#if DEVELOPMENT_DEBUG)

i.e.
    RESERVE_GPIO(DESCRIPTIVE_NAME, GPIO_PORT_0, GPIO_PIN_1, PID_GPIO);    //Reserve P_01 as Generic Purpose I/O
*/
    
    // Reserve GPIOs for HMC5883L sensor
    RESERVE_GPIO(ADXL_GPIO_SDA, HMC5883L_SDA_PORT, HMC5883L_SDA_PIN, PID_I2C_SDA);
    RESERVE_GPIO(ADXL_GPIO_SCL, HMC5883L_SCL_PORT, HMC5883L_SCL_PIN, PID_I2C_SCL);
    RESERVE_GPIO(LED_PIN, GPIO_LED_PORT, GPIO_LED_PIN, PID_GPIO);
	  
}

#endif

void set_pad_functions(void)
{
/*
* Configure application ports.
i.e.
    GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_1, OUTPUT, PID_GPIO, false ); // Set P_01 as Generic purpose Output
*/

#ifdef __DA14586__
    // disallow spontaneous flash wake-up
    GPIO_ConfigurePin(SPI_EN_PORT, SPI_EN_PIN, OUTPUT, PID_SPI_EN, true);
#endif
    
    // Configure GPIOs for the HMC5883L sensor
    GPIO_ConfigurePin(HMC5883L_SDA_PORT, HMC5883L_SDA_PIN, INPUT_PULLUP, PID_I2C_SDA, true);
    GPIO_ConfigurePin(HMC5883L_SCL_PORT, HMC5883L_SCL_PIN, INPUT_PULLUP, PID_I2C_SCL, true);
    GPIO_ConfigurePin(GPIO_LED_PORT, GPIO_LED_PIN, OUTPUT, PID_GPIO, false);
}



void periph_init(void)
{
    // Configuration struct for I2C
    static const i2c_cfg_t i2c_cfg = {
    .clock_cfg.ss_hcnt = I2C_SS_SCL_HCNT_REG_RESET,
    .clock_cfg.ss_lcnt = I2C_SS_SCL_LCNT_REG_RESET,
    .clock_cfg.fs_hcnt = I2C_FS_SCL_HCNT_REG_RESET,
    .clock_cfg.fs_lcnt = I2C_FS_SCL_LCNT_REG_RESET,
    .restart_en = I2C_RESTART_ENABLE,
    .speed = I2C_SPEED_MODE,
    .mode = I2C_MODE_MASTER,
    .addr_mode = I2C_ADDRESS_MODE,
    .address = HMC5883L_I2C_ADDRESS,
    .tx_fifo_level = 1,
    .rx_fifo_level = 1,
    };

#ifndef __DA14531__
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));
    SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 1);
#endif

    //rom patch
    patch_func();

    //Init pads
    set_pad_functions();
    
    // Initialize the I2C module
    i2c_init(&i2c_cfg);

    // Enable the pads
    SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 1);
		
	HMC5883L_init(); //Initialise the HMC5883L
//L3G4200D_init();
}
