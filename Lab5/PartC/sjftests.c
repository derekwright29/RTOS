#include <stdlib.h>
#include <stdio.h>
#include "ctest.h"
#include "sjf.h"
#include <time.h>

/****************************************************************************** / 
 *          TEST CASE #1
 ****************************************************************************** / */

CTEST_DATA(shortestjobfirst) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(shortestjobfirst) {
    int execution[] = {2, 2, 5};
    data->size = sizeof(execution) / sizeof(execution[0]);

    init(data->task, execution, data->size);
    shortest_job_first(data->task, data->size);
}

CTEST2(shortestjobfirst, test_init) {
    int execution[] = {2, 2, 5};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time);
    }
}

CTEST2(shortestjobfirst, test_wait) {
    int wait_times[] = {0,2,4};
    for (int i = 0; i < data->size; i++) {
        printf("\n wait time is: %d\n", data->task[i].waiting_time);
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
        // printf("\nAvg wait time is: %f\n", calculate_average_wait_time(data->task, data->size));
        // printf("\nAvg wait time is: %f\n", (float)4/3);
        ASSERT_EQUAL((float)calculate_average_wait_time(data->task, data->size) == (float)(2.0), 1);

    }
}

CTEST2(shortestjobfirst, test_turn) {
    int turn_times[] = {2,4,9};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
        ASSERT_EQUAL(((float)calculate_average_turn_around_time(data->task, data->size) == (float)(5.0)), 1);
    }
}

/****************************************************************************** / 
 *          TEST CASE #2
 ****************************************************************************** / */

CTEST_DATA(shortestjobfirst2) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(shortestjobfirst2) {
    int execution[] = {5, 2, 8};
    data->size = sizeof(execution) / sizeof(execution[0]);

    init(data->task, execution, data->size);
    shortest_job_first(data->task, data->size);
}

CTEST2(shortestjobfirst2, test_init) {
    int execution[] = {5, 2, 8};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time);
    }
}

CTEST2(shortestjobfirst2, test_wait) {
    int wait_times[] = {2,0,7};
    for (int i = 0; i < data->size; i++) {
        printf("\n wait time is: %d\n", data->task[i].waiting_time);
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
        // printf("\nAvg wait time is: %f\n", calculate_average_wait_time(data->task, data->size));
        // printf("\nAvg wait time is: %f\n", (float)4/3);
        ASSERT_EQUAL((float)calculate_average_wait_time(data->task, data->size) == (float)(3.0), 1);

    }
}

CTEST2(shortestjobfirst2, test_turn) {
    int turn_times[] = {7,2,15};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
        ASSERT_EQUAL(((float)calculate_average_turn_around_time(data->task, data->size) == (float)(8.0)), 1);
    }
}

/****************************************************************************** / 
 *          TEST CASE #3
 ****************************************************************************** / */

CTEST_DATA(shortestjobfirst3) {
    struct task_t task[5];
    int size;
};

CTEST_SETUP(shortestjobfirst3) {
    int execution[] = {5, 2, 8, 1, 6};
    data->size = sizeof(execution) / sizeof(execution[0]);

    init(data->task, execution, data->size);
    shortest_job_first(data->task, data->size);
}

CTEST2(shortestjobfirst3, test_init) {
    int execution[] = {5, 2, 8, 1, 6};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time);
    }
}

CTEST2(shortestjobfirst3, test_wait) {
    int wait_times[] = {3, 1, 14, 0, 8};
    for (int i = 0; i < data->size; i++) {
        printf("\n wait time is: %d\n", data->task[i].waiting_time);
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
        // printf("\nAvg wait time is: %f\n", calculate_average_wait_time(data->task, data->size));
        // printf("\nAvg wait time is: %f\n", (float)4/3);
        ASSERT_EQUAL((float)calculate_average_wait_time(data->task, data->size) == (float)(5.2), 1);

    }
}

CTEST2(shortestjobfirst3, test_turn) {
    int turn_times[] = {8, 3, 22, 1, 14};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
        ASSERT_EQUAL(((float)calculate_average_turn_around_time(data->task, data->size) == (float)(9.6)), 1);
    }
}

/****************************************************************************** / 
 *          TEST CASE #4
 ****************************************************************************** / */

CTEST_DATA(shortestjobfirst4) {
    struct task_t task[8];
    int size;
};

CTEST_SETUP(shortestjobfirst4) {
    int execution[] = {25, 2, 1, 4, 8, 1, 6, 6};
    data->size = sizeof(execution) / sizeof(execution[0]);

    init(data->task, execution, data->size);
    shortest_job_first(data->task, data->size);
}

CTEST2(shortestjobfirst4, test_init) {
    int execution[] = {25, 2, 1, 4, 8, 1, 6, 6};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time);
    }
}

CTEST2(shortestjobfirst4, test_wait) {
    int wait_times[] = {28, 2, 0, 4, 20, 1, 8, 14};
    for (int i = 0; i < data->size; i++) {
        printf("\n wait time is: %d\n", data->task[i].waiting_time);
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
    }
}

CTEST2(shortestjobfirst4, test_turn) {
    int turn_times[] = {53, 4, 1, 8, 28, 2, 14, 20};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
    }
}

/****************************************************************************** / 
 *          TEST CASE #5
 ****************************************************************************** / */

CTEST_DATA(shortestjobfirst5) {
    struct task_t task[7];
    int size;
};

CTEST_SETUP(shortestjobfirst5) {
    int execution[] = {12, 4, 3, 2, 1, 0, -1};
    data->size = sizeof(execution) / sizeof(execution[0]);

    init(data->task, execution, data->size);
    shortest_job_first(data->task, data->size);
}

CTEST2(shortestjobfirst5, test_init) {
    int execution[] = {12, 4, 3, 2, 1, 0, 0};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time);
    }
}

CTEST2(shortestjobfirst5, test_wait) {
    int wait_times[] = {10,6,3,1,0,0,0};
    for (int i = 0; i < data->size; i++) {
        printf("\n wait time is: %d\n", data->task[i].waiting_time);
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
    }
}

CTEST2(shortestjobfirst5, test_turn) {
    int turn_times[] = {22, 10, 6, 3, 1, 0,0};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
    }
}


