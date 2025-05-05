/*
    * Custom Type Definitions
    *
    * Purpose: This file contains custom type definitions for the project.
    * 
    * Date: 28/04/2025
    * 
    * Author: BEDROSSIAN Paul <paul.bedrossian92@gmail.com>    
*/
#ifndef CUSTOM_TYPE_H
#define CUSTOM_TYPE_H

#include <lwip/ip4_addr.h>

// Connection types
typedef enum {
    UDP_T, 
    TCP_T
} conn_t;

// Packet format for IRU
typedef struct {
    ip4_addr_t * src_addr;           
    ip4_addr_t * dest_addr;
    ip4_addr_t * next_hop_addr;
    conn_t proto;
    int dest_port;      
    size_t payload_len;
    void* payload;
} packet_t;

// Position format
typedef struct {
    double x;
    double y;
    double z;
} position_t;

#endif