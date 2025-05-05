/*
    * Forward packet receive by an interface to the lwIP stack
    *
    * Date : 25/04/2025
    * 
    * Purpose : Forward packets receive by an interface to the lwIP stack
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/
#include "forwarder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void* udp_redirect(void* port_ptr) {
    int port = *(int*)port_ptr;
    free(port_ptr);
    int server_sock;
    struct sockaddr_in server_addr, dest_addr;

    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock < 0) {
        perror("[ERROR] [FRWDR] Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr("192.168.1.2");
    dest_addr.sin_port = htons(port);

    int broadcast_enable = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
        perror("[ERROR] [FRWDR] Failed to set socket broadcast option");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("[ERROR] [FRWDR] Socket binding failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        char buffer[BUFFER_SIZE];

        int bytes_received = recvfrom(server_sock, buffer, sizeof(buffer), 0,
                                      (struct sockaddr*)&client_addr, &client_len);
        if (bytes_received < 0) {
            perror("[ERROR] [FRWDR] Packet reception failed");
            continue;
        }

        
        // Check if the packet comes from the lwIP stack
        if (client_addr.sin_addr.s_addr == inet_addr("192.168.1.2")){
            // Send the packet to the all network (because it's the only possible cause of receiving our own packet)
            memset(&dest_addr, 0, sizeof(dest_addr));
            dest_addr.sin_family = AF_INET;
            dest_addr.sin_addr.s_addr = inet_addr("10.0.0.255");
            dest_addr.sin_port = htons(port);
            int bytes_sent = sendto(server_sock, buffer, bytes_received, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
            if (bytes_sent < 0) {
                perror("[ERROR] [FRWDR] Packet forwarding failed");
            }
            // Free the buffer and continue to the next iteration
            memset(buffer, 0, sizeof(buffer));
            continue;
        }

        memset(&dest_addr, 0, sizeof(dest_addr));
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_addr.s_addr = inet_addr("192.168.1.2");
        dest_addr.sin_port = htons(port);
        int bytes_sent = sendto(server_sock, buffer, bytes_received, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
        // Free the buffer and continue to the next iteration
        memset(buffer, 0, sizeof(buffer));
        if (bytes_sent < 0) {
            perror("[ERROR] [FRWDR] Packet forwarding failed");
            
        }

    }

    close(server_sock);  

    return NULL;
}


/*
    * Function  : forward_packet
    * Purpose   : Forward a packet to the lwIP stack
    * Input     : int port - Port number to bind the UDP socket
    *           : conn_t type - Connection type (UDP or TCP)
    * Output    : None
*/
int init_forwarder(int port, conn_t type) {
    int* port_ptr = malloc(sizeof(int));
    if (port_ptr == NULL) {
        perror("[ERROR] [FRWDR] Memory allocation failed");
        return -1;
    }
    *port_ptr = port;
    pthread_t listener_thread;

    if (type == UDP_T) {
        if (pthread_create(&listener_thread, NULL, udp_redirect, port_ptr) != 0) {
            perror("[ERROR] [FRWDR] Failed to create UDP listener thread");
            free(port_ptr);
            exit(EXIT_FAILURE);
        }
    } else if (type == TCP_T) {
        printf("[ERROR] [FRWDR] TCP redirection not supported yet");
        return -1;
    } else {
        printf("[ERROR] [FRWDR] Connection type not exist");
        return -1;
    }

    pthread_detach(listener_thread);
    return 0;
}
