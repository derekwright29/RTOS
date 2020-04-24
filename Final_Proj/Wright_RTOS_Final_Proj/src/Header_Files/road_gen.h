#ifndef __ROAD_GEN_H
#define __ROAD_GEN_H

#include  <kernel/include/os.h>
#include <common/include/rtos_utils.h>
#include "my_vect.h"
#include "vehicle.h"
#include "fifo.h"

// TODO: move to road_gen.h/c
typedef struct RoadLayout		// Purple items are only relevant for specific 3-D enhancements.
{
	char Name[16];			// null terminated
	uint16_t RoadWidth;			// cm
	uint16_t NumWaypoints;		//
	const int_vect_t* Waypoints;				// Array of waypoints.  First and last 2 are not to be driven
}road_description_t;

typedef enum road_conditions {
    ASPHALT = 1,
    EARTH = 2,
    GRAVEL = 3,
    SAND = 4,
    ICE = 5
}road_condition_t;

#define MAX_NUM_WAYPOINTS 	50


extern road_description_t course;
extern road_condition_t road_cond;
extern float course_headings[MAX_NUM_WAYPOINTS];



/*
 * Course definitions
 * */
#define SPARSE_R_WAYPOINTS 		 { (int_vect_t){20,120},	(int_vect_t){20,110},	(int_vect_t){20,100},	(int_vect_t){20,80},	(int_vect_t){20,60},\
									(int_vect_t){30,30},	(int_vect_t){50,20}, 	(int_vect_t){64,25},	(int_vect_t){80,40},	(int_vect_t){85,60},\
									(int_vect_t){80,75},	(int_vect_t){70,85},	(int_vect_t){60,90},	(int_vect_t){55,100},	(int_vect_t){80,105},\
									(int_vect_t){105,110},	(int_vect_t){115,120}    }

#define SPARSE_R_HEADINGS		{ 					0.,	 					0., 					0., 					0., 					0.,\
								  	  	  	  	  0.32175, 				  1.107,				1.91382,				2.32395, 				2.896613,\
												  3.4633,				3.92699,				4.248741,				3.60524,				1.7681919,\
												  1.7681919,				2.35619}

#define DENSE_R_WAYPOINTS		{(int_vect_t) {10,10}}

extern const int_vect_t Sparse_R[20];
extern const float Sparse_R_Headings[20];
extern const int_vect_t Dense_R[100];

extern uint16_t waypoint_index;

#define ROAD_DESC_SPARSE_R_DEFAULT		(road_description_t){"Sparse R", 20, 17, Sparse_R}
#define ROAD_CONDITION_DEFAULT			(road_condition_t) ASPHALT

// Req to stay 5 waypoints ahead
extern InputFifo2_t WayPointBuffer;
#define ROAD_GEN_WP_BUFFER_SIZE			10


#define ROADGEN_TASK_PRIO					29u
#define ROADGEN_TASK_STK_SIZE				1024u
CPU_STK  RoadGenTaskStk[ROADGEN_TASK_STK_SIZE];
OS_TCB   RoadGenTaskTCB;

OS_SEM new_waypoint_sem;
OS_TMR road_gen_test_timer;


void create_roadgen_task(void);
void delete_roadgen_task(void);

void roadgen_init(void );

void create_roadgen_sems(void );

void RoadGenTask(void *p_arg);

void create_roadgen_test_timer(void);

void RoadGenTimerCallback(void);











#endif /* __ROAD_GEN_H*/
