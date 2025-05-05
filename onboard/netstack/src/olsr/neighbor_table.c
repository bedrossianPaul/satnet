/*
    * Neighbor table
    *
    * Date : 24/04/2025
    * 
    * Purpose : Give utils to initialize the network of lwIP.
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/

#include <neighbor_table.h>
#include <olsropts.h>

static neighbors_table_t *neighbors_table = NULL;

void init_neighbor_table() {
    // Initialize the neighbor table
    neighbors_table = malloc(sizeof(neighbors_table_t));
    if (neighbors_table == NULL) {
        perror("[ERROR] [OLSR] Failed to allocate memory for neighbor table");
        exit(EXIT_FAILURE);
    }
    neighbors_table->size = 0;
}

void add_neighbor(ip_addr_t *ip, int willingness, ip_addr_t *neighbors, int neighbors_count) {
    for (int i = 0; i < neighbors_table->size; i++) {
        if (ip_addr_cmp(ip, &neighbors_table->neighbors[i].ip)) {
            neighbors_table->neighbors[i].last_seen = 0;
            neighbors_table->neighbors[i].willingness = willingness;
            memcpy(neighbors_table->neighbors[i].neigh, neighbors, sizeof(ip_addr_t) * neighbors_count);
            neighbors_table->neighbors[i].neigh_count = neighbors_count;
            return;
        }
    }

    if (neighbors_table->size >= MAX_NEIGHBORS) {
        printf("[ERROR] [OLSR] Neighbor table is full\n");
        return;
    }

    neighbor_t *new_neighbor = &neighbors_table->neighbors[neighbors_table->size++];
    new_neighbor->ip = *ip;
    new_neighbor->last_seen = 0;
    new_neighbor->willingness = willingness;
    memcpy(new_neighbor->neigh, neighbors, sizeof(ip_addr_t) * neighbors_count);
    new_neighbor->neigh_count = neighbors_count;
}


int serialize_neighbors(uint8_t *buffer, int max_len) {
    int len = 0;
    for (int i = 0; i < neighbors_table->size; i++) {
        if (len + sizeof(ip_addr_t) > max_len) break;
        memcpy(buffer + len, &neighbors_table->neighbors[i].ip, sizeof(ip_addr_t));
        len += sizeof(ip_addr_t);
    }
    return len;
}

void deserialize_neighbors(uint8_t *buffer, int len, ip_addr_t *out_ips) {
    if (len % sizeof(ip_addr_t) != 0) {
        printf("[ERROR] [OLSR] Invalid length for deserialization\n");
        return;
    }

    int count = len / sizeof(ip_addr_t);
    if (count > MAX_NEIGHBORS) {
        printf("[ERROR] [OLSR] Too many neighbors to deserialize\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        memcpy(&out_ips[i], buffer + i * sizeof(ip_addr_t), sizeof(ip_addr_t));
    }

}

void print_neighbors_table() {
    if (neighbors_table == NULL) {
        printf("[INFO] Neighbor table is not initialized.\n");
        return;
    }

    printf("Neighbor Table (%u entries):\n", neighbors_table->size);
    for (uint32_t i = 0; i < neighbors_table->size; i++) {
        neighbor_t *n = &neighbors_table->neighbors[i];
        char ip_str[16];
        ip4addr_ntoa_r(&n->ip, ip_str, sizeof(ip_str));
        printf("  ➤ Neighbor %u:\n", i + 1);
        printf("     IP         : %s\n", ip_str);
        printf("     Last seen  : %d\n", n->last_seen);
        printf("     Willingness: %d\n", n->willingness);
        printf("     Neighbors  : %d\n", n->neigh_count);
        for (int j = 0; j < n->neigh_count; j++) {
            char neigh_ip_str[16];
            ip4addr_ntoa_r(&n->neigh[j], neigh_ip_str, sizeof(neigh_ip_str));
            printf("        - %s\n", neigh_ip_str);
        }
        printf("- - - - - - - - - - - - - - - - - -\n");
    }
}



