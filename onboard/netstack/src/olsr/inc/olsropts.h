/*
    * OLSR - Options file
    * 
    * Date : 24/04/2025
    * 
    * Purpose : Set the OLSR options.
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/
#ifndef OLSR_OPTS_H
#define OLSR_OPTS_H

#include <lwip/ip_addr.h>

#define MAX_NEIGHBORS 16
#define MAX_MPRS 16

#define HELLO_INTERVAL 5000 // ms

#define TIMEOUT_NEIGHBOR 30000 // ms


#define WILLINGNESS_ALWAYS 7
#define WILLINGNESS_DEFAULT 3
#define WILLINGNESS_NEVER 0

typedef struct {
    int neighbor_count;
    ip_addr_t *neighbors;
    int mprs_count;
    ip_addr_t *mprs;
    int willingness;
} hello_msg_t;

typedef struct {
    int last_seen;
    int willingness;
} olsr_one_hop_neighbor_t;

typedef struct {
    int *via_neighbors;
    int count;
} olsr_two_hop_neighbors_t;

#endif