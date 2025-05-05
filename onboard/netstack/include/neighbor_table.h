/*
    * Neighbor table
    *
    * Date : 24/04/2025
    * 
    * Purpose : Give utils to initialize the network of lwIP.
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/

#ifndef NEIGHBOR_TABLE_H
#define NEIGHBOR_TABLE_H

#include <lwip/ip_addr.h>
#include <lwip/ip_addr.h>
#include <olsropts.h>

typedef struct {
    ip_addr_t ip;
    int last_seen;
    int willingness;
    ip_addr_t neigh[MAX_NEIGHBORS];
    int neigh_count;
} neighbor_t;

typedef struct {
    neighbor_t neighbors[MAX_NEIGHBORS];
    uint32_t size;
} neighbors_table_t;

/*
    * Init the neighbor table
    *  
    * 
*/
void init_neighbor_table();

/*
    * Add or update a neighbor in the table
    * 
    * @param ip the ip of the neighbor
    * @param willingness the willingness of the neighbor
    * @param neighbors the neighbors of the neighbor
    * @param neighbors_count the number of neighbors of the neighbor
    * 
*/
void add_neighbor(ip_addr_t * ip, int willingness, ip_addr_t * neighbors, int neighbors_count);

/*
    * serialize the neighbor table (only the ip)
    *
    * @param buffer the buffer to serialize the neighbor table
    * @param max_len the max length of the buffer
    * @return the length of the serialized buffer
*/
int serialize_neighbors(uint8_t *buffer, int max_len);

/*
    * deserialize the neighbor table
    *
    * @param buffer the buffer to deserialize the neighbor table
    * @param len the length of the buffer
    * @param out_ips the output buffer to store the deserialized ips
*/
void deserialize_neighbors(uint8_t *buffer, int len, ip_addr_t *out_ips);

/*
    * Print the neighbor table
    *
*/
// TODO : remove that after testing
void print_neighbors_table();

#endif