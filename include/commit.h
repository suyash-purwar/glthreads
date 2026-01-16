#ifndef COMMIT_H
#define COMMIT_H

#include <time.h>
#include "glthread.h"

typedef struct commit_thread {
    glthread_node_t* head;
} commit_thread_t;

typedef struct commit {
    time_t timestamp;
    char email[41];
    char user_name[21];
    char message[101];
    glthread_node_t node;
} commit_t;

#endif
