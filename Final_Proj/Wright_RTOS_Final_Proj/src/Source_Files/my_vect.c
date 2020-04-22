#include <my_vect.h>
#include <math.h>



float vect_mag(vect_t vector) {
	if (vector.x == 0.0 && vector.y == 0.0) {
		return 0.0;
	}
    return sqrt(pow((vector.x),2) + pow(vector.y,2));
}

vect_t vect_parallel(vect_t reference_vect, float magnitude) {
    if (vect_mag(reference_vect) ==0.0) {
        return (vect_t){0,0};
    }
    return vect_mult(reference_vect, magnitude / vect_mag(reference_vect));
}

vect_t vect_orth(vect_t reference_vect, float magnitude, vect_orth_ref_angle_t angle) {
    vect_t new_vect = {0,0};
    if (vect_mag(reference_vect) == 0.0) {
        return new_vect;
    }
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

vect_t vect_get_unitvector(float rads) {
	return (vect_t) {cos(rads),sin(rads)};
}


float vect_get_heading(vect_t vector) {
	return atan(vector.y / vector.x);
}


int16_t int_vect_mag(int_vect_t vector){
	if (vector.x == 0 && vector.y == 0) {
		return 0;
	}
	return (int16_t) sqrt(pow((vector.x),2) + pow(vector.y,2));
}
int16_t int_vect_inner_prod(int_vect_t vect_a, vect_t vect_b) {
	return (int16_t) ((vect_a.x * vect_b.x) + (vect_a.y * vect_b.y));
}

int_vect_t int_vect_parallel(int_vect_t reference_vect, int16_t magnitude) {
	if (int_vect_mag(reference_vect) ==0) {
		return (int_vect_t){0,0};
	}
	return int_vect_mult(reference_vect, magnitude / int_vect_mag(reference_vect));
}
int_vect_t int_vect_orth(int_vect_t reference_vect,int16_t magnitude, vect_orth_ref_angle_t angle){
    int_vect_t new_vect = {0,0};
    if (int_vect_mag(reference_vect) == 0) {
        return new_vect;
    }
    if (angle == RIGHT_NINETY) {
        new_vect =  (int_vect_t) {reference_vect.y, -1* reference_vect.x};
    }
    else if (angle == LEFT_NINETY) {
        new_vect = (int_vect_t) {-1*reference_vect.y, reference_vect.x};
    }
    return int_vect_mult(new_vect, magnitude / int_vect_mag(reference_vect));
}
int_vect_t int_vect_plus(int_vect_t vect_a, int_vect_t vect_b) {
	return (int_vect_t) {vect_a.x + vect_b.x, vect_a.y + vect_b.y};
}
int_vect_t int_vect_mult(int_vect_t vect_a, int16_t scalar) {
	return (int_vect_t) {vect_a.x * scalar, vect_a.y * scalar};
}



// not a useful function for int16_t's i don't think...
//
//int_vect_t int_vect_get_unitvector(int16_t degrees) {
//	float rads = (float) M_PI * degrees / 180 ;
//	return (int_vect_t) {cos(rads),sin(rads)};
//
//}
// gets arrow where 0 degrees is defined "straight up" when looking at the screen with buttons on the bottom
//#define int_vect_LCD_get_unitvector(deg)		int_vect_get_unitvector(deg - 90)
//













