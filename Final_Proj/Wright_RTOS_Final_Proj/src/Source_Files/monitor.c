

#include "monitor.h"









void f_create_vehicle_monitor_task() {
	RTOS_ERR err;
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


void f_create_alert_timer() {
	RTOS_ERR err;
	//Create Timer
	OSTmrCreate(&no_change_timer,
				"Alert Timer",
				NO_CHANGE_TICKS,
				0,
				OS_OPT_TMR_ONE_SHOT,
				(OS_TMR_CALLBACK_PTR)AlertTimerCallback,
				DEF_NULL,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void f_monitor_task_init() {
    stats.max_speed = 0;
    stats.time_on_course = 0;
    stats.best_time = 0;
	return;
//	RTOS_ERR err;
//	//Create Timer
//	OSTmrCreate(&no_change_timer,
//				"Alert Timer",
//				100,
//				0,
//				OS_OPT_TMR_ONE_SHOT,
//				(OS_TMR_CALLBACK_PTR)AlertTimerCallback,
//				DEF_NULL,
//				&err);
//		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}




VehicleAlert_t f_vehicle_get_alert(int16_t speed, Direction_t dir, bool isTimeout) {
	VehicleAlert_t alert = 0;
	if (isTimeout) {
		alert |= ALERT_DIRECTION_TIMEOUT;
	}
	if (speed > SPEED_LIMIT || speed < -SPEED_LIMIT) {
		alert |= ALERT_SPEED_LIMIT;
	}
	if ((speed > TURN_SPEED_LIMIT && dir != DIR_STRAIGHT) || (speed < -TURN_SPEED_LIMIT && dir != DIR_STRAIGHT)) {
		alert |= ALERT_TURN_SPEED_LIMIT;
	}
	return (alert == 0) ? ALERT_NO_ALERT : alert;
}

void FAlertTimerCallback() {
	RTOS_ERR err;
	OSFlagPost(&f_dir_flags,
			DIR_TIMEOUT,
			OS_OPT_POST_FLAG_SET,
			&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void FVehicleMonitorTask(void * p_arg) {
	RTOS_ERR err;

	while(1) {

        //check slip parameter
        //send warning to LED sem if warning
        //send game_over sem if game over

        //check waypoint
        //send warning to LED sem if warning
        //send game_over sem if game over.

		OSTimeDly(MONITOR_TASK_PERIOD_TICKS,
				OS_OPT_TIME_DLY,
					&err);
	} //end while
} // end monitor task
