#include <stdlib.h>
#include <stdio.h>
#include "ctest.h"
#include "priority.h"

/****************************************************************************** / 
 *          TEST CASE #1
 ****************************************************************************** / */
CTEST_DATA(priority) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(priority) {
    int execution[] = {1, 2, 3};
    int priority[] = {1, 2, 3};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, priority, data->size);
    printf("\nCTEST: Out of Init\n");
    priority_schedule(data->task, data->size);
}

CTEST2(priority, test_init) {
    int execution[] = {1, 2, 3};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time); 
    }
}

CTEST2(priority, test_wait) {
    int wait_times[] = {0,2,3};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
        ASSERT_EQUAL(1, (float)calculate_average_wait_time(data->task, data->size) == (float)(5./3));

    }
}

CTEST2(priority, test_turn) {
    int turn_times[] = {1,4,6};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
        ASSERT_EQUAL(1, ((float)calculate_average_turn_around_time(data->task, data->size) == (float)(11.0/3)));
    }
}

/****************************************************************************** / 
 *          TEST CASE #1
 ****************************************************************************** / */
CTEST_DATA(priority2) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(priority2) {
    int execution[] = {1, 3, 4};
    int priority[] = {1, 2, 3};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, priority, data->size);
    printf("\nCTEST: Out of Init\n");
    priority_schedule(data->task, data->size);
}

CTEST2(priority2, test_init) {
    int execution[] = {1, 3, 4};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time); 
    }
}

CTEST2(priority2, test_wait) {
    int wait_times[] = {0,3,4};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
        ASSERT_EQUAL(1, (float)calculate_average_wait_time(data->task, data->size) == (float)(7./3));

    }
}

CTEST2(priority2, test_turn) {
    int turn_times[] = {1,6,8};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
        ASSERT_EQUAL(1, ((float)calculate_average_turn_around_time(data->task, data->size) == (float)(5.0)));
    }
}

/****************************************************************************** / 
 *          TEST CASE #3
 ****************************************************************************** / */
CTEST_DATA(priority3) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(priority3) {
    int execution[] = {4,2, 6};
    int priority[] = {4, 7, 1};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, priority, data->size);
    printf("\nCTEST: Out of Init\n");
    priority_schedule(data->task, data->size);
}

CTEST2(priority3, test_init) {
    int execution[] = {1, 2, 3};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time); 
    }
}

CTEST2(priority3, test_wait) {
    int wait_times[] = {0,2,3};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
        ASSERT_EQUAL(1, (float)calculate_average_wait_time(data->task, data->size) == (float)(5./3));

    }
}

CTEST2(priority3, test_turn) {
    int turn_times[] = {1,4,6};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
        ASSERT_EQUAL(1, ((float)calculate_average_turn_around_time(data->task, data->size) == (float)(11.0/3)));
    }
}
