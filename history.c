/**
 * @file
 *
 * Contains shell history data structures and retrieval functions.
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"
#include "clist.h"

struct clist *list = NULL;

void hist_init(unsigned int limit)
{
    list = clist_create(limit, sizeof(char *));

}

void hist_destroy(void)
{
    clist_destroy(list);
}

void hist_add(const char *cmd)
{
    clist_add(list, &cmd);
}

void hist_print(void)
{
    struct clist_iterator iter = clist_create_iter();
    int num = 1;
    void *elem;
    while ((elem = clist_iterate_rev(list, &iter)) != NULL) {
        char *command = *((char **) elem);
        int cmd_num = list->capacity > list->insertions ? num : (int) list->insertions - (int) list->capacity + num;
        printf("  %d %s\n", cmd_num, command);
        num++;
    }
    fflush(stdout);
}

const char *hist_search_prefix(char *prefix)
{
    struct clist_iterator iter = clist_create_iter();
    void *elem;
    while ((elem = clist_iterate(list, &iter)) != NULL) {
        char *command = *((char **) elem);
        if (strncmp(command, prefix, strlen(prefix)) == 0) {
	    return command;
	}
    }
    return NULL;
}

const char *hist_search_cnum(int command_number)
{
    void *elem = clist_get(list, command_number - 1);
    if (elem != NULL) {
	char *command = *((char **) elem);
	return command;
    } else {
	return NULL;
    }
}

int hist_last_cnum(void)
{
    return list->insertions;
}

