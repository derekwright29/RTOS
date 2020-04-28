#ifndef __PHYS_MODEL_H
#define __PHYS_MODEL_H

#include <math.h>
#include <stdint.h>
#include "my_vect.h"
#include "mycapsense.h"
#include "lcd.h"
#include "road_gen.h"
#include "final_proj.h"

#ifndef M_PI
    #define M_PI 		3.14159265358979323846
	#define M_PI_2		1.57079632679489661923
	#define M_PI_4    	0.78539816339744830962
#endif

extern float capsense_turn_value;


/**************************************************************************************/
/* Typedefs
 * *************************************************************************************/


typedef enum tire_type {
    TOURISM = 1,
	HIGH_PERF = 2,
    TRUCK =3
}tire_t;

/**************************************************************************************/
/* Physics/ Vehicle Description
 * *************************************************************************************/

#define PHYS_MODEL_TIME_STEP       		0.05 //seconds
#define PHYS_MODEL_PERIOD_TICKS			50

#define G               				9.8  //m/s^2

#define DEFAULT_MU       				3
#define FRICTION_TURN_FUDGE_FACTOR		0.9


#define PHYSICS_MODEL_STRUCT_DEFAULT  {(vect_t){0,0},(vect_t){0,0},(vect_t){0,0},(vect_t){64,64},\
											(float)-M_PI_2, (float)0.0, (float)0.0,\
											NULL, ASPHALT}		// veh_desc, road_cond

#define VEHICLE_DESC_STRUCT_DEFAULT {"Clunker",\
										10,\
										100,\
										800,\
										10,\
										170,\
										80,\
										TOURISM\
										}

#define VEHICLE_DESC_STRUCT_CLUNKER {"Clunker",\
										10,\
										100,\
										800,\
										15,\
										170,\
										80,\
										TOURISM\
										}

#define VEHICLE_DESC_STRUCT_SPORT {"Sport's Car",\
										8,\
										180,\
										1200,\
										9,\
										170,\
										160,\
										HIGH_PERF\
										}
#define VEHICLE_DESC_STRUCT_TRUCK {"Truck",\
										30,\
										120,\
										1500,\
										24,\
										200,\
										100,\
										TRUCK\
										}


//TODO: move to vehicle.h once you delete all the lab7 stuff.
typedef struct vehicle_info_struct
{
	char VehicleName[16];	// null terminated
	uint16_t Mass;		// kg
	uint16_t MaxSpeed;		// m/s
	uint16_t PowerDelta;	//kJ/m
	uint16_t TurnRadius;		// m, curb-to-curb
	uint16_t VehicleWidth;	// cm

	// Vehicle characteristics for slippage
	uint16_t slipSpeed;
	tire_t TireType;				// 0:truck, 1:tourism, 2:highperformance
}vehicle_description_t;
//Note: for road-excursion calculations, vehicle is assumed to have a circular footprint





typedef enum vehicle_warning {
	NO_ERROR = 0,
    SLIP_ERROR = 1,
    ROAD_ERROR = 2
}vehicle_warning_t;





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

OS_TMR phys_model_timer;
OS_MUTEX vehicle_state_mutex;

void create_physics_model_task(void);
void delete_physics_model_task(void);
void create_phys_model_timer(void );
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
extern road_condition_t road_cond;
extern road_description_t course;

extern vehicle_description_t clunker;
extern vehicle_description_t sportscar;
extern vehicle_description_t truck;




/********************************************************************************************************
 * Function Prototypes
 * *******************************************************************************************************/

vehicle_warning_t phys_model_take_step(phys_model_t * p_model, int16_t power_applied, float turn);


float get_friction(road_condition_t cond, tire_t tire_typ);
vehicle_warning_t check_slip(float, float);


float decideTurn(bool * state_array);

float determine_az(phys_model_t *p_model);

void get_road_params(float * accel_mult, float *turn_mult, float* fric_coeff);

#endif /* __PHYS_MODEL_H */
