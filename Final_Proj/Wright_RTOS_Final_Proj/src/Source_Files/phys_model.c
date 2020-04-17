#include <phys_model.h>

phys_model_t vehicle_model = PHYSICS_MODEL_STRUCT_DEFAULT;

//power applied is the value controlled by the buttons
//turn is the value controlled by the capsense. Should be between -1 and 1
vehicle_warning_t phys_model_take_step(phys_model_t * p_model, vect_t power_applied, float turn) {
    vehicle_warning_t warning = 0;
    vect_t new_forw_a;
    float friction = get_friction (p_model->road, p_model->vehicle->TireType);
    if (vect_mag(p_model->v) == 0. && vect_mag(p_model->forw_a) == 0.) {
        //needs a kick
        new_forw_a = vect_parallel(p_model->v, 1.);
    }
    else {
    	//new accel in direction of motion is (force applied - friction force) / mass
        new_forw_a = vect_parallel(p_model->v, vect_mag(power_applied) / vect_mag(p_model->v) / p_model->vehicle->Mass - friction* G);
    }
    //get orthogonal vector to direction of travel in the inside curve of the turn angle.
    vect_t new_turn_a = vect_orth(p_model->v, pow(vect_mag(p_model->v),2) / turn, turn < 0 ? LEFT_NINETY : RIGHT_NINETY);

    float slip_value = sqrt(pow(vect_mag(new_forw_a),2) + pow(vect_mag(new_turn_a),2));
    warning |= check_slip(slip_value, friction);


    //update p_model

    p_model->forw_a = new_forw_a;
    p_model->turn_a = new_turn_a;
    vect_t v_inc = vect_mult(vect_plus(new_turn_a, new_forw_a), PHYS_MODEL_TIME_STEP);
    vect_t p_inc = vect_mult(p_model->v, PHYS_MODEL_TIME_STEP);

    p_model->v = vect_plus(p_model->v, v_inc);
    p_model->p = vect_plus(p_model->p, p_inc);

    p_model->az = vect_parallel(p_model->v, turn*M_PI);		//turn is a value [-1, 1]. Putting it in rads by multiplying by PI.
    //Not doing 3D
    p_model->el = (vect_t){0,0};
    p_model->roll = (vect_t){0,0};

    return warning;
}


void create_physics_model_task() {
	RTOS_ERR err;
	//vehicle_desc, road_cond is init'ed in Menu task
	//once we are here we can set the model
	vehicle_model.vehicle = &vehicle_desc;
	vehicle_model.road = road_cond;

	OSTaskCreate(&PhysicsModelTaskTCB,                          /* Create the Speed Task.                               */
				 "Physics Model Task",
				  PhysicsModelTask,
				  DEF_NULL,
				  PHYSICS_MODEL_TASK_PRIO,
				 &PhysicsModelTaskStk[0],
				 (PHYSICS_MODEL_TASK_STK_SIZE / 10u),
				 PHYSICS_MODEL_TASK_STK_SIZE,
				  0u,
				  0u,
				  DEF_NULL,
				 (OS_OPT_TASK_STK_CLR),
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void PhysicsModelTask(void* p_arg) {
	RTOS_ERR err;
	vect_t app_power;
	float turn;
	while(1) {
		// get button info
			//get mutex
			app_power = vect_parallel(vehicle_model.v, 1);
			//release mutex
		//get capsense info
			//get mutex
			turn = 0;
			//release mutex
		//update phys model
		//get mustex on p_model
		phys_model_take_step(&vehicle_model, app_power, turn);
		//send Flag to LCD to update ------> if LCD detects we got to the waypoint, send flag to roadgen to get new waypoint and add to 5-deep ready queue.


		//delay
		OSTimeDly((OS_TICK)PHYS_MODEL_TIME_STEP*1000, OS_OPT_TIME_PERIODIC, &err);
	}

}








float get_friction(road_condition_t cond, tire_t tire_type) {
    float friction = DEFAULT_MU * cond * tire_type;
    return friction;
    
}

vehicle_warning_t check_slip(float slip_f, float friction_f) {
	return ((slip_f > friction_f) ? SLIP_ERROR: NO_ERROR);
}
