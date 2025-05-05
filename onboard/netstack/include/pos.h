/*
    * Pos module
    *
    * Date : 05/05/2025
    * 
    * Purpose : Thread to get position of a nanosat based on the config file (config/orbits.conf)
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#ifndef POS_H
#define POS_H

#include <lwip/ip_addr.h>
#include <stdbool.h>
#include <custom_type.h>

/*
    * Function to get the position of a nanosat based on the config file (config/orbits.conf)
    * 
    * @param ip_addr The IP address of the nanosat
    * @return the position of the nanosat.
    * 
    * 
*/
position_t* get_position(ip4_addr_t *ip_addr);

/*
    * Function to check if a packet is in the theorical range of the receiver
    * 
    * @param sender the current position of the receiver
    * @return true if the packet is in the theorical range of the current nanosat, false otherwise.
    * 
    * 
*/
bool is_in_range(position_t *sender);

#endif