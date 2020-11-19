/**
 ****************************************************************************************
 *
 * @file user_custs1_def.h
 *
 * @brief Custom Server 1 (CUSTS1) profile database definitions.
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

#ifndef _USER_CUSTS1_DEF_H_
#define _USER_CUSTS1_DEF_H_

/**
 ****************************************************************************************
 * @defgroup USER_CONFIG
 * @ingroup USER
 * @brief Custom Server 1 (CUSTS1) profile database definitions.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "attm_db_128.h"

/*
 * DEFINES
 ****************************************************************************************
 */

// Service 1 of the custom server 1
#define DEF_SVC1_UUID_128                  {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00}
#define DEF_SVC2_UUID_128                  {0x2F, 0x2A, 0x93, 0xA6, 0xBD, 0xD8, 0x41, 0x52, 0xAC, 0x0B, 0x10, 0x99, 0x2E, 0xC6, 0xFE, 0xED}

#define DEF_SVC1_DIGITAL_COMPASS_DATA_UUID_128     {0x20, 0xEE, 0x8D, 0x0C, 0xE1, 0xF0, 0x4A, 0x0C, 0xB3, 0x25, 0xDC, 0x53, 0x6A, 0x68, 0x86, 0x2D}
#define DEF_SVC1_ACCEL_Y_DATA_UUID_128     {0x20, 0xEE, 0x8D, 0x0C, 0xE1, 0xF0, 0x4A, 0x0C, 0xB3, 0x25, 0xDC, 0x53, 0x6A, 0x68, 0x86, 0x2E}
#define DEF_SVC1_ACCEL_Z_DATA_UUID_128     {0x20, 0xEE, 0x8D, 0x0C, 0xE1, 0xF0, 0x4A, 0x0C, 0xB3, 0x25, 0xDC, 0x53, 0x6A, 0x68, 0x86, 0x2F}
#define DEF_SVC2_G_DATA_UUID_128           {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11}


#define DEF_SVC1_ACCEL_X_DATA_CHAR_LEN     8
#define DEF_SVC1_ACCEL_Y_DATA_CHAR_LEN     8
#define DEF_SVC1_ACCEL_Z_DATA_CHAR_LEN     8
#define DEF_SVC2_G_DATA_CHAR_LEN           6


#define DEF_SVC1_ACCEL_X_DATA_USER_DESC     "Magnetometer Data"
#define DEF_SVC1_ACCEL_Y_DATA_USER_DESC     "Custom 1"
#define DEF_SVC1_ACCEL_Z_DATA_USER_DESC     "Custom 2"
#define DEF_SVC2_G_DATA_USER_DESC           "Custom Service 2"
 

/// Custom1 Service Data Base Characteristic enum
enum
{
    // Custom Service 1
    SVC1_IDX_SVC = 0,

    SVC1_IDX_DIGITAL_COMPASS_DATA_CHAR,
    SVC1_IDX_DIGITAL_COMPASS_DATA_VAL,
		SVC1_IDX_DIGITAL_COMPASS_NTF_CFG,
    SVC1_IDX_DIGITAL_COMPASS_DATA_USER_DESC,
	
    SVC1_IDX_ACCEL_Y_DATA_CHAR,
    SVC1_IDX_ACCEL_Y_DATA_VAL,
		SVC1_IDX_ACCEL_Y_NTF_CFG,
    SVC1_IDX_ACCEL_Y_DATA_USER_DESC,
	
    SVC1_IDX_ACCEL_Z_DATA_CHAR,
    SVC1_IDX_ACCEL_Z_DATA_VAL,
		SVC1_IDX_ACCEL_Z_NTF_CFG,
    SVC1_IDX_ACCEL_Z_DATA_USER_DESC,
		
		// Custom Service 2
		SVC2_IDX_SVC,

    SVC2_IDX_G_DATA_CHAR,
    SVC2_IDX_G_DATA_VAL,
		SVC2_IDX_G_NTF_CFG,
    SVC2_IDX_G_DATA_USER_DESC,
	
    CUSTS1_IDX_NB
};

/// @} USER_CONFIG

#endif // _USER_CUSTS1_DEF_H_