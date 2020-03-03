/*
 * led_driver.c
 *
 *  Created on: Feb 27, 2020
 *      Author: goat
 */


#include "buttons.h"
#include "mycapsense.h"
#include  <kernel/include/os.h>
#include  <kernel/include/os_trace.h>
#include <lab6.h>


void create_led_task(void) {
	RTOS_ERR err;
	OSTaskCreate(&LEDTaskTCB,                     /* Create the LED Task.                               */
				 "LED Task",
				  (OS_TASK_PTR)LEDTask,			  /* Ptr to Task Function */
				  DEF_NULL,					   	  /* Ptr to Task argument (none) */
				  LED_TASK_PRIO,				  /* Task Priority */
				  &LEDTaskStk[0],				  /* Ptr to Task Stack */
				  (LED_TASK_STK_SIZE / 10u),
				  LED_TASK_STK_SIZE,			  /* Task Stack Size */
				  0u,
				  0u,
				  DEF_NULL,
				  (OS_OPT_TASK_STK_CLR),
				  &err);
APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

}


void LEDTask()
{
	RTOS_ERR err;
	uint8_t button0_state = 0;
	uint8_t button1_state = 0;
	uint8_t cap_state = 0;
	void * msg_read;
	uint32_t msg_size;
	//init LEDs
	while(1)
	{
		msg_read = OSQPend(&ITC_Queue,
				0,
				OS_OPT_PEND_BLOCKING,
				(OS_MSG_SIZE *)&msg_size,
				(CPU_TS *)0,
				&err);
/*   Check error code.                           */
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		uint8_t state = *(uint8_t*)msg_read;
		if (state & CAPSENSE_MSG_MASK) {
			// this means the msg came from capsense
			cap_state = state & CAP_STATE_MASK; //mask off the state bits
		}
		else {
			//this means msg came from button
			button0_state = state & BUTTON_0_MASK;
			button1_state = (state & BUTTON_1_MASK)>>1;
		}
		decide_led_outs(button0_state, button1_state, cap_state);
	}
}

void decide_led_outs(uint8_t button0, uint8_t button1, uint8_t capstate) {
	uint8_t buttons_only = 0;
	// no touch sensed on capsense
	if (capsense_isNeither(capstate)) {
		buttons_only = 1;
	}

	// don't take capstate value into account
	if(buttons_only) {
		if (button_isNeither(button0, button1)) {
			// same as no buttons pressed
			GPIO_PinOutClear(LED0_port, LED0_pin);
			GPIO_PinOutClear(LED1_port, LED1_pin);
		}
		else{
			if (!button0) {
				GPIO_PinOutSet(LED0_port, LED0_pin);
				GPIO_PinOutClear(LED1_port, LED1_pin);
			}
			else if (!button1) {
				GPIO_PinOutSet(LED1_port, LED1_pin);
				GPIO_PinOutClear(LED0_port, LED0_pin);
			}
		}
	}
	else {
		//means we have a one-sided capstate.
		if (button_isNeither(button0, button1)) {
			if(capsense_isLeft(capstate)) {
				GPIO_PinOutSet(LED0_port, LED0_pin);
				GPIO_PinOutClear(LED1_port, LED1_pin);
			}
			else if (capsense_isRight(capstate)) {
				GPIO_PinOutSet(LED1_port, LED1_pin);
				GPIO_PinOutClear(LED0_port, LED0_pin);
			}

		}
		else {
			if (capsense_isLeft(capstate) || !button0) {
				GPIO_PinOutSet(LED0_port, LED0_pin);
			}
			if (capsense_isRight(capstate) || !button1) {
				GPIO_PinOutSet(LED1_port, LED1_pin);
			}
		}
	}
}
