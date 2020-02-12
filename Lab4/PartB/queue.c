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

void recycle(struct node_t** head)
{
    struct node_t* old_head = (*head);
    struct node_t* new_head = (*head)->next;
    (*head) = new_head;
    push(head, old_head->task);
}

void push(struct node_t** head, struct task_t* task) {
    struct node_t *new_node = create_new_node(task);
    struct node_t *cur_node = *head;
    struct node_t *prev_node;
    // length helps to know if the queue is empty
    // int length = 0;
    if (is_empty(head)) {
        *head = new_node;
        return;
    }
    //traverse the queue
    while (cur_node != NULL)
    {
        // length++;
        prev_node = cur_node;
        cur_node = cur_node->next;
    }
    //set last entry to the new_node;
    prev_node->next = new_node;
    // if (length == 0)
    // {
    //     // special case where we have to actually manipulate the double pointer passed in
    //     *head = new_node;
    // }
    // else
    // {
    //     prev_node->next = new_node;
    // }
        

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
