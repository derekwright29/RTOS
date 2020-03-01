/*
 * buttons.h
 *
 *  Created on: Jan 23, 2020
 *      Author: goat
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include  <stdint.h>
#include  <kernel/include/os.h>
#include  <kernel/include/os_trace.h>
#include "gpio.h"

/*************************
 * Button Task defines
 ***********************/
#define BUTTON_TASK_PRIO					21u
#define BUTTON_TASK_STK_SIZE				1024u
CPU_STK  ButtonTaskStk[BUTTON_TASK_STK_SIZE];
OS_TCB   ButtonTaskTCB;

// Flag defines
#define BUTTON_0_FLAG		0x1
#define BUTTON_1_FLAG       0x2

#define  BUTTON_0_MASK		0x1
#define  BUTTON_1_MASK		0x2

// a
#define BUTTON_MSG_MASK			0x4

// Enumerated type for flag
typedef enum Button_Flag {
	BUTTON0_RELEASED = 0,
	BUTTON0_PRESSED = 1,
	BUTTON1_RELEASED = 2,
	BUTTON1_PRESSED = 4,

}Button_Flag_t;

//***********************************************************************************
// global variables
//***********************************************************************************

/* Button 0 Event Message*/
OS_FLAG_GRP button_flags;
extern OS_Q ITC_Queue;
/**
 * ButtonTask()
 * ___________________
 * @description 	This function executed the Button Task.
 * 					It will listen to either a timer or gpio interrupts
 * 					in order to determine when to update the button state.
 * 					It will then create a message to send to the message Q
 * 					which activates the LED Task.
 *
 * @parameters
 * 	- None
 *
 * @return
 *		- None
 */
void  ButtonTask (void );

/**
 * start_button_task()
 * -------------------
 * @description This function simply makes the OS call to start the Button task.
 *
 * @param	None
 *
 * @return	None
 *
 */
void create_button_task(void);
/**
 * start_button_task()
 * -------------------
 * @description This function simply makes the OS call to create the button flag group
 *
 * @param	None
 *
 * @return	None
 *
 */
void button_create_osFlag(void);

/**
 * sample_button()
 * ___________________
 * @description This function returns the state of a pushbutton.
 * 				Two macro functions are provided to specifically
 * 				sample pushbutton 0 and 1.
 *
 * @parameters
 * 	- uint8_t button_pin: the pin number we want the state of.
 * 							In our case, either 6 or 7.
 * @return
 *		- uint8_t: 1 if button is UNpressed, 0 if button is PRESSED.
 */
uint8_t sample_button(uint8_t button_pin);
#define sample_button0(...)		sample_button(BSP_GPIO_PB0_PIN)
#define sample_button1(...)		sample_button(BSP_GPIO_PB1_PIN)


/**
 * button_isNeither()
 * ___________________
 * @description This function is an XNOR of the button states.
 * 				A performance spec given was that both buttons
 * 				pressed should behave the same as if neither were pressed.
 * 				This corresponds to an XOR/XNOR operation. The decide() function assumes
 * 				active low LEDS, therefore we use XNOR here.
 *
 * @parameters
 * 	- uint8_t button0: state of button0; 1 if unpressed, 0 if pressed
 * 	- uint8_t button1: state of button1

 * @return
 *		- uint8_t: 1 if button states are the same, 0 else.
 */
uint8_t button_isNeither(uint8_t butt0, uint8_t butt1);




#endif /* BUTTONS_H_ */
