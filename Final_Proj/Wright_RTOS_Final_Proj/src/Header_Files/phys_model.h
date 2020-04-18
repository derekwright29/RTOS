#ifndef __PHYS_MODEL_H
#define __PHYS_MODEL_H

#include <math.h>
#include <stdint.h>
#include "my_vect.h"
#include "mycapsense.h"
#include "lcd.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
	#define M_PI_2		1.57079632679489661923
#endif

extern float capsense_turn_value;

/**************************************************************************************/
/* Physics/ Vehicle Description
 * *************************************************************************************/

#define PHYS_MODEL_TIME_STEP       0.1 //seconds

#define G               9.8  //m/s^2

#define DEFAULT_MU       .05


#define PHYSICS_MODEL_STRUCT_DEFAULT  {(vect_t){0,0},(vect_t){0,0},(vect_t){0,0},(vect_t){64,64},\
											(float)-M_PI_2, (float)0.0, (float)0.0,\
											NULL, ASPHALT}		// veh_desc, road_cond

#define VEHICLE_DESC_STRUCT_DEFAULT {"Clunker",\
										1234,\
										93,\
										10,\
										595,\
										170,\
										616,\
										TOURISM\
										}

#define VEHICLE_DESC_STRUCT_CLUNKER {"Clunker",\
										1234,\
										93,\
										10,\
										595,\
										170,\
										616,\
										TOURISM\
										}

#define VEHICLE_DESC_STRUCT_SPORT {"Sport's Car",\
										834,\
										180,\
										20,\
										395,\
										170,\
										216,\
										HIGH_PERF\
										}
#define VEHICLE_DESC_STRUCT_TRUCK {"Truck",\
										2234,\
										250,\
										20,\
										895,\
										200,\
										316,\
										TRUCK\
										}



typedef enum tire_type {
    TOURISM = 1,
	HIGH_PERF = 2,
    TRUCK =3
}tire_t;

// TODO: move to road_gen.h/c
typedef struct RoadLayout		// Purple items are only relevant for specific 3-D enhancements.
{
	char Name[16];			// null terminated
	uint16_t RoadWidth;			// cm
	uint16_t NumWaypoints;		//
	struct {
	int16_t x;			// m
	int16_t y;			// m
	}* Waypoints;				// Array of waypoints.  First and last 2 are not to be driven
}road_description_t;

typedef enum road_conditions {
    ASPHALT = 1,
    EARTH = 2,
    GRAVEL = 3,
    SAND = 4,
    ICE = 5
}road_condition_t;

typedef enum vehicle_warning {
	NO_ERROR = 0,
    SLIP_ERROR = 1,
    ROAD_ERROR = 2
}vehicle_warning_t;


//TODO: move to vehicle.h once you delete all the lab7 stuff.
typedef struct vehicle_info_struct
{
	char VehicleName[16];	// null terminated
	uint16_t Mass;		// kg
	uint16_t MaxPower;		// kW
	uint16_t PowerDelta;	//kW
	uint16_t TurnRadius;		// cm, curb-to-curb
	uint16_t VehicleWidth;	// cm
	
	// Viewing Info, for 3-D POV
//	uint16_t EyeballHeightAboveGround;	// cm
//	uint16_t HorizontalAngleOfView;		// degrees
//	uint16_t ViewInclination;			// degrees (positive=up)

	// Vehicle characteristics for slippage
	uint16_t DragArea;				// Cd*CrossSectionalArea as: Cd * Ft^2 * 100
	tire_t TireType;				// 0:truck, 1:tourism, 2:highperformance
}vehicle_description_t;
//Note: for road-excursion calculations, vehicle is assumed to have a circular footprint


typedef struct physics_params {
    //el, roll, will always be zero for me
    vect_t forw_a,turn_a,v,p;
	float az,el,roll;
    vehicle_description_t * vehicle;
    road_condition_t road;
}phys_model_t;

/************************************
 * End Physics/Vehicle Description
 * **************************************************/

/*******************************************************************************************************************************
 * Vehicle Tasks defines
 ***********************/
/* Vehicle Monitor Task*/
#define PHYSICS_MODEL_TASK_PRIO					25u
#define PHYSICS_MODEL_TASK_STK_SIZE				1024u
CPU_STK  PhysicsModelTaskStk[PHYSICS_MODEL_TASK_STK_SIZE];
OS_TCB   PhysicsModelTaskTCB;

void create_physics_model_task(void);
void PhysicsModelTask(void *p_arg);

/********************************************************************************************************
 * End Task-specific Description
 * *******************************************************************************************************/
/*******************************************************************************************************************************
 * Vehicle Tasks defines
 ***********************/
/* Vehicle Monitor Task*/
#define PHYSICS_MODEL_TASK_PRIO					25u
#define PHYSICS_MODEL_TASK_STK_SIZE				1024u
CPU_STK  PhysicsModelTaskStk[PHYSICS_MODEL_TASK_STK_SIZE];
OS_TCB   PhysicsModelTaskTCB;

void create_physics_model_task(void);
void PhysicsModelTask(void *p_arg);

/********************************************************************************************************
 * End Task-specific Description
 * *******************************************************************************************************/


/********************************************************************************************************
 * Globals needed
 * *******************************************************************************************************/
vehicle_description_t vehicle_desc;
extern phys_model_t vehicle_model;
road_condition_t road_cond;



/********************************************************************************************************
 * Function Prototypes
 * *******************************************************************************************************/

vehicle_warning_t phys_model_take_step(phys_model_t * p_model, int16_t power_applied, float turn);


float get_friction(road_condition_t cond, tire_t tire_typ);
vehicle_warning_t check_slip(float, float);


float decideTurn(bool * state_array);

#endif /* __PHYS_MODEL_H */
