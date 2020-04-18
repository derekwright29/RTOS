/*
 * menu.c
 *
 *  Created on: Apr 17, 2020
 *      Author: goat
 */


#include "menu.h"



void MenuTask(void *p_arg) {
	RTOS_ERR err;
	InputValue_t ret;
	bool isStartMenu = true, isPress = false, isScroll = false, selectIsActive = false;
	uint8_t selection=0, screen=0;

	menu_init();

	while(1) {
		while(isStartMenu) {
			write_menu(screen, selection, selectIsActive);
			//block on button input
			OSSemPend(&button_sem,
							0,
							OS_OPT_PEND_BLOCKING,
							(CPU_TS*)0,
							&err);
			APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
			while(!InputFifo_isEmpty((InputFifo_t *)&FIFO_Button0)) {
				InputFifo_Get((InputFifo_t *)&FIFO_Button0, &ret);
				isPress = true;
			}
			while(!InputFifo_isEmpty((InputFifo_t *)&FIFO_Button1)) {
				InputFifo_Get((InputFifo_t *)&FIFO_Button1, &ret);
				isScroll = true;
				selection++;
			}
			if ((screen == WELCOME) && (isPress || isScroll)) {
				screen++;
			}
			else if (screen > WELCOME && isScroll) {
				if(!selectIsActive) {
					selection = 0;
					selectIsActive = true;
				}
			else if (screen > WELCOME && isPress && selectIsActive) {
				switch(screen) {
					case CHOOSE_CAR:
						vehicle_model.vehicle = &(car_choices[selection]);
						break;
					case CHOOSE_COND:
						vehicle_model.road = (road_condition_t) selection + 1;
						break;
					case CHOOSE_COURSE:
						course = (road_condition_t) course_choices[selection];
						break;
				}
				screen++;
				if (screen == READY_TO_START) {
					toggle_menu_control();
				}
			}


			}

		} // end start-screen while
		OSTimeDly(100, OS_OPT_TIME_PERIODIC, &err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	}//end outer while


} // end MenuTask







void create_menu_task(void) {
	RTOS_ERR err;
	OSTaskCreate(&MenuTaskTCB,                          /* Create the IDLE Task.                               */
					 "Menu Task",
					  MenuTask,
					  DEF_NULL,
					  MENU_TASK_PRIO,
					 &MenuTaskStk[0],
					 (MENU_TASK_STK_SIZE / 10u),
					  MENU_TASK_STK_SIZE,
					  0u,
					  0u,
					  DEF_NULL,
					 (OS_OPT_TASK_STK_CLR),
					 &err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}


void menu_init(void) {
	EMSTATUS status;
	//init display
	DISPLAY_Init();

	/* Initialize the display module. */
	status = DISPLAY_Init();
	if (DISPLAY_EMSTATUS_OK != status) {
	while (1) ;
	}

	/* Initialize the DMD module for the DISPLAY device driver. */
	status = DMD_init(0);
	if (DMD_OK != status) {
	while (1) ;
	}

	status = GLIB_contextInit(&menu_context);
	if (GLIB_OK != status) {
	while (1) ;
	}
	menu_context.backgroundColor = White;
	menu_context.foregroundColor = Black;
	GLIB_setFont(&menu_context, (GLIB_Font_t *)&GLIB_FontNarrow6x8);
	GLIB_clear(&menu_context);
	DMD_updateDisplay();
}

void create_game_over_semaphore(void) {
	RTOS_ERR err;

	//Create Model Sem
	OSSemCreate(&game_over_sem,
				"Game Over Semaphore",
				0,
				&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}


void write_menu(uint8_t screen, uint8_t selection, bool select_active) {

	switch (screen) {
		case WELCOME:
			GLIB_drawString(&menu_context, "Welcome to the Game!", 20,
	                  5,
	                  5,
	                  0);
			GLIB_drawString(&menu_context, "Press a button", 14,
				                  20,
				                  60,
				                  0);

			break;
		case CHOOSE_CAR:
			GLIB_clear(&menu_context);
			GLIB_drawString(&menu_context, "Choose your Car:", 20,
	                  5,
	                  5,
	                  0);
			GLIB_drawString(&menu_context, "Clunker", 7,
							                CENTER_X - GLIB_FONT_WIDTH*1,
							                  40,
							                  0);
			GLIB_drawString(&menu_context, "Sports Car", 10,
											CENTER_X - GLIB_FONT_WIDTH*3,
							                  60,
							                  0);
			GLIB_drawString(&menu_context, "Truck", 5,
											CENTER_X - GLIB_FONT_WIDTH*1,
							                  80,
							                  0);
			break;
		case CHOOSE_COURSE:
			break;
		case CHOOSE_COND:
			break;
		default:
			// error: reset to welcome
			screen = WELCOME;
			break;
	}
	DMD_updateDisplay();

}
