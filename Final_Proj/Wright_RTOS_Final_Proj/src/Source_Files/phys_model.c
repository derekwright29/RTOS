#include <phys_model.h>

phys_model_t vehicle_model = PHYSICS_MODEL_STRUCT_DEFAULT;

//power applied is the value controlled by the buttons
//turn is the value controlled by the capsense. Should be between -1 and 1
vehicle_warning_t phys_model_take_step(phys_model_t * p_model, int16_t power_applied, float turn) {
    vehicle_warning_t warning = 0;
    vect_t new_forw_a;
    vect_t new_turn_a;
    float friction = get_friction (p_model->road, p_model->vehicle->TireType);
    if (vect_mag(p_model->v) == 0. && vect_mag(p_model->forw_a) == 0. && (power_applied != 0)) {
        //needs a kick
    	vect_t u_vect = vect_LCD_get_unitvector(p_model->az*M_PI);
        p_model->v = u_vect;
    }
    else {
    	//new accel in direction of motion is (force applied - friction force) / mass
        new_forw_a = vect_parallel(p_model->v, power_applied*p_model->vehicle->PowerDelta / vect_mag(p_model->v) / p_model->vehicle->Mass - friction* G*vect_mag(p_model->v));
    }
    //get orthogonal vector to direction of travel in the inside curve of the turn angle.
    if (turn == 0) {
    	new_turn_a = (vect_t){0.,0.};
    }
    else {
    	new_turn_a = vect_orth(p_model->v, pow(vect_mag(p_model->v),2) / turn, turn < 0 ? LEFT_NINETY : RIGHT_NINETY);
    }

    float slip_value = sqrt(pow(vect_mag(new_forw_a),2) + pow(vect_mag(new_turn_a),2));
    warning |= check_slip(slip_value, friction);


    //update p_model
    vect_t p_inc = vect_mult(p_model->v, PHYS_MODEL_TIME_STEP);
    p_model->p = vect_plus(p_model->p, p_inc);

    vect_t v_inc = vect_mult(vect_plus(p_model->forw_a, p_model->turn_a), PHYS_MODEL_TIME_STEP);
    p_model->v = vect_plus(p_model->v, v_inc);

    p_model->forw_a = new_forw_a;
    p_model->turn_a = new_turn_a;

    p_model->az += turn*M_PI;		//turn is a value [-1, 1]. Putting it in rads by multiplying by PI.
    //Not doing 3D
    p_model->el = 0.0;
    p_model->roll = 0.0;

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
	//dummy variable to store button flags
	InputValue_t ret;

	//capsense stuff
	init_capsense();
	bool cap_state[CSEN_CHANNELS] = {false};

	//phys_mod stuff
	int16_t pow_cnt= 0;
	float turn;

	while(1) {
		pow_cnt = 0;
		// get button info
		while(!InputFifo_isEmpty((InputFifo_t *)&FIFO_Button0)){
			InputFifo_Get((InputFifo_t *)&FIFO_Button0, &ret);
			pow_cnt++;
		}
		while(!InputFifo_isEmpty((InputFifo_t *)&FIFO_Button1)) {
			InputFifo_Get((InputFifo_t *)&FIFO_Button1, &ret);
			pow_cnt--;
		}
			//get mutex
			//release mutex
		//get capsense info
		sample_capsense();
		//decides direction and also updates cap_state... I know...
		turn = decideTurn(cap_state);

		//update phys model
		//get mustex on p_model
		phys_model_take_step(&vehicle_model, pow_cnt, turn);
		//send Flag to LCD AND Monitor Task to update ------> if Monitor detects error or  we got to the waypoint, send flag to Menu/LCD for game over and roadgen to get new waypoint and add to 5-deep ready queue.
		OSSemPost(&phys_model_update_sem, OS_OPT_POST_ALL, &err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

		//delay
		OSTimeDly((OS_TICK)(PHYS_MODEL_TIME_STEP*1000.), OS_OPT_TIME_PERIODIC, &err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	}

}





float decideTurn(bool * state_array) {
	uint8_t i;
	for(i = 0; i < CSEN_CHANNELS; i++) {
		state_array[i] = capsenseIsPressed[i];
	}
	if (state_array[0] && !state_array[1] && !state_array[2] && !state_array[3])
		return -1.0;
	else if (((uint8_t)state_array[0]+(uint8_t)state_array[1]) > ((uint8_t)state_array[2]+(uint8_t)state_array[3]))
		return -0.5;
	else if (!state_array[0] && !state_array[1] && !state_array[2] && state_array[3])
		return 1.0;
	else if (((uint8_t)state_array[0]+(uint8_t)state_array[1]) < ((uint8_t)state_array[2]+(uint8_t)state_array[3]))
		return 0.5;
	else
		return 0.0;
}






float get_friction(road_condition_t cond, tire_t tire_type) {
    float friction = DEFAULT_MU * cond * tire_type;
    return friction;
    
}

vehicle_warning_t check_slip(float slip_f, float friction_f) {
	return ((slip_f > friction_f) ? SLIP_ERROR: NO_ERROR);
}



