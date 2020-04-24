

#include "monitor.h"





stats_t stats;


void f_create_vehicle_monitor_task() {
	RTOS_ERR err;
	f_monitor_task_init();
	OSTmrStart(&monitor_task_timer, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	OSTaskCreate(&FVehicleMonitorTaskTCB,                          /* Create the Monitor Task.                               */
				 "FMonitor Task",
				 FVehicleMonitorTask,
				  DEF_NULL,
				  FVEHICLE_MONITOR_TASK_PRIO,
				 &FVehicleMonitorTaskStk[0],
				 (FVEHICLE_MONITOR_TASK_STK_SIZE / 10u),
				 FVEHICLE_MONITOR_TASK_STK_SIZE,
				  0u,
				  0u,
				  DEF_NULL,
				 (OS_OPT_TASK_STK_CLR),
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void f_delete_vehicle_monitor_task() {
	RTOS_ERR err;
	// clean up to reset state
	//undo init, basically
	OSTmrDel(&monitor_task_timer, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	OSTmrDel(&LED0_timer, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	OSTmrDel(&LED1_timer, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	OSSemDel(&monitor_wakeup_sem, OS_OPT_DEL_NO_PEND, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	OSTaskDel(&FVehicleMonitorTaskTCB, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}



void f_create_monitor_task_timer() {
	RTOS_ERR err;
	//Create Timer
	OSTmrCreate(&monitor_task_timer,
				"Monitor Task Timer",
				0,
				1,						//1* 100ms = 100ms -> 10Hz
				OS_OPT_TMR_PERIODIC,
				(OS_TMR_CALLBACK_PTR)MonitorTimerCallback,
				DEF_NULL,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void f_monitor_task_init() {
	RTOS_ERR err;
    stats.max_speed = 0;
    stats.time_on_course = 0;
    stats.success = false;

    OSSemCreate(&monitor_wakeup_sem,
    			"Monitor Wakeup Sem", 0,
				 &err);
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
    f_create_monitor_task_timer();

	return;
}



void FVehicleMonitorTask(void * p_arg) {
	RTOS_ERR err;
	int_vect_t active_wp;
	bool ret;
	float slip, friction, accel_cent;
	while(1) {

		OSSemPend(&monitor_wakeup_sem, 0,OS_OPT_PEND_BLOCKING,0,&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

        //check slip parameter
//		slip = friction - accel_cent;
        //send warning to LED sem if warning
		if (slip < 0) {
			//send game_over sem to Menu if game over
			OSFlagPost(&game_over_flag,
					FAIL_SLIP,
					OS_OPT_POST_FLAG_SET,
					&err);
					APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		}
		else if (slip < MONITOR_SLIP_THRESHOLD) {
			//start LED toggling
			OSTmrStart(&LED0_timer, &err);
			APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		}


        //check waypoint
		ret = InputFifo2_Peek(&WayPointBuffer, 0, &active_wp);
		if (ret == false) {
			//means the buffer is empty, means we reached all our goals
			OSFlagPost(&game_over_flag,
					FAIL_NO_FAIL,
					OS_OPT_POST_FLAG_SET,
					&err);
			APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		}


        //send warning to LED sem if warning
        //send game_over sem if game over.
		//send new_waypoint sem to Road_gen if success
		//if last waypoint, send game_over to Menu, success

		//check speed against max speed, update
		if (vect_mag(vehicle_model.v) > stats.max_speed) {
			stats.max_speed = vect_mag(vehicle_model.v);
		}
		//TODO: check timing
		stats.time_on_course += .1;

		//

	} //end while
} // end monitor task

void MonitorTimerCallback (void ) {
	RTOS_ERR err;
	OSSemPost(&monitor_wakeup_sem,
			   OS_OPT_POST_1, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	return;
}

void f_create_monitor_timers(void) {
	RTOS_ERR err;

	OSTmrCreate(&LED0_timer, "Offroad Alert LED Timer", 0,
				5, OS_OPT_TMR_PERIODIC, LED0TimerCallback, 0, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	OSTmrCreate(&LED0_timer, "Offroad Alert LED Timer", 0,
					5, OS_OPT_TMR_PERIODIC, LED0TimerCallback, 0, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void LED0TimerCallback(void) {
	GPIO_PinOutToggle(LED0_port, LED0_pin);
}

void LED1TimerCallback(void) {
	GPIO_PinOutToggle(LED1_port, LED1_pin);
}
