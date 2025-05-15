/*
    * List data structure
    *
    * Date : 15/05/2025
    * 
    * Purpose : List data structure
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/
#include <list.h>
#include <stdlib.h>
#include <string.h>

list_t *list_new(size_t element_size) {
    list_t *list = malloc(sizeof(list_t));
    if (list == NULL) {
        return NULL;
    }
    list->data = malloc(LIST_DEFAULT_CAPACITY * element_size);
    if (list->data == NULL) {
        free(list);
        return NULL;
    }
    list->element_size = element_size;
    list->capacity = LIST_DEFAULT_CAPACITY;
    list->size = 0;
    pthread_mutex_init(&list->lock, NULL);
    return list;
}


void list_add(list_t *list, const void *element) {
    pthread_mutex_lock(&list->lock);
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * list->element_size);
    }
    memcpy((char *)list->data + list->size * list->element_size, element, list->element_size);
    list->size++;
    pthread_mutex_unlock(&list->lock);
}

void list_remove(list_t *list, int index) {
    pthread_mutex_lock(&list->lock);
    if (index < 0 || index >= list->size) {
        pthread_mutex_unlock(&list->lock);
        return;
    }
    memmove((char *)list->data + index * list->element_size,
            (char *)list->data + (index + 1) * list->element_size,
            (list->size - index - 1) * list->element_size);
    list->size--;

    if (list->size < list->capacity / 4 && list->capacity > LIST_DEFAULT_CAPACITY) {
        int new_capacity = list->capacity / 2;
        if (new_capacity < LIST_DEFAULT_CAPACITY) {
            new_capacity = LIST_DEFAULT_CAPACITY;
        }

        void *new_data = realloc(list->data, new_capacity * list->element_size);
        if (new_data != NULL) {
            list->data = new_data;
            list->capacity = new_capacity;
        }
    }
    pthread_mutex_unlock(&list->lock);
}

void *list_get(list_t *list, int index){
    if (index < 0 || index >= list->size) {
        return NULL;
    }
    return (char *)list->data + index * list->element_size;
}

void list_foreach(list_t *list, void (*func)(void *)) {
    pthread_mutex_lock(&list->lock);
    if (!list || !func) {
        pthread_mutex_unlock(&list->lock);
        return;
    }
    for (int i = 0; i < list->size; i++) {
        void *element = (char *)list->data + i * list->element_size;
        func(element);
    }
    pthread_mutex_unlock(&list->lock);
}

void list_free(list_t *list) {
    if (list != NULL) {
        pthread_mutex_destroy(&list->lock);
        free(list->data);
        free(list);
    }
}