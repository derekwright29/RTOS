/*
 * general_tasks.h
 *
 *  Created on: Mar 1, 2020
 *      Author: goat
 */

#ifndef SRC_HEADER_FILES_GENERAL_TASKS_H_
#define SRC_HEADER_FILES_GENERAL_TASKS_H_

#include  <kernel/include/os.h>
#include "em_emu.h"
#include "lab7.h"

#define IDLE_TASK_PRIO							30u
#define IDLE_TASK_STK_SIZE					1024u
CPU_STK  IdleTaskStk[IDLE_TASK_STK_SIZE];
OS_TCB   IdleTaskTCB;

void create_idle_task(void);

void IDLETask(void * p_arg);

#endif /* SRC_HEADER_FILES_GENERAL_TASKS_H_ */
