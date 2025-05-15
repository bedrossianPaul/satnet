/*
    * Table data structure - Header
    *
    * Date : 11/05/2025
    * 
    * Purpose : Table data structure
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#ifndef TABLE_H
#define TABLE_H

#include <stdbool.h>
#include <pthread.h>
#include <stddef.h>

// Default capacity of the table
// This is the initial size of the table when it is created
// It will be resized if the number of entries exceeds this value
#define CAPACITY_DEFAULT 10

// Data structure of an entry
typedef struct {
    int key;
    void *value;
} table_entry_t;

// Data structure of the table
typedef struct {
    table_entry_t *entries;
    int size;
    int capacity;
    size_t value_size;
    pthread_mutex_t lock; // Mutex for thread safety
} table_t;

/*
    * Initializes a new table
    * @param entry_size Size of the entry value type
    * @return Pointer to the new table
*/
table_t *table_new(size_t entry_size);

/*
    * Insert a new entry in the table
    * @param table Pointer to the table
    * @param key Key of the entry
    * @param value Pointer to the value of the entry
    * @return True if the entry was inserted, false if the key already exists
    * @note The value is copied to the table, so the caller is responsible for freeing it
*/
bool table_insert(table_t *table, int key, void *value);

/*
    * Update an entry in the table
    * @param table Pointer to the table
    * @param key Key of the entry
    * @param value Pointer to the new value of the entry
    * @return True if the entry was updated, false if the key does not exist
*/
bool table_update(table_t *table, int key, void *value);

/*
    * Remove an entry from the table
    * @param table Pointer to the table
    * @param key Key of the entry
    * @return True if the entry was removed, false if the key does not exist
*/
bool table_remove(table_t *table, int key);

/*
    * Get an entry from the table
    * @param table Pointer to the table
    * @param key Key of the entry
    * @return Pointer to the value of the entry, or NULL if the key does not exist
*/
void *table_get(table_t *table, int key);

/*
    * Check if the table contains a key
    * @param table Pointer to the table
    * @param key Key of the entry
    * @return True if the table contains the key, false otherwise
*/
bool table_contains(table_t *table, int key);

/*
    * Clear the table
    * @param table Pointer to the table
    * @note This will free all entries in the table but not the table itself
*/
void table_clear(table_t *table);

/*
    * Check if the table is empty
    * @param table Pointer to the table
    * @return True if the table is empty, false otherwise
*/
bool table_is_empty(table_t *table);

/*
    * Get the number of entries in the table
    * @param table Pointer to the table
    * @return Number of entries in the table
*/
int table_size(table_t *table);

/*
    * Destroy the table
    * @param table Pointer to the table
    * @note This will free all entries in the table and the table itself
*/
void table_destroy(table_t *table);

/*
    * Print debug information about the table
    * @param table Pointer to the table
    * @param print_value Function to print the value of an entry
    * @note This will print the size and capacity of the table and all entries
*/
void table_print_debug(table_t *table, void (*print_value)(void *));

#endif