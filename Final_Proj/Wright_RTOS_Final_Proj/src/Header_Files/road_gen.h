#ifndef __ROAD_GEN_H
#define __ROAD_GEN_H

#include  <kernel/include/os.h>
#include <common/include/rtos_utils.h>
#include "my_vect.h"

// TODO: move to road_gen.h/c
typedef struct RoadLayout		// Purple items are only relevant for specific 3-D enhancements.
{
	char Name[16];			// null terminated
	uint16_t RoadWidth;			// cm
	uint16_t NumWaypoints;		//
	int_vect_t* Waypoints;				// Array of waypoints.  First and last 2 are not to be driven
}road_description_t;

typedef enum road_conditions {
    ASPHALT = 1,
    EARTH = 2,
    GRAVEL = 3,
    SAND = 4,
    ICE = 5
}road_condition_t;


extern road_description_t course;
extern road_condition_t road_cond;



/*
 * Course definitions
 * */
#define SPARSE_R_WAYPOINTS 		 { (int_vect_t){15,0},		(int_vect_t){0,0},		(int_vect_t){10,0},		(int_vect_t){20,0},		(int_vect_t){50,10},\
									(int_vect_t){80,5},		(int_vect_t){130,20}, 	(int_vect_t){135,110},	(int_vect_t){135,190},	(int_vect_t){170,225},\
									(int_vect_t){205,210},	(int_vect_t){205,190},	(int_vect_t){150,100},	(int_vect_t){165,50},	(int_vect_t){200,40},\
									(int_vect_t){210,38},	(int_vect_t){230,39}    }

#define DENSE_R_WAYPOINTS		{(int_vect_t) {10,10}}

extern const int_vect_t Sparse_R[20];
extern const int_vect_t DenseR[100];

#define ROAD_DESC_SPARSE_R_DEFAULT		(road_description_t){"Sparse R", 20, 17, SparseR}
#define ROAD_CONDITION_DEFAULT			(road_condition_t) ASPHALT

// Req to stay 5 waypoints ahead
extern int_vect_t WayPointBuffer[5];
ROAD_GEN_WP_BUFFER_SIZE			10


#define ROADGEN_TASK_PRIO					27u
#define ROADGEN_TASK_STK_SIZE				1024u
CPU_STK  RoadGenTaskStk[ROADGEN_TASK_STK_SIZE];
OS_TCB   RoadGenTaskTCB;

OS_SEM new_waypoint_sem;


void create_roadgen_task(void);

void roadgen_init(void );

void create_roadgen_sems(void );

void RoadGenTask(void *p_arg);











#endif /* __ROAD_GEN_H*/
