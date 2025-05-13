/*
    * Unit test for the table module.
    * 
    * Date : 13/05/2025
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void print_int(void *value) {
    printf("%d", *(int *)value);
}

int main() {
    table_t *table = table_new(sizeof(int));
    assert(table != NULL);

    int a = 10, b = 20, c = 30;
    assert(table_insert(table, 1, &a));
    assert(table_insert(table, 2, &b));
    assert(table_insert(table, 3, &c));

    assert(table_size(table) == 3);

    printf("Table après insertion :\n");
    table_print_debug(table, print_int);

    int *val = (int *)table_get(table, 2);
    assert(val != NULL && *val == 20);

    int d = 99;
    assert(table_update(table, 2, &d));
    val = (int *)table_get(table, 2);
    assert(val != NULL && *val == 99);

    // 6. Suppression de la clé 1
    assert(table_remove(table, 1));
    assert(table_size(table) == 2);

    assert(table_contains(table, 3));
    assert(!table_contains(table, 1));

    table_clear(table);
    assert(table_is_empty(table));
    assert(table_size(table) == 0);

    table_destroy(table);

    printf("All tests passed.\n");

    return 0;
}