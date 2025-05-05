/*
    * UDP interface to init, send and receive UDP packets
    * 
    * Date : 24/04/2025
    * 
    * Purpose : Aims to init, send and receive UDP packets with lwIP.
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/
#ifndef UDP_COMM_H
#define UDP_COMM_H

#include <lwip/ip4_addr.h>
#include <custom_type.h>
#include <lwip/udp.h>


/*
    * Function  : udp_comm_init
    * Purpose   : Initialize the UDP communication
    * Input     : port - Port number to bind the UDP socket
    * Output    : 0 on success, -1 on failure
*/
struct udp_pcb *udp_comm_init_receiver(int port);


/*
    * Function  : udp_comm_init_sender
    * Purpose   : Initialize the UDP sender
    * Output    : Pointer to the initialized UDP PCB, NULL on failure
*/
struct udp_pcb *udp_comm_init_sender();

/*
    * Function  : udp_comm_send
    * Purpose   : Send a UDP packet
    * Input     : data - Data to send
    *           : addr - Destination IP address
    *           : port - Destination port number
    * Output    : 0 on success, -1 on failure
*/
int udp_comm_send(struct udp_pcb *upcb, packet_t * pkt, const ip_addr_t *addr, uint16_t port);

/*
    * Function  : udp_comm_set_callback
    * Purpose   : Set the callback function for incoming UDP packets
    * Input     : callback - Function to call when a packet is received
    * Output    : None
    * Note      : The callback function should match the signature of udp_recv_fn
*/
void udp_comm_set_callback(struct udp_pcb *upcb, udp_recv_fn callback, void *arg);

/*
    * Function to remove and free a UDP PCB. 
    * Parameters :
    *   - upcb : The UDP PCB to remove.
    * Returns :
    *   - None.
    * Purpose :
    *   This function frees the resources associated with a UDP PCB and removes it from the system.
*/
void udp_comm_remove(struct udp_pcb *upcb);

packet_t* deserialize_packet(const char *buffer, size_t buf_size);

#endif
