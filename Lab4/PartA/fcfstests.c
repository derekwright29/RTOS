#include <stdlib.h>
#include "ctest.h"
#include "fcfs.h"

/****************************************************************************** / 
 *          FCFS TEST SUITE. There are 4 test cases, 12 total tests.
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

/**
 * This test confirms initialization operation
 * It checks the PIDs as well as the execution times, to make sure they match
 */ 
CTEST2(firstcomefirstserved, test_init) {
    int execution[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, data->task[i].process_id);
        ASSERT_EQUAL(execution[i], data->task[i].execution_time);
    }
}

/**
 * This test confirms wait times are correct
 * I have hard-coded the expected values, calculated by hand
 * wait time is the same as turn time - execution time
 * 
 * This test also confirms the average wait time is correct.
 * I've only included this averaging test once, because it is so simple
 */
CTEST2(firstcomefirstserved, test_waitingtime) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, data->task[i].waiting_time);
        ASSERT_EQUAL(((float)4.0 == calculate_average_wait_time(data->task, data->size)), 1);
    }
}

/**
 * This test confirms turnaround times are correct
 * I have hard-coded the expected values, calculated by hand
 * This value is always execution time + wait time.
 * 
 * This test also confirms the average turnaround time is correct.
 * I've only included this averaging test once, because it is so simple
 */
CTEST2(firstcomefirstserved, test_turnaround){
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i+1, data->task[i].turnaround_time);
        ASSERT_EQUAL(((float)5.0 == calculate_average_turn_around_time(data->task, data->size)), 1);
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

/**
 * This test confirms initialization operation
 * It checks the PIDs as well as the execution times, to make sure they match
 */
CTEST2(firstcomefirstserved2, test_init) {
    int execution[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, data->task[i].process_id);
        ASSERT_EQUAL(execution[i], data->task[i].execution_time);
    }
}

/**
 * This test confirms wait times are correct
 * I have hard-coded the expected values, calculated by hand
 */
CTEST2(firstcomefirstserved2, test_waitingtime) {
    int wait_times[] = {0,1,3,6, 10};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(wait_times[i], data->task[i].waiting_time);
    }
}

/**
 * This test confirms turnaround times are correct
 * I have hard-coded the expected values, calculated by hand
 * This value is always execution time + wait time.
 */
CTEST2(firstcomefirstserved2, test_turnaround){
    int turn_time[] ={1,3,6,10,15};
    for (int i = 0; i < data->size; i++) {
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

/**
 * This test confirms initialization operation
 * It checks the PIDs as well as the execution times, to make sure they match
 */
CTEST2(firstcomefirstserved3, test_init) {
    int execution[] = {5,1,3,10,3};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, data->task[i].process_id);
        ASSERT_EQUAL(execution[i], data->task[i].execution_time);
    }
}

/**
 * This test confirms wait times are correct
 * I have hard-coded the expected values, calculated by hand
 */
CTEST2(firstcomefirstserved3, test_waitingtime) {
    int wait_times[] = {0,5,6,9,19};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(wait_times[i], data->task[i].waiting_time);
    }
}

/**
 * This test confirms turnaround times are correct
 * I have hard-coded the expected values, calculated by hand
 * This value is always execution time + wait time.
 */
CTEST2(firstcomefirstserved3, test_turnaround){
    int turn_time[] ={5,6,9,19,22};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_time[i], data->task[i].turnaround_time);
    }
}

/****************************************************************************** / 
 *          TEST CASE #4
 ****************************************************************************** / */

 CTEST_DATA(firstcomefirstserved4) {
    struct task_t task[5];
    int size;
};

CTEST_SETUP(firstcomefirstserved4) {
    int execution[] = {-10, 1, 3, 0, 4};
    data->size = sizeof(execution) / sizeof(execution[0]);

    init(data->task, execution, data->size);
    first_come_first_served(data->task, data->size);
}

/**
 * This tests that negative values are initialized correctly
 *  Because there is no negative time, I set it to 0.
 * */
CTEST2(firstcomefirstserved4, test_process) {
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
CTEST2(firstcomefirstserved4, test_wait) {
    int wait_times[] = {0, 0, 1, 4, 4};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(wait_times[i], data->task[i].waiting_time);
    }
}

/**
 * This tests that negative values are handled correctly
 *  Turn time is the same as wait time for the invalid tasks
 * */
CTEST2(firstcomefirstserved4, test_turnaround){
    int turn_time[] ={0,1,4,4,8};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_time[i], data->task[i].turnaround_time);
    }
}