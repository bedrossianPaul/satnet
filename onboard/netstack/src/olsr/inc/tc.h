/*
    * TC - Topology Control
    *
    * Date : 06/05/2025
    * 
    * Purpose : Select MPR (Multi Point Relay) nodes and send TC messages to the network
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#ifndef TC_H
#define TC_H
#include <olsropts.h>
#include <custom_type.h>
#include <ip_addr.h>

typedef struct {
    ip_addr_t originator_ip;
    uint16_t seq_number;
    uint16_t advertised_count;
    ip_addr_t advertised_neighbors[MAX_MPRS];
} tc_message_t;

/*
    * Function: generate_tc_message
    * 
    * Purpose: Generate a TC message with the given originator IP and MPRs
    * 
    * Parameters:
    *   - originator_ip: The IP address of the originator
    *   - mprs: Array of MPRs to be included in the TC message
    *   - count: Number of MPRs in the array
    * 
    * Returns:
    *   - A tc_message_t structure containing the generated TC message
*/
packet_t send_tc(ip_addr_t originator_ip, ip_addr_t *mprs, int count);

//TODO : make this function
void select_mprs();

/*
    * Function: listen_tc
    * 
    * Purpose: Listen for incoming TC messages and process them
    * 
    * Parameters:
    *   - arg: Pointer to the argument passed to the thread
    * 
    * Returns:
    *   - None
*/
void listen_tc(void* arg);

#endif