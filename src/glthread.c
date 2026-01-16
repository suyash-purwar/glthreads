#include <stdlib.h>
#include <stdio.h>
#include "glthread.h"

#define LIST_SIZE 2
#define GROWTH_FACTOR 2

void glthread_init_node(glthread_node_t *node) {
    node->next = NULL;
    node->prev = NULL;
}

int glthread_add_node(glthread_node_t **head_node, glthread_node_t *node, const Position position) {
    switch (position) {
        case HEAD:
            glthread_add_node_at_head(head_node, node);
            return 0;
        case TAIL:
            glthread_add_node_at_tail(head_node, node);
            return 0;
        default:
            return -1;
    }
}

void glthread_add_node_at_head(glthread_node_t **head_node, glthread_node_t *node) {
    if (*head_node != NULL)
        (*head_node) -> prev = node;

    glthread_node_t *temp = *head_node;
    *head_node = node;
    node->next = temp;
    node->prev = NULL;
}

void glthread_add_node_at_tail(glthread_node_t **head_node, glthread_node_t *node) {
    glthread_node_t *curr_node = *head_node;

    if (curr_node == NULL) {
        *head_node = node;
        return;
    }

    while (curr_node->next != NULL) {
        curr_node = curr_node->next;
    }

    curr_node->next = node;
    node->prev = curr_node;
}

int glthread_remove_node(glthread_node_t *head_node, const Position position) {
    switch (position) {
        case HEAD:
            glthread_remove_node_at_head(head_node);
            return 0;
        case TAIL:
            glthread_remove_node_at_tail(head_node);
            return 0;
        default:
            return -1;
    }
}

void glthread_remove_node_at_head(glthread_node_t *head_node) {
    glthread_node_t *temp = head_node;
    head_node = head_node->next;
    head_node->prev = NULL;
    free(temp);
}

void glthread_remove_node_at_tail(glthread_node_t *head_node) {
    glthread_node_t *curr_node = head_node;

    while (curr_node->next != NULL) {
        curr_node = curr_node->next;
    }

    curr_node->prev->next = NULL;
    free(curr_node);
}

void glthread_remove_all(glthread_node_t *head_node) {
    glthread_node_t *curr_node = head_node;

    while (curr_node != NULL) {
        glthread_node_t *next_node = curr_node->next;

        curr_node -> next = NULL;
        curr_node -> prev = NULL;

        curr_node = next_node;
    }
}

size_t glthread_len(const glthread_node_t *head_node) {
    const glthread_node_t *curr_node = head_node;
    size_t size = 0;

    while (curr_node != NULL) {
        curr_node = curr_node->next;
        size++;
    }

    return size;
}

void glthread_foreach(const glthread_node_t *head_node, void cb(glthread_node_t* node, size_t index)) {
    const glthread_node_t *curr_node = head_node;
    size_t index = 0;

    while (curr_node != NULL) {
        cb((void*) curr_node, index);

        curr_node = curr_node->next;
        index++;
    }
}

glthread_node_t **glthread_where(glthread_node_t *head_node, unsigned char cb(glthread_node_t* node, size_t index),
                      size_t *out_list_size) {
    glthread_node_t *curr_node = head_node;
    size_t glthread_index = 0;

    glthread_node_t **filtered_nodes_list = malloc(LIST_SIZE * sizeof(void *));
    size_t filtered_nodes_list_max_size = LIST_SIZE;
    size_t filtered_nodes_list_size = 0;

    while (curr_node != NULL) {
        if (cb(curr_node, glthread_index)) {
            filtered_nodes_list[filtered_nodes_list_size] = curr_node;
            filtered_nodes_list_size++;
        }

        if (filtered_nodes_list_size == filtered_nodes_list_max_size && curr_node->next != NULL) {
            filtered_nodes_list_max_size *= GROWTH_FACTOR;
            glthread_node_t **temp_filtered_nodes_list = realloc(filtered_nodes_list,
                                                      filtered_nodes_list_max_size * sizeof(void *));

            if (temp_filtered_nodes_list == NULL) {
                perror("Failed to allocate memory");
                exit(-1);
            }

            filtered_nodes_list = temp_filtered_nodes_list;
            temp_filtered_nodes_list = NULL;
        }

        curr_node = curr_node->next;
        glthread_index++;
    }

    *out_list_size = filtered_nodes_list_size;

    return filtered_nodes_list;
}
