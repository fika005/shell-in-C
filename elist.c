/**
 * @file
 *
 * Implements the elastic list data structure to hold command tokens
 */
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>

#include "elist.h"

#define DEFAULT_INIT_SZ 10
#define RESIZE_MULTIPLIER 2

struct elist {
    size_t capacity;         /*!< Storage space allocated for list items */
    size_t size;             /*!< The actual number of items in the list */
    size_t item_sz;          /*!< Size of the items stored in the list */
    void *element_storage;   /*!< Pointer to the beginning of the array */
    bool freed;              /*!< whether the element_storage is freed */
} elist;

bool idx_is_valid(struct elist *list, size_t idx);

struct elist *elist_create(size_t list_sz, size_t item_sz)
{
    struct elist *list = malloc(sizeof(struct elist));
    if (list == NULL) {
        return NULL;
    }
    list->size = 0;
    list->capacity = (list_sz == 0 ? DEFAULT_INIT_SZ:list_sz);
    list->item_sz = item_sz;
    void *first_element = calloc(list->capacity, item_sz);
    if (first_element == NULL) {
        free(list);
        return NULL;
    }
    list->element_storage = first_element;
    list->freed = false;
    return list;
}
 
void elist_destroy(struct elist *list)
{
    if (!list->freed) {
        elist_clear_mem(list);
    }
    free(list);
}

int elist_set_capacity(struct elist *list, size_t capacity)
{
    if (capacity < list->capacity) {
        list->capacity = capacity;
        list->size = (capacity > list->size ? list->size:capacity);
        return 0;
    }
    list->element_storage = realloc(list->element_storage, capacity * list->item_sz);
    if (list->element_storage == NULL) {
        return -1;
    }
    list->capacity = capacity;
    list->size = (capacity > list->size ? list->size:capacity);
    return 0;
}

size_t elist_capacity(struct elist *list)
{
    return list->capacity;
}
ssize_t elist_add(struct elist *list, void *item)
{
    int flag = 0;
    if (list->size == list->capacity) {
        size_t capacity = (list->capacity == 0 ? DEFAULT_INIT_SZ:list->capacity * RESIZE_MULTIPLIER);
        flag = elist_set_capacity(list, capacity);
    }
    if (flag == 0) {
        ssize_t idx = list->size++;
        elist_set(list, idx, item);
        return idx;
    }
    return -1;
}

void *elist_add_new(struct elist *list)
{
    if (list->size == list->capacity) {
        size_t capacity = (list->capacity == 0 ? DEFAULT_INIT_SZ:list->capacity * RESIZE_MULTIPLIER);
        elist_set_capacity(list, capacity);
    }
    return elist_get(list, list->size++);
}

int elist_set(struct elist *list, size_t idx, void *item)
{
    if (idx_is_valid(list, idx)) {
        memcpy((char *) list->element_storage + idx * list->item_sz, item, list->item_sz);
        return 0;
    }
    return -1;
}

void *elist_get(struct elist *list, size_t idx)
{
    if(idx_is_valid(list, idx)){
        return (char *) list->element_storage + idx * list->item_sz;
    } else {
        return NULL;
    } 
}

size_t elist_size(struct elist *list)
{
    return list->size;
}

int elist_remove(struct elist *list, size_t idx)
{
    if (idx_is_valid(list, idx)) {
        memcpy(elist_get(list, idx), elist_get(list, idx + 1), (list->size - idx -1) * list->item_sz);
        list->size--;
        return 0;
    }
    return -1;
}

void elist_clear(struct elist *list)
{
    list->size = 0;
}

void elist_clear_mem(struct elist *list)
{
    memset(list->element_storage, 0, list->size * list->item_sz);
    elist_clear(list);
    free(list->element_storage);
    list->freed = true;
}

ssize_t elist_index_of(struct elist *list, void *item)
{
    ssize_t i = 0;
    while (i < list->size) {
        if (memcmp(elist_get(list, i), item, list->item_sz) == 0) {
            break;
        }
        i++;
    }
    if(i < list->size) {
        return i;
    } else {
        return -1;
    }
}

void elist_sort(struct elist *list, int (*comparator)(const void *, const void *))
{
    qsort(list->element_storage, list->size, list->item_sz, comparator);
}

bool idx_is_valid(struct elist *list, size_t idx)
{
    return (idx < list->size);
}
