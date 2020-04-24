

#include "monitor.h"
#include <math.h>





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
    f_create_monitor_timers();

	return;
}



void FVehicleMonitorTask(void * p_arg) {
	RTOS_ERR err;
	int_vect_t active_wp;
	bool ret;
	bool course_active = false;
	bool led1_blinking = false;
	bool led0_blinking = false;
	float slip=.1, friction, accel_cent, gate_heading;
	while(1) {

		OSSemPend(&monitor_wakeup_sem, 0,OS_OPT_PEND_BLOCKING,0,&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

        //check slip parameter
		//TODO: implement
//		slip = friction - accel_cent;
        //send warning to LED sem if warning
		if (slip < 0) {
			//send game_over sem to Menu if game over
			GPIO_PinOutSet(LED0_port, LED0_pin);
			OSFlagPost(&game_over_flag,
					FAIL_SLIP,
					OS_OPT_POST_FLAG_SET,
					&err);
			APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		}
		else if (slip < MONITOR_SLIP_THRESHOLD) {
			//start LED toggling
			if(!led0_blinking) {
				OSTmrStart(&LED0_timer, &err);
				APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
			}
			led0_blinking = true;
		}
		else {
			if (led0_blinking) {
				OSTmrStop(&LED0_timer, OS_OPT_TMR_NONE, 0,&err);
				APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
			}
			led0_blinking = false;
			GPIO_PinOutClear(LED0_port, LED0_pin);
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

		gate_heading = course_headings[waypoint_index-ROAD_GEN_WP_BUFFER_SIZE];
		if (gate_heading < M_PI_2 && gate_heading >= 0.) {
			//quadrant 1
			//second part of this if statement is limitting the window we can activate, so that it is not a pillar that extends to end of screen.
			if (vehicle_model.p.y < (tan(gate_heading)*(vehicle_model.p.x - active_wp.x) + active_wp.y) && (vehicle_model.p.y > (tan(gate_heading)*(vehicle_model.p.x - active_wp.x) + active_wp.y) - 5)) {
				// means y has crossed gate, now check x
				if((vehicle_model.p.x > (active_wp.x - cos(gate_heading)*course.RoadWidth/2)) && (vehicle_model.p.x < (active_wp.x + cos(gate_heading)*course.RoadWidth/2))){
					if(!course_active)
						course_active = true;
					// within bounds of gate. success, post to road_gen
					OSSemPost(&new_waypoint_sem, OS_OPT_POST_ALL, &err);
					APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
				}
				else {
					if (course_active) {
						OSFlagPost(&game_over_flag, FAIL_OFFROAD, OS_OPT_POST_FLAG_SET, &err);
						APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
						GPIO_PinOutSet(LED1_port, LED1_pin);
					}
				}
			}

		}
		else if (gate_heading >= M_PI_2 && gate_heading < M_PI) {
			//quadrant 2
			if (vehicle_model.p.x > ((cos(gate_heading)*(vehicle_model.p.y - active_wp.y)/sin(gate_heading))+active_wp.x) && (vehicle_model.p.x < ((cos(gate_heading)*(vehicle_model.p.y - active_wp.y)/sin(gate_heading))+active_wp.x) + 5)) {
				// we are "right of gate", check if y is in bounds
				if ((vehicle_model.p.y > (active_wp.y - sin(gate_heading)*course.RoadWidth/2)) && (vehicle_model.p.y < (active_wp.y + sin(gate_heading)*course.RoadWidth/2))) {
					if(!course_active)
						course_active = true;
					// within bounds of gate. success, post to road_gen
					OSSemPost(&new_waypoint_sem, OS_OPT_POST_ALL, &err);
					APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
				}
				else {
					if(course_active) {
						OSFlagPost(&game_over_flag, FAIL_OFFROAD, OS_OPT_POST_FLAG_SET, &err);
						APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
						GPIO_PinOutSet(LED1_port, LED1_pin);
					}
				}
			}
		}
		if ((gate_heading < (3*M_PI_2)) && (gate_heading >= M_PI)) {
			//quadrant 3
			if (vehicle_model.p.y > (tan(gate_heading)*(vehicle_model.p.x - active_wp.x) + active_wp.y) && (vehicle_model.p.y < (tan(gate_heading)*(vehicle_model.p.x - active_wp.x) + active_wp.y) + 5)) {
				// means y has crossed gate, now check x
				if((vehicle_model.p.x > (active_wp.x + cos(gate_heading)*course.RoadWidth/2)) && (vehicle_model.p.x < (active_wp.x - cos(gate_heading)*course.RoadWidth/2))){
					if(!course_active)
						course_active = true;
					// within bounds of gate. success, post to road_gen
					OSSemPost(&new_waypoint_sem, OS_OPT_POST_ALL, &err);
					APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
				}
				else {
					if (course_active) {
						OSFlagPost(&game_over_flag, FAIL_OFFROAD, OS_OPT_POST_FLAG_SET, &err);
						APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
						GPIO_PinOutSet(LED1_port, LED1_pin);
					}
				}
			}

		}
		else if (gate_heading >= 3*M_PI_2 && gate_heading < M_TWOPI) {
			//quadrant 4
			if ((vehicle_model.p.x < ((cos(gate_heading)*(vehicle_model.p.x - active_wp.y)/sin(gate_heading))+active_wp.x)) && (vehicle_model.p.x > ((cos(gate_heading)*(vehicle_model.p.x - active_wp.y)/sin(gate_heading))+active_wp.x) - 5)) {
				// we are "right of gate", check if y is in bounds
				if ((vehicle_model.p.y > (active_wp.y + sin(gate_heading)*course.RoadWidth/2)) && (vehicle_model.p.y < (active_wp.y - sin(gate_heading)*course.RoadWidth/2))) {
					if(!course_active)
						course_active = true;
					// within bounds of gate. success, post to road_gen
					OSSemPost(&new_waypoint_sem, OS_OPT_POST_ALL, &err);
					APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
				}
				else {
					if(course_active) {
						OSFlagPost(&game_over_flag, FAIL_OFFROAD, OS_OPT_POST_FLAG_SET, &err);
						APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
						GPIO_PinOutSet(LED1_port, LED1_pin);
					}
				}
			}
		}

		float distance = fabs(cos(gate_heading)*(active_wp.x - vehicle_model.p.x) + sin(gate_heading)*(vehicle_model.p.y - active_wp.y));
		if (distance > MONITOR_DISTANCE_THRESHOLD) {
			//start LED blink
			if (!led1_blinking) {
				OSTmrStart(&LED1_timer, &err);
				APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
			}
			led1_blinking = true;
		}
		else {
			if(led1_blinking) {
				OSTmrStop(&LED1_timer, OS_OPT_TMR_NONE, 0, &err);
				APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
			}
			led1_blinking = false;

			GPIO_PinOutClear(LED1_port, LED1_pin);
		}

		//check speed against max speed, update
		if (vect_mag(vehicle_model.v) > stats.max_speed) {
			stats.max_speed = vect_mag(vehicle_model.v);
		}
		//TODO: check timing
		stats.time_on_course += MONITOR_TASK_PERIOD;

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
				5, OS_OPT_TMR_PERIODIC, (OS_TMR_CALLBACK_PTR)LED0TimerCallback, 0, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	OSTmrCreate(&LED1_timer, "Offroad Alert LED Timer", 0,
					5, OS_OPT_TMR_PERIODIC, (OS_TMR_CALLBACK_PTR)LED1TimerCallback, 0, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void LED0TimerCallback(void) {
	GPIO_PinOutToggle(LED0_port, LED0_pin);
}

void LED1TimerCallback(void) {
	GPIO_PinOutToggle(LED1_port, LED1_pin);
}

