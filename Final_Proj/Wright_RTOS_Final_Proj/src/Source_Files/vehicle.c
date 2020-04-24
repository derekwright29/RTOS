/*
 * vehicle.c
 *
 *  Created on: Mar 2, 2020
 *      Author: goat
 */
#include <stdint.h>
#include "fifo.h"
#include "vehicle.h"
#include "mycapsense.h"
#include <common/include/rtos_utils.h>

void create_vehicle_semaphores() {
	RTOS_ERR err;

	//Create Model Sem
	OSSemCreate(&phys_model_update_sem,
				"Model Update Semaphore",
				0,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	//Create Speed Sem
	OSSemCreate(&button_sem,
				"Accel Update Semaphore",
				0,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

}

void create_vehicle_mutexes() {
	RTOS_ERR err;

	//Create Speed Mutex
	OSMutexCreate(&accel_mutex,
				"Accel Data Mutex",
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	//Create Dir Mutex
	OSMutexCreate(&turn_mutex,
				  "Turn Data Mutex",
				  &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);


}

void create_vehicle_flags() {
	RTOS_ERR err;

	//Create Speed Flag
	OSFlagCreate(&accel_flags,
				 "Accel Flag Group",
				 0,
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	//Create Dir Flag
	OSFlagCreate(&turn_flags,
				"Direction Flag Group",
				0,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);



	//Create Alert Flag
	OSFlagCreate(&alert_flags,
				 "Alerts Flag Group",
				 0,
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

}
