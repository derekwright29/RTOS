/*
 * lcd.h
 *
 *  Created on: Mar 1, 2020
 *      Author: goat
 */
#ifndef SRC_HEADER_FILES_LCD_H_
#define SRC_HEADER_FILES_LCD_H_

#include <glib.h>
#include <glib_color.h>
#include "display.h"
#include "dmd.h"
#include "final_proj.h"
#include "textdisplay.h"
#include "retargettextdisplay.h"
#include "phys_model.h"

/************************/
/* Task Defines			*/
/************************/
#define LCD_TASK_PRIO					19u
#define LCD_TASK_STK_SIZE				1024u
CPU_STK  LcdTaskStk[LCD_TASK_STK_SIZE];
OS_TCB   LcdTaskTCB;

/********************************
 * Logical/Operational defines  *
 ********************************/
#define LCD_PERIOD						100 //in ms
#define LCD_PERIOD_TICKS				100

#define DIR_STR_MAX_LENGTH				36
#define SPEED_STR_MAX_LENGTH			30
#define DIR_SUBSTR_MAX_LENGTH			11
#define SPD_SUBSTR_MAX_LENGTH			40


/********************************
 * GLIB defines *
 ********************************/
#define GLIB_FONT_WIDTH   (glibContext.font.fontWidth + glibContext.font.charSpacing)
#define GLIB_FONT_HEIGHT  (glibContext.font.fontHeight)

/* Center of display */
#define CENTER_X (glibContext.pDisplayGeometry->xSize / 2)
#define CENTER_Y (glibContext.pDisplayGeometry->ySize / 2)

#define MAX_X (glibContext.pDisplayGeometry->xSize - 1)
#define MAX_Y (glibContext.pDisplayGeometry->ySize - 1)

#define MIN_X           0
#define MIN_Y           0


/* The GLIB context */
static GLIB_Context_t   glibContext;

#define CAR_CENTER_Y 	MAX_Y - 10
#define CAR_CENTER_X 	CENTER_X
#define CAR_RADIUS 		10
#define DEG2RAD			(float)M_PI / 180.
#define RAD2DEG			(float)180./M_PI


#define ARROW_LEN				20
#define ARROW_TIP_LEN			3

#define TIP_RADIUS				sqrt(pow((double)ARROW_TIP_LEN,2.) + pow((double)ARROW_LEN-ARROW_TIP_LEN, 2.))
#define TIP_RADIAN_OFFSET		atan((double)ARROW_TIP_LEN/(double)(ARROW_LEN-ARROW_TIP_LEN))


/************************
 * global variables		*
 ************************/
/* Global glib context */
GLIB_Context_t gc;

extern OS_SEM phys_model_update_sem;



/**
 * create_lcd_task()
 * ----------------
 * @description This function simply makes the OS call to start the LCD task.
 *
 * @param	None
 *
 * @return	None
 *
 */
void create_lcd_task(void);

/**
 * LCDTask()
 * ----------------
 * @description This function is the main loop for the LCD task.
 * 				The task will wakeup periodically to update the LCD screen with the current data.
 *
 * @param	None
 *
 * @return	None
 *
 */
void LCDTask(void * p_arg);


/**
 * lcd_init()
 * ----------------
 * @description This task performs the init of the LCD
 *
 * @param	None
 *
 * @return	None
 *
 */
void lcd_init(void);




#endif /* SRC_HEADER_FILES_LCD_H_ */
