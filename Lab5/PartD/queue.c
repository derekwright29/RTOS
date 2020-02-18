#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct node_t* create_queue(struct task_t* task, int size) {
    //initialize our queue pointer.
    struct node_t *head = NULL;
    // push <size> items to a new queue.
    // push() handles the fact that head is NULL (empty queue).
    // push() also handles sorting
    for (int i = 0; i < size; i++)
    {
        push(&head, &task[i]);
    }
    return head;
}

struct node_t* create_new_node(struct task_t* task) {
    //allocate memory for a node
    struct node_t *new_node = malloc(sizeof(struct node_t));
    //populate node data
    new_node->task = task;
    // assume this new node is the "end" of its queue
    new_node->next = NULL;
    return new_node;
}

struct task_t* peek(struct node_t** head) {
    return (*head)->task;
}

/**
 * Pop needs to maintain the validity of the double pointer head.
 * Pop also needs to free the pointer pointed to by head when passed into this function.
 * Therefore, I create two temp nodes to safely update head, while also safely freeing
 * the old top of the queue
 * */
void pop(struct node_t** head) {
    struct node_t* old_head = (*head);
    struct node_t* new_head = (*head)->next;
    (*head) = new_head;
    free(old_head);
}


/****************************************************************************
 * Push-related defines
 * These give portability to different "sorted" scheduling algorithms (SJF, Prio, etc)
 * *****************************************************************************/
#define OP_SORT_ORDER_ASCENDING     >
#define OP_SORT_ORDER_DESCENDING    <

// sorting_value is the member variable of the task that determines order
#define SORTING_VALUE               priority
// sort operator determines if the sorted list should be ascending or descending wrt the sorting value.
#define __SORT_OPERATOR__           OP_SORT_ORDER_DESCENDING
/*  Push Description:
* ------------------------
 * Push needs to create a new node, populate it with task, and add it to the queue
 * Push needs to traverse the queue to find the proper place to insert.
 * I also wanted push to be able to handle (*head == NULL) because of my create_queue() function.
 * SORTING_VALUE is the member variable of task that will dictate how the queue is sorted.
 * In this case it is priority. Lower priority values are assumed to be highest priority.
 * In the case of the same priority, the first task with that priority will be ahead of any that follow.
 * */
void push(struct node_t** head, struct task_t* task) {
    struct node_t *new_node = create_new_node(task);
    struct node_t *cur_node = *head;
    struct node_t *prev_node = *head;
    int new_sort = task->SORTING_VALUE, length = 0;
    // First check if we have an empty queue. If so, we're done.
    if (is_empty(head)) {
        *head = new_node;
        return;
    }
    //traverse the queue
    while (cur_node != NULL )
    {
        length++;
        if (peek(&cur_node)->SORTING_VALUE __SORT_OPERATOR__ new_sort) {
            break;
        }
        prev_node = cur_node;
        cur_node = cur_node->next;
    }
    if (cur_node == NULL) {
        //set last entry to the new_node;
        prev_node->next = new_node;
    }
    else {
        if(length == 1){
            //special case, where *head itself needs to be changed.
            *head = new_node;
            (*head)->next = cur_node;
            return;
        }
        //insert new_node
        struct node_t * temp = prev_node->next;
        prev_node->next = new_node;
        new_node->next = temp;
    }
}

// simple boolean function. If *head == NULL, that means there is no node, no data.
int is_empty(struct node_t** head) {
    return ((*head) == NULL);
}

void empty_queue(struct node_t** head) {
    if (*head == NULL){
        //already empty. OR, we can't do anything with this.
        return;
    }
    struct node_t *cur_head = *head, *dummy_head; 
    // Free each node pointed to by head.
    while (!is_empty(&cur_head))
    {
        dummy_head = cur_head;
        pop(&cur_head);
        free(dummy_head);
    }
    //other functions expect (*head) to be NULL when empty. This closes the loop there.
    (*head) = NULL;
}

/**
 * update_priority description
 * ---------------------------
 * This function handles all the priority changes that can happen dynamically
 * In order to not modify push() and pop() at all from other implementations of the queue,
 *  -- which is how it should be --, 
 * This function is reponsible for making the queue ALWAYS sorted.
 * To accomplish this, I basically perform a queue_copy operation to a new queue
 * This allows push() to handle the sorting, as it should.
 * I set *head to the new queue pointer, and pop the old queue, freeing the memory there.
 * */
void update_priority(struct node_t** head, int time) {
    struct node_t *cur_node = *head;
    //flag to let me know if priorities have changed.
    int prio_changed_flag = 0;
    //traverse the queue
    while(!is_empty(&cur_node)) {
        if (peek(&cur_node)->execution_time == time) {
            // first priority change rule
            cur_node->task->priority *= 4;
            prio_changed_flag = 1;
        }
        if (cur_node->task->left_to_execute == time) {
            // second priority change rule
            cur_node->task->priority *= 2;
            prio_changed_flag = 1;
        }
        cur_node = cur_node->next;
    }
    if(prio_changed_flag){
        // I only want to do the work of creating a new queue when I have to
        struct node_t *new_head = create_queue(NULL, 0);
        while(!is_empty(head)) {
            //push node from old queue onto new queue
            push(&new_head, peek(head));
            // pop from old queue
            pop(head);
            //repeat while old_queue is not empty
        }
        //give new queue pointer to caller of this function.
        *head = new_head;
        prio_changed_flag = 0;
    }
    
}