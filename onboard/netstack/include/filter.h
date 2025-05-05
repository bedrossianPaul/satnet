/*
    * Filter module
    *
    * Date : 05/05/2025
    * 
    * Purpose : Filter module for IRU component 
    *           This module discard package that are not in the theorical range of the 
    *           current nanosat (based on the config file : netsatck/config).
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/
#ifndef FILTER_H
#define FILTER_H

#include <lwip/ip_addr.h>
#include <custom_type.h>

/*
    * Function to filter a packet based on the theorical range of the current nanosat
    * 
    * @param p the packet to filter
    * @return the filtered packet if it is in the theorical range of the current nanosat, NULL otherwise.
    * 
*/
packet_t* filter(packet_t* p);


#endif