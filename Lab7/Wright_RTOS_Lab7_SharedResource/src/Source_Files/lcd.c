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
#include <common/include/rtos_utils.h>


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
			OSSchedLock(&err);
			APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
			switch(vehicle_dir.dir) {
				case DIR_FAR_LEFT:
					strcpy(dir_substr, "Hard Left ");
					break;
				case DIR_LEFT:
					strcpy(dir_substr, "Left      ");
					break;
				case DIR_RIGHT:
					strcpy(dir_substr, "Right     ");
					break;
				case DIR_FAR_RIGHT:
					strcpy(dir_substr, "Hard Right");
					break;
				case DIR_STRAIGHT:
				default:
					strcpy(dir_substr, "Straight  ");
					break;
			}
			printf("The current speed is %d mph   \nThe current direction is %s\033H", vehicle_speed.speed, dir_substr);
			OSSchedUnlock(&err);
			APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
			/* Updated display */
			updated = true;
		}
		OSTimeDly(LCD_PERIOD_TICKS,
				OS_OPT_TIME_DLY,
				&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	} // end while
} // end LCD task

void lcd_init() {
	/* Initialize the display module. */
	DISPLAY_Init();

	/* Retarget stdio to a text display. */
	if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK) {
	while (1) ;
	}

}
