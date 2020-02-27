/***************************************************************************//**
 * @file
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.
 * The software is governed by the sections of the MSLA applicable to Micrium
 * Software.
 *
 ******************************************************************************/

/*
*********************************************************************************************************
*
*                                             EXAMPLE MAIN
*
* File : ex_main.c
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <bsp_os.h>
#include  "bsp.h"

#include  <cpu/include/cpu.h>
#include  <common/include/common.h>
#include  <kernel/include/os.h>
#include  <kernel/include/os_trace.h>

#include  <common/include/lib_def.h>
#include  <common/include/rtos_utils.h>
#include  <common/include/toolchains.h>
#include <lab6.h>

#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "bspconfig.h"
#include "main.h"
#include "app.h"
#include "capsense.h"
#include "../cpu_cfg_local.h"

/*
*********************************************************************************************************
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define BUTTON_TASK_PRIO						21u
#define CAPSENSE_TASK_PRIO						20u
#define LED_TASK_PRIO							19u
#define MAIN_START_TASK_PRIO              		18u
#define IDLE_TASK_PRIO							62u


#define MAIN_START_TASK_STK_SIZE         512u
#define BUTTON_TASK_STK_SIZE				1024u
#define CAPSENSE_TASK_STK_SIZE				1024u
#define LED_TASK_STK_SIZE					1024u
#define IDLE_TASK_STK_SIZE					1024u

/*
*********************************************************************************************************
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/

/* Task Stacks.                                    */
static  CPU_STK  MainStartTaskStk[MAIN_START_TASK_STK_SIZE];
static  CPU_STK  ButtonTaskStk[BUTTON_TASK_STK_SIZE];
static  CPU_STK  CapsenseTaskStk[CAPSENSE_TASK_STK_SIZE];
static  CPU_STK  LEDTaskStk[LED_TASK_STK_SIZE];
static  CPU_STK  IdleTaskStk[LED_TASK_STK_SIZE];

/* Task TCBs.                                      */
static  OS_TCB   MainStartTaskTCB;
static  OS_TCB   ButtonTaskTCB;
static  OS_TCB   CapsenseTaskTCB;
static  OS_TCB   LEDTaskTCB;
static  OS_TCB   IdleTaskTCB;


/* Global variable definitions */
volatile uint8_t slide;	//position(s) of slider. Uses four channels
volatile uint8_t butt0=1, butt1=1;	// values go to 0 when button is pressed; 1 when unpressed. Init to 1.
volatile uint32_t msticks = 0; // gives 4 billion some milliseconds. I do not account for wrapping...

extern bool capsenseIsPressed[CSEN_CHANNELS];
/* Button 0 Event Message*/
extern OS_FLAG_GRP button_flags;


/* ICT Message Queue */
static OS_Q ICT_Queue;

/* Capsense Semaphore */
static OS_SEM capsense_sem;
/* Capsense Periodic Timer */
OS_TMR capsense_timer;

/*
*********************************************************************************************************
*********************************************************************************************************
*                                       LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

/* Task main functions*/

static  void  MainStartTask (void  *p_arg);		// the void *p_arg is what OSTaskCreate expects. Not strictly necessary.
static  void  ButtonTask (void );
static  void  CapsenseTask (void);
static  void  LEDTask (void);
static  void  IDLETask (void *p_arg);

static void CapsenseTimerCallback(OS_TMR p_tmr, void *p_arg);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C applications. It is assumed that your code will
*               call main() once you have performed all necessary initialization.
*
* Argument(s) : None.
*
* Return(s)   : None.
*
* Note(s)     : None.
*********************************************************************************************************
*/

int  main (void)
{
    RTOS_ERR  err;


    BSP_SystemInit();                                           /* Initialize System.                                   */
    CPU_Init();                                                 /* Initialize CPU.                                      */

    OS_TRACE_INIT();
    OSInit(&err);                                               /* Initialize the Kernel.                               */
                                                                /*   Check error code.                                  */
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);


    OSTaskCreate(&MainStartTaskTCB,                          /* Create the Start Task.                               */
                 "Main Start Task",
                  MainStartTask,
                  DEF_NULL,
                  MAIN_START_TASK_PRIO,
                 &MainStartTaskStk[0],
                 (MAIN_START_TASK_STK_SIZE / 10u),
                  MAIN_START_TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);

/*   Check error code.                                  */
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);


    OSStart(&err);                                              /* Start the kernel.                                    */
/*   Check error code.                                  */
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);


    return (1);
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/**
 *  TIMER CALLBACKS
 */
static void CapsenseTimerCallback(OS_TMR p_tmr, void *p_arg) {
	RTOS_ERR err;
	OSSemPost(&capsense_sem, OS_OPT_POST_1, &err);
}


/*
*********************************************************************************************************
*                                          Ex_MainStartTask()
*
* Description : This is the task that will be called by the Startup when all services are initializes
*               successfully.
*
* Argument(s) : p_arg   Argument passed from task creation. Unused, in this case.
*
* Return(s)   : None.
*
* Notes       : None.
*********************************************************************************************************
*/

static void ButtonTask()
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
		OSQPost(&ICT_Queue,
				(void *)&msg,
				(OS_MSG_SIZE)1,
				OS_OPT_POST_FIFO,
				&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	}

}
static void CapsenseTask()
{
	RTOS_ERR err;
	//init capsense
	init_capsense();
	bool is_start = OSTmrStart(&capsense_timer,
			&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	APP_RTOS_ASSERT_DBG((is_start == DEF_TRUE), 1);
	uint8_t left_cap;
	uint8_t right_cap;
	uint8_t isChanged = 0;
	while(1)
	{
		OSSemPend(&capsense_sem,
					0,
					OS_OPT_PEND_BLOCKING,
					(CPU_TS*)0,
					&err);
		/*   Check error code.                           */
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		// get status
		sample_capsense();
		if ((uint8_t)capsenseIsPressed[0] == left_cap && (uint8_t)capsenseIsPressed[2] == right_cap) {
			isChanged = 0;
		}
		else {
			isChanged = 1;
			// I only use two channels because with the provided CSEN code ,
			// two of the four channels initialized did not work as expected.
			left_cap = (uint8_t)capsenseIsPressed[0];// | (uint8_t)capsenseIsPressed[1];
			right_cap = (uint8_t)capsenseIsPressed[2];// | (uint8_t)capsenseIsPressed[3];
		}
		//create message and post
		uint8_t cap_msg = left_cap | (right_cap << 1) | CAPSENSE_MSG_MASK;
		if (isChanged) {
			OSQPost(&ICT_Queue,
					(void *) &cap_msg,
					(OS_MSG_SIZE)1,
					OS_OPT_POST_FIFO,
					&err);
			APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		}
	}
}

static void LEDTask()
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
		msg_read = OSQPend(&ICT_Queue,
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

static void IDLETask(void * p_arg)
{
	while(1)
	{
		EMU_EnterEM1();
	}
}


RTOS_ERR  glob_err;
static  void  MainStartTask (void  *p_arg)
{

    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

    Common_Init(&glob_err);                                          /* Call common module initialization example.           */
    APP_RTOS_ASSERT_CRITICAL(glob_err.Code == RTOS_ERR_NONE, ;);

    BSP_OS_Init();                                              /* Initialize the BSP. It is expected that the BSP ...  */
                                                                /* ... will register all the hardware controller to ... */
                                                                /* ... the platform manager at this moment.             */
    BSP_LedsInit();

    //Create Sem
    OSSemCreate(&capsense_sem,
    		"Capsense timer semaphore",
			0,
			&glob_err);
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(glob_err) == RTOS_ERR_NONE), 1);

    //Create Flag
    OSFlagCreate(&button_flags,
    			"Button Flag Group",
				0,
				&glob_err);
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(glob_err) == RTOS_ERR_NONE), 1);

    // Create Q
	OSQCreate(&ICT_Queue,
		   "Inter-Task Message Queue",
			20,
			&glob_err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(glob_err) == RTOS_ERR_NONE), 1);

	//Create Timer
	OSTmrCreate(&capsense_timer,
				"Capsense 100ms Timer",
				2,
				2,
				OS_OPT_TMR_PERIODIC,
				(OS_TMR_CALLBACK_PTR)CapsenseTimerCallback,
				DEF_NULL,
				&glob_err);

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
                         &glob_err);
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(glob_err) == RTOS_ERR_NONE), 1);
        OSTaskCreate(&CapsenseTaskTCB,                          /* Create the Capsense Task.                               */
                         "Capsense Task",
						 (OS_TASK_PTR)CapsenseTask,
                          DEF_NULL,
                          CAPSENSE_TASK_PRIO,
                         &CapsenseTaskStk[0],
                         (CAPSENSE_TASK_STK_SIZE / 10u),
                          CAPSENSE_TASK_STK_SIZE,
                          0u,
                          0u,
                          DEF_NULL,
                         (OS_OPT_TASK_STK_CLR),
                         &glob_err);
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(glob_err) == RTOS_ERR_NONE), 1);

        OSTaskCreate(&LEDTaskTCB,                          /* Create the LED Task.                               */
                         "LED Task",
                          (OS_TASK_PTR)LEDTask,
                          DEF_NULL,
                          LED_TASK_PRIO,
                         &LEDTaskStk[0],
                         (LED_TASK_STK_SIZE / 10u),
                          LED_TASK_STK_SIZE,
                          0u,
                          0u,
                          DEF_NULL,
                         (OS_OPT_TASK_STK_CLR),
                         &glob_err);
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(glob_err) == RTOS_ERR_NONE), 1);
        OSTaskCreate(&IdleTaskTCB,                          /* Create the Start Task.                               */
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
                         &glob_err);
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(glob_err) == RTOS_ERR_NONE), 1);

}
