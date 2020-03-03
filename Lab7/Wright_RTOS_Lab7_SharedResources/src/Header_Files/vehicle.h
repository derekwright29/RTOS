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
#include "lab7.h"
#include <time.h>

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
	SPEED_INCREASE = 1,
	SPEED_DECREASE = 2,
}SpeedFlag_t;

// define this as independent bits to be used in direction flag as well.
typedef enum Direction {
	DIR_FAR_LEFT = 1,
	DIR_LEFT = 2,
	DIR_RIGHT = 4,
	DIR_FAR_RIGHT = 8,
} Direction_t;

typedef struct VehicleDir {
	Direction_t veh_dir;
	time_t      time_since_change;
	uint16_t    left_cnt;
	uint16_t    right_cnt;
}VehicleDir_t;


typedef enum VehicleAlert {
	ALERT_DIRECTION_TIMEOUT = 1,
	ALERT_SPEED_LIMIT = 2,
	ALERT_TURN_SPEED_LIMIT = 4,
}VehicleAlert_t;

/*************************
 * LOCAL GLOBAL VARIABLES
 ************************/
OS_FLAG_GRP speed_flags;
OS_FLAG_GRP dir_flags;

OS_SEM speed_sem;
OS_SEM dir_sem;


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
