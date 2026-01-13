#include <time.h>
#include "glthread.h"

#ifndef COMMIT_H
#define COMMIT_H

typedef struct commit {
    time_t timestamp;
    char email[41];
    char user_name[21];
    char message[101];
    glthread_node_t node;
} commit_t;

#endif
