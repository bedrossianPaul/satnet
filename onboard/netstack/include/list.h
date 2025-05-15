/*
    * List data structure
    *
    * Date : 15/05/2025
    * 
    * Purpose : List data structure
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <pthread.h>

#define LIST_DEFAULT_CAPACITY 4

typedef struct list {
    void *data;
    size_t element_size;
    int capacity;
    int size;
    pthread_mutex_t lock;
} list_t;

/*
    * Create new list
    * @param element_size Size of the elements in the list
    * @return Pointer to the new list
    * @note The list is initialized with a default capacity of LIST_DEFAULT_CAPACITY
*/
list_t *list_new(size_t element_size);

/*
    * Add an element to the list
    * @param list Pointer to the list
    * @param element Pointer to the element to add
    * @note The list will be resized if it is full
    * @note The element is copied into the list you can free it after the call
*/
void list_add(list_t *list, const void *element);

/*
    * Remove an element from the list
    * @param list Pointer to the list
    * @param index Index of the element to remove
*/
void list_remove(list_t *list, int index);

/*
    * Get an element from the list
    * @param list Pointer to the list
    * @param index Index of the element to fetch
    * @return Pointer to the element or NULL if not found
*/
void *list_get(list_t *list, int index);

/*
    * Iterate over the list and apply a function to each element
    * @param list Pointer to the list
    * @param func Pointer to the function to apply
*/
void list_foreach(list_t *list, void (*func)(void *));

/*
    * Free the list
    * @param list Pointer to the list
    * @note The list will be freed and all elements will be freed
*/
void list_free(list_t *list);

#endif