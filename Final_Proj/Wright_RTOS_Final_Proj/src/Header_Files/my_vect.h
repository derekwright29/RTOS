#ifndef _MY_VECT_H
#define _MY_VECT_H

#include <math.h>
typedef struct vector {
    float x, y;
} vect_t;

typedef enum orthogonal_directions {
    RIGHT_NINETY,
    LEFT_NINETY
} vect_orth_ref_angle_t;


float vect_mag(vect_t vector);
float vect_inner_prod(vect_t vect_a, vect_t vect_b);

vect_t vect_parallel(vect_t reference_vect, float magnitude);
vect_t vect_orth(vect_t reference_vect,float magnitude, vect_orth_ref_angle_t angle);
vect_t vect_plus(vect_t vect_a, vect_t vect_b);
vect_t vect_mult(vect_t vect_a, float scalar);

vect_t vect_get_unitvector(float rads);
// gets arrow where 0 degrees is defined "straight up" when looking at the screen with buttons on the bottom
#define vect_LCD_get_unitvector(rad)		vect_get_unitvector(rad - M_PI_2)
#endif /*_MY_VECT_H*/
