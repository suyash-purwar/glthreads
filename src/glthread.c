#include <stdlib.h>
#include "glthread.h"


void* glthread_node_container(const glthread_t* glthread, const glthread_node_t* node) {
    return (char*)node - glthread -> offset;
}

glthread_t* glthread_init(glthread_node_t* head_node, const size_t offset) {
    glthread_t* glthread = malloc(sizeof(glthread_t));

    if (head_node != NULL) {
        glthread -> head = head_node;
    }

    glthread -> offset = offset;

    return glthread;
}

void glthread_init_node(glthread_node_t* node) {
    node -> next = NULL;
    node -> prev = NULL;
}

int glthread_add_node(glthread_t* glthread, glthread_node_t* node, const Position position) {
    if (position == HEAD)
        return glthread_add_node_at_head(glthread, node);

    if (position == TAIL)
        return glthread_add_node_at_tail(glthread, node);

    return -1;
}

int glthread_add_node_at_head(glthread_t* glthread, glthread_node_t* node) {
    glthread_node_t* temp = glthread -> head;
    glthread -> head = node;
    node -> next = temp;
    node -> prev = NULL;

    return 0;
}

int glthread_add_node_at_tail(glthread_t* glthread, glthread_node_t* node) {
    glthread_node_t* curr_node = glthread -> head;

    if (curr_node == NULL) {
        glthread -> head = node;
        return 0;
    }

    while (curr_node -> next != NULL) {
        curr_node = curr_node -> next;
    }

    curr_node -> next = node;
    node -> prev = curr_node;

    return 0;
}

void glthread_free(const glthread_t* glthread) {
    const glthread_node_t* curr_node = glthread -> head;

    while (curr_node != NULL) {
        const glthread_node_t* next_node = curr_node -> next;

        void* container_struct_addr = glthread_node_container(glthread, curr_node);

        curr_node = next_node;

        free(container_struct_addr);
    }

    free((void*)glthread);
    glthread = NULL;
}
