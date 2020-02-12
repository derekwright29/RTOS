#include <stdlib.h>
#include <stdio.h>
#include "ctest.h"
#include "fcfs.h"

/****************************************************************************** / 
 *          FCFS TEST SUITE. There are 3 test cases, 9 total tests.
 ****************************************************************************** / */


/****************************************************************************** / 
 *          TEST CASE #1
 ****************************************************************************** / */
// Note: the name in the first parameter slot must match all tests in that group
CTEST_DATA(firstcomefirstserved) {
    struct task_t task[9];
    int size;
};

CTEST_SETUP(firstcomefirstserved) {
    int execution[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, data->size);
    first_come_first_served(data->task, data->size);
}

CTEST2(firstcomefirstserved, test_process) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, data->task[i].process_id);
        printf("PID is: %d\n",data->task[i].process_id);
    }
}

CTEST2(firstcomefirstserved, test_waitingtime) {
    for (int i = 0; i < data->size; i++) {
        printf("waiting time is: %d\n",data->task[i].waiting_time);
        ASSERT_EQUAL(i, data->task[i].waiting_time);
    }
}

CTEST2(firstcomefirstserved, test_turnaround){
    for (int i = 0; i < data->size; i++) {
        printf("turnaround time is: %d\n",data->task[i].waiting_time);
        ASSERT_EQUAL(i+1, data->task[i].turnaround_time);
    }
}

/****************************************************************************** / 
 *          TEST CASE #2
 ****************************************************************************** / */
// Note: the name in the first parameter slot must match all tests in that group
CTEST_DATA(firstcomefirstserved2) {
    struct task_t task[5];
    int size;
};

CTEST_SETUP(firstcomefirstserved2) {
    int execution[] = {1, 2, 3, 4, 5};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, data->size);
    first_come_first_served(data->task, data->size);
}

CTEST2(firstcomefirstserved2, test_process) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, data->task[i].process_id);
        printf("PID is: %d\n",data->task[i].process_id);
    }
}

CTEST2(firstcomefirstserved2, test_waitingtime) {
    int wait_times[] = {0,1,3,6, 10};
    for (int i = 0; i < data->size; i++) {
        printf("waiting time is: %d\n",data->task[i].waiting_time);
        ASSERT_EQUAL(wait_times[i], data->task[i].waiting_time);
    }
}

CTEST2(firstcomefirstserved2, test_turnaround){
    int turn_time[] ={1,3,6,10,15};
    for (int i = 0; i < data->size; i++) {
        printf("turnaround time is: %d\n",data->task[i].waiting_time);
        ASSERT_EQUAL(turn_time[i], data->task[i].turnaround_time);
    }
}

/****************************************************************************** / 
 *          TEST CASE #3
 ****************************************************************************** / */
// Note: the name in the first parameter slot must match all tests in that group
CTEST_DATA(firstcomefirstserved3) {
    struct task_t task[5];
    int size;
};

CTEST_SETUP(firstcomefirstserved3) {
    int execution[] = {5,1,3,10,3};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, data->size);
    first_come_first_served(data->task, data->size);
}

CTEST2(firstcomefirstserved3, test_process) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, data->task[i].process_id);
        printf("PID is: %d\n",data->task[i].process_id);
    }
}

CTEST2(firstcomefirstserved3, test_waitingtime) {
    int wait_times[] = {0,5,6,9,19};
    for (int i = 0; i < data->size; i++) {
        printf("waiting time is: %d\n",data->task[i].waiting_time);
        ASSERT_EQUAL(wait_times[i], data->task[i].waiting_time);
    }
}

CTEST2(firstcomefirstserved3, test_turnaround){
    int turn_time[] ={5,6,9,19,22};
    for (int i = 0; i < data->size; i++) {
        printf("turnaround time is: %d\n",data->task[i].waiting_time);
        ASSERT_EQUAL(turn_time[i], data->task[i].turnaround_time);
    }
}