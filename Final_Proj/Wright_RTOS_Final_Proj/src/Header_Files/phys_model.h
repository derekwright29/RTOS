#include <math.h>
#include <stdint.h>

#include "Header_Files/my_vect.h"
#define TIME_STEP       0.1 //seconds

#define G               9.8  //m/s^2

#define DEFAULT_MU       .1

#define PHYSICS_MODEL_STRUCT_DEFAULT  {0,0,0,0,0,0}

typedef enum tire_type {
    TOURISM = 1,
    TRUCK = 2,
    HIGH_PERF=3
}tire_t;


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
    ASPHALT = 5,
    EARTH = 4,
    GRAVEL = 3,
    SAND = 2,
    ICE = 1
}road_condition_t;

typedef enum vehicle_warning {
    SLIP_ERROR,
    ROAD_ERROR
}vehicle_warning_t;

typedef struct vehicle_info_struct
{
	char VehicleName[16];	// null terminated
	uint16_t Mass;		// kg
	uint16_t MaxPower;		// kW
	uint16_t TurnRadius;		// cm, curb-to-curb
	uint16_t VehicleWidth;	// cm
	
	// Viewing Info, for 3-D POV
	uint16_t EyeballHeightAboveGround;	// cm
	uint16_t HorizontalAngleOfView;		// degrees
	uint16_t ViewInclination;			// degrees (positive=up)

	// Vehicle characteristics for slippage
	uint16_t DragArea;				// Cd*CrossSectionalArea as: Cd * Ft^2 * 100
	tire_t TireType;				// 0:truck, 1:tourism, 2:highperformance
}vehicle_description_t;
//Note: for road-excursion calculations, vehicle is assumed to have a circular footprint


typedef struct physics_params {
    //el, roll, will always be zero for me
    vect_t forw_a,turn_a,v,p,az,el,roll;
    vehicle_description_t * vehicle;
}phys_model_t;






vehicle_warning_t phys_model_take_step(phys_model_t * p_model, vect_t power_applied, float turn, road_condition_t road_cond);


float get_friction(road_condition_t cond, tire_t tire_typ);
vehicle_warning_t check_slip(float);
