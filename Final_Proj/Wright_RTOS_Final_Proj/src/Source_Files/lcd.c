/*
 * lcd.c
 *
 *  Created on: Mar 2, 2020
 *      Author: goat
 */

#include "lcd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "dmd.h"
#include <common/include/rtos_utils.h>
#include "road_gen.h"
#include "fifo.h"

static EMSTATUS GLIB_drawCar(GLIB_Context_t *pContext, int16_t veh_x, int16_t veh_y, double degrees, bool clear_flag) {
	EMSTATUS status = GLIB_OK;
	if(clear_flag)
		GLIB_clear(pContext);

	int x_rotation = round(veh_x + ARROW_LEN*sin((degrees)*DEG2RAD));
	int y_rotation = round(veh_y - ARROW_LEN*cos((degrees)*DEG2RAD));
	int lx_rotation = round(veh_x +TIP_RADIUS*sin((degrees*DEG2RAD) - TIP_RADIAN_OFFSET));
	int ly_rotation = round(veh_y -TIP_RADIUS*cos((degrees*DEG2RAD) - TIP_RADIAN_OFFSET));
	int rx_rotation = round(veh_x +TIP_RADIUS*sin((degrees*DEG2RAD) + TIP_RADIAN_OFFSET));
	int ry_rotation = round(veh_y -TIP_RADIUS*cos((degrees*DEG2RAD) + TIP_RADIAN_OFFSET));


	GLIB_drawCircle(pContext, veh_x, veh_y, CAR_RADIUS);

	GLIB_drawLine(pContext, veh_x, veh_y, x_rotation, y_rotation);

	GLIB_drawLine(pContext, lx_rotation, ly_rotation, x_rotation, y_rotation);
	GLIB_drawLine(pContext, rx_rotation, ry_rotation, x_rotation, y_rotation);
	DMD_updateDisplay();

	return status;
}


void create_lcd_task() {
	RTOS_ERR err;
	/* Create the LCD Task.                               */
	OSTaskCreate(&LcdTaskTCB,
				 "LCD Task",
				  LCDTask,
				  DEF_NULL,
				  LCD_TASK_PRIO,
				 &LcdTaskStk[0],
				 (LCD_TASK_STK_SIZE / 10u),
				 LCD_TASK_STK_SIZE,
				  0u,
				  0u,
				  DEF_NULL,
				 (OS_OPT_TASK_STK_CLR),
				 &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

}

void delete_lcd_task() {
	RTOS_ERR err;

	OSTaskDel(&LcdTaskTCB, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);


}


void LCDTask(void *p_arg) {
	RTOS_ERR err;
	char dir_substr[DIR_SUBSTR_MAX_LENGTH], dir_substr1[DIR_SUBSTR_MAX_LENGTH];
	char spd_substr[SPD_SUBSTR_MAX_LENGTH], spd_substr1[SPD_SUBSTR_MAX_LENGTH];
	int16_t veh_x, veh_y;
	int16_t disp_vehx;
	int16_t disp_vehy;
	float veh_spd;
	float arrow_angle, last_angle=0;
	float lcd_angle_offset = 90;
	lcd_init();
	while(1) {
		OSSemPend(&phys_model_update_sem,
					0,
					OS_OPT_PEND_BLOCKING,
					(CPU_TS*)0,
					&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		// get vehicle position
		veh_x = (int16_t)round(vehicle_model.p.x); //positive modulus
		veh_y = (int16_t)round(vehicle_model.p.y);


		if (vehicle_model.p.x < 0) {
			OSMutexPend(&vehicle_state_mutex,0,OS_OPT_PEND_BLOCKING, 0 , &err);
			while (vehicle_model.p.x < 0)
				vehicle_model.p.x += MAX_X; // serves as modulus for negative value
			OSMutexPost(&vehicle_state_mutex, OS_OPT_POST_NONE,&err);
		}
		else if (vehicle_model.p.x > MAX_X) {
			OSMutexPend(&vehicle_state_mutex,0,OS_OPT_PEND_BLOCKING, 0 , &err);
			vehicle_model.p.x = (float)(((int)round(vehicle_model.p.x)) % (MAX_X+1));
			OSMutexPost(&vehicle_state_mutex, OS_OPT_POST_NONE,&err);
		}
		if (vehicle_model.p.y < 0) {
			OSMutexPend(&vehicle_state_mutex,0,OS_OPT_PEND_BLOCKING, 0 , &err);
			while (vehicle_model.p.y < 0)
				vehicle_model.p.y += MAX_Y;
			OSMutexPost(&vehicle_state_mutex, OS_OPT_POST_NONE,&err);
		}
		else if (vehicle_model.p.y > MAX_X) {
			OSMutexPend(&vehicle_state_mutex,0,OS_OPT_PEND_BLOCKING, 0 , &err);
			vehicle_model.p.y = (float)(((int)round(vehicle_model.p.y)) % (MAX_Y+1));
			OSMutexPost(&vehicle_state_mutex, OS_OPT_POST_NONE,&err);
		}

		arrow_angle = (vehicle_model.az * RAD2DEG) + lcd_angle_offset;
		if(fabs(arrow_angle - last_angle) > M_PI_2) {
			arrow_angle = arrow_angle;
		}
		last_angle = arrow_angle;

		//remap model coordinates to LCD coordinates.
		disp_vehy = veh_y;
		disp_vehx = (veh_x);


		veh_spd = 0.01*(((int16_t)vect_mag(vehicle_model.v))*100);

		OSSchedLock(&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		strcpy(spd_substr1, "Speed: ");
		gcvt(veh_spd, 5, spd_substr);
		strcpy(dir_substr1, "Turn: ");
		gcvt(capsense_turn_value, 3, dir_substr);

		strcat(spd_substr1, spd_substr);
		strcat(dir_substr1, dir_substr);

		GLIB_clear(&glibContext);
		LCD_draw_gates();
		 GLIB_drawString(&glibContext, spd_substr1, 20, 5, 5, 0);
//		 GLIB_drawString(&glibContext, dir_substr1, 20, 5, 25, 0);
		strcpy(spd_substr1, "Radius: ");
		veh_spd = capsense_turn_value ? 1/capsense_turn_value : 0;
		gcvt(veh_spd, 3, spd_substr);
		strcat(spd_substr1, spd_substr);
//		 GLIB_drawString(&glibContext, spd_substr1, 20, 5, 105, 0);
		GLIB_drawCar(&glibContext, disp_vehx, disp_vehy, -10*capsense_turn_value + arrow_angle, 0);
//			printf("The current speed is %d mph   \nThe current direction is %s\033H", vehicle_speed.speed, dir_substr);

		OSSchedUnlock(&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
		/* Updated display */

	} // end while
} // end LCD task

void lcd_init() {
	EMSTATUS status;
	/* Initialize the display module. */
	DISPLAY_Init();

	/* Initialize the display module. */
	  status = DISPLAY_Init();
	  if (DISPLAY_EMSTATUS_OK != status) {
		while (1) ;
	  }

	  /* Initialize the DMD module for the DISPLAY device driver. */
	  status = DMD_init(0);
	  if (DMD_OK != status) {
		while (1) ;
	  }

	  status = GLIB_contextInit(&glibContext);
	  if (GLIB_OK != status) {
		while (1) ;
	  }
	  glibContext.backgroundColor = White;
	  glibContext.foregroundColor = Black;
	  GLIB_setFont(&glibContext, (GLIB_Font_t *)&GLIB_FontNarrow6x8);
	  GLIB_clear(&glibContext);
	  DMD_updateDisplay();

	/* Retarget stdio to a text display. */
//	if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK) {
//	while (1) ;
//	}

}


void LCD_draw_gates() {
	if (waypoint_index == 0)
		//road_gen hasn't started up yet
		return;
	int_vect_t curr_wp;
	bool end_sequence = false;
	uint8_t wp_cnt = 0;
	float gate_heading;
	uint8_t num_to_print = InputFifo2_getNumItems(&WayPointBuffer);
	if (waypoint_index >= (course.NumWaypoints - LCD_NUM_GATES_TO_DRAW + ROAD_GEN_WP_BUFFER_SIZE)) {
		end_sequence = true;
	}
	if (num_to_print >= LCD_NUM_GATES_TO_DRAW){
		num_to_print = LCD_NUM_GATES_TO_DRAW;
	}
	// this is subtle. When the road generation is done, we just want to take the remaining gates in the queue
	while (     (wp_cnt < num_to_print) && (InputFifo2_Peek(&WayPointBuffer, wp_cnt, (InputValue2_t *)&curr_wp))    ) {
		if (!end_sequence) {
			gate_heading = M_PI_2 + course_headings[waypoint_index - (ROAD_GEN_WP_BUFFER_SIZE) + wp_cnt];
		}
		else {
			gate_heading = M_PI_2 + course_headings[course.NumWaypoints - num_to_print + wp_cnt];
		}
		// delta waypoint to be added to center waypoint to form gate
		vect_t d_wp = vect_orth(vect_get_unitvector(gate_heading), ((float)course.RoadWidth) / 2., LEFT_NINETY);
		int_vect_t dwp = (int_vect_t) {round(d_wp.x), round(d_wp.y)};
		GLIB_drawLine(&glibContext, curr_wp.x + dwp.x, curr_wp.y + dwp.y,
				curr_wp.x - dwp.x, curr_wp.y - dwp.y);
		GLIB_drawCircleFilled(&glibContext, curr_wp.x, curr_wp.y, 2);
		wp_cnt++;
	}


}
