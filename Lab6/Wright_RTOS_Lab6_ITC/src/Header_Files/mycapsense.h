/*
 * mycapsense.h
 *
 *  Created on: Feb 27, 2020
 *      Author: goat
 */

#ifndef MYCAPSENSE_H_
#define MYCAPSENSE_H_

#include <stdint.h>
#include "em_cmu.h"
#include "em_acmp.h"
#include "gpio.h"
#include "em_csen.h"
#include "bspconfig.h"
#include "capsenseconfig.h"
#include  <kernel/include/os.h>

#define CAPSENSE_ISRIGHT_MASK		0x2
#define CAPSENSE_ISLEFT_MASK		0x1

/* CSEN Capsense Defines */
//#define CAPSENSE_CHANNELS       { csenSingleSelAPORT1XCH0, csenSingleSelAPORT1YCH1, csenSingleSelAPORT1XCH2, csenSingleSelAPORT1YCH3 }
#define CSEN_CHANNELS           4             /**< Number of channels in use for capsense */
#define CAPSENSE_THRESHOLD		25000u

#define CAPSENSE_MSG_MASK		0x4

#define  CAP_STATE_MASK			0x3

bool capsenseIsPressed[CSEN_CHANNELS];
extern OS_Q ITC_Queue;


/**
 * Capsense OS Task Defines
 */
#define CAPSENSE_TASK_PRIO					20u
#define CAPSENSE_TASK_STK_SIZE				1024u
CPU_STK  CapsenseTaskStk[CAPSENSE_TASK_STK_SIZE];
OS_TCB   CapsenseTaskTCB;

/* Capsense Semaphore */
OS_SEM capsense_sem;

/* Capsense Periodic Timer */
OS_TMR capsense_timer;


/**
 * create_capsense_timer()
 * ----------------
 * @description
 * 	 - This function creates the capsense semaphore.
 */
void create_capsense_sem(void );

/**
 * create_capsense_timer()
 * ----------------
 * @description
 * 	 - This function creates the capsense timer.
 * 	 - It initializes the capsense to a period of 100ms
 * 	 - It makes the timer periodic.
 *
 */
void create_capsense_timer(void);

/**
 * create_capsense_timer()
 * ----------------
 * @description
 * 	 - This function creates the capsense timer.
 * 	 - It initializes the capsense to a period of 100ms
 * 	 - It makes the timer periodic.
 *
 */
void create_capsense_task(void);


/**
 * CapsenseTask()
 * ----------------
 * @description
 * 	 - This function defines the capsense task operation.
 * 	 - It will sample the capsense and send a message to the OSQueue
 * 	 - This task pends on the semaphore which is posted to by the OSTimer
 *
 */
void  CapsenseTask (void);

/**
 * CapsenseTimerCallback()
 * -------------------------
 * @description
 * 	- This function is the Timer Callback for the Capsense OSTimer
 * 	- All it does is post to the capsense semaphore, telling
 * 	- the capsense task it is time to run.
 */
void CapsenseTimerCallback(OS_TMR p_tmr, void *p_arg);

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

#endif /* MYCAPSENSE_H_ */
