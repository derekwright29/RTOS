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

#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "bspconfig.h"
#include "main.h"
#include "app.h"
#include "capsense.h"
#include "lab3.h"

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
#define IDLE_TASK_PRIO						255u


#define MAIN_START_TASK_STK_SIZE         512u
#define BUTTON_TASK_STK_SIZE				1024u
#define CAPSENSE_TASK_STK_SIZE				1024u
#define LED_TASK_STK_SIZE					1024u

/*
*********************************************************************************************************
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/

                                                                /* Start Task Stack.                                    */
static  CPU_STK  MainStartTaskStk[MAIN_START_TASK_STK_SIZE];
                                                                /* Start Task TCB.                                      */
static  OS_TCB   MainStartTaskTCB;
static  OS_TCB   ButtonTaskTCB;
static  OS_TCB   CapsenseTaskTCB;
static  OS_TCB   LEDTaskTCB;

static  CPU_STK  ButtonTaskStk[BUTTON_TASK_STK_SIZE];
static  CPU_STK  CapsenseTaskStk[CAPSENSE_TASK_STK_SIZE];
static  CPU_STK  LEDTaskStk[LED_TASK_STK_SIZE];

/* Global variable definitions */
volatile uint8_t slide;	//position(s) of slider. Uses four channels
volatile uint8_t butt0=1, butt1=1;	// values go to 0 when button is pressed; 1 when unpressed. Init to 1.
volatile uint32_t msticks = 0; // gives 4 billion some milliseconds. I do not account for wrapping...


/*
*********************************************************************************************************
*********************************************************************************************************
*                                       LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

static  void  MainStartTask (void  *p_arg);

static  void  ButtonTask (void );
static  void  CapsenseTask (void);
static  void  LEDTask (void);
static  void  IDLETask (void);


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
	//init buttons
	gpio_open();
	while(1)
	{
		butt0 = sample_button0();
		butt1 = sample_button1();
		OSTimeDly(100u, OS_OPT_TIME_PERIODIC, &err);
		/*   Check error code.                           */
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	}

}
static void CapsenseTask()
{
	RTOS_ERR err;
	//init capsense
	CAPSENSE_Init();
	while(1)
	{
		slide = sample_capsense();
		OSTimeDly(100u, OS_OPT_TIME_PERIODIC, &err);
		/*   Check error code.                           */
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	}
}

static void LEDTask()
{
	RTOS_ERR err;
	//init LEDs
	while(1)
	{
		decide_led_outs(butt0,butt1,slide);
		OSTimeDly(100u, OS_OPT_TIME_PERIODIC, &err);
		/*   Check error code.                           */
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	}
}

static  void  MainStartTask (void  *p_arg)
{
    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    Common_Init(&err);                                          /* Call common module initialization example.           */
    APP_RTOS_ASSERT_CRITICAL(err.Code == RTOS_ERR_NONE, ;);

    BSP_OS_Init();                                              /* Initialize the BSP. It is expected that the BSP ...  */
                                                                /* ... will register all the hardware controller to ... */
                                                                /* ... the platform manager at this moment.             */
    BSP_LedsInit();
    OSTaskCreate(&ButtonTaskTCB,                          /* Create the Button Task.                               */
                         "Button Task",
                          ButtonTask,
                          DEF_NULL,
                          BUTTON_TASK_PRIO,
                         &ButtonTaskStk[0],
                         (BUTTON_TASK_STK_SIZE / 10u),
                          BUTTON_TASK_STK_SIZE,
                          0u,
                          0u,
                          DEF_NULL,
                         (OS_OPT_TASK_STK_CLR),
                         &err);
        OSTaskCreate(&CapsenseTaskTCB,                          /* Create the Capsense Task.                               */
                         "Capsense Task",
                          CapsenseTask,
                          DEF_NULL,
                          CAPSENSE_TASK_PRIO,
                         &CapsenseTaskStk[0],
                         (CAPSENSE_TASK_STK_SIZE / 10u),
                          CAPSENSE_TASK_STK_SIZE,
                          0u,
                          0u,
                          DEF_NULL,
                         (OS_OPT_TASK_STK_CLR),
                         &err);

        OSTaskCreate(&LEDTaskTCB,                          /* Create the LED Task.                               */
                         "LED Task",
                          LEDTask,
                          DEF_NULL,
                          LED_TASK_PRIO,
                         &LEDTaskStk[0],
                         (LED_TASK_STK_SIZE / 10u),
                          LED_TASK_STK_SIZE,
                          0u,
                          0u,
                          DEF_NULL,
                         (OS_OPT_TASK_STK_CLR),
                         &err);
}
