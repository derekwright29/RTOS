#include "road_gen.h"


/**
 * GLOBAL VARIABLES
 */


const int_vect_t Sparse_R[20] = SPARSE_R_WAYPOINTS;
const int_vect_t FlyingQ[100] = FLYING_Q_WAYPOINTS;

const float Sparse_R_Headings[20] = SPARSE_R_HEADINGS;

road_description_t course = ROAD_DESC_SPARSE_R_DEFAULT;
float course_headings[MAX_NUM_WAYPOINTS];
road_condition_t road_cond = ROAD_CONDITION_DEFAULT;
uint16_t waypoint_index;

//
InputFifo2_t WayPointBuffer;
static RTOS_ERR  glob_err;

void RoadGenTask(void *p_arg) {
	RTOS_ERR err;
	int_vect_t ret; //dummy var

	// should be ROAD_GEN_WP_BUFFER_SIZE - 1
	waypoint_index = InputFifo2_getNumItems(&WayPointBuffer);
	while(1) {
//		wait for need for new waypoint
		 OSSemPend(&new_waypoint_sem,
		 			0,
		 			OS_OPT_PEND_BLOCKING,
		 			(CPU_TS*)0,
		 			&err);
		 APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		if (waypoint_index >= course.NumWaypoints){
			InputFifo2_Get(&WayPointBuffer, (InputValue2_t* )&ret);
			//keep incrementing for the sake of the LCD task
			waypoint_index++;
			//skip everything else
			continue;
		}
		//clear old wp
		InputFifo2_Get(&WayPointBuffer, (InputValue2_t* )&ret);
		while (InputFifo2_getNumItems(&WayPointBuffer) < ROAD_GEN_WP_BUFFER_SIZE) {
			//add new wp
			InputFifo2_Put(&WayPointBuffer, (InputValue2_t) course.Waypoints[waypoint_index++]);
		}


		// TODO: remove this. This will Pend on the Monitor task eventually, and then LCD won't care, it'll just
		// draw whatever is in the FIFO.
		// post to lcd task.
//		OSSemPost(&phys_model_update_sem, OS_OPT_POST_ALL, &err);
//		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

		//delay
//		OSTimeDlyHMSM(0,1,0,0,(OS_OPT_TIME_PERIODIC | OS_OPT_TIME_HMSM_STRICT), &err);
//		OSTimeDly((OS_TICK)(10.*1000.), OS_OPT_TIME_DLY, &err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	}// end while(1)
	OSTaskDel(&RoadGenTaskTCB, &glob_err);
	APP_RTOS_ASSERT_CRITICAL(glob_err.Code == RTOS_ERR_NONE, ;);
	return;
}


void create_roadgen_task() {
	RTOS_ERR err;

	roadgen_init();

	OSTaskCreate(&RoadGenTaskTCB,                          /* Create the Road Gen Task.                               */
				 "Road Gen Task",
				  RoadGenTask,
				  DEF_NULL,
				  ROADGEN_TASK_PRIO,
				 &RoadGenTaskStk[0],
				 (ROADGEN_TASK_STK_SIZE / 10u),
				 ROADGEN_TASK_STK_SIZE,
				  0u,
				  0u,
				  DEF_NULL,
				 (OS_OPT_TASK_STK_CLR),
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void delete_roadgen_task() {
	RTOS_ERR err;

	OSSemDel(&new_waypoint_sem, OS_OPT_DEL_ALWAYS, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	OSTaskDel(&RoadGenTaskTCB, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}


void roadgen_init(void ) {
	InputFifo2_resetFifo(&WayPointBuffer);
	waypoint_index = 0;
	create_roadgen_sems();
//	create_roadgen_test_timer();
	while (InputFifo2_getNumItems(&WayPointBuffer) < ROAD_GEN_WP_BUFFER_SIZE) {
		//init/fill waypoint buffer for LCD
		InputFifo2_Put(&WayPointBuffer, course.Waypoints[waypoint_index++]);
	}
//	OSTmrStart(&road_gen_test_timer, &err);
//	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void create_roadgen_sems(void ) {
	RTOS_ERR err;
	//Create Model Sem
	OSSemCreate(&new_waypoint_sem,
				"New Waypoint Semaphore",
				0,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	return;
}

void create_roadgen_test_timer(void) {
	RTOS_ERR err;
	OSTmrCreate(&road_gen_test_timer,
					"Road Gen Timer",
					0,				// hope this is 5 seconds. 50 * 100ms/tmr_tick
					50,
					OS_OPT_TMR_PERIODIC,
					(OS_TMR_CALLBACK_PTR)RoadGenTimerCallback,
					DEF_NULL,
					&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}


// only used when monitor task does not operate
void RoadGenTimerCallback(void) {
	RTOS_ERR err;
	OSSemPost(&new_waypoint_sem,
				OS_OPT_POST_1,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}
