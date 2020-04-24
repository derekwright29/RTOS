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
	uint8_t start_signal;

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
			}
			if ((screen == WELCOME) && (isPress || isScroll)) {
				isPress = false;
				isScroll = false;
				screen++;
			}
			else if (screen > WELCOME && isScroll) {
				isScroll = false;
				if(!selectIsActive) {
					selection = 0;
					selectIsActive = true;
				}
				else {
					selection++;
					selection %= 3; //number of options at each step
				}
			}
			else if (screen > WELCOME && isPress && selectIsActive) {
				isPress = false;
				switch(screen) {
					case CHOOSE_CAR:
//						vehicle_model.vehicle = (const vehicle_description_t*) &(car_choices[selection]);
						break;
					case CHOOSE_COND:
						vehicle_model.road = (road_condition_t) selection + 1;
						break;
					case CHOOSE_COURSE:
//						course = (road_condition_t) course_choices[selection];
						break;
				}
				screen++;
				selectIsActive = false;
				if (screen == READY_TO_START) {
					release_menu_control();
					isStartMenu = false;
				}
			}
			OSTimeDly(100, OS_OPT_TIME_PERIODIC, &err);
			APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		} // end start-screen while

		// wait for end of game
		failure_t fail_flag = OSFlagPend(&game_over_flag,
									ALL_FAIL_FLAGS,
									0,
									OS_OPT_PEND_BLOCKING,
									(CPU_TS*)0,
									&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		// now we have received Game Over signa
		//Delete tasks in order to restart them.
		take_menu_control();
		screen = GAME_OVER;
		while(!isStartMenu) {

			//TODO: implement
			write_gameover(screen, fail_flag, stats);

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
			}

			if ((screen == GAME_OVER) && (isPress || isScroll)) {
				isPress = false;
				isScroll = false;
				screen++;
			}
			else if (screen == STATS && (isPress || isScroll)) {
				isScroll = false;
				isScroll = false;
				screen = WELCOME;
				isStartMenu = true;
			}
			OSTimeDly(100, OS_OPT_TIME_PERIODIC, &err);
			APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);


// screen states and actions here.



		}


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

//	 car_choices[0] =  (vehicle_description_t *)&clunker;
//	 car_choices[1] =  (vehicle_description_t *)&sport;
//	 car_choices[2] =  (vehicle_description_t *)&truck;

	create_game_over_flag();

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

void create_game_over_flag(void) {
	RTOS_ERR err;

	//Create Model Sem
	OSFlagCreate(&game_over_flag,
				"Game Over Flag",
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

			GLIB_clear(&menu_context);
			GLIB_drawString(&menu_context, "Choose your Course:", 20,
	                  5,
	                  5,
	                  0);
			GLIB_drawString(&menu_context, "Dense R", 7,
							                CENTER_X - GLIB_FONT_WIDTH*1,
							                  40,
							                  0);
			GLIB_drawString(&menu_context, "Sparse R", 8,
											CENTER_X - GLIB_FONT_WIDTH*3,
							                  60,
							                  0);
			GLIB_drawString(&menu_context, "Random", 6,
											CENTER_X - GLIB_FONT_WIDTH*1,
							                  80,
							                  0);
			break;
		case CHOOSE_COND:
			GLIB_clear(&menu_context);
			GLIB_drawString(&menu_context, "Choose Conditions:", 20,
	                  5,
	                  5,
	                  0);
			GLIB_drawString(&menu_context, "ASPHALT", 7,
							                CENTER_X - GLIB_FONT_WIDTH*1,
							                  40,
							                  0);
			GLIB_drawString(&menu_context, "GRAVEL", 6,
											CENTER_X - GLIB_FONT_WIDTH*3,
							                  60,
							                  0);
			GLIB_drawString(&menu_context, "ICE", 3,
											CENTER_X - GLIB_FONT_WIDTH*1,
							                  80,
							                  0);
			break;
		default:
			// error: reset to welcome
			screen = WELCOME;
			break;
	}
	if (select_active) {
		GLIB_drawCircleFilled(&menu_context, 64,
        40+20*selection + 4, 3);
	}

	DMD_updateDisplay();
}

void write_gameover(uint8_t screen, failure_t failure_cause, stats_t stats) {
	GLIB_clear(&menu_context);

	char print_buf[50];

	switch(screen) {
		case GAME_OVER:
			if(failure_cause == FAIL_NO_FAIL) {
				GLIB_drawString(&menu_context, "Congrats! You ", 14, 30,30,0);
				GLIB_drawString(&menu_context, "completed the course", 20, 5,70,0);
				GLIB_drawString(&menu_context, "For Stats:", 20, 40,90,0);
				GLIB_drawString(&menu_context, "press a button", 14, 20,110,0);
			}
			else{
				GLIB_drawString(&menu_context, "You Failed! ", 14, 30,30,0);
				GLIB_drawString(&menu_context, "Cause of Failure:", 20, 5,70,0);
				get_failure_string(failure_cause , print_buf);
				GLIB_drawString(&menu_context, print_buf, 20, 40,90,0);
				GLIB_drawString(&menu_context, "press a button", 14, 20,110,0);
			}
			break;
		case STATS:
			GLIB_drawString(&menu_context, "Stats ", 14, 30,30,0);
			GLIB_drawString(&menu_context, "Time on Course:", 13, 5,70,0);
			gcvt(stats.time_on_course, 4, print_buf);
			GLIB_drawString(&menu_context, print_buf, 13, 90,70,0);

			GLIB_drawString(&menu_context, "Max Speed:", 13, 5,90,0);
			gcvt(stats.max_speed, 4, print_buf);
			GLIB_drawString(&menu_context, print_buf, 13, 90,90,0);

			break;
	}
	return;
}

void release_menu_control(void) {
	create_physics_model_task();
	create_roadgen_task();
	create_lcd_task();
	// f_create_vehicle_monitor_task();
	return;
}

void take_menu_control(void) {
	delete_physics_model_task();
	delete_roadgen_task();
//	delete_lcd_task();
	 f_delete_vehicle_monitor_task();
	return;
}

void redraw_selection(uint8_t screen, uint8_t selection) {
	char msg[12];
	switch(screen) {
		case CHOOSE_CAR:
			if (selection == 0)
				strcpy(msg, "Clunker");
			else if (selection == 1)
				strcpy(msg, "Sports Car");
			else if (selection == 2)
				strcpy(msg, "Truck");
			break;
		case CHOOSE_COURSE:
					if (selection == 0)
						strcpy(msg, "Dense R");
					else if (selection == 1)
						strcpy(msg, "Sparse R");
					else if (selection == 2)
						strcpy(msg, "Random");
					break;
		case CHOOSE_COND:
					if (selection == 0)
						strcpy(msg, "ASPHALT");
					else if (selection == 1)
						strcpy(msg, "GRAVEL");
					else if (selection == 2)
						strcpy(msg, "ICE");
					break;
	}
	GLIB_drawString(&menu_context, msg, 12,
					CENTER_X - GLIB_FONT_WIDTH*3,
					  40 +selection*20,
					  0);
}

void get_failure_string(failure_t fail, char * buf) {
	if (fail == FAIL_SLIP) {
		strcpy(buf, "Slip Fail");
	}
	else if (fail == FAIL_OFFROAD) {
		strcpy(buf, "Offroad Fail");
	}
}
