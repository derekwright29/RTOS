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
#include "lab6.h"
#include "mycapsense.h"


/**
 * LED Task Defines
 */
//Priority
#define LED_TASK_PRIO						19u
//task stack size to allocate
#define LED_TASK_STK_SIZE					1024u
//task stack
CPU_STK  LEDTaskStk[LED_TASK_STK_SIZE];
//task TCB
OS_TCB   LEDTaskTCB;

/****************************
 * Global Variables
 ****************************/
extern OS_Q ITC_Queue;

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
 * start_led_task()
 * ----------------
 * @description This function simply makes the OS call to start the LED task.
 *
 * @param	None
 *
 * @return	None
 *
 */
void create_led_task(void);


/**
 * decide_led_outs()
 * ___________________
 * @description This function controls the states of the leds
 * 				given our control inputs: the two buttons, and
 * 				the state of the capactive touch sensor
 *
 * @parameters
 * 	- uint8_t button0: state of button0; 1 if unpressed, 0 if pressed
 * 	- uint8_t button1: state of button1
 * 	- uint8_t capstate: This implementation	uses four channels of the capsense
 * 						The least significant 4 bits correspond to the state
 * 						of each channel (0th bit ~ 0th channel).
 * 						Masks are provided above to extract left/right channels.
 *
 * @return
 *		- void: the function manipulates the leds and does not need to return anything.
 */
void decide_led_outs(uint8_t button0, uint8_t button1, uint8_t capstate);





#endif /* LED_DRIVER_H_ */
