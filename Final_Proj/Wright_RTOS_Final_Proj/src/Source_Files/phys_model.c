#include <phys_model.h>
#include <math.h>

//power applied is the value controlled by the buttons
//turn is the value controlled by the capsense. Should be between -1 and 1
vehicle_warning_t phys_model_take_step(phys_model_t * p_model, vect_t power_applied, float turn, road_condition_t road_cond) {
    vehicle_warning_t warning = 0;
    vect_t new_forw_a;
    float friction = get_friction (road_cond, p_model->vehicle->TireType);
    if (vect_mag(p_model->v) == 0.) {
        //needs a kick
        new_forw_a = vect_parallel(1, p_model->v);
    }
    else {
        new_forw_a = vect_parallel(vect_mag(power_applied) / vect_mag(p_model->v) / p_model->vehicle->Mass - friction* G * (p_model->vehicle->Mass), p_model->v); //new accel in direction of motion is force applied - friction*
    }
    //get orthogonal vector to direction of travel in the inside curve of the turn angle.
    vect_t new_turn_a = vect_orth(pow(vect_mag(p_model->v),2) / turn, p_model->v, turn < 0 ? LEFT_NINETY : RIGHT_NINETY);

    float slip_value = sqrt(pow(vect_mag(new_forw_a),2) + pow(vect_mag(new_turn_a),2));
    warning |= check_slip(slip_value);


    //update p_model

    p_model->forw_a = new_forw_a;
    p_model->turn_a = new_turn_a;
    vect_t v_inc = vect_mult(vect_plus(new_turn_a, new_forw_a), TIME_STEP);

    p_model->v = vect_plus(p_model->v, v_inc);

    return warning;
}


float get_friction(road_condition_t cond, tire_t tire_type) {
    float friction = DEFAULT_MU * cond * tire_type;
    return friction;
    
}
