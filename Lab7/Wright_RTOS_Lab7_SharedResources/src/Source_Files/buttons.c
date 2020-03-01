/*
 * buttons.c
 *
 *  Created on: Jan 23, 2020
 *      Author: goat
 */


#include "buttons.h"
#include  <stdint.h>

void ButtonTask()
{
	RTOS_ERR err;
	uint8_t msg = 0;
	//init buttons
	gpio_open();
	while(1)
	{
		OSFlagPend(&button_flags,
				BUTTON_0_FLAG | BUTTON_1_FLAG,
				0,
				OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME + OS_OPT_PEND_BLOCKING,
				DEF_NULL,
				&err);
		/*   Check error code.                           */
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		msg = 0;
		// get state from sample
		uint8_t butt0 = sample_button0();
		uint8_t butt1 = sample_button1();

		// create message
		// message will be a two-bit value, least sig. bit corresponding to butt0 state
		// second bit will correspond to butt1 state.
		msg = (uint8_t)(butt0 | (butt1 << 1));
		OSQPost(&ITC_Queue,
				(void *)&msg,
				(OS_MSG_SIZE)1,
				OS_OPT_POST_FIFO,
				&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	}

}
void create_button_task() {
	RTOS_ERR err;
	OSTaskCreate(&ButtonTaskTCB,                          /* Create the Button Task.                               */
             "Button Task",
			 (OS_TASK_PTR)ButtonTask,
             DEF_NULL,
			 BUTTON_TASK_PRIO,
			 &ButtonTaskStk[0],
			 (BUTTON_TASK_STK_SIZE / 10u),
			 BUTTON_TASK_STK_SIZE,
             0u,
             0u,
             DEF_NULL,
             (OS_OPT_TASK_STK_CLR),
             &err);
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void button_create_osFlag() {
	RTOS_ERR err;
	//Create Flag
	 OSFlagCreate(&button_flags,
			 	  "Button Flag Group",
				   0,
				   &err);
	 APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}


uint8_t sample_button(uint8_t button_pin) {
	//read button state
	//return button state
	return GPIO_PinInGet(BSP_GPIO_PB0_PORT, button_pin);
}

uint8_t button_isNeither(uint8_t butt0, uint8_t butt1) {
	return !(butt0 ^ butt1);
}
