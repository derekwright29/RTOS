/*
 * general_tasks.c
 *
 *  Created on: Mar 2, 2020
 *      Author: goat
 */

#include "general_tasks.h"

void create_idle_task() {
	RTOS_ERR err;
    OSTaskCreate(&IdleTaskTCB,                          /* Create the IDLE Task.                               */
				 "My Idle Task",
				  IDLETask,
				  DEF_NULL,
				  IDLE_TASK_PRIO,
				 &IdleTaskStk[0],
				 (IDLE_TASK_STK_SIZE / 10u),
				  IDLE_TASK_STK_SIZE,
				  0u,
				  0u,
				  DEF_NULL,
				 (OS_OPT_TASK_STK_CLR),
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void IDLETask(void * p_arg)
{
	while(1)
	{
		EMU_EnterEM1();
	}
}
