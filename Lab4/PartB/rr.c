#include "rr.h"
#include "queue.h"
#include <stdio.h>

// The init function will init member variables of our task struct
// There are three that need to be updated, 
// while 2 more will be updated by the RR algortihm
void init(struct task_t *task, int *execution, int size) {
    for (int i = 0; i < size; i++)
    {
        task[i].process_id = i;
        // ternary operator catches negative execution values, and sets them to 0
        task[i].execution_time = execution[i] < 0 ? 0: execution[i];
        // left to execute == execution time on init
        task[i].left_to_execute = execution[i] < 0 ? 0: execution[i]; 
    }
}

/*This is the Round Robin algorithm. When this function returns, 
 * the task buffer will be updated with wait and turnaround times.
 * A queue is initialized in this function, and at the end the queue
 * is empty, meaning there are no tasks in the queue, and all pointers
 * malloc'd during its initialization have been freed.
 * 
 * The operation of this algorithm is that each task gets only a 
 * quantum of time to execute. If its execution time is longer, it
 * yields to the next task and is placed at the end of the queue.
*/
void round_robin(struct task_t *task, int quantum, int size) {
    int time_elapsed = 0;
    // Create Queue based on the task array
    struct node_t* head = create_queue(task, size);

    // Process each "task" in round robin fashion
    
    while(!is_empty(&head))
    {
        //get current task data
        struct task_t *cur_task = peek(&head);
        if(cur_task->left_to_execute > quantum)
        {
            //cur_task needs to be recycled
            //update time variables
            time_elapsed += quantum;
            cur_task->left_to_execute -= quantum;
            /* Note ***** 
              wait and turnaround don't need to be updated here because
              both of those values can be calculate in absolute terms
              from time_elapsed and execution_time.
              There is no need for incremental updates.
            */

            // recycle the task to back of queue
            // note pop() is not used b/c I don't want to reallocate space
            struct node_t* old_head = (head);
            head = head->next;
            push(&head, old_head->task);
        }
        else
        {
            //cur_task is done
            time_elapsed += cur_task->left_to_execute;
            cur_task->left_to_execute = 0;
            //update wait and turn times.
            cur_task->waiting_time = time_elapsed - cur_task->execution_time;
            cur_task->turnaround_time = time_elapsed;
            //remove for good the cur_task
            pop(&head);
        }
    }
    // the queue should already be empty. This is just in case 
    empty_queue(&head);
}

// Same as in FCFS
float calculate_average_wait_time(struct task_t *task, int size) {
    float sum = 0.0;
    for (int i = 0; i< size; i++) {
        sum += (float) task[i].waiting_time;
    }
    
    return sum / size;
}

float calculate_average_turn_around_time(struct task_t *task, int size) {
    float sum = 0.0;
    for (int i = 0; i< size; i++) {
        sum += (float) task[i].turnaround_time;
    }
    
    return sum / size;
}