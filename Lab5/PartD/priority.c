#include "priority.h"
#include "queue.h"
#include <stdio.h>

void init(struct task_t *task, int *execution, int *priority, int size) {
    for (int i = 0; i < size; i++)
    {
        task[i].process_id = i;
        // ternary operator catches negative execution values, and sets them to 0
        task[i].execution_time = execution[i] < 0 ? 0: execution[i];
        task[i].priority = priority[i];// < 0 ? 0: priority[i];
        task[i].left_to_execute = execution[i] < 0 ? 0: execution[i];
    }
}

void priority_schedule(struct task_t *task, int size) {
    int time_elapsed = 0;
    // Create Queue based on the task array in the correct order
    struct node_t *head = create_queue(task, size);
    // Each task can be processed for a time interval of 1 (i.e quantum time of 1)
    // You can process by pushing and popping items from the queue
    // You must recalculate the priorities after every turn
    while(!is_empty(&head)) {
        //get current task
        struct task_t *cur_task = peek(&head);
        //update variables to be incremented each loop
        cur_task->left_to_execute--;
        time_elapsed++;

        if(cur_task->left_to_execute <= 0){
            // Should be 0, but it COULD be negative.
            // less than or equal to zero to handle the case of a 0 execution time on init.
            // update time_elapsed that handles 0-execution-time tasks also.
            time_elapsed += cur_task->left_to_execute;
            pop(&head);
            cur_task->waiting_time = time_elapsed - cur_task->execution_time;
            cur_task->turnaround_time = time_elapsed;
        }
        update_priority(&head, time_elapsed);
    }
}

// a simple average with waiting_time variable
float calculate_average_wait_time(struct task_t *task, int size) {
    float sum = 0.0;
    for (int i = 0; i< size; i++) {
        sum += (float) task[i].waiting_time;
    }
    
    return sum / size;
}

// copy/paste from above, but with the different member variable, turnaround_time instead.
float calculate_average_turn_around_time(struct task_t *task, int size) {
    float sum = 0.0;
    for (int i = 0; i< size; i++) {
        sum += (float) task[i].turnaround_time;
    }  
    return sum / size;
}