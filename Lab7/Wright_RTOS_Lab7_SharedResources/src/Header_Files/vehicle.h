/*
 * vehicle.h
 *
 *  Created on: Mar 1, 2020
 *      Author: goat
 */

#ifndef SRC_HEADER_FILES_VEHICLE_H_
#define SRC_HEADER_FILES_VEHICLE_H_

#include  <stdint.h>
#include  <kernel/include/os.h>
#include <time.h>
#include "buttons.h"

/*************************
 * Vehicle Tasks defines
 ***********************/
/* Vehicle Monitor Task*/
#define VEHICLE_MONITOR_TASK_PRIO					24u
#define VEHICLE_MONITOR_TASK_STK_SIZE				1024u
CPU_STK  VehicleMonitorTaskStk[VEHICLE_MONITOR_TASK_STK_SIZE];
OS_TCB   VehicleMonitorTaskTCB;

/* Speed Setpoint Task*/
#define SPEED_TASK_PRIO					25u
#define SPEED_TASK_STK_SIZE				1024u
CPU_STK  SpeedTaskStk[VEHICLE_MONITOR_TASK_STK_SIZE];
OS_TCB   SpeedTaskTCB;

#define DIRECTION_TASK_PRIO					26u
#define DIRECTION_TASK_STK_SIZE				1024u
CPU_STK  DirectionTaskStk[VEHICLE_MONITOR_TASK_STK_SIZE];
OS_TCB   DirectionTaskTCB;


/**
 * Logic/Operational Defines
 */
#define NO_CHANGE_TIMEOUT  				5000	// in ms = 5sec

#define DIRECTION_TASK_PERIOD			50		// in ms
#define DIRECTION_TASK_PERIOD_TICKS		1

#define SPEED_INCREMENT					5		// in mph
#define SPEED_LIMIT						75
#define TURN_SPEED_LIMIT				45


/** **********************
 * Vehicle Type Defines
 ** ***********************/
typedef struct VehicleSpeed {
	uint8_t speed_cnt;
	uint8_t inc_cnt;
	uint8_t dec_cnt;
	int16_t speed;
} VehicleSpeed_t;

typedef enum SpeedFlag {
	SPEED_CHANGED = 1,
}SpeedFlag_t;

// define this as independent bits to be used in direction flag as well.
typedef enum Direction {
	DIR_FAR_LEFT = 1,
	DIR_LEFT = 2,
	DIR_RIGHT = 4,
	DIR_FAR_RIGHT = 8,
	DIR_STRAIGHT = 16,
} Direction_t;

#define DIR_FLAGS_ALL 	(DIR_FAR_LEFT | DIR_LEFT | DIR_RIGHT | DIR_FAR_RIGHT | DIR_STRAIGHT )

typedef struct VehicleDir {
	Direction_t dir;
	time_t      time_since_change;
	uint16_t    left_cnt;
	uint16_t    right_cnt;
}VehicleDir_t;


typedef enum VehicleAlert {
	ALERT_DIRECTION_TIMEOUT = 1,
	ALERT_SPEED_LIMIT = 2,
	ALERT_TURN_SPEED_LIMIT = 4,
	ALERT_NO_ALERT = 8,
}VehicleAlert_t;
#define ALERT_FLAGS_ALL 	(ALERT_DIRECTION_TIMEOUT | ALERT_SPEED_LIMIT | ALERT_TURN_SPEED_LIMIT)

/*************************
 * LOCAL GLOBAL VARIABLES
 ************************/
OS_FLAG_GRP speed_flags;
OS_FLAG_GRP dir_flags;
OS_FLAG_GRP alert_flags;

OS_SEM speed_change_sem;
OS_MUTEX speed_mutex;
OS_MUTEX dir_mutex;

OS_TMR no_change_timer;

/* Vehicle Info data structures */
VehicleSpeed_t vehicle_speed;
VehicleDir_t vehicle_dir;


/** **********************
 * Vehicle Function Prototypes
 ** ***********************/

/**
 * create_vehicle_speed_task()
 * ----------------
 * @description This function simply makes the OS call to start the Speed task.
 *
 * @param	None
 *
 * @return	None
 *
 */
void create_vehicle_speed_task(void);

/**
 * create_vehicle_dir_task()
 * ----------------
 * @description This function simply makes the OS call to start the Direction task.
 *
 * @param	None
 *
 * @return	None
 *
 */
void create_vehicle_dir_task(void);


/**
 *  * vehicle_get_alert()
 * ----------------
 * @description This function returns the current alert state of the system
 *
 * @param	Vehicle Speed
 * 			Vehicle Direction
 *
 * @return	Vehicle Alert based on the struct
 *
 */
VehicleAlert_t vehicle_get_alert(int16_t speed, Direction_t dir);

/**
 * create_vehicle_monitor_task()
 * ----------------
 * @description This function simply makes the OS call to start the Monitor task.
 *
 * @param	None
 *
 * @return	None
 *
 */
void create_vehicle_monitor_task(void);

/**
 * VehicleSpeedTask()
 * ----------------
 * @description This function is the main function of the Speed Task
 *
 * @param	None
 *
 * @return	None
 *
 */
void VehicleSpeedTask(void * p_arg);



/**
 * decideDirection()
 * ----------------
 * @description This function decides the direction we are going based on Capsense measurements
 * 				I enforce strict input in order to trigger a hard turn (has to be ONLY the far left or right channel)
 * 				For the rest, I add up the left side and right side, and go with which has more votes
 * 				If they are the same, we go straight.
 *
 * 				Because I'm lazy, it also updates the bool array passed in to be a copy of capsenseIsPressed
 *
 * @param	state_array is a bool array the same length as capsenseIsPressed
 *
 * @return	The direction we have chosen
 *
 */
Direction_t decideDirection(bool * state_array);
/**
 * VehicleDirectionTask()
 * ----------------
 * @description This function is the main function of the Direction Task
 *
 * @param	None
 *
 * @return	None
 *
 */
void VehicleDirectionTask(void * p_arg);


/**
 * * VehicleDirectionTask()
 * ----------------
 * @description This function is the callback function of the Alert Timer
 * 				It will post the alert flag to the LED task.
 *
 * @param	None
 *
 * @return	None
 *
 */
void AlertTimerCallback(void);

/**
 * VehicleMonitorTask()
 * ----------------
 * @description This function is the main function of the Monitor Task
 *
 * @param	None
 *
 * @return	None
 *
 */
void VehicleMonitorTask(void * p_arg);



#endif /* SRC_HEADER_FILES_VEHICLE_H_ */
