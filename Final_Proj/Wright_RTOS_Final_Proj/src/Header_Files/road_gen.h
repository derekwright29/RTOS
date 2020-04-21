#ifndef __ROAD_GEN_H
#define __ROAD_GEN_H

#include "final_proj.h"
#include "my_vect.h"

road_description_t course;

/*
 * Course definitions
 * */
#define SPARSE_R_WAYPOINTS 		 { (int_vect_t){15,0},		(int_vect_t){0,0},		(int_vect_t){10,0},		(int_vect_t){20,0},		(int_vect_t){50,10},\
									(int_vect_t){80,5},		(int_vect_t){130,20}, 	(int_vect_t){135,110},	(int_vect_t){135,190},	(int_vect_t){170,225},\
									(int_vect_t){205,210},	(int_vect_t){205,190},	(int_vect_t){150,100},	(int_vect_t){165,50},	(int_vect_t){200,40},\
									(int_vect_t){210,38},	(int_vect_t){230,39}    }

#define DENSE_R_WAYPOINTS		{(int_vect_t) {10,10}}

const int_vect_t Sparse_R[100] = SPARSE_R_WAYPOINTS;
const int_vect_t DenseR[100] = DENSE_R_WAYPOINTS;









#endif /* __ROAD_GEN_H*/
