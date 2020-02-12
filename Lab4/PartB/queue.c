#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct node_t* create_queue(struct task_t* task, int size) {
    struct node_t *head = NULL;
    for (int i = 0; i < size; i++)
    {
        push(&head, &task[i]);
    }
    return head;
}

struct node_t* create_new_node(struct task_t* task) {
    struct node_t *new_node = malloc(sizeof(struct node_t));
    new_node->task = task;
    new_node->next = NULL;
    return new_node;
}

struct task_t* peek(struct node_t** head) {
    return (*head)->task;
}

void pop(struct node_t** head) {
    struct node_t* old_head = (*head);
    struct node_t* new_head = (*head)->next;
    (*head) = new_head;
    free(old_head);
}



void push(struct node_t** head, struct task_t* task) {
    struct node_t *new_node = create_new_node(task);
    struct node_t *cur_node = *head;
    struct node_t *prev_node;
    
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

int is_empty(struct node_t** head) {
    return ((*head) == NULL);
}

void empty_queue(struct node_t** head) {
    if (*head == NULL){
        //already empty. OR, we can't do anything with this.
        return;
    }
    struct node_t *cur_head = *head, *dummy_head;
    printf("probe print line\n\n");
    //invalidate head's data, thereby emptying the queue
    (*head)->task = NULL;
    (*head)->next = NULL;
    
    //free all the pointers
    while (!is_empty(&cur_head))
    {
        dummy_head = cur_head;
        pop(&cur_head);
        free(dummy_head);
    }  
}

/**
 * We couldn't add functions to oru queue API, but I would've added this one.
 * It simply performs the job of putting a slow task to the back of the queue
 * without free-ing and reallocating, which is what using pop() would force.
 * That's because pop() frees the node we are popping.
 * */
void recycle(struct node_t** head)
{
    struct node_t* old_head = (*head);
    struct node_t* new_head = (*head)->next;
    (*head) = new_head;
    push(head, old_head->task);
}