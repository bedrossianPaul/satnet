/*
    * Filter module
    *
    * Date : 05/05/2025
    * 
    * Purpose : Filter module for IRU component 
    *           This module discard package that are not in the theorical range of the 
    *           current nanosat (based on the config file : netsatck/config/filter.conf).
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#include <filter.h>
#include <pos.h>


packet_t* filter(packet_t* p) {
    // Check if the packet is NULL
    if (p == NULL) {
        return NULL;
    }
    // Get the source address of the packet
    ip4_addr_t* src_addr = p->src_addr;
    // Check if the source address is NULL
    if (src_addr == NULL) {
        return NULL;
    }
    // Check if the source address is in the theorical range of the current nanosat
    position_t * sender = get_position(src_addr);
    if (sender == NULL) {
        return NULL;
    }
    if (!is_in_range(sender)) {
        free(sender);
        return NULL;
    }

    // Free the sender position
    free(sender);

    return p;
}