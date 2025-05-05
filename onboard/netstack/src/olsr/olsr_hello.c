/*
    * OLSR Hello protocol
    * 
    * Date : 24/04/2025
    * 
    * Purpose : Discover neighbors on the network
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/
#include <olsr_hello.h>
#include <olsropts.h>
#include <iru.h>
#include <lwip/sys.h>
#include <unistd.h>
#include <lwip/ip_addr.h>
#include <lwip/pbuf.h>

void* send_hello(void* arg) {

    ip4_addr_t dest_ip4;
    IP4_ADDR(&dest_ip4, 10, 0, 0, 255);
    ip_addr_t dest;
    ip_addr_copy_from_ip4(dest, dest_ip4);


    while (1) {
        hello_msg_t *msg = malloc(sizeof(hello_msg_t));
        msg->timestamp = sys_now();
    
        packet_t *packet = malloc(sizeof(packet_t));
        packet->dest_addr = &dest;
        packet->src_addr = NULL;
        packet->proto = UDP_T;
        packet->dest_port = 1698;
        packet->payload_len = sizeof(hello_msg_t);
        packet->payload = msg;
    
        iru_send(packet);
    
        // Libération
        free(msg);
        free(packet);
    
        sleep(HELLO_INTERVAL / 1000);
    }
    
    
}


void* listen_hello(void* arg) {
    int proc_id = *((int*)arg);

    while (1) { 
        packet_t *p = iru_recv(proc_id, 1698, UDP_T);
        if (p == NULL) {
            printf("[ERROR] [OLSR] Failed to receive packet: NULL pointer\n");
        } else {
            // TODO : Process the received packet 
            hello_msg_t *msg = (hello_msg_t *)p->payload;
            printf("[LOG] [OLSR] Hello packet received from %s:%d\n", ip4addr_ntoa(p->src_addr), 1698);
            printf("             - Timestamp: %u\n", msg->timestamp);
            printf("             - Willingness: %u\n", msg->willingness);
            printf("             - Neighbor count: %u\n", msg->neighbor_count);
            for (int i = 0; i < msg->neighbor_count; i++) {
                printf("             - Neighbor %d: %s\n", i, ip4addr_ntoa(&msg->neighbors[i]));
            }
            // Free the received packet
            free(p->src_addr);
            free(p->dest_addr);
            free(p->next_hop_addr);
            free(p->payload);
            free(p);
        }
        sleep(1); // Sleep for 1 second before checking again TODO : remove that
    }

    return NULL;
}

void init_olsr_hello(int proc_id) {
    
    pthread_t listen_thread;  // Le thread qui va écouter

    // Crée un thread qui exécute listen_hello
    if (pthread_create(&listen_thread, NULL, listen_hello, (void*)&proc_id) != 0) {
        printf("[ERROR] [OLSR] Failed to create listen_hello thread\n");
    }

    pthread_t send_thread;  // Le thread qui va écouter

    // Crée un thread qui exécute listen_hello
    if (pthread_create(&send_thread, NULL, send_hello, (void*)&proc_id) != 0) {
        printf("[ERROR] [OLSR] Failed to create send_hello thread\n");
    }

}


