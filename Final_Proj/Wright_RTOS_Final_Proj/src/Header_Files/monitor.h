#ifndef _MONITOR_H
#define _MONITOR_H

#include "final_proj.h"

#include <kernel/include/os.h>


#define FVEHICLE_MONITOR_TASK_PRIO					25u
#define FVEHICLE_MONITOR_TASK_STK_SIZE				1024u
CPU_STK  FVehicleMonitorTaskStk[VEHICLE_MONITOR_TASK_STK_SIZE];
OS_TCB   FVehicleMonitorTaskTCB;

// on own timer/timing?
#define FMONITOR_TASK_PERIOD_TICKS		100		//in ms

typedef enum FVehicleAlert {
	FALERT_DIRECTION_TIMEOUT = 1,
	FALERT_SPEED_LIMIT = 2,
	FALERT_TURN_SPEED_LIMIT = 4,
	FALERT_NO_ALERT = 8,
}FVehicleAlert_t;
#define F_ALERT_FLAGS_ALL 	(FALERT_DIRECTION_TIMEOUT | FALERT_SPEED_LIMIT | FALERT_TURN_SPEED_LIMIT | FALERT_NO_ALERT)

typedef struct {
    float max_speed;
    float time_on_course;
    float best_time;
} stats_t;


/*************************
 * LOCAL GLOBAL VARIABLES
 ************************/
OS_FLAG_GRP f_speed_flags;
OS_FLAG_GRP f_dir_flags;
OS_FLAG_GRP f_alert_flags;


stats_t stats;





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
void f_create_vehicle_monitor_task(void);

/**
 * monitor_task_init()
 * ----------------
 * @description This function inits the constructs necessary for the monitor task
 *
 * @param	None
 *
 * @return	None
 *
 */
void f_monitor_task_init(void);

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
void FVehicleMonitorTask(void * p_arg);


/**
 * create_vehicle_flags()
 * ----------------
 * @description This function creates all the flag groups used by multiple tasks
 * 				To be called in the main task so that no race conditions exist with accessing the flags
 *
 * @param	None
 *
 * @return	None
 *
 */
void f_create_vehicle_flags(void);

#endif /* _MONITOR_H */
