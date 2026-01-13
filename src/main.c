#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "glthread.h"
#include "commit.h"

#define MAX_EMAIL_LENGTH 41
#define MAX_USER_NAME_LENGTH 21
#define MAX_MESSAGE_LENGTH 101

commit_t* create_commit(const char* email, const char* user_name, const char* message) {
    commit_t* commit = malloc(sizeof(commit_t));

    strncpy(commit -> email, email, MAX_EMAIL_LENGTH - 1);
    strncpy(commit -> user_name, user_name, MAX_USER_NAME_LENGTH - 1);
    strncpy(commit -> message, message, MAX_MESSAGE_LENGTH - 1);

    commit -> timestamp = time(NULL);

    glthread_init_node(&commit -> node);

    return commit;
}

void print_commit_details(commit_t* commit) {
    printf("User: %s\n", commit -> user_name);
    printf("Email: %s\n", commit -> email);
    printf("Message: %s\n", commit -> message);

    const struct tm* local_time = localtime(&commit -> timestamp);
    printf("Commit time: %s\n", asctime(local_time));
}

int main(void) {
    glthread_t* commit_thread = glthread_init(NULL, offsetof(commit_t, node));

    commit_t* commit1 = create_commit("suyashpurwar4035@gmail.com", "suyash", "Initial commit");

    glthread_add_node(commit_thread, &commit1 -> node, TAIL);

    commit_t* c1 = glthread_node_container(commit_thread, commit_thread -> head);

    print_commit_details(c1);

    glthread_free(commit_thread);

    return 0;
}