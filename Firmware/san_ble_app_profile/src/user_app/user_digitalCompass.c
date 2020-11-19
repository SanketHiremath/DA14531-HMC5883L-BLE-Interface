/**
 ****************************************************************************************
 *
 * @file user_accelerometer.c
 *
 * @brief Accelerometer project source code.
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

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration
#include "gap.h"
#include "app_easy_timer.h"
#include "user_digitalCompass.h"
#include "co_bt.h"
#include "gpio.h"
#include "app_api.h"
#include "app.h"
#include "prf_utils.h"
#include "custs1.h"
#include "user_custs1_def.h"
#include "user_periph_setup.h"
#include "user_HMC5883L.h"
#include "custs1_task.h"
#include <math.h>

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

// Manufacturer Specific Data ADV structure type
struct mnf_specific_data_ad_structure
{
    uint8_t ad_structure_size;
    uint8_t ad_structure_type;
    uint8_t company_id[APP_AD_MSD_COMPANY_ID_LEN];
    uint8_t proprietary_data[APP_AD_MSD_DATA_LEN];
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

uint8_t app_connection_idx                      __attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
timer_hnd app_adv_data_update_timer_used        __attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
timer_hnd app_param_update_request_timer_used   __attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
int16_t X_data									__attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
int16_t Y_data									__attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
int16_t Z_data                 __attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
uint8_t X_string[8] 							__attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
uint8_t Y_string[8] 							__attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
uint8_t Z_string[8]								__attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
uint8_t X_timer     							__attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
uint8_t Y_timer     							__attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
uint8_t Z_timer    								__attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
uint8_t g_timer    								__attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
struct mnf_specific_data_ad_structure mnf_data  __attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
// Index of manufacturer data in advertising data or scan response data (when MSB is 1)
uint8_t mnf_data_index                          __attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
uint8_t stored_adv_data_len                     __attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
uint8_t stored_scan_rsp_data_len                __attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
uint8_t stored_adv_data[ADV_DATA_LEN]           __attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY
uint8_t stored_scan_rsp_data[SCAN_RSP_DATA_LEN] __attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/

/**
 ****************************************************************************************
 * @brief Initialize Manufacturer Specific Data
 * @return void
 ****************************************************************************************
 */
static void mnf_data_init()
{
    mnf_data.ad_structure_size = sizeof(struct mnf_specific_data_ad_structure ) - sizeof(uint8_t); // minus the size of the ad_structure_size field
    mnf_data.ad_structure_type = GAP_AD_TYPE_MANU_SPECIFIC_DATA;
    mnf_data.company_id[0] = APP_AD_MSD_COMPANY_ID & 0xFF; // LSB
    mnf_data.company_id[1] = (APP_AD_MSD_COMPANY_ID >> 8 )& 0xFF; // MSB
    mnf_data.proprietary_data[0] = 0;
    mnf_data.proprietary_data[1] = 0;
}

/**
 ****************************************************************************************
 * @brief Update Manufacturer Specific Data
 * @return void
 ****************************************************************************************
 */
static void mnf_data_update()
{    
//    HMC5883L_readXYZYZ(mnf_data.proprietary_data);
}

/**
 ****************************************************************************************
 * @brief Add an AD structure in the Advertising or Scan Response Data of the 
  *       GAPM_START_ADVERTISE_CMD parameter struct.
 * @param[in] cmd               GAPM_START_ADVERTISE_CMD parameter struct
 * @param[in] ad_struct_data    AD structure buffer
 * @param[in] ad_struct_len     AD structure length
 * @param[in] adv_connectable   Connectable advertising event or not. It controls whether 
 *                              the advertising data use the full 31 bytes length or only 
 *                              28 bytes (Document CCSv6 - Part 1.3 Flags). 
 * @return void
 */
static void app_add_ad_struct(struct gapm_start_advertise_cmd *cmd, void *ad_struct_data, uint8_t ad_struct_len, uint8_t adv_connectable)
{
    uint8_t adv_data_max_size = (adv_connectable) ? (ADV_DATA_LEN - 3) : (ADV_DATA_LEN);
    
    if ((adv_data_max_size - cmd->info.host.adv_data_len) >= ad_struct_len)
    {
        // Append manufacturer data to advertising data
        memcpy(&cmd->info.host.adv_data[cmd->info.host.adv_data_len], ad_struct_data, ad_struct_len);

        // Update Advertising Data Length
        cmd->info.host.adv_data_len += ad_struct_len;
        
        // Store index of manufacturer data which are included in the advertising data
        mnf_data_index = cmd->info.host.adv_data_len - sizeof(struct mnf_specific_data_ad_structure);
    }
    else if ((SCAN_RSP_DATA_LEN - cmd->info.host.scan_rsp_data_len) >= ad_struct_len)
    {
        // Append manufacturer data to scan response data
        memcpy(&cmd->info.host.scan_rsp_data[cmd->info.host.scan_rsp_data_len], ad_struct_data, ad_struct_len);

        // Update Scan Response Data Length
        cmd->info.host.scan_rsp_data_len += ad_struct_len;
        
        // Store index of manufacturer data which are included in the scan response data
        mnf_data_index = cmd->info.host.scan_rsp_data_len - sizeof(struct mnf_specific_data_ad_structure);
        // Mark that manufacturer data is in scan response and not advertising data
        mnf_data_index |= 0x80;
    }
    else
    {
        // Manufacturer Specific Data do not fit in either Advertising Data or Scan Response Data
        ASSERT_WARNING(0);
    }
    // Store advertising data length
    stored_adv_data_len = cmd->info.host.adv_data_len;
    // Store advertising data
    memcpy(stored_adv_data, cmd->info.host.adv_data, stored_adv_data_len);
    // Store scan response data length
    stored_scan_rsp_data_len = cmd->info.host.scan_rsp_data_len;
    // Store scan_response data
    memcpy(stored_scan_rsp_data, cmd->info.host.scan_rsp_data, stored_scan_rsp_data_len);
}

/**
 ****************************************************************************************
 * @brief Advertisement data update timer callback function.
 * @return void
 ****************************************************************************************
*/
static void adv_data_update_timer_cb()
{
    // If mnd_data_index has MSB set, manufacturer data is stored in scan response
    uint8_t *mnf_data_storage = (mnf_data_index & 0x80) ? stored_scan_rsp_data : stored_adv_data;

    // Update manufacturer data
    mnf_data_update();

    // Update the selected fields of the advertising data (manufacturer data)
    memcpy(mnf_data_storage + (mnf_data_index & 0x7F), &mnf_data, sizeof(struct mnf_specific_data_ad_structure));

    // Update advertising data on the fly
    app_easy_gap_update_adv_data(stored_adv_data, stored_adv_data_len, stored_scan_rsp_data, stored_scan_rsp_data_len);
    
    // Restart timer for the next advertising update
    app_adv_data_update_timer_used = app_easy_timer(APP_ADV_DATA_UPDATE_TO, adv_data_update_timer_cb);
}

/**
 ****************************************************************************************
 * @brief Parameter update request timer callback function.
 * @return void
 ****************************************************************************************
*/
static void param_update_request_timer_cb()
{
    app_easy_gap_param_update_start(app_connection_idx);
    app_param_update_request_timer_used = EASY_TIMER_INVALID_TIMER;
}

/**
 **************************************************************************************************
 * @brief User defined initialization function
 * @return void
 **************************************************************************************************
 */
void user_app_init(void)
{
    app_param_update_request_timer_used = EASY_TIMER_INVALID_TIMER;
    
    // Initialize Manufacturer Specific Data
    mnf_data_init();
    
    // Initialize Advertising and Scan Response Data
    memcpy(stored_adv_data, USER_ADVERTISE_DATA, USER_ADVERTISE_DATA_LEN);
    stored_adv_data_len = USER_ADVERTISE_DATA_LEN;
    memcpy(stored_scan_rsp_data, USER_ADVERTISE_SCAN_RESPONSE_DATA, USER_ADVERTISE_SCAN_RESPONSE_DATA_LEN);
    stored_scan_rsp_data_len = USER_ADVERTISE_SCAN_RESPONSE_DATA_LEN;
    
    //Initialize timer handlers
    X_timer = EASY_TIMER_INVALID_TIMER;
    Y_timer = EASY_TIMER_INVALID_TIMER;
    Z_timer = EASY_TIMER_INVALID_TIMER;
    g_timer = EASY_TIMER_INVALID_TIMER;
    
    default_app_on_init();
}

/**
 **************************************************************************************************
 * @brief User defined function which which will be called when the device starts advertising
 * @return void
 **************************************************************************************************
 */
void user_app_adv_start(void)
{
    // Schedule the next advertising data update
    app_adv_data_update_timer_used = app_easy_timer(APP_ADV_DATA_UPDATE_TO, adv_data_update_timer_cb);
    
    struct gapm_start_advertise_cmd* cmd;
    cmd = app_easy_gap_undirected_advertise_get_active();
    
    // Add manufacturer data to initial advertising or scan response data, if there is enough space
    app_add_ad_struct(cmd, &mnf_data, sizeof(struct mnf_specific_data_ad_structure), 1);

    app_easy_gap_undirected_advertise_start();
}

/**
 **************************************************************************************************
 * @brief User defined function which which will be called when the device establishes a connection
 * @param[in] connection_idx Connection ID number
 * @param[in] param          Pointer to GAPC_CONNECTION_REQ_IND message
 * @return void
 **************************************************************************************************
 */
void user_app_connection(uint8_t connection_idx, struct gapc_connection_req_ind const *param)
{
	  GPIO_SetActive(GPIO_LED_PORT, GPIO_LED_PIN);
    if (app_env[connection_idx].conidx != GAP_INVALID_CONIDX)
    {
        app_connection_idx = connection_idx;

        // Stop the advertising data update timer
        app_easy_timer_cancel(app_adv_data_update_timer_used);

        // Check if the parameters of the established connection are the preferred ones.
        // If not then schedule a connection parameter update request.
        if ((param->con_interval < user_connection_param_conf.intv_min) ||
            (param->con_interval > user_connection_param_conf.intv_max) ||
            (param->con_latency != user_connection_param_conf.latency) ||
            (param->sup_to != user_connection_param_conf.time_out))
        {
            // Connection params are not these that we expect
            app_param_update_request_timer_used = app_easy_timer(APP_PARAM_UPDATE_REQUEST_TO, param_update_request_timer_cb);
        }
    }
    else
    {
        // No connection has been established, restart advertising
        user_app_adv_start();
    }

    default_app_on_connection(connection_idx, param);
}

/**
 *********************************************************************************************
 * @brief User defined function to handle the completion of undirected advertisement.
 * @param[in] status    Status code
 * @return void
 *********************************************************************************************
 */
void user_app_adv_undirect_complete(uint8_t status)
{
    // If advertising was canceled then update advertising data and start advertising again
    if (status == GAP_ERR_CANCELED)
    {
        user_app_adv_start();
    }
}

/**
 *********************************************************************************************
 * @brief User defined function which which will be called when the device closes a connection
 * @param[in] param Pointer to GAPC_DISCONNECT_IND message
 * @return void
 *********************************************************************************************
 */
void user_app_disconnect(struct gapc_disconnect_ind const *param)
{
	  GPIO_SetInactive(GPIO_LED_PORT, GPIO_LED_PIN);
    // Cancel the parameter update request timer
    if (app_param_update_request_timer_used != EASY_TIMER_INVALID_TIMER)
    {
        app_easy_timer_cancel(app_param_update_request_timer_used);
        app_param_update_request_timer_used = EASY_TIMER_INVALID_TIMER;
    }
		
    // Stop the notification timers
    if(X_timer != EASY_TIMER_INVALID_TIMER){
        app_easy_timer_cancel(X_timer);
        X_timer = EASY_TIMER_INVALID_TIMER;
    }
    if(Y_timer != EASY_TIMER_INVALID_TIMER){
        app_easy_timer_cancel(Y_timer);
        Y_timer = EASY_TIMER_INVALID_TIMER;
    }
    if(Z_timer != EASY_TIMER_INVALID_TIMER){
        app_easy_timer_cancel(Z_timer);
        Z_timer = EASY_TIMER_INVALID_TIMER;
    }
    if(g_timer != EASY_TIMER_INVALID_TIMER){
        app_easy_timer_cancel(g_timer);
        g_timer = EASY_TIMER_INVALID_TIMER;
    }
			
    // Update manufacturer data for the next advertsing event
    mnf_data_update();
    // Restart Advertising
    user_app_adv_start();
}

/**
 ****************************************************************************************
 * @brief Helper function to convert a raw measurement to a string.
 * @param[in]  input   Input raw measurement value
 * @param[out] s       Pointer to the output string
 * @return Length of string
 ****************************************************************************************
 */
uint8_t user_int_to_string(int16_t input, uint8_t *s){
	uint8_t length = 1;
	if(input < 0){
		s[0] = '-';
	} else {
		s[0] = ' ';
	}
	input = abs(input);
	if(input  >= 10000){
		s[length++] = '0' + ((input / 10000) % 10);
	}
	if(input  >= 1000){
		s[length++] = '0' + ((input / 1000) % 10);
	}
	if(input  >= 100){
		s[length++] = '0' + ((input / 100) % 10);
	}
	if(input  >= 10){
		s[length++] = '0' + ((input / 10) % 10);
	}
	
	s[length++] = '0' + (input% 10);
	return length;
}

/**
 ****************************************************************************************
 * @brief User defined function to send a notification of the X-axis acceleration value
 * @return void
 ****************************************************************************************
 */
void user_svc1_accel_X_send_ntf()
{	
	  int8_t MSB , LSB = 0;
	
	 HMC5883L_readXYZ();
      
	  MSB = (X_data & 0xFF00) >> 8;
	  LSB = (X_data & 0x00FF) >> 0;
	  X_string[0] = MSB;
	  X_string[1] = LSB;
	  MSB = (Y_data & 0xFF00) >> 8;
	  LSB = (Y_data & 0x00FF) >> 0;
	  X_string[2] = MSB;
	  X_string[3] = LSB;
	  MSB = (Z_data & 0xFF00) >> 8;
	  LSB = (Z_data & 0x00FF) >> 0;
	  X_string[4] = MSB;
	  X_string[5] = LSB;	
	
    //Allocate a new message
    struct custs1_val_ntf_ind_req* req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,                   //Message id
                                                          prf_get_task_from_id(TASK_ID_CUSTS1), //Target task
                                                          TASK_APP,                             //Source of the message
                                                          custs1_val_ntf_ind_req,               //The type of structure in the message,
                                                          6);                       //How many bytes of data will be added

    //Initialize message fields
    req->conidx = 0;
    req->notification = true;
    req->handle = SVC1_IDX_DIGITAL_COMPASS_DATA_VAL;
    req->length = 6;
    memcpy(req->value, X_string, 6);

    //Send the message to the task
    ke_msg_send(req);
    
    //Set a timer for 100 ms (10*10)
    X_timer = app_easy_timer(NOTIFICATION_DELAY / 10, user_svc1_accel_X_send_ntf);
}

/**
 ****************************************************************************************
 * @brief User defined function which handles notification enablement/disablement
 *        of X-axis acceleration
 * @param[in] param   Pointer to a struct which delivers the enabling/disabling of
 *                    notifications
 * @return void
 ****************************************************************************************
 */
void user_svc1_accel_X_wr_ntf_handler(struct custs1_val_write_ind const *param)
{
    //Check if the client has subscribed to notifications
	if(param->value[0])
    {
		//Start the timer if it's not running
		if(X_timer == EASY_TIMER_INVALID_TIMER)
        {
            X_timer = app_easy_timer(10, user_svc1_accel_X_send_ntf);
		}
	}
	else
    {
		//If the client has unsubscribed, invalidate the timer
		if(X_timer != EASY_TIMER_INVALID_TIMER)
        {
			app_easy_timer_cancel(X_timer);
			X_timer = EASY_TIMER_INVALID_TIMER;
		}
	}
}

/**
 ****************************************************************************************
 * @brief User defined function to send a notification of the Y-axis acceleration value
 * @return void
 ****************************************************************************************
 */
void user_svc1_accel_Y_send_ntf()
{
    //Construct the string to send as a notification
//    uint8_t string_length = user_int_to_string(HMC5883L_read_Y() * 3.9, Y_string);               //Read data and multipy by 3.9 to get acceleration in mg
//	
//    struct custs1_val_ntf_ind_req* req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
//                                                          prf_get_task_from_id(TASK_ID_CUSTS1),
//                                                          TASK_APP,
//                                                          custs1_val_ntf_ind_req,
//                                                          string_length);
//    //Initialize message fields
//    req->conidx = 0;
//    req->notification = true;
//    req->handle = SVC1_IDX_ACCEL_Y_DATA_VAL;
//    req->length = string_length;
//    memcpy(req->value, Y_string, string_length);

//    //Send the message to the task
//    ke_msg_send(req);
//    
//    //Set a timer for 100 ms (10*10)
//    Y_timer = app_easy_timer(NOTIFICATION_DELAY / 10, user_svc1_accel_Y_send_ntf);
}

/**
 ****************************************************************************************
 * @brief User defined function which handles notification enablement/disablement
 *        of Y-axis acceleration
 * @param[in] param   Pointer to a struct which delivers the enabling/disabling of
 *                    notifications
 * @return void
 ****************************************************************************************
 */
void user_svc1_accel_Y_wr_ntf_handler(struct custs1_val_write_ind const *param)
{
    //Check if the client has subscribed to notifications
//    if(param->value[0])
//    {
//        //Start the timer if it's not running
//        if(Y_timer == EASY_TIMER_INVALID_TIMER)
//        {
//            Y_timer = app_easy_timer(10, user_svc1_accel_Y_send_ntf);
//		}
//	}
//	else
//    {
//        //If the client has unsubscribed, invalidate the timer
//        if(Y_timer != EASY_TIMER_INVALID_TIMER)
//        {
//			app_easy_timer_cancel(Y_timer);
//			Y_timer = EASY_TIMER_INVALID_TIMER;
//		}
//	}
}

/**
 ****************************************************************************************
 * @brief User defined function to send a notification of the Z-axis acceleration value
 * @return void
 ****************************************************************************************
 */
void user_svc1_accel_Z_send_ntf(void)
{
    //Construct the string to send as a notification
//    uint8_t string_length = user_int_to_string(HMC5883L_read_Z() * 3.9, Z_string);               //Read data and multipy by 3.9 to get acceleration in mg
//    
//    struct custs1_val_ntf_ind_req* req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
//                                                          prf_get_task_from_id(TASK_ID_CUSTS1),
//                                                          TASK_APP,
//                                                          custs1_val_ntf_ind_req,
//                                                          string_length);
//    //Initialize message fields
//    req->conidx = 0;
//    req->notification = true;
//    req->handle = SVC1_IDX_ACCEL_Z_DATA_VAL;
//    req->length = string_length;
//    memcpy(req->value, Z_string, string_length);

//    //Send the message to the task
//    ke_msg_send(req);
//    
//    //Set a timer for 100 ms (10*10)
//    Z_timer = app_easy_timer(NOTIFICATION_DELAY / 10, user_svc1_accel_Z_send_ntf);
}

/**
 ****************************************************************************************
 * @brief User defined function which handles notification enablement/disablement
 *        of Z-axis acceleration
 * @param[in] param   Pointer to a struct which delivers the enabling/disabling of
 *                    notifications
 * @return void
 ****************************************************************************************
 */
void user_svc1_accel_Z_wr_ntf_handler(struct custs1_val_write_ind const *param)
{
    //Check if the client has subscribed to notifications
//	if(param->value[0])
//    {
//        //Start the timer if it's not running
//        if(Z_timer == EASY_TIMER_INVALID_TIMER)
//        { 
//			Z_timer = app_easy_timer(10, user_svc1_accel_Z_send_ntf);
//		}
//	}
//	else
//    {
//        //If the client has unsubscribed, invalidate the timer
//		if(Z_timer != EASY_TIMER_INVALID_TIMER)
//        {
//			app_easy_timer_cancel(Z_timer);
//			Z_timer = EASY_TIMER_INVALID_TIMER;
//		}
//	}
}

/**
 ****************************************************************************************
 * @brief User defined function called when the g timer fires.
 * @return void
 ****************************************************************************************
 */
void user_svc2_g_timer_cb_handler(void)
{
//    struct custs1_val_ntf_ind_req *req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
//                                                          prf_get_task_from_id(TASK_ID_CUSTS1),
//                                                          TASK_APP,
//                                                          custs1_val_ntf_ind_req,
//                                                          DEF_SVC2_G_DATA_CHAR_LEN);
//    uint8_t xyz[DEF_SVC2_G_DATA_CHAR_LEN];
//    
//    //Read 3-axes measurement
//    HMC5883L_readXYZYZ(xyz);
//    
//    //Initialize message fields
//    req->handle = SVC2_IDX_G_DATA_VAL;
//    req->length = DEF_SVC2_G_DATA_CHAR_LEN;
//    req->notification = true;
//    memcpy(req->value, xyz, DEF_SVC2_G_DATA_CHAR_LEN);
//    
//    //Send the message to the task
//    ke_msg_send(req);
//    
//    //Set a timer for 100 ms (10*10)
//    g_timer = app_easy_timer(NOTIFICATION_DELAY / 10, user_svc2_g_timer_cb_handler);
}

/**
 ****************************************************************************************
 * @brief User defined function which handles notification enablement/disablement
 * @param[in] param   Pointer to a struct which delivers the enabling/disabling of
 *                    notifications
 * @return void
 ****************************************************************************************
 */
void user_svc2_g_wr_ntf_handler(struct custs1_val_write_ind const *param)
{
    //Check if the client has subscribed to notifications
//	if(param->value[0])
//    {
//        //Start the timer if it's not running
//        if(g_timer == EASY_TIMER_INVALID_TIMER)
//        {
//			g_timer = app_easy_timer(10, user_svc2_g_timer_cb_handler);
//		}
//	}
//	else
//    {
//        //If the client has unsubscribed, invalidate the timer
//        if(g_timer != EASY_TIMER_INVALID_TIMER)
//        {
//			app_easy_timer_cancel(g_timer);
//			g_timer = EASY_TIMER_INVALID_TIMER;
//		}
//	}
}

/**
 ****************************************************************************************
 * @brief User defined function which processes Write Notifications.
 * @param[in] msgid   Type of the message
 * @param[in] param   Pointer to the message to be processed
 * @param[in] dest_id Destination task id
 * @param[in] src_id  Source task id
 * @return void
 ****************************************************************************************
 */
void user_catch_rest_hndl(ke_msg_id_t const msgid,
                          void const *param,
                          ke_task_id_t const dest_id,
                          ke_task_id_t const src_id)
{
    switch(msgid)
    {		
		case CUSTS1_VAL_WRITE_IND:
        {
            struct custs1_val_write_ind const *msg_param = (struct custs1_val_write_ind const *)(param);

            switch (msg_param->handle)
            {
                case SVC1_IDX_DIGITAL_COMPASS_NTF_CFG:
                    user_svc1_accel_X_wr_ntf_handler(msg_param);
                    break;

                case SVC1_IDX_ACCEL_Y_NTF_CFG:
                    user_svc1_accel_Y_wr_ntf_handler(msg_param);
                    break;

                case SVC1_IDX_ACCEL_Z_NTF_CFG:
                    user_svc1_accel_Z_wr_ntf_handler(msg_param);
                    break;

                case SVC2_IDX_G_NTF_CFG:
                    user_svc2_g_wr_ntf_handler(msg_param);
                    break;
								
                default:
                    break;
            }
        } break;
				
		case CUSTS1_VAL_NTF_CFM:
        {
            struct custs1_val_ntf_cfm const *msg_param = (struct custs1_val_ntf_cfm const *)(param);

            switch (msg_param->handle)
            {
                case SVC1_IDX_DIGITAL_COMPASS_DATA_VAL:
                    break;

                case SVC1_IDX_ACCEL_Y_DATA_VAL:
                    break;

                case SVC1_IDX_ACCEL_Z_DATA_VAL:
                    break;

                default:
                    break;
            }
        } break;
        
        default:
            break;
    }
}

/// @} APP
