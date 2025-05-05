/*
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
#include <olsropts.h>

typedef struct {
    ip_addr_t * ip;
    uint8_t status;
    uint32_t last_seen;
    uint32_t willingness;
} neighbor_t;



#endif