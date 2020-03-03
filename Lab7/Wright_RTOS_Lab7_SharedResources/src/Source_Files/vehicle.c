/*
 * vehicle.c
 *
 *  Created on: Mar 2, 2020
 *      Author: goat
 */

#include "vehicle.h"

void create_vehicle_speed_task() {
	RTOS_ERR err;
	OSTaskCreate(&SpeedTaskTCB,                          /* Create the Speed Task.                               */
				 "Speed Setpoint Task",
				  VehicleSpeedTask,
				  DEF_NULL,
				  SPEED_TASK_PRIO,
				 &SpeedTaskStk[0],
				 (SPEED_TASK_STK_SIZE / 10u),
				 SPEED_TASK_STK_SIZE,
				  0u,
				  0u,
				  DEF_NULL,
				 (OS_OPT_TASK_STK_CLR),
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void create_vehicle_dir_task() {
	RTOS_ERR err;
	OSTaskCreate(&VehicleMonitorTaskTCB,                          /* Create the Direction Task.                               */
				 "Direction Task",
				 VehicleDirectionTask,
				  DEF_NULL,
				  DIRECTION_TASK_PRIO,
				 &VehicleMonitorTaskStk[0],
				 (DIRECTION_TASK_STK_SIZE / 10u),
				 DIRECTION_TASK_STK_SIZE,
				  0u,
				  0u,
				  DEF_NULL,
				 (OS_OPT_TASK_STK_CLR),
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void create_vehicle_monitor_task() {
	RTOS_ERR err;
	OSTaskCreate(&VehicleMonitorTaskTCB,                          /* Create the Monitor Task.                               */
				 "Monitor Task",
				 VehicleMonitorTask,
				  DEF_NULL,
				  VEHICLE_MONITOR_TASK_PRIO,
				 &VehicleMonitorTaskStk[0],
				 (VEHICLE_MONITOR_TASK_STK_SIZE / 10u),
				 VEHICLE_MONITOR_TASK_STK_SIZE,
				  0u,
				  0u,
				  DEF_NULL,
				 (OS_OPT_TASK_STK_CLR),
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void speed_task_init(){
	// create_speed_flag
	RTOS_ERR err;
	//Create Flag
	OSFlagCreate(&speed_flags,
				 "Speed Flag Group",
				 0,
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	//Create Sem
	OSSemCreate(&speed_sem,
				"Speed Update Semaphore",
				0,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void VehicleSpeedTask(void * p_arg) {
	speed_task_init();


	while(1){

	}

}

void vehicle_task_init() {
	// create_speed_flag
	RTOS_ERR err;
	//Create Flag
	OSFlagCreate(&dir_flags,
				"Direction Flag Group",
				0,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

}

void VehicleDirectionTask(void * p_arg) {


	while(1) {

	}


}

void VehicleMonitorTask(void * p_arg) {

}
