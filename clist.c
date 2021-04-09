#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "clist.h"

struct clist {
    cnode *head;
    size_t insertions;
    size_t capacity;
    size_t item_sz;
    void *element_storage;
};
struct cnode {
    struct cnode * next;
    struct cnode * previous;
    void * data;
};
typedef struct cnode cnode;

struct clist_iterator {
    unsigned int initialized : 1;
    size_t idx;
};

struct clist_iterator clist_create_iter(void) {
    struct clist_iterator it = { 0 };
    return it;
}

/**
 * Iterates through the provided list, starting with the most recent insertion
 * and working backward until the end of the list is reached. Each invocation of
 * the function retrieves the next element, with NULL indicating the end of the
 * list has been reached. 'iter' is used to track state through the iteration.
 */
void *clist_iterate(struct clist *list, struct clist_iterator *iter) {
    iter->idx = list->insertions;
    while (iter->idx != 0) 
    {
        iter->idx--;
    }
    

}

/**
 * Iterates through the provided list, starting with the least-recently inserted
 * element and proceeding forward to the beginning (most recent) element of the
 * list.  NULL indicates the start of the list has been reached. 'iter' is used
 * to track state through the iteration.
 */
void *clist_iterate_rev(struct clist *list, struct clist_iterator *iter) {
    iter->idx = 0;
    while (iter->idx != list->insertions) 
    {
        iter->idx++;
    }
}



int main(void)
{
    struct clist *list = clist_create(5, sizeof(int));

    int x;
    x = 0;
    clist_add(list, &x);

    x = 1; clist_add(list, &x);
    x = 2; clist_add(list, &x);
    x = 3; clist_add(list, &x);
    x = 4; clist_add(list, &x);
    x = 5; clist_add(list, &x);
    x = 6; clist_add(list, &x);

    int *y = clist_get(list, 2);
    printf("y = %d\n\n", *y);

    void *elem;
    struct clist_iterator iter = clist_create_iter();
    while ((elem = clist_iterate(list, &iter)) != NULL) {
        int ie = *((int *) elem);
        printf("-> %d\n", ie);
    }

    puts("\n\n");

    struct clist_iterator iter2 = clist_create_iter();
    while ((elem = clist_iterate_rev(list, &iter2)) != NULL) {
        int ie = *((int *) elem);
        printf("-> %d\n", ie);
    }

    clist_destroy(list);

    return 0;
}
