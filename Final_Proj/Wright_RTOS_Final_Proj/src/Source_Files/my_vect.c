#include <my_vect.h>
#include <math.h>


float vect_mag(vect_t vector) {
    return sqrt(pow((vector.x),2) + pow(vector.y,2));
}

vect_t vect_parallel(float magnitude, vect_t reference_vect) {
    return vect_mult(reference_vect, magnitude / vect_mag(reference_vect));
}

vect_t vect_orth(float magnitude, vect_t reference_vect, vect_orth_ref_angle_t angle) {
    vect_t new_vect = {0,0};
    if (angle == RIGHT_NINETY) {
        new_vect =  (vect_t) {reference_vect.y, -1* reference_vect.x};
    }
    else if (angle == LEFT_NINETY) {
        new_vect = (vect_t) {-1*reference_vect.y, reference_vect.x};
    }
    return vect_mult(new_vect, magnitude / vect_mag(reference_vect));

}
vect_t vect_plus(vect_t vect_a, vect_t vect_b) {
    return (vect_t) {vect_a.x + vect_b.x, vect_a.y + vect_b.y};
}
vect_t vect_mult(vect_t vect_a, float scalar) {
    return (vect_t) {vect_a.x * scalar, vect_a.y * scalar};
}
float vect_inner_prod(vect_t vect_a, vect_t vect_b) {
    return (vect_a.x * vect_b.x) + (vect_a.y * vect_b.y);
}
