/*
 * lcd.h
 *
 *  Created on: Mar 1, 2020
 *      Author: goat
 */

#ifndef SRC_HEADER_FILES_LCD_H_
#define SRC_HEADER_FILES_LCD_H_


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
 * LCDTimerCallback()
 * ----------------
 * @description This function signals to the LCD task to wake up, using the semaphore
 *
 * @param	None
 *
 * @return	None
 *
 */
void LCDTimerCallback(void * p_arg);


#endif /* SRC_HEADER_FILES_LCD_H_ */
