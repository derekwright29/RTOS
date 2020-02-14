#include "sjf.h"
#include "queue.h"
#include <stdio.h>

//The init function will initalize the execution_time and process_id values for each task
void init(struct task_t *task, int *execution, int size) {
    for (int i = 0; i < size; i++)
    {
        task[i].process_id = i;
        // ternary operator catches negative execution values, and sets them to 0
        task[i].execution_time = execution[i] < 0 ? 0: execution[i];
    }
}

void shortest_job_first(struct task_t *task, int size) {
    int time_elapsed = 0;
    printf("\nIn SJF setup \n");
    // Hints:
    // 1. Create Queue based on the task array in the correct order
    struct node_t * head = create_queue(task, size);
    // 2. Process each "task" until completion
    // 3. You can process by popping items from the queue
    while(!is_empty(&head))
    {
        int i = 0;
        printf("\nIn SJF loop #%d\n", i++);
        //get current task
        struct task_t *cur_task = peek(&head);
        //update wait and turn times
        cur_task->turnaround_time = time_elapsed + cur_task->execution_time;
        cur_task->waiting_time = time_elapsed;
        printf("\nProcess #%d has wait time %d\n", cur_task->process_id, cur_task->waiting_time);
        printf("Process #%d has turn time %d\n", cur_task->process_id, cur_task->turnaround_time);
        time_elapsed += cur_task->execution_time;
        pop(&head);
    }
    //shouldn't be necessary.
    empty_queue(&head);
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