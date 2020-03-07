/*
 * lcd.c
 *
 *  Created on: Mar 2, 2020
 *      Author: goat
 */

#include "lcd.h"
#include <string.h>
#include <stdio.h>
#include "display.h"
#include "dmd.h"


void create_lcd_task() {
	RTOS_ERR err;
	/* Create the LCD Task.                               */
	OSTaskCreate(&LcdTaskTCB,
				 "LCD Task",
				  LCDTask,
				  DEF_NULL,
				  LCD_TASK_PRIO,
				 &LcdTaskStk[0],
				 (LCD_TASK_STK_SIZE / 10u),
				 LCD_TASK_STK_SIZE,
				  0u,
				  0u,
				  DEF_NULL,
				 (OS_OPT_TASK_STK_CLR),
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

}


void LCDTask(void *p_arg) {
	RTOS_ERR err;
	//void  OSSchedLock (RTOS_ERR  *p_err)
	//void  OSSchedUnlock (RTOS_ERR  *p_err)
	char speed_str[SPEED_STR_MAX_LENGTH], dir_str[DIR_STR_MAX_LENGTH];
	char dir_substr[DIR_SUBSTR_MAX_LENGTH];
	int16_t cur_spd = 0;
	Direction_t cur_dir = DIR_STRAIGHT;
	bool updated = true;
	lcd_init();
	while(1) {
		if ((cur_dir != vehicle_dir.dir) || (cur_spd != vehicle_speed.speed)) {
			updated = false;
			cur_dir = vehicle_dir.dir;
			cur_spd = vehicle_speed.speed;
		}
		if (!updated) {
			printf(speed_str, "The current speed is %d mph\n", vehicle_speed.speed);
			switch(vehicle_dir.dir) {
				case DIR_FAR_LEFT:
					strcpy(dir_substr, "Hard Left");
					break;
				case DIR_LEFT:
					strcpy(dir_substr, "Left");
					break;
				case DIR_STRAIGHT:
					strcpy(dir_substr, "Straight");
					break;
				case DIR_RIGHT:
					strcpy(dir_substr, "Right");
					break;
				case DIR_FAR_RIGHT:
					strcpy(dir_substr, "Hard Right");
					break;
				default:
					strcpy(dir_substr, "Unknown!");
					break;
			}
			printf(dir_str, "The current direction is %s\n", dir_substr);

			/* Updated display */
			updated = true;
		}
		OSTimeDly(LCD_PERIOD_TICKS,
				OS_OPT_TIME_PERIODIC,
				&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	} // end while
} // end LCD task

void lcd_init() {
//	EMSTATUS status;

//	/* Initialize display module */
//	status = DISPLAY_Init();
//	if (DISPLAY_EMSTATUS_OK != status) {
//	while (true)
//	  ;
//	}
//
//	/* Initialize the DMD module for the DISPLAY device driver. */
////	status = DMD_init(0);
////	if (DMD_OK != status) {
////		while (true)
////			;
////	}
//
//	status = GLIB_contextInit(&gc);
//	if (GLIB_OK != status) {
//		while (true)
//			;
//	}

	/* Initialize the display module. */
	DISPLAY_Init();

	/* Retarget stdio to a text display. */
	if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK) {
	while (1) ;
	}

}
