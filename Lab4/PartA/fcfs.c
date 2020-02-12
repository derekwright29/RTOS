#include "fcfs.h"
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

/*This is the FCFS algorithm. When this function returns, 
 * the task buffer will be updated with wait time and turnaround times.
 * A queue is initialized in this function, and at the end the queue
 * is empty, meaning there are no tasks in the queue, and all pointers
 * malloc'd during its initialization have been freed.
 * 
 * The operation of this algorithm is simple. The first task
 * in the queue takes as long as it needs to execute, then is 
 * removed from the queue and the next task served.
*/
void first_come_first_served(struct task_t *task, int size) {
    int time_elapsed = 0;
    // Create Queue based on the task array
    struct node_t* head = create_queue(task, size);
    // Pop off the front item until there are no more tasks
    while (!is_empty(&head)) 
    {
        //get current task
        struct task_t *cur_task = peek(&head);
        //update stat info on current task based on time elapsed and its execution time
        cur_task->turnaround_time = time_elapsed +  cur_task->execution_time;
        cur_task->waiting_time = time_elapsed;
        //update time_elapsed. Simulates the task running to completion
        time_elapsed += cur_task->execution_time;
        //remove cur_task from the queue.
        pop(&head); 
    }
    // the queue should already be empty. This is just in case 
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