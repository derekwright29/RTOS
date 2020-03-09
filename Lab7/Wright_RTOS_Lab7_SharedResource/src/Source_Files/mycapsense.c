/*
 * mycapsense.c
 *
 *  Created on: Feb 27, 2020
 *      Author: goat
 */

#include "mycapsense.h"
#include <common/include/rtos_utils.h>

/* CSEN globals: given by class instructors */
CSEN_Init_TypeDef csenInit = CSEN_INIT_DEFAULT;
CSEN_InitMode_TypeDef csenInitMode = CSEN_INITMODE_DEFAULT;
CSEN_InputSel_TypeDef channelList[CSEN_CHANNELS] = CAPSENSE_CHANNELS;


void create_capsense_sem() {
	RTOS_ERR err;
	//Create Sem
	OSSemCreate(&capsense_sem,
			"Capsense timer semaphore",
			0,
			&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

}


void create_capsense_timer() {
	RTOS_ERR err;
//Create Timer
	OSTmrCreate(&capsense_timer,
				"Capsense 100ms Timer",
				2,
				2,
				OS_OPT_TMR_PERIODIC,
				(OS_TMR_CALLBACK_PTR)CapsenseTimerCallback,
				DEF_NULL,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}


void create_capsense_task() {
	RTOS_ERR err;
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
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

}

void CapsenseTask()
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
		if ((((uint8_t)capsenseIsPressed[0] | (uint8_t)capsenseIsPressed[1]) == left_cap) && (((uint8_t)capsenseIsPressed[0] | (uint8_t)capsenseIsPressed[1]) == right_cap)) {
			isChanged = 0;
		}
		else {
			isChanged = 1;
			// I only use two channels because with the provided CSEN code ,
			// two of the four channels initialized did not work as expected.
			left_cap = (uint8_t)capsenseIsPressed[0] | (uint8_t)capsenseIsPressed[1];
			right_cap = (uint8_t)capsenseIsPressed[2] | (uint8_t)capsenseIsPressed[3];
		}
		//create message and post
		uint8_t cap_msg = left_cap | (right_cap << 1) | CAPSENSE_MSG_MASK;
		if (isChanged) {
			OSQPost(&ITC_Queue,
					(void *) &cap_msg,
					(OS_MSG_SIZE)1,
					OS_OPT_POST_FIFO,
					&err);
			APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		}
	}
}
/**
 *  TIMER CALLBACK
 */
void CapsenseTimerCallback(OS_TMR p_tmr, void *p_arg) {
	RTOS_ERR err;
	OSSemPost(&capsense_sem, OS_OPT_POST_1, &err);
}

void init_capsense() {
	/* CAPSENSE Init */
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_CSEN_HF, true);

	CSEN_Init(CSEN, &csenInit);
	csenInitMode.convSel = csenDMRes16;
	csenInitMode.accMode = csenAccMode4;
}

uint8_t sample_capsense() {
	uint32_t i;
	uint32_t csResult = 0;
	/* Reset CAPSENSE state variables before updating */
	for (i = CSEN_CHANNELS; i > 0; i--)
	{
	  capsenseIsPressed[i-1] = 0;
	}

	/* Loop over CAPTOUCH channels and take CSEN measurements */
	for (i = CSEN_CHANNELS; i > 0; i--)
	{
	  /* Disable CSEN before updating channel to measure */
	  CSEN_Disable(CSEN);
	  csenInitMode.singleSel = channelList[i-1];
	  CSEN_InitMode(CSEN, &csenInitMode);
	  /* Re-enable CSEN */
	  CSEN_Enable(CSEN);

	  /* Start a CAPSENSE conversion */
	  CSEN_Start(CSEN);
	  /* Wait for measurement to finish */
	  while (CSEN_IsBusy(CSEN));

	  /* Store result in local variable */
	  csResult = CSEN_DataGet(CSEN);

	  /* Update global variable */
	  if (csResult > CAPSENSE_THRESHOLD) {
		  capsenseIsPressed[i-1] = true;
	  }
	  else {
		  capsenseIsPressed[i-1] = false;
	  }
	}
	return 0;

//	CAPSENSE_Sense();
////	get four channels, store in one variable to avoid too many variables
//	return CAPSENSE_getPressed(0) | (CAPSENSE_getPressed(1) << 1) | (CAPSENSE_getPressed(2) << 2) | (CAPSENSE_getPressed(3) << 3);
}


uint8_t capsense_isLeft(uint8_t capstate) {
	return (capstate & CAPSENSE_ISLEFT_MASK ) > 0;
}

uint8_t capsense_isRight(uint8_t capstate) {
	return (capstate & CAPSENSE_ISRIGHT_MASK) > 0;
}

uint8_t capsense_isNeither(uint8_t capstate) {
	if ((capstate & CAPSENSE_ISLEFT_MASK) && (capstate & CAPSENSE_ISRIGHT_MASK)) {
		//means both sides of slider are touched
		return 1;
	}
	if (!(capstate & CAPSENSE_ISLEFT_MASK) && !(capstate & CAPSENSE_ISRIGHT_MASK)) {
		//means neither side is touched
		return 1;
	}
	return 0;
}

bool capsense_isChanged(bool * state_array) {
	for (uint8_t i = 0; i < CSEN_CHANNELS; i++)
	{
		if (state_array[i] != capsenseIsPressed[i])
			return true;
	}
	return false;
}




