/*
 * menu.h
 *
 *  Created on: Apr 17, 2020
 *      Author: goat
 */

#ifndef SRC_HEADER_FILES_MENU_H_
#define SRC_HEADER_FILES_MENU_H_

#include <glib.h>
#include <glib_color.h>
#include "display.h"
#include "dmd.h"
#include "final_proj.h"
#include <string.h>
/************************/
/* Task Defines			*/
/************************/
#define MENU_TASK_PRIO					19u
#define MENU_TASK_STK_SIZE				1024u
CPU_STK  MenuTaskStk[MENU_TASK_STK_SIZE];
OS_TCB   MenuTaskTCB;


OS_SEM game_over_sem;

static GLIB_Context_t menu_context;


const vehicle_description_t clunker = VEHICLE_DESC_STRUCT_CLUNKER;
const vehicle_description_t sport = VEHICLE_DESC_STRUCT_SPORT;
const vehicle_description_t truck = VEHICLE_DESC_STRUCT_TRUCK;

const vehicle_description_t car_choices[] = {   clunker, sport, truck   };




enum screentypes {
	WELCOME,
	CHOOSE_CAR,
	CHOOSE_COURSE,
	CHOOSE_COND,
	READY_TO_START,
};




void toggle_menu_control(void);

void create_menu_task(void);

void MenuTask(void *p_arg);

void menu_init(void);

void create_game_over_semaphore(void);

void write_menu(uint8_t screen, uint8_t selection, bool select_active);

#endif /* SRC_HEADER_FILES_MENU_H_ */
