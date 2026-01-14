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

char *get_commit_time(const commit_t *commit) {
    const time_t timestamp = commit->timestamp;
    const struct tm *local_time = localtime(&timestamp);
    char *time_str = asctime(local_time);
    const size_t time_str_len = strlen(time_str);

    time_str[time_str_len - 2] = '\0';

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

void print_commit(void *arg) {
    commit_t *commit = arg;

    char *time_str = get_commit_time(commit);

    printf("User: %s\n", commit->user_name);
    printf("Email: %s\n", commit->email);
    printf("Message: %s\n", commit->message);
    printf("Commit time: %s\n", time_str);

    printf("-------------------------------------\n");
}

void print_commit_timelog(void *arg, const size_t index) {
    const commit_t *commit = arg;

    char *time_str = get_commit_time(commit);

    printf("%lu) %s | %s\n", index, time_str, commit->message);
}

unsigned char find_committers(const void *arg, const size_t index) {
    const char *name = "suyash";
    const commit_t *commit = arg;

    return strcmp(commit->user_name, name) == 0;
}

void print_commits_list(commit_t **commits, size_t commits_len) {
    for (size_t i = 0; i < commits_len; i++) {
        print_commit(commits[i]);
    }
}

int main(void) {
    glthread_t *commit_thread = glthread_init(NULL, offsetof(commit_t, node));
    glthread_t *branch1 = glthread_init(NULL, offsetof(commit_t, node));

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

    glthread_add_node(commit_thread, &commit1->node, TAIL);
    glthread_add_node(commit_thread, &commit2->node, HEAD);
    glthread_add_node(commit_thread, &commit3->node, TAIL);
    glthread_add_node(commit_thread, &commit4->node, HEAD);
    glthread_add_node(commit_thread, &commit5->node, HEAD);
    glthread_add_node(commit_thread, &commit7->node, TAIL);
    glthread_add_node(commit_thread, &commit6->node, HEAD);
    glthread_add_node(commit_thread, &commit10->node, HEAD);
    glthread_add_node(commit_thread, &commit8->node, HEAD);
    glthread_add_node(commit_thread, &commit9->node, TAIL);

    glthread_print_node(commit_thread, commit_thread->head, print_commit);
    glthread_print_node(commit_thread, commit_thread->head->next, print_commit);

    const size_t commit_thread_len = glthread_len(commit_thread);
    const size_t branch1_len = glthread_len(branch1);

    printf("%lu\n", commit_thread_len);
    printf("%lu\n", branch1_len);

    glthread_foreach(commit_thread, print_commit_timelog);

    size_t *commits_length;
    commit_t **commits = (commit_t **) glthread_where(commit_thread, find_committers, commits_length);

    print_commits_list(commits, *commits_length);
    printf("%lu\n", *commits_length);

    glthread_free(commit_thread);
    glthread_free(branch1);

    free(commits);

    return 0;
}
