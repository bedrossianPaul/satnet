/*
    * This is the IRU (Interal Router Unit).

    * Purpose : This component make the interface between the IRU and the UDP comm and TCP comm module.
    * It is also used to route packets between the different modules and nanosats.
    * With this component we can use lwIP with threads.
    * 
    * Date : 28/04/2025
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#ifndef IRU_H
#define IRU_H

#include <custom_type.h>
#include <proc_queue.h>
#include <lwip/ip_addr.h>
#include <pthread.h>

typedef struct {
    proc_queue_t *queue;
    int port;
    int proc_id;
} callback_args_t;

/*
    * Function : init_iru
    * Purpose : Initialize the IRU component.
    * Parameters : None
    * Return : 0 if success, -1 if error
    * Note : This function should be called before using any other IRU functions.
*/
int init_iru(ip_addr_t * ip);

/*
    * Function : register_process_iru
    * Purpose : Register a process to the IRU.
    * Parameters : iru_process_t *proc : The process to register
    * Return : process id.
    * Note : The pocess will need is ID to send and receive packets.
    *        The ID is generated by the IRU and is unique for each process.
    *        A negative return value means error while adding the process.
*/
int register_process_iru();

/*
    * Function : iru_send
    * Purpose : Send a packet to the IRU.
    * Parameters : char *data : The data to send
    *              int size : The size of the data
    * Return : 0 if success, -1 if error
*/
int iru_send(packet_t *packet);

/*
    * Function : iru_recv
    * Purpose : Receive a packet from the IRU .
    * Parameters : char *data : The data to receive
    *              int size : The size of the data
    * Return : 0 if success, -1 if error
*/
packet_t *iru_recv(int proc_id, int port, conn_t conn_type);

#endif