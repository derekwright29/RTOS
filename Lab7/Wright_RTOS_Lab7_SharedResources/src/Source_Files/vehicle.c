/*
 * vehicle.c
 *
 *  Created on: Mar 2, 2020
 *      Author: goat
 */
#include "fifo.h"
#include "vehicle.h"
#include "mycapsense.h"

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
	// Init data structure
	vehicle_speed.dec_cnt = 0;
	vehicle_speed.inc_cnt = 0;
	vehicle_speed.speed_cnt = 0;
	vehicle_speed.speed = 5*vehicle_speed.speed_cnt;
	//Create Flag
	OSFlagCreate(&speed_flags,
				 "Speed Flag Group",
				 0,
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	//Create Sem
	OSSemCreate(&speed_change_sem,
				"Speed Update Semaphore",
				0,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	//Create Mutex
	OSMutexCreate(&speed_mutex,
				"Speed Data Mutex",
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}


void VehicleSpeedTask(void * p_arg) {
	RTOS_ERR err;
	speed_task_init();
	InputValue_t ret;
	while(1){
		//wait for speed change. Else sleep
		OSSemPend(&speed_change_sem,
				  0,
				  OS_OPT_PEND_BLOCKING,
				  (CPU_TS*)0,
				  &err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

		//acquire mutex to allow us to change vehicle_speed struct
		OSMutexPend(&speed_mutex,
				    0,
					OS_OPT_PEND_BLOCKING,
					(CPU_TS*)0,
					&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

		//empty Button 0 FIFO. Practically there will likely be either 0 or 1 items
		while(!InputFifo_isEmpty((InputFifo_t *)&FIFO_Button0)) {
			InputFifo_Get((InputFifo_t *)&FIFO_Button0, &ret);
			vehicle_speed.inc_cnt++;
			vehicle_speed.speed_cnt++;
			vehicle_speed.speed = 5*vehicle_speed.speed_cnt;
		}
		//empty Button 1 FIFO. Practically there will likely be either 0 or 1 items
		while(!InputFifo_isEmpty((InputFifo_t *)&FIFO_Button1)) {
			InputFifo_Get((InputFifo_t *)&FIFO_Button1, &ret);
			vehicle_speed.dec_cnt++;
			vehicle_speed.speed_cnt--;
			vehicle_speed.speed = 5*vehicle_speed.speed_cnt;
		}
		// release mutex
		OSMutexPost(&speed_mutex,
					OS_OPT_POST_NONE,		//TODO: test out wiht NO_SCHED. Wait til the flag is set to RESCHEDULE.
					&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

		//send flag to monitor task
		OSFlagPost(&speed_flags,
					SPEED_CHANGED,
					OS_OPT_POST_FLAG_SET,
					&err);
	}

}

void direction_task_init() {
	// create_speed_flag
	RTOS_ERR err;
	//Create Flag
	OSFlagCreate(&dir_flags,
				"Direction Flag Group",
				0,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	OSMutexCreate(&dir_mutex,
				  "Direction Data Mutex",
				  &err);

}

void VehicleDirectionTask(void * p_arg) {
	RTOS_ERR err;
	direction_task_init();
	init_capsense();
	bool cap_state[CSEN_CHANNELS] = {false};
	bool isChanged = false;
	Direction_t cur_dir;
	while(1) {
		sample_capsense();
		if (capsense_isChanged(cap_state)) {
			isChanged = true;
			//decides direction and also updates cap_state... I know...
			cur_dir = decideDirection(cap_state);
		}
		// update direction
//		OSMutexPend(&dir_mutex,
//					0,
//					&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		vehicle_dir.veh_dir = cur_dir;
		vehicle_dir.left_cnt += (cur_dir == DIR_FAR_LEFT || cur_dir == DIR_LEFT);
		vehicle_dir.right_cnt += (cur_dir == DIR_FAR_RIGHT || cur_dir == DIR_RIGHT);

		if (isChanged) {
			vehicle_dir.time_since_change = 0;
			OSTmrStart(&no_change_timer,
					&err);
			APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
//			OSFlagPost(&dir_flags,
//					cur_dir,
//					&err);
			APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
			isChanged = false;
		}
	}
}
Direction_t decideDirection(bool * state_array) {
	uint8_t i;
	for(i = 0; i < CSEN_CHANNELS; i++) {
		state_array[i] = capsenseIsPressed[i];
	}
	if (state_array[0] && !state_array[1] && !state_array[2] && !state_array[3])
		return DIR_FAR_LEFT;
	else if (((uint8_t)state_array[0]+(uint8_t)state_array[1]) > ((uint8_t)state_array[2]+(uint8_t)state_array[3]))
		return DIR_LEFT;
	else if (!state_array[0] && !state_array[1] && !state_array[2] && state_array[3])
		return DIR_FAR_RIGHT;
	else if (((uint8_t)state_array[0]+(uint8_t)state_array[1]) < ((uint8_t)state_array[2]+(uint8_t)state_array[3]))
		return DIR_RIGHT;
	else
		return DIR_STRAIGHT;
}

void init_monitor_task() {
	RTOS_ERR err;
	//Create Timer
	OSTmrCreate(&no_change_timer,
				"Alert Timer",
				100,
				0,
				OS_OPT_TMR_ONE_SHOT,
				(OS_TMR_CALLBACK_PTR)AlertTimerCallback,
				DEF_NULL,
				&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);


}

void AlertTimerCallback() {
	RTOS_ERR err;
//	OSFlagPost(&alert_flags,
//			ALERT_DIRECTION_TIMEOUT,
//			&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void VehicleMonitorTask(void * p_arg) {
	RTOS_ERR err;
	bool got_dir = false;
	bool got_spd = false;
	init_monitor_task();
	OSTmrStart(&no_change_timer,
			&err);
	Direction_t cur_dir = 0;
	SpeedFlag_t cur_spd = 0;

	while(1) {
		cur_dir = OSFlagPend(&dir_flags,
							0xFF,
							0,
							OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME + OS_OPT_PEND_NON_BLOCKING,
							DEF_NULL,
							&err);
		/*   Check error code.                           */
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		if (cur_dir) {
			got_dir = true;
		}
		cur_spd = OSFlagPend(&speed_flags,
						SPEED_CHANGED,
						0,
						OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME + OS_OPT_PEND_NON_BLOCKING,
						DEF_NULL,
						&err);
		/*   Check error code.                           */
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		if (cur_spd){
			got_spd = true;
		}
		if (got_dir) {
			//acquire mutex
			//update  direction data struct
			//release mutex
		}
		if (got_spd) {
			//acquire mutex
			//update speed
			//release mutex
		}
		if (got_spd || got_dir) {
			//check for speed/dir errors
			//post to alert flags if conditions met.
		}



	}

}
