#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct node_t* create_queue(struct task_t* task, int size) {
    //initialize our queue pointer.
    struct node_t *head = NULL;
    // push <size> items to a new queue.
    // push() handles the fact that head is NULL.
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

/*
 * Push needs to create a new node, populate it with task, and add it to the queue
 * Push needs to traverse the queue to find the end.
 * I also wanted push to be able to handle (*head == NULL) because of my create_queue() function.
 * */
void push(struct node_t** head, struct task_t* task) {
    struct node_t *new_node = create_new_node(task);
    struct node_t *cur_node = *head;
    struct node_t *prev_node;
    // First check if we have an empty queue. If so, we're done.
    if (is_empty(head)) {
        *head = new_node;
        return;
    }
    //traverse the queue
    while (cur_node != NULL)
    {
        prev_node = cur_node;
        cur_node = cur_node->next;
    }
    //set last entry to the new_node;
    prev_node->next = new_node;
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
