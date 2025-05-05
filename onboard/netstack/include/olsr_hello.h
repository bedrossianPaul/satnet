/*
    * OLSR Hello protocol
    * 
    * Date : 24/04/2025
    * 
    * Purpose : Discover neighbors on the network
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/

#ifndef OLSR_HELLO_H
#define OLSR_HELLO_H

#include <lwip/ip_addr.h>
#include <olsropts.h>

typedef struct {
    uint32_t timestamp;
    uint8_t willingness;
    uint8_t neighbor_count;
    uint8_t neighbors[MAX_NEIGHBORS][sizeof(ip_addr_t)];
} hello_msg_t;


void init_olsr_hello(int proc_id);

#endif