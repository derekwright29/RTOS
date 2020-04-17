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

#define GLIB_FONT_WIDTH   (glibContext.font.fontWidth + glibContext.font.charSpacing)
#define GLIB_FONT_HEIGHT  (glibContext.font.fontHeight)

/* Center of display */
#define CENTER_X (glibContext.pDisplayGeometry->xSize / 2)
#define CENTER_Y (glibContext.pDisplayGeometry->ySize / 2)

#define MAX_X (glibContext.pDisplayGeometry->xSize - 1)
#define MAX_Y (glibContext.pDisplayGeometry->ySize - 1)

#define MIN_X           0
#define MIN_Y           0

#define INIT_DEMO_NO    0

#define MAX_STR_LEN     48

/* The GLIB context */
static GLIB_Context_t   glibContext;

#define CAR_CENTER_Y 	MAX_Y - 10
#define CAR_CENTER_X 	CENTER_X
#define CAR_RADIUS 		10
#define DEG2RAD			3.1415826535 / 180.


#define ARROW_LEN				20
#define ARROW_TIP_LEN			3

#define TIP_LX_BASE		CAR_CENTER_X
#define TIP_LY_BASE		CAR_CENTER_Y

#define TIP_RADIUS				sqrt(pow((double)ARROW_TIP_LEN,2.) + pow((double)ARROW_LEN-ARROW_TIP_LEN, 2.))
#define TIP_RADIAN_OFFSET		atan((double)ARROW_TIP_LEN/(double)(ARROW_LEN-ARROW_TIP_LEN))

static EMSTATUS GLIB_drawCar(GLIB_Context_t *pContext, double degrees, bool clear_flag) {
	EMSTATUS status = GLIB_OK;
	if(clear_flag)
		GLIB_clear(pContext);

	int x_rotation = round(CAR_CENTER_X + ARROW_LEN*sin((degrees)*DEG2RAD));
	int y_rotation = round(CAR_CENTER_Y - ARROW_LEN*cos((degrees)*DEG2RAD));
	int lx_rotation = round(TIP_LX_BASE +TIP_RADIUS*sin((degrees*DEG2RAD) - TIP_RADIAN_OFFSET));
	int ly_rotation = round(TIP_LY_BASE -TIP_RADIUS*cos((degrees*DEG2RAD) - TIP_RADIAN_OFFSET));
	int rx_rotation = round(TIP_LX_BASE +TIP_RADIUS*sin((degrees*DEG2RAD) + TIP_RADIAN_OFFSET));
	int ry_rotation = round(TIP_LY_BASE -TIP_RADIUS*cos((degrees*DEG2RAD) + TIP_RADIAN_OFFSET));


	GLIB_drawCircle(pContext, CAR_CENTER_X, CAR_CENTER_Y, CAR_RADIUS);

	GLIB_drawLine(pContext, CAR_CENTER_X, CAR_CENTER_Y, x_rotation, y_rotation);

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
	char spd_substr[DIR_SUBSTR_MAX_LENGTH];
	int16_t cur_spd = 0;
	Direction_t cur_dir = DIR_STRAIGHT;
	bool updated = true;
	double degree = 0;
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
			sprintf(spd_substr, "Speed: %d\n", vehicle_speed.speed);
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
			}
			GLIB_clear(&glibContext);
			GLIB_drawString(&glibContext, spd_substr, strlen(spd_substr), 5, 5, 0);
			GLIB_drawString(&glibContext, dir_substr, strlen(dir_substr), 15, 15, 0);
			GLIB_drawCar(&glibContext, degree, 0);
//			printf("The current speed is %d mph   \nThe current direction is %s\033H", vehicle_speed.speed, dir_substr);
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
