/*
 * lcd.h
 *
 *  Created on: Mar 1, 2020
 *      Author: goat
 */

#ifndef SRC_HEADER_FILES_LCD_H_
#define SRC_HEADER_FILES_LCD_H_

#include "lab7.h"
#include <glib.h>
#include <glib_color.h>
#include "display.h"
#include "dmd.h"
#include "textdisplay.h"
#include "retargettextdisplay.h"

/************************/
/* Task Defines			*/
/************************/
#define LCD_TASK_PRIO					23u
#define LCD_TASK_STK_SIZE				1024u
CPU_STK  LcdTaskStk[VEHICLE_MONITOR_TASK_STK_SIZE];
OS_TCB   LcdTaskTCB;

/********************************
 * Logical/Operational defines  *
 ********************************/
#define LCD_PERIOD						100 //in ms
#define LCD_PERIOD_TICKS				100

#define DIR_STR_MAX_LENGTH				36
#define SPEED_STR_MAX_LENGTH			30
#define DIR_SUBSTR_MAX_LENGTH			11


/************************
 * global variables		*
 ************************/
/* Global glib context */
GLIB_Context_t gc;


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
