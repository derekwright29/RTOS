/*
 * lab7.h
 *
 *  Created on: Feb 26, 2020
 *      Author: goat (Derek Wright)
 *
 *     This header file contains the specific functions relating to Lab 7
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
 *     Since this is a specific application to lab7, here it is.
 *
 *     Additionally, multiple interface functions exist here, like
 *     isRight(), isLeft(), isNeither() functions. These don't belong in
 *     capsense.c or gpio.c. Macros like CAPSENSE_ISLEFT_MASK provide flexible
 *     changes to possible interface applications
 *
 *
 *
 *
 */

#ifndef LAB7_H_
#define LAB7_H_


#include <stdint.h>

#include "em_cmu.h"
#include "em_acmp.h"
#include "gpio.h"
#include "buttons.h"
#include "mycapsense.h"
#include "led_driver.h"


/* Control the mode of operation of main: use interrupts (1) or not (0)? */
#define LAB2_USE_INTERRUPTS		1






#endif /* LAB7_H_ */
