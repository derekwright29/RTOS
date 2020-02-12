#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct node_t* create_queue(struct task_t* task, int size) {
    struct node_t *head = NULL;
    for (int i = 0; i < size; i++)
    {
        printf("\nHead value: %lu \n", (unsigned long)(head));
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
    int length = 0;
    //traverse the queue
    while (cur_node != NULL)
    {
        length++;
        prev_node = cur_node;
        cur_node = cur_node->next;
    }
    //set last entry to the new_node;
    if (length == 0)
    {
        // special case where we have to actually manipulate the double pointer passed in
        *head = new_node;
    }
    else
    {
        printf("\nLength is %d\n", length);
        prev_node->next = new_node;
    }
        

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
    //invalidate head's data, thereby emptying the queue
    (*head)->task = NULL;
    (*head)->next = NULL;
    while (!is_empty(&cur_head))
    {
        dummy_head = cur_head;
        pop(&cur_head);
        free(dummy_head);
    }

}
