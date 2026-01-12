#include "glthread.h"

#ifndef COMMIT_H
#define COMMIT_H

typedef struct commit {
    char name[20];
    char email[14];
    char user_name[10];
    char message[50];
    glthread_node_t node;
} commit_t;

#endif
