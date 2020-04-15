/*
 * led_driver.h
 *
 *  Created on: Feb 27, 2020
 *      Author: goat
 */

#ifndef LED_DRIVER_H_
#define LED_DRIVER_H_

#include <stdint.h>

#include "em_cmu.h"
#include "em_acmp.h"
#include "gpio.h"
#include "buttons.h"
#include "mycapsense.h"


/**
 * LED Task Defines
 */
//Priority
#define LED_TASK_PRIO						22u
//task stack size to allocate
#define LED_TASK_STK_SIZE					1024u
//task stack
CPU_STK  LEDTaskStk[LED_TASK_STK_SIZE];
//task TCB
OS_TCB   LEDTaskTCB;

/****************************
 * Global Variables
 ****************************/

//Task function Prototype
/**
 * LEDTask()
 * ---------
 * @description	This function defines the operation of the LED Task
 * 				It keeps track of the button state and the cap_state,
 * 				and updates the LED outs accordingly.
 *
 */
void  LEDTask (void);


/**
 * create_led_task()
 * ----------------
 * @description This function simply makes the OS call to start the LED task.
 *
 * @param	None
 *
 * @return	None
 *
 */
void create_led_task(void);



#endif /* LED_DRIVER_H_ */
