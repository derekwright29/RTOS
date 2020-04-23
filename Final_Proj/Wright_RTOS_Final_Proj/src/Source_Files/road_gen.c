#include "road_gen.h"


/**
 * GLOBAL VARIABLES
 */


const int_vect_t Sparse_R[20] = SPARSE_R_WAYPOINTS;
const int_vect_t DenseR[100] = DENSE_R_WAYPOINTS;

road_description_t course = ROAD_DESC_SPARSE_R_DEFAULT;
road_condition_t road_cond = ROAD_CONDITION_DEFAULT;

//
InputFifo2_t WayPointBuffer;
int_vect_t ret;

void RoadGenTask(void *p_arg) {
	RTOS_ERR err;

	uint16_t waypoint_index = 0;
	while(1) {
		//wait for need for new waypoint
		// OSSemPend(&new_waypoint_sem,
		// 			0,
		// 			OS_OPT_PEND_BLOCKING,
		// 			(CPU_TS*)0,
		// 			&err);
		// APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		if (waypoint_index >= course.NumWaypoints){
			//end task
			break;
		}
		//clear old wp
		InputFifo2_Get(&WayPointBuffer, &ret);
		while (InputFifo2_getNumItems(&WayPointBuffer) < ROAD_GEN_WP_BUFFER_SIZE) {
			//add new wp
			InputFifo2_Put(&WayPointBuffer, course.Waypoints[waypoint_index]);
		}
		// TODO: remove this. This will Pend on the Monitor task eventually, and then LCD won't care, it'll just
		// draw whatever is in the FIFO.
		// post to lcd task.
		OSSemPost(&phys_model_update_sem, OS_OPT_POST_ALL, &err);

		OSTimeDly(1000, OS_OPT_TIME_PERIODIC, &err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	}// end while(1)
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

void roadgen_init(void ) {
	WayPointBuffer.num_items = 0;
	create_roadgen_sems();
	while (InputFifo2_getNumItems(&WayPointBuffer) < ROAD_GEN_WP_BUFFER_SIZE) {
		//init/fill waypoint buffer for LCD
		InputFifo2_Put(&WayPointBuffer, course.Waypoints[waypoint_index]);
}
}

void create_roadgen_sems(void ) {
	return;
}
