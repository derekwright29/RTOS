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

#define IDLE_TASK_PRIO							62u
#define IDLE_TASK_STK_SIZE					1024u
CPU_STK  IdleTaskStk[IDLE_TASK_STK_SIZE];
OS_TCB   IdleTaskTCB;

void create_idle_task(void);

#endif /* SRC_HEADER_FILES_GENERAL_TASKS_H_ */
