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



static EMSTATUS GLIB_drawCar(GLIB_Context_t *pContext, int16_t veh_x, int16_t veh_y, double degrees, bool clear_flag) {
	EMSTATUS status = GLIB_OK;
	if(clear_flag)
		GLIB_clear(pContext);

	int x_rotation = round(veh_x + ARROW_LEN*sin((degrees)*DEG2RAD));
	int y_rotation = round(veh_y - ARROW_LEN*cos((degrees)*DEG2RAD));
	int lx_rotation = round(veh_x +TIP_RADIUS*sin((degrees*DEG2RAD) - TIP_RADIAN_OFFSET));
	int ly_rotation = round(veh_y -TIP_RADIUS*cos((degrees*DEG2RAD) - TIP_RADIAN_OFFSET));
	int rx_rotation = round(veh_x +TIP_RADIUS*sin((degrees*DEG2RAD) + TIP_RADIAN_OFFSET));
	int ry_rotation = round(veh_y -TIP_RADIUS*cos((degrees*DEG2RAD) + TIP_RADIAN_OFFSET));


	GLIB_drawCircle(pContext, veh_x, veh_y, CAR_RADIUS);

	GLIB_drawLine(pContext, veh_x, veh_y, x_rotation, y_rotation);

	GLIB_drawLine(pContext, lx_rotation, ly_rotation, x_rotation, y_rotation);
	GLIB_drawLine(pContext, rx_rotation, ry_rotation, x_rotation, y_rotation);
	DMD_updateDisplay();

	return status;
}


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
	char spd_substr[SPD_SUBSTR_MAX_LENGTH];
	float veh_x, veh_y;
	double degree = 0;
	float rads, veh_spd;
	lcd_init();
	while(1) {
		if (OSSchedLockNestingCtr) {
			//unlock scheduler
			OSSchedLockNestingCtr = 0;
		}
		OSSemPend(&phys_model_update_sem,
					0,
					OS_OPT_PEND_BLOCKING,
					(CPU_TS*)0,
					&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		// get vehicle position
		veh_x = vehicle_model.p.x;
		veh_y = vehicle_model.p.y;
		rads = vehicle_model.az;
		veh_spd = vect_mag(vehicle_model.v);

		OSSchedLock(&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		sprintf(spd_substr, "Speed: %.2f\n", veh_spd);
		switch(vehicle_dir.dir) {
			case DIR_FAR_LEFT:
				strcpy(dir_substr, "Hard Left ");
				degree = -45.;
				break;
			case DIR_LEFT:
				strcpy(dir_substr, "Left      ");
				degree = -15.;
				break;
			case DIR_RIGHT:
				strcpy(dir_substr, "Right     ");
				degree = 15.;
				break;
			case DIR_FAR_RIGHT:
				strcpy(dir_substr, "Hard Right");
				degree = 45.;
				break;
			case DIR_STRAIGHT:
			default:
				degree = 0.;
				strcpy(dir_substr, "Straight  ");
				break;
		} // end switch

		GLIB_clear(&glibContext);
		GLIB_drawString(&glibContext, spd_substr, strlen(spd_substr), 5, 5, 0);
//		GLIB_drawString(&glibContext, dir_substr, strlen(dir_substr), 15, 15, 0);
		GLIB_drawCar(&glibContext, veh_x, veh_y, rads, 0);
//			printf("The current speed is %d mph   \nThe current direction is %s\033H", vehicle_speed.speed, dir_substr);
		OSSchedUnlock(&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		/* Updated display */

	} // end while
} // end LCD task

void lcd_init() {
	EMSTATUS status;
	/* Initialize the display module. */
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

	  status = GLIB_contextInit(&glibContext);
	  if (GLIB_OK != status) {
		while (1) ;
	  }
	  glibContext.backgroundColor = White;
	  glibContext.foregroundColor = Black;
	  GLIB_setFont(&glibContext, (GLIB_Font_t *)&GLIB_FontNarrow6x8);
	  GLIB_clear(&glibContext);
	  DMD_updateDisplay();

	/* Retarget stdio to a text display. */
//	if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK) {
//	while (1) ;
//	}

}
