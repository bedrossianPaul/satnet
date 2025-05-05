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
    TODO make this logic 
*/



