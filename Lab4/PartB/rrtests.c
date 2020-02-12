#include <stdlib.h>
#include <stdio.h>
#include "ctest.h"
#include "rr.h"

/****************************************************************************** / 
 *          Round Robin TEST SUITE. There are 2 test cases, 6 total tests.
 ****************************************************************************** / */

/****************************************************************************** / 
 *          TEST CASE #1
 ****************************************************************************** / */
CTEST_DATA(roundrobin) {
    struct task_t task[4];
    int size;
};

CTEST_SETUP(roundrobin) {
    int execution[] = {1, 2, 3, 4};
    data->size = sizeof(execution) / sizeof(execution[0]);
    int quantum = 2;

    init(data->task, execution, data->size);
    round_robin(data->task, quantum, data->size);
}

CTEST2(roundrobin, test_process) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}
CTEST2(roundrobin, test_wait) {
    int wait_times[] = {0, 1, 5, 6};
    for (int i = 0; i < data->size; i++) {
        printf("\nWait time for Process %d is: %d\n", data->task[i].process_id, data->task[i].waiting_time);
        ASSERT_EQUAL(wait_times[i], data->task[i].waiting_time);
    }
}

CTEST2(roundrobin, test_turnaround){
    int turn_time[] ={1,3,8,10};
    for (int i = 0; i < data->size; i++) {
        printf("turnaround time is: %d\n",data->task[i].turnaround_time);
        ASSERT_EQUAL(turn_time[i], data->task[i].turnaround_time);
    }
}

/****************************************************************************** / 
 *           TEST CASE #2
 ****************************************************************************** / */

CTEST_DATA(roundrobin2) {
    struct task_t task[5];
    int size;
};

CTEST_SETUP(roundrobin2) {
    int execution[] = {5, 1, 3, 10, 3};
    data->size = sizeof(execution) / sizeof(execution[0]);
    int quantum = 2;

    init(data->task, execution, data->size);
    round_robin(data->task, quantum, data->size);
}

CTEST2(roundrobin2, test_process) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}
CTEST2(roundrobin2, test_wait) {
    int wait_times[] = {11, 2, 9, 12, 12};
    for (int i = 0; i < data->size; i++) {
        printf("\nWait time for Process %d is: %d\n", data->task[i].process_id, data->task[i].waiting_time);
        ASSERT_EQUAL(wait_times[i], data->task[i].waiting_time);
    }
}

CTEST2(roundrobin2, test_turnaround){
    int turn_time[] ={16,3,12,22,15};
    for (int i = 0; i < data->size; i++) {
        printf("turnaround time is: %d\n",data->task[i].turnaround_time);
        ASSERT_EQUAL(turn_time[i], data->task[i].turnaround_time);
    }
}