/*
 * lab3.h
 *
 *  Created on: Feb 26, 2020
 *      Author: goat (Derek Wright)
 *
 *     This header file contains the specific functions relating to Lab 2
 *     of ECEN 3002 - RTOS.
 *
 *     All general-use functions do not belong here, and should be placed in
 *     their own header/source files.
 *
 *     For example, "decide_led_outs" is a specific function that will only
 *     relate to Lab2 of this class. I refuse to put that in gpio.h/.c
 *
 *     More on the edge, though, is the sample_capsense function.
 *     I put these here because there are multiple ways to interact
 *     with this sensor. For this lab, I used CAPSENSE_getPressed(),
 *     but there are others, like CAPSENSE_getSliderPosition().
 *     Since this is a specific application to lab2, here it is.
 *
 *     Additionally, multiple interface functions exist here, like
 *     isRight(), isLeft(), isNeither() functions. These don't belong in
 *     capsense.c or gpio.c. Macros like CAPSENSE_ISLEFT_MASK provide flexible
 *     changes to possible interface applications
 *
 *     Finally, the control compile-time switches are placed here:
 *     LAB2_USE_INTERRUPTS switches between polling and interrupt implementation.
 *
 *
 */

#ifndef SRC_HEADER_FILES_LAB6_H_
#define SRC_HEADER_FILES_LAB6_H_


#include <stdint.h>
#include "bspconfig.h"
#include "capsenseconfig.h"
#include "em_cmu.h"
#include "em_acmp.h"
#include "gpio.h"
//#include "capsense.h"
#include "em_csen.h"

/* Control the mode of operation of main: use interrupts (1) or not (0)? */
#define LAB2_USE_INTERRUPTS		1

#define CAPSENSE_ISRIGHT_MASK		0x2
#define CAPSENSE_ISLEFT_MASK		0x1

/* CSEN Capsense Defines */
//#define CAPSENSE_CHANNELS       { csenSingleSelAPORT1XCH0, csenSingleSelAPORT1YCH1, csenSingleSelAPORT1XCH2, csenSingleSelAPORT1YCH3 }
#define CSEN_CHANNELS           4             /**< Number of channels in use for capsense */
#define CAPSENSE_THRESHOLD		25000u

#define CAPSENSE_MSG_MASK		0x4
#define BUTTON_MSG_MASK			0x4

#define  BUTTON_0_MASK			0x1
#define  BUTTON_1_MASK			0x2
#define  CAP_STATE_MASK			0x3

bool capsenseIsPressed[CSEN_CHANNELS];

// Enumerated type for flag
typedef enum Button_Flag {
	BUTTON0_RELEASED = 0,
	BUTTON0_PRESSED = 1,
	BUTTON1_RELEASED = 2,
	BUTTON1_PRESSED = 4,

}Button_Flag_t;


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

/**
 * This calls the routines given by the TA for CSEN module.
 */
void init_capsense(void);
/**
 * sample_capsense()
 * ___________________
 * @description This function senses 4 channels of the capsense
 * 				and returns the state of all four, stored in a
 * 				single uint8_t
 *
 * @parameters
 * 		-none
 * @return
 *		- uint8_t: The least significant 4 bits correnspond to the four channels measured
 *					0th channel corresponds to 0th bit of the return value.
 *					A 1 means that the channel is pressed, while a 0 means it isn't pressed
 */
uint8_t sample_capsense();

/**
 * capsense_isLeft()
 * ___________________
 * @description This function uses a mask to determine if the left side
 * 				of the capsense is touched.
 *
 * @parameters
 * 	- uint8_t capstate: a uint8_t returned by sample_capsense()

 * @return
 *		- uint8_t: 1 if left side is pressed, 0 else.
 */
uint8_t capsense_isLeft(uint8_t capstate);

/**
 * capsense_isRight()
 * ___________________
 * @description This function uses a mask to determine if the right side
 * 				of the capsense is touched.
 *
 * @parameters
 * 	- uint8_t capstate: a uint8_t returned by sample_capsense()

 * @return
 *		- uint8_t: 1 if right side is pressed, 0 else.
 */
uint8_t capsense_isRight(uint8_t capstate);

/**
 * capsense_isNeither()
 * ___________________
 * @description Similar to the button_isNeither() function, we need to know
 * 				if both sides of the capsense are touched, in which case
 * 				we do nothing.
 *
 * @parameters
 * 	- uint8_t capstate: a uint8_t returned by sample_capsense()

 * @return
 *		- uint8_t: 1 if both/neither side is pressed, 0 else.
 */
uint8_t capsense_isNeither(uint8_t capstate);



#endif /* SRC_HEADER_FILES_LAB6_H_ */
