#include <phys_model.h>

phys_model_t vehicle_model = PHYSICS_MODEL_STRUCT_DEFAULT;

//power applied is the value controlled by the buttons
//turn is the value controlled by the capsense. Should be between -1 and 1
vehicle_warning_t phys_model_take_step(phys_model_t * p_model, int16_t power_applied, float turn) {
	RTOS_ERR err;
	bool turning = false;
	float last_az = -M_PI_2;

	int8_t accel = power_applied > 0 ? 1 : (power_applied == 0 ? 0 : -1);


	OSSchedLock(&err);
    vehicle_warning_t warning = 0;
    vect_t ac; //centripetal accel
    vect_t v_inc; // a*dt
    vect_t p_inc; // v*dt
    float radius = (float)(p_model->vehicle->TurnRadius) / fabs(turn);
    float v_mag = vect_mag(p_model->v);
    vect_orth_ref_angle_t turn_dir = turn < 0 ? LEFT_NINETY : RIGHT_NINETY;
    vect_t af;
    if ((v_mag < 0.001) && (power_applied == 0)) {
    	af = (vect_t) {0,0};
    }
    //slow to stop method
    else if ((v_mag < 4) && (power_applied == 0)) {
    	//ramp down
    	af = vect_parallel(p_model->v, v_mag * -0.1);
    }
    else
    	af = vect_parallel(vect_get_unitvector(p_model->az), accel*p_model->vehicle->PowerDelta / p_model->vehicle->Mass);// - DEFAULT_MU* G);
    if (turn == 0) {
    	v_inc = vect_mult(af,PHYS_MODEL_TIME_STEP);
    }

    else {
    	turning = true;
    	vect_t center = (vect_plus(vect_orth(vect_get_unitvector(p_model->az), radius, turn_dir), p_model->p));
    	vect_t r = vect_plus(center, vect_mult(p_model->p, -1)); // c - p gives r
    	ac = vect_parallel(r, pow(v_mag,2) / radius);
    	v_inc = vect_mult(vect_plus(af, ac), PHYS_MODEL_TIME_STEP);
    }

    p_inc = vect_mult(p_model->v, PHYS_MODEL_TIME_STEP);

    p_model->v = vect_plus(p_model->v, v_inc);
    if (vect_mag(p_model->v) < .01){
    	p_model->v = (vect_t) {0.,0.};
    }
    p_model->p = vect_plus(p_model->p, p_inc);

    if (turning && !power_applied){// for friction: && (vect_mag(af) > 0)) {

    	p_model->v = vect_parallel(p_model->v, v_mag); //* FRICTION_TURN_FUDGE_FACTOR
    }

//    float friction = get_friction (p_model->road, p_model->vehicle->TireType);
//    if (vect_mag(p_model->v) == 0. && vect_mag(p_model->forw_a) == 0. && (power_applied != 0)) {
//        //needs a kick
//    	vect_t u_vect = vect_LCD_get_unitvector(p_model->az*M_PI);
//        p_model->v = u_vect;
//    }
//    else {
    	//new accel in direction of motion is (force applied - friction force) / mass
//        new_forw_a = vect_parallel(vect_get_unitvector(p_model->az), power_applied*p_model->vehicle->PowerDelta / p_model->vehicle->Mass); //- friction* G*v_mag);
//    }

//    float slip_value = sqrt(pow(vect_mag(new_forw_a),2) + pow(vect_mag(new_turn_a),2));
//    warning |= check_slip(slip_value, friction);


    //update p_model


//    vect_t v_inc = vect_mult(vect_plus(p_model->forw_a, p_model->turn_a), PHYS_MODEL_TIME_STEP);
    if(vect_mag(p_model->turn_a))
    p_model->v = vect_parallel(vect_plus(p_model->v, v_inc), vect_mag(p_model->v));

    // clip speed if necessary
    if(vect_mag(p_model->v) > p_model->vehicle->MaxSpeed)
    	p_model->v = vect_parallel(p_model->v, p_model->vehicle->MaxSpeed);

    //az is inherently encoded in velocity vector, so just grab it from there.
    if (vect_mag(p_model->v) > 0.001) {
    	p_model->az = determine_az(p_model);
    }
    else if (vect_mag(p_model->v) < 0.001 && v_mag > 0.001) // is velocity was useful, store which direction we were pointing.
    	last_az = p_model->az;
    else
    	p_model->az = last_az;
    //Not doing 3D
    p_model->el = 0.0;
    p_model->roll = 0.0;
    OSSchedUnlock(&err);

    return warning;
}


void create_physics_model_task() {
	RTOS_ERR err;
	//vehicle_desc, road_cond is init'ed in Menu task
	//once we are here we can set the model
	vehicle_model.vehicle = &vehicle_desc;
	vehicle_model.road = road_cond;

	OSMutexCreate(&vehicle_state_mutex, "Vehicle State Mutex", &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
//	create_phys_model_timer();

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

void delete_physics_model_task() {
	RTOS_ERR err;
	OSTaskDel(&PhysicsModelTaskTCB, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

void create_phys_model_timer(void ) {
	// TODO: configure timer task tick to be 10 ms (100Hz) haven't found a way yet.
//	RTOS_ERR err;
//	OSTmrCreate(&phys_model_timer,
//					"Phys Model Timer",
//					0,
//					5,
//					OS_OPT_TMR_PERIODIC,
//					(OS_TMR_CALLBACK_PTR)RoadGenTimerCallback,
//					DEF_NULL,
//					&err);
//	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	return;
}

void PhysicsModelTask(void* p_arg) {
	RTOS_ERR err;

	//capsense stuff
	init_capsense();
	bool cap_state[CSEN_CHANNELS] = {false};

	//phys_mod stuff
	int16_t pow_cnt= 0;

	while(1) {
		pow_cnt = 0;
		// get button info
		pow_cnt += button0_isPressed();
		pow_cnt -= button1_isPressed();
		if ((vect_mag(vehicle_model.v) < 4) && (pow_cnt < 0))
			pow_cnt = 0; //don't allow reverse yet.

		//get capsense info
		sample_capsense();
		//decides direction and also updates cap_state... I know...
		capsense_turn_value = decideTurn(cap_state);

		//update phys model
		//get mustex on p_model
		OSMutexPend(&vehicle_state_mutex,0,OS_OPT_PEND_BLOCKING, 0 , &err);
		phys_model_take_step(&vehicle_model, pow_cnt, capsense_turn_value);
		OSMutexPost(&vehicle_state_mutex, OS_OPT_POST_NONE,&err);
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
		return 1.0;
	else if (((uint8_t)state_array[0]+(uint8_t)state_array[1]) > ((uint8_t)state_array[2]+(uint8_t)state_array[3]))
		return 0.5;
	else if (!state_array[0] && !state_array[1] && !state_array[2] && state_array[3])
		return -1.0;
	else if (((uint8_t)state_array[0]+(uint8_t)state_array[1]) < ((uint8_t)state_array[2]+(uint8_t)state_array[3]))
		return -0.5;
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

float determine_az(phys_model_t *p_model) {
	if (p_model->v.x > 0)
		return vect_get_heading(p_model->v); // quadrants I and II
	else {
		return M_PI + vect_get_heading(p_model->v); // quadrants III and IV
	}
}




