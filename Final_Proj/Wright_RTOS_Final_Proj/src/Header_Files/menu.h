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
#include "monitor.h"
/************************/
/* Task Defines			*/
/************************/
#define MENU_TASK_PRIO					19u
#define MENU_TASK_STK_SIZE				1024u
CPU_STK  MenuTaskStk[MENU_TASK_STK_SIZE];
OS_TCB   MenuTaskTCB;


OS_FLAG_GRP game_over_flag;

static GLIB_Context_t menu_context;


//vehicle_description_t clunker = VEHICLE_DESC_STRUCT_CLUNKER;
//vehicle_description_t sport = VEHICLE_DESC_STRUCT_SPORT;
//vehicle_description_t truck = VEHICLE_DESC_STRUCT_TRUCK;
//
//vehicle_description_t * car_choices[3];


#ifndef TYPES_DEFINED_
#define TYPES_DEFINED_
typedef enum FailModes {
	FAIL_NO_FAIL = 1,
	FAIL_SLIP = 2,
	FAIL_OFFROAD = 4
}failure_t;

#define ALL_FAIL_FLAGS		(FAIL_NO_FAIL | FAIL_SLIP | FAIL_OFFROAD)


typedef struct {
    float max_speed;
    float time_on_course;
    float distance_travelled;
    bool success;
    failure_t cause_of_fail;
} stats_t;
#endif


enum screentypes {
	WELCOME,
	CHOOSE_CAR,
	CHOOSE_COURSE,
	CHOOSE_COND,
	READY_TO_START,
	GAME_OVER,
	STATS
};

#define START_NOW_SIGNAL  (uint8_t) 1


void release_menu_control(void);
void take_menu_control(void);

void create_menu_task(void);

void MenuTask(void *p_arg);

void menu_init(void);

void create_game_over_flag(void);

void write_menu(uint8_t screen, uint8_t selection, bool select_active);

void write_gameover(uint8_t screen, failure_t failure_cause, stats_t stats);

void redraw_selection(uint8_t screen, uint8_t selection) ;

void get_failure_string(failure_t fail, char * buf);

#endif /* SRC_HEADER_FILES_MENU_H_ */
