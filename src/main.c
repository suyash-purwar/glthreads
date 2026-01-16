#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "glthread.h"
#include "commit.h"

#define MAX_EMAIL_LENGTH 41
#define MAX_USER_NAME_LENGTH 21
#define MAX_MESSAGE_LENGTH 101

GLTHREAD_NODE_CONTAINER(commit_container, commit_t, node);

char *get_commit_time(const commit_t *commit) {
    const time_t timestamp = commit->timestamp;
    const struct tm *local_time = localtime(&timestamp);
    char *time_str = asctime(local_time);
    const size_t time_str_len = strlen(time_str);

    time_str[time_str_len - 1] = '\0';

    return time_str;
}

commit_t *create_commit(const char *email, const char *user_name, const char *message) {
    commit_t *commit = malloc(sizeof(commit_t));

    strncpy(commit->email, email, MAX_EMAIL_LENGTH - 1);
    strncpy(commit->user_name, user_name, MAX_USER_NAME_LENGTH - 1);
    strncpy(commit->message, message, MAX_MESSAGE_LENGTH - 1);

    commit->timestamp = time(NULL);

    glthread_init_node(&commit->node);

    return commit;
}

void print_commit(glthread_node_t* node) {
    commit_t *commit = commit_container(node);

    char *time_str = get_commit_time(commit);

    printf("User: %s\n", commit->user_name);
    printf("Email: %s\n", commit->email);
    printf("Message: %s\n", commit->message);
    printf("Commit time: %s\n", time_str);

    printf("-------------------------------------\n");
}

void print_commit_timelog(glthread_node_t *node, const size_t index) {
    commit_t *commit = commit_container(node);

    char *time_str = get_commit_time(commit);

    printf("%lu) %s | %s\n", index + 1, time_str, commit->message);
}

unsigned char find_committers(glthread_node_t* node, const size_t index) {
    const char *name = "suyash";
    const commit_t *commit = commit_container(node);

    return strcmp(commit->user_name, name) == 0;
}

void print_commits_list(glthread_node_t **nodes, size_t nodes_len) {
    for (size_t i = 0; i < nodes_len; i++) {
        print_commit(nodes[i]);
    }
}

int main(void) {
    commit_thread_t master_branch = { NULL };
    commit_thread_t branch1 = { NULL };

    commit_t *commit1 = create_commit("suyashpurwar4035@gmail.com", "suyash", "Initial commit");
    commit_t *commit2 = create_commit("suyash@couchbase.com", "suyashp", "Made it in!");
    commit_t *commit3 = create_commit("ash@tigerbeetle.com", "purwarsuyash", "systems engineer");
    commit_t *commit4 = create_commit("suyash@turso.com", "suyash", "go systems!");
    commit_t *commit5 = create_commit("suyash@nvidia.com", "suyash", "GPU goes brrrr");
    commit_t *commit6 = create_commit("suyash@texassystems.com", "suyash", "fix x86");
    commit_t *commit7 = create_commit("suyash@cloudflare.com", "suyash", "cloud");
    commit_t *commit8 = create_commit("suyash@nutanix.com", "suyash", "feat: wal");
    commit_t *commit9 = create_commit("suyash@clickhouse.com", "purwarsuyash", "feat: db");
    commit_t *commit10 = create_commit("suyash@es.com", "suyash", "feat: inverted index");

    glthread_add_node(&master_branch.head, &commit1->node, TAIL);
    glthread_add_node(&master_branch.head, &commit2->node, HEAD);
    glthread_add_node(&master_branch.head, &commit3->node, TAIL);
    glthread_add_node(&master_branch.head, &commit4->node, HEAD);
    glthread_add_node(&master_branch.head, &commit5->node, HEAD);
    glthread_add_node(&master_branch.head, &commit7->node, TAIL);
    glthread_add_node(&master_branch.head, &commit6->node, HEAD);
    glthread_add_node(&master_branch.head, &commit10->node, HEAD);
    glthread_add_node(&master_branch.head, &commit8->node, HEAD);
    glthread_add_node(&master_branch.head, &commit9->node, TAIL);

    print_commit(master_branch.head);
    print_commit(&commit4 -> node);

    const size_t master_branch_len = glthread_len(master_branch.head);
    const size_t branch1_len = glthread_len(branch1.head);

    printf("%lu\n", master_branch_len);
    printf("%lu\n", branch1_len);

    glthread_foreach(master_branch.head, print_commit_timelog);

    size_t nodes_length;
    glthread_node_t **nodes = glthread_where(master_branch.head, find_committers, &nodes_length);

    print_commits_list(nodes, nodes_length);
    printf("%lu\n", nodes_length);

    printf("%s\n", commit_container(commit5 -> node.prev) -> message);

    // TODO: Fix glthread_remove_all function
    // glthread_remove_all(master_branch.head);
    // glthread_remove_all(branch1.head);

    // free(commits);

    return 0;
}
