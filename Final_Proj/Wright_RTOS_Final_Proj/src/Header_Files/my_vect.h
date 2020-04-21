#ifndef _MY_VECT_H
#define _MY_VECT_H

#include <math.h>
#include <stdint.h>
typedef struct vector {
    float x, y;
} vect_t;

typedef struct int_vector {
    int16_t x, y;
} int_vect_t;

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


int16_t int_vect_mag(int_vect_t vector);
int16_t int_vect_inner_prod(int_vect_t vect_a, vect_t vect_b);

int_vect_t int_vect_parallel(int_vect_t reference_vect, int16_t magnitude);
int_vect_t int_vect_orth(int_vect_t reference_vect,int16_t magnitude, vect_orth_ref_angle_t angle);
int_vect_t int_vect_plus(int_vect_t vect_a, int_vect_t vect_b);
int_vect_t int_vect_mult(int_vect_t vect_a, int16_t scalar);

int_vect_t int_vect_get_unitvector(int16_t degrees);
// gets arrow where 0 degrees is defined "straight up" when looking at the screen with buttons on the bottom
#define int_vect_LCD_get_unitvector(deg)		int_vect_get_unitvector(deg - 90)



#endif /*_MY_VECT_H*/
