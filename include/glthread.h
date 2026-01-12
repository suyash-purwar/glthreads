#ifndef GLTHREAD_H
#define GLTHREAD_H

typedef struct glthread_node {
    struct glthread_node* prev;
    struct glthread_node* next;
} glthread_node_t;

typedef struct {
    glthread_node_t* head;
    unsigned int offset;
} glthread_t;

#endif