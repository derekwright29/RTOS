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

/**
 * This tests the tasks initialize properly, by checking PID and execution times
 * */
CTEST2(roundrobin, test_process) {
    int execution[] = {1, 2, 3, 4};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time);
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


/****************************************************************************** / 
 *          TEST CASE #3
 ****************************************************************************** / */

 CTEST_DATA(roundrobin3) {
    struct task_t task[5];
    int size;
};

CTEST_SETUP(roundrobin3) {
    int execution[] = {-10, 1, 3, 0, 4};
    data->size = sizeof(execution) / sizeof(execution[0]);
    int quantum = 2;

    init(data->task, execution, data->size);
    round_robin(data->task, quantum, data->size);
}

/**
 * This tests that negative values are initialized correctly
 *  Because there is no negative time, I set it to 0.
 * */
CTEST2(roundrobin3, test_process) {
    int execution[] = {0, 1, 3, 0, 4};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time);
    }
}
/**
 * This tests that negative/zero values are handled correctly
 *  Wait time will be the same as turn time for these invalid tasks.
 * */
CTEST2(roundrobin3, test_wait) {
    int wait_times[] = {0, 0, 3, 3, 4};
    for (int i = 0; i < data->size; i++) {
        printf("\nWait time for Process %d is: %d\n", data->task[i].process_id, data->task[i].waiting_time);
        ASSERT_EQUAL(wait_times[i], data->task[i].waiting_time);
    }
}

/**
 * This tests that negative values are handled correctly
 *  Turn time is the same as wait time for the invalid tasks
 * */
CTEST2(roundrobin3, test_turnaround){
    int turn_time[] ={0,1,6,3,8};
    for (int i = 0; i < data->size; i++) {
        printf("turnaround time is: %d\n",data->task[i].turnaround_time);
        ASSERT_EQUAL(turn_time[i], data->task[i].turnaround_time);
    }
}

/****************************************************************************** / 
 *          TEST CASE #4
 ****************************************************************************** / */

 CTEST_DATA(roundrobin4) {
    struct task_t task[5];
    int size;
};

CTEST_SETUP(roundrobin4) {
    int execution[] = {-1, 11, 3, 20, 4};
    data->size = sizeof(execution) / sizeof(execution[0]);
    int quantum = 5;

    init(data->task, execution, data->size);
    round_robin(data->task, quantum, data->size);
}

/**
 * This tests that quantum = 5 is handled
 * */
CTEST2(roundrobin4, test_process) {
    int execution[] = {0, 11, 3, 20, 4};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time);
    }
}
/**
 * This tests that quantum = 5 is handled
 * */
CTEST2(roundrobin4, test_wait) {
    int wait_times[] = {0, 17, 5, 18, 13};
    for (int i = 0; i < data->size; i++) {
        printf("\nWait time for Process %d is: %d\n", data->task[i].process_id, data->task[i].waiting_time);
        ASSERT_EQUAL(wait_times[i], data->task[i].waiting_time);
    }
}

/**
 * This tests that quantum = 5 is handled
 * */
CTEST2(roundrobin4, test_turnaround){
    int turn_time[] ={0, 28, 8, 38, 17};
    for (int i = 0; i < data->size; i++) {
        printf("turnaround time is: %d\n",data->task[i].turnaround_time);
        ASSERT_EQUAL(turn_time[i], data->task[i].turnaround_time);
    }
}

/****************************************************************************** / 
 *          TEST CASE #5
 ****************************************************************************** / */

 CTEST_DATA(roundrobin5) {
    struct task_t task[5];
    int size;
};

CTEST_SETUP(roundrobin5) {
    int execution[] = {-1, 2, 3, 1, 4};
    data->size = sizeof(execution) / sizeof(execution[0]);
    int quantum = 1;

    init(data->task, execution, data->size);
    round_robin(data->task, quantum, data->size);
}

/**
 * This tests that quantum = 1 is handled
 * */
CTEST2(roundrobin5, test_process) {
    int execution[] = {0, 2, 3, 1, 4};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time);
    }
}
/**
 * This tests that quantum = 1 is handled
 * */
CTEST2(roundrobin5, test_wait) {
    int wait_times[] = {0, 3, 5, 2, 6};
    for (int i = 0; i < data->size; i++) {
        printf("Wait time for Process %d is: %d\n", data->task[i].process_id, data->task[i].waiting_time);
        ASSERT_EQUAL(wait_times[i], data->task[i].waiting_time);
    }
}

/**
 * This tests that quantum = 1 is handled
 * */
CTEST2(roundrobin5, test_turnaround){
    int turn_time[] ={0, 5, 8, 3, 10};
    for (int i = 0; i < data->size; i++) {
        printf("turnaround time is: %d\n",data->task[i].turnaround_time);
        ASSERT_EQUAL(turn_time[i], data->task[i].turnaround_time);
    }
}