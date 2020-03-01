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
#include <lab7.h>
#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "bspconfig.h"
#include "main.h"
#include "cmu.h"
#include "gpio.h"
#include "mycapsense.h"
#include "buttons.h"
#include "led_driver.h"
#include "../cpu_cfg_local.h"

/*
*********************************************************************************************************
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

//#define BUTTON_TASK_PRIO						21u
//#define CAPSENSE_TASK_PRIO						20u
//#define LED_TASK_PRIO							19u
#define MAIN_START_TASK_PRIO              		18u
#define IDLE_TASK_PRIO							62u


#define MAIN_START_TASK_STK_SIZE         512u
//#define BUTTON_TASK_STK_SIZE				1024u
//#define CAPSENSE_TASK_STK_SIZE				1024u
//#define LED_TASK_STK_SIZE					1024u
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
//static  CPU_STK  ButtonTaskStk[BUTTON_TASK_STK_SIZE];
//static  CPU_STK  CapsenseTaskStk[CAPSENSE_TASK_STK_SIZE];
//static  CPU_STK  LEDTaskStk[LED_TASK_STK_SIZE];
static  CPU_STK  IdleTaskStk[LED_TASK_STK_SIZE];

/* Task TCBs.                                      */
static  OS_TCB   MainStartTaskTCB;
//static  OS_TCB   ButtonTaskTCB;
//static  OS_TCB   CapsenseTaskTCB;
//static  OS_TCB   LEDTaskTCB;
static  OS_TCB   IdleTaskTCB;


/* Global variable definitions */
volatile uint8_t slide;	//position(s) of slider. Uses four channels
volatile uint8_t butt0=1, butt1=1;	// values go to 0 when button is pressed; 1 when unpressed. Init to 1.
volatile uint32_t msticks = 0; // gives 4 billion some milliseconds. I do not account for wrapping...

//extern bool capsenseIsPressed[CSEN_CHANNELS];


/* ITC Message Queue */
OS_Q ITC_Queue;

/*
*********************************************************************************************************
*********************************************************************************************************
*                                       LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

/* Task main functions*/

static  void  MainStartTask (void  *p_arg);		// the void *p_arg is what OSTaskCreate expects. Not strictly necessary.
//static  void  ButtonTask (void );
//static  void  CapsenseTask (void);
//static  void  LEDTask (void);
static  void  IDLETask (void *p_arg);

//static void CapsenseTimerCallback(OS_TMR p_tmr, void *p_arg);


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
    //init clock tree
//    cmu_open();

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



    // Create Q
	OSQCreate(&ITC_Queue,
		   "Inter-Task Message Queue",
			20,
			&glob_err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(glob_err) == RTOS_ERR_NONE), 1);

	button_create_osFlag();
	create_button_task();

	create_capsense_sem();
	create_capsense_timer();
    create_capsense_task();

     create_led_task();
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
