/*
    * Table data structure - Implementation
    *
    * Date : 11/05/2025
    * 
    * Purpose : Table data structure
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#include <table.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

table_t *table_new(size_t entry_size) {
    table_t *table = malloc(sizeof(table_t));
    if (!table) {
        return NULL;
    }
    table->capacity = CAPACITY_DEFAULT;
    table->entries = malloc(sizeof(table_entry_t) * table->capacity);
    if (!table->entries) {
        free(table);
        return NULL;
    }
    table->value_size = entry_size;
    table->size = 0;
    pthread_mutex_init(&table->lock, NULL);
    return table;
}

bool table_insert(table_t *table, int key, void *value) {
    pthread_mutex_lock(&table->lock);
    // Check if the key already exists
    if (table_contains(table, key)) {
        pthread_mutex_unlock(&table->lock);
        return false;
    }
    // Resize the table if necessary
    if (table->size >= table->capacity) {
        int new_capacity = table->capacity * 2;
        table_entry_t *new_entries = realloc(table->entries, sizeof(table_entry_t) * new_capacity);
        if (!new_entries) {
            pthread_mutex_unlock(&table->lock);
            return false; // Memory allocation failed
        }
        table->entries = new_entries;
        table->capacity = new_capacity;
    }
    table->entries[table->size].key = key;
    table->entries[table->size].value = malloc(table->value_size);
    if (!table->entries[table->size].value) {
        pthread_mutex_unlock(&table->lock);
        return false; // Memory allocation failed
    }
    memcpy(table->entries[table->size].value, value, table->value_size);
    table->size++;
    pthread_mutex_unlock(&table->lock);
    return true;
}

bool table_update(table_t *table, int key, void *value) {
    pthread_mutex_lock(&table->lock);
    for (int i = 0; i < table->size; i++) {
        if (table->entries[i].key == key) {
            free(table->entries[i].value);
            table->entries[i].value = malloc(table->value_size);
            if (!table->entries[i].value) {
                pthread_mutex_unlock(&table->lock);
                return false; // Memory allocation failed
            }
            memcpy(table->entries[i].value, value, table->value_size);
            pthread_mutex_unlock(&table->lock);
            return true;
        }
    }
    pthread_mutex_unlock(&table->lock);
    return false; // Key not found
}

bool table_remove(table_t *table, int key) {
    pthread_mutex_lock(&table->lock);
    for (int i = 0; i < table->size; i++) {
        if (table->entries[i].key == key) {
            free(table->entries[i].value);
            table->entries[i] = table->entries[table->size - 1];
            table->size--;
            if (table->size <= table->capacity / 4) {
                int new_capacity = table->capacity / 4;
                if (new_capacity < CAPACITY_DEFAULT) {
                    new_capacity = CAPACITY_DEFAULT; // Min capacity
                }
                table_entry_t *new_entries = realloc(table->entries, sizeof(table_entry_t) * new_capacity);
                if (new_entries) {
                    table->entries = new_entries;
                    table->capacity = new_capacity;
                }
            }
            pthread_mutex_unlock(&table->lock);
            return true;
        }
    }
    pthread_mutex_unlock(&table->lock);
    return false; // Key not found
}

void *table_get(table_t *table, int key) {
    pthread_mutex_lock(&table->lock);
    for (int i = 0; i < table->size; i++) {
        if (table->entries[i].key == key) {
            pthread_mutex_unlock(&table->lock);
            return table->entries[i].value;
        }
    }
    pthread_mutex_unlock(&table->lock);
    return NULL; // Key not found
}

bool table_contains(table_t *table, int key) {
    pthread_mutex_lock(&table->lock);
    for (int i = 0; i < table->size; i++) {
        if (table->entries[i].key == key) {
            pthread_mutex_unlock(&table->lock);
            return true;
        }
    }
    pthread_mutex_unlock(&table->lock);
    return false; // Key not found
}

void table_clear(table_t *table) {
    pthread_mutex_lock(&table->lock);
    for (int i = 0; i < table->size; i++) {
        free(table->entries[i].value);
    }
    free(table->entries);
    table->size = 0;
    table->capacity = CAPACITY_DEFAULT;
    table->entries = malloc(sizeof(table_entry_t) * table->capacity);
    pthread_mutex_unlock(&table->lock);
}

bool table_is_empty(table_t *table) {
    return table->size == 0;
}

int table_size(table_t *table) {
    return table->size;
}

void table_destroy(table_t *table) {
    for (int i = 0; i < table->size; i++) {
        free(table->entries[i].value);
    }
    free(table->entries);
    pthread_mutex_destroy(&table->lock);
    free(table);
}

void table_print_debug(table_t *table, void (*print_value)(void *)) {
    pthread_mutex_lock(&table->lock);
    printf("Table size: %d, capacity: %d\n", table->size, table->capacity);
    for (int i = 0; i < table->size; i++) {
        printf("  [%d] Key: %d, Value: ", i, table->entries[i].key);
        print_value(table->entries[i].value);
        printf("\n");
    }
    pthread_mutex_unlock(&table->lock);
}