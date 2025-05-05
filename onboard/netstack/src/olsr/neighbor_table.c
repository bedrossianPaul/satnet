/*
    * 
    * Date : 24/04/2025
    * 
    * Purpose : Give utils to initialize the network of lwIP.
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/

#include <neighbor_table.h>
#include <lwip/ip_addr.h>
#include <stdlib.h>
#include <string.h>
#include <olsropts.h>

static neighbor_t neighbor_table_1_hop[MAX_NEIGHBORS];
static neighbor_t neighbor_table_2_hop[MAX_NEIGHBORS];
static neighbor_t MPR_table[MAX_MPR];

/*
    TODO refaire ca proprement
    TODO faire un tableau de voisinage
    TODO faire un tableau de MPR
    TODO faire un tableau de 2 hop
*/

void add_neighbor(ip_addr_t *ip, uint8_t status, uint32_t last_seen, uint32_t willingness) {
    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        if (neighbor_table_1_hop[i].ip == NULL) {
            neighbor_table_1_hop[i].ip = ip;
            neighbor_table_1_hop[i].status = status;
            neighbor_table_1_hop[i].last_seen = last_seen;
            neighbor_table_1_hop[i].willingness = willingness;
            break;
        }
    }
}

ip_addr_t **get_neighbors() {
    ip_addr_t **neighbors = malloc(MAX_NEIGHBORS * sizeof(ip_addr_t *));
    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        if (neighbor_table_1_hop[i].ip != NULL) {
            neighbors[i] = neighbor_table_1_hop[i].ip;
        } else {
            neighbors[i] = NULL;
        }
    }
    return neighbors;
}

void remove_neighbor(ip_addr_t *ip) {
    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        if (neighbor_table_1_hop[i].ip != NULL && ip_addr_cmp(neighbor_table_1_hop[i].ip, ip)) {
            neighbor_table_1_hop[i].ip = NULL;
            break;
        }
    }
}


