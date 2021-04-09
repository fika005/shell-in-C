#include <stddef.h>
#include <stdio.h>
#include<stdlib.h>

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
    int num = 0;
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
    // TODO: Retrieves the most recent command starting with 'prefix', or NULL
    // if no match found.
    return NULL;
}

const char *hist_search_cnum(int command_number)
{
    char *command = *((char **) clist_get(list, command_number));
    return command;
}

unsigned int hist_last_cnum(void)
{
    return list->insertions - 1;
}

