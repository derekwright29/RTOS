/*
 * lab7.h
 *
 *  Created on: Feb 26, 2020
 *      Author: goat (Derek Wright)
 *
 *     This header file contains the specific functions relating to Lab 7
 *     of ECEN 3002 - RTOS.
 *
 *     All general-use functions do not belong here, and should be placed in
 *     their own header/source files.
 *
 *     Also, this is a good place to put all necessary includes, to be easily ported around the project
 *
 *     IMPORTANT: To use this as an include hub, you MUST have an ifndef at the top of EVERY HEADER FILE!!!
 *
 *
 */

#ifndef LAB7_H_
#define LAB7_H_


#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "bspconfig.h"
#include "main.h"
#include "fifo.h"
#include "cmu.h"
#include "../cpu_cfg_local.h"
#include "em_cmu.h"
#include "em_acmp.h"
#include "gpio.h"
#include "buttons.h"
#include "mycapsense.h"
#include "led_driver.h"
#include "vehicle.h"
#include "lcd.h"
#include "general_tasks.h"
#include  <kernel/include/os.h>
#include <common/include/rtos_utils.h>
/* Control the mode of operation of main: use interrupts (1) or not (0)? */
#define LAB2_USE_INTERRUPTS		1


/**
 * Globals that multiple files might need
 */







#endif /* LAB7_H_ */
