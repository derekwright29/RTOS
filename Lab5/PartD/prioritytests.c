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

// This test tests the PID and execution_tiems were set properly.
CTEST2(priority, test_init) {
    int execution[] = {1, 2, 3};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time); 
    }
}

// This test confirms the wait times were computed properly
// I hand calculated the wait_times array
CTEST2(priority, test_wait) {
    int wait_times[] = {0,2,3};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
        ASSERT_EQUAL(1, (float)calculate_average_wait_time(data->task, data->size) == (float)(5./3));

    }
}

// This test confirms the turn times were computed properly
// I hand calculated the turn_times array
CTEST2(priority, test_turn) {
    int turn_times[] = {1,4,6};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
        ASSERT_EQUAL(1, ((float)calculate_average_turn_around_time(data->task, data->size) == (float)(11.0/3)));
    }
}
// This tests that the update_priority correctly updated the priorities 
// of each node as it ran through.
CTEST2(priority, test_end_prios) {
    int end_prios[] = {1, 8, 12};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(end_prios[i], (int)data->task[i].priority);
    }
}

/****************************************************************************** / 
 *          TEST CASE #2
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

// This test tests the PID and execution_tiems were set properly.
CTEST2(priority2, test_init) {
    int execution[] = {1, 3, 4};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time); 
    }
}

// This test confirms the wait times were computed properly
// I hand calculated the wait_times array
CTEST2(priority2, test_wait) {
    int wait_times[] = {0,3,4};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
        ASSERT_EQUAL(1, (float)calculate_average_wait_time(data->task, data->size) == (float)(7./3));

    }
}

// This test confirms the turn times were computed properly
// I hand calculated the turn_times array
CTEST2(priority2, test_turn) {
    int turn_times[] = {1,6,8};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
        ASSERT_EQUAL(1, ((float)calculate_average_turn_around_time(data->task, data->size) == (float)(5.0)));
    }
}
// This tests that the update_priority correctly updated the priorities 
// of each node as it ran through.
CTEST2(priority2, test_end_prios) {
    int end_prios[] = {1, 16, 24};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(end_prios[i], (int)data->task[i].priority);
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
    priority_schedule(data->task, data->size);
}

// This test tests the PID and execution_tiems were set properly.
CTEST2(priority3, test_init) {
    int execution[] = {4,2, 6};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time); 
    }
}

// This test confirms the wait times were computed properly
// I hand calculated the wait_times array
CTEST2(priority3, test_wait) {
    int wait_times[] = {6,10,0};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
        ASSERT_EQUAL(1, (float)calculate_average_wait_time(data->task, data->size) == (float)(16./3));

    }
}

// This test confirms the turn times were computed properly
// I hand calculated the turn_times array
CTEST2(priority3, test_turn) {
    int turn_times[] = {10,12,6};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
        ASSERT_EQUAL(1, ((float)calculate_average_turn_around_time(data->task, data->size) == (float)(28.0/3)));
    }
}

// This tests that the update_priority correctly updated the priorities 
// of each node as it ran through.
CTEST2(priority3, test_end_prios) {
    int end_prios[] = {32, 56, 2};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(end_prios[i], (int)data->task[i].priority);
    }
}

/****************************************************************************** / 
 *          TEST CASE #4
 ****************************************************************************** / */
CTEST_DATA(priority4) {
    struct task_t task[4];
    int size;
};

// This setup is meant to test 0 task time, as well as negative priority.
// I set negative priority to 0, in effect locking it in as the most important task,
// since our update() function uses only multiplication.
CTEST_SETUP(priority4) {
    int execution[] = {4,2, 6, 0};
    int priority[] = {4, 7, 1, -1};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, priority, data->size);
    priority_schedule(data->task, data->size);
}

// This test tests the PID and execution_tiems were set properly.
CTEST2(priority4, test_init) {
    int execution[] = {4,2, 6, 0};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time); 
    }
}

// This test confirms the wait times were computed properly
// I hand calculated the wait_times array
CTEST2(priority4, test_wait) {
    int wait_times[] = {6,10,0,0};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
    }
}

// This test confirms the turn times were computed properly
// I hand calculated the turn_times array
CTEST2(priority4, test_turn) {
    int turn_times[] = {10,12,6,0};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
    }
}

// This tests that the update_priority correctly updated the priorities 
// of each node as it ran through.
CTEST2(priority4, test_end_prios) {
    int end_prios[] = {32, 56, 2, 0};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(end_prios[i], (int)data->task[i].priority);
    }
}

/****************************************************************************** / 
 *          TEST CASE #5
 ****************************************************************************** / */
CTEST_DATA(priority5) {
    struct task_t task[4];
    int size;
};

// This test is meant to have lots of priority switches.
CTEST_SETUP(priority5) {
    int execution[] = {10,2, 1, 7};
    int priority[] = {12, 2, 6, 3};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, priority, data->size);
    priority_schedule(data->task, data->size);
}

// This test tests the PID and execution_tiems were set properly.
CTEST2(priority5, test_init) {
    int execution[] = {10,2, 1, 7};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
        ASSERT_EQUAL(execution[i], (int)data->task[i].execution_time); 
    }
}

// This test confirms the wait times were computed properly
// I hand calculated the wait_times array
CTEST2(priority5, test_wait) {
    int wait_times[] = {9, 9, 19, 5};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(wait_times[i], (int)data->task[i].waiting_time);
    }
}

// This test confirms the turn times were computed properly
// I hand calculated the turn_times array
CTEST2(priority5, test_turn) {
    int turn_times[] = {19, 11, 20, 12};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(turn_times[i], (int)data->task[i].turnaround_time);
    }
}

// This tests that the update_priority correctly updated the priorities 
// of each node as it ran through.
CTEST2(priority5, test_end_prios) {
    int end_prios[] = {48,16,48,24};
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(end_prios[i], (int)data->task[i].priority);
    }
}