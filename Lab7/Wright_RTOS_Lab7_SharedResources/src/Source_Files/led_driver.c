/*
 * led_driver.c
 *
 *  Created on: Feb 27, 2020
 *      Author: goat
 */


#include <lab7.h>
#include "buttons.h"
#include "mycapsense.h"
#include  <kernel/include/os.h>
#include  <kernel/include/os_trace.h>


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
	VehicleAlert_t alert = ALERT_NO_ALERT;
	bool alerted = false;
	//init LEDs
	while(1)
	{
		alert = OSFlagPend(&alert_flags,
				ALERT_FLAGS_ALL,
				0,
				OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME + OS_OPT_PEND_BLOCKING,
				DEF_NULL,
				&err);
		/*   Check error code.                           */
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		if (alert == ALERT_SPEED_LIMIT || alert == ALERT_TURN_SPEED_LIMIT) {
			GPIO_PinOutSet(LED0_port, LED0_pin);
			alerted = true;
		}
		else if (alert == ALERT_DIRECTION_TIMEOUT) {
			GPIO_PinOutSet(LED1_port, LED1_pin);
			alerted = true;
		}
		else if (alert == ALERT_NO_ALERT && alerted){
			GPIO_PinOutClear(LED0_port, LED0_pin);
			GPIO_PinOutClear(LED1_port, LED1_pin);
			alerted = false;
		}
	}
}
