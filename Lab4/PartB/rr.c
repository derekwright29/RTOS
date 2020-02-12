#include "rr.h"
#include "queue.h"
#include <stdio.h>

void init(struct task_t *task, int *execution, int size) {
    for (int i = 0; i < size; i++)
    {
        task[i].process_id = i;
        task[i].execution_time = execution[i];
        task[i].left_to_execute = execution[i]; // left to execute == execution time on init
    }
}

void round_robin(struct task_t *task, int quantum, int size) {
    int time_elapsed = 0;
    // Create Queue based on the task array
    struct node_t* head = create_queue(task, size);

    // Process each "task" in round robin fashion
    
    while(!is_empty(&head))
    {
        struct task_t *cur_task = peek(&head);
        printf("\n Task ID %d has %d time units remaining\n", cur_task->process_id, cur_task->left_to_execute);
        if(cur_task->left_to_execute > quantum)
        {
            printf("\nProcess ID %d is too slow. Recycling\n", cur_task->process_id);
            //needs to be recycled
            time_elapsed += quantum;
            cur_task->left_to_execute -= quantum;
            int executed_so_far = cur_task->execution_time - cur_task->left_to_execute;
            cur_task->waiting_time = time_elapsed - executed_so_far;
            cur_task->turnaround_time = time_elapsed;
            // recycle(&head); - Note these 3 lines perform *most* of pop, 
            //   while avoiding free-ing and reallocating.
            //     I wrote a function to do this, but can't implement it
            //      because of the rules of the lab.
            struct node_t* old_head = (head);
            head = head->next;
            push(&head, old_head->task);
        }
        else
        {
            printf("\nProcess ID %d is done. Popping\n", cur_task->process_id);
            time_elapsed += cur_task->left_to_execute;
            cur_task->left_to_execute = 0;
            int executed_so_far = cur_task->execution_time;
            cur_task->waiting_time = time_elapsed - executed_so_far;
            cur_task->turnaround_time = time_elapsed;
            pop(&head);
        }
        
    }
    // the queue should already be empty. This is just in case 
    empty_queue(&head);
}

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