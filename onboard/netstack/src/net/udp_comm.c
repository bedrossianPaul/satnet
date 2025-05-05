/*
    * UDP interface to init, send and receive UDP packets
    * 
    * Date : 24/04/2025
    * 
    * Purpose : Aims to init, send and receive UDP packets with lwIP.
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/
#include "udp_comm.h"
#include <string.h>
#include <stdio.h>

char* serialize_packet(const packet_t *pkt, size_t *out_size) {
    if (!pkt || 
        !pkt->src_addr || 
        !pkt->dest_addr || 
        !pkt->next_hop_addr || 
        !pkt->payload) {
        printf("[ERROR] [UDP_COMM] One or more fields in packet are NULL\n");
        return NULL;
    }
    
    size_t total_size = 0;
    char *buffer;
    
    // Calcul de la taille totale
    total_size += sizeof(ip4_addr_t); // src_addr
    total_size += sizeof(ip4_addr_t); // dest_addr
    total_size += sizeof(ip4_addr_t); // next_hop_addr
    total_size += sizeof(conn_t);     // proto
    total_size += sizeof(int);        // dest_port
    total_size += sizeof(size_t);     // payload_len
    total_size += pkt->payload_len;   // payload
    
    // Allocation de mémoire pour le buffer
    buffer = (char*)malloc(total_size);
    if (!buffer) {
        perror("[ERROR] [UDP_COMM]  Memory allocation failed");
        return NULL;
    }
    
    // Sérialisation
    char *ptr = buffer;
    
    // Copie de src_addr
    memcpy(ptr, pkt->src_addr, sizeof(ip4_addr_t));
    ptr += sizeof(ip4_addr_t);
    
    // Copie de dest_addr
    memcpy(ptr, pkt->dest_addr, sizeof(ip4_addr_t));
    ptr += sizeof(ip4_addr_t);
    
    // Copie de next_hop_addr
    memcpy(ptr, pkt->next_hop_addr, sizeof(ip4_addr_t));
    ptr += sizeof(ip4_addr_t);
    
    // Copie de proto
    memcpy(ptr, &pkt->proto, sizeof(conn_t));
    ptr += sizeof(conn_t);
    
    // Copie de dest_port
    memcpy(ptr, &pkt->dest_port, sizeof(int));
    ptr += sizeof(int);
    
    // Copie de payload_len
    memcpy(ptr, &pkt->payload_len, sizeof(size_t));
    ptr += sizeof(size_t);
    
    // Copie du payload
    memcpy(ptr, pkt->payload, pkt->payload_len);
    
    *out_size = total_size; 
    return buffer;
}

packet_t* deserialize_packet(const char *buffer, size_t buf_size) {
    if (!buffer || buf_size < sizeof(ip4_addr_t) * 3 + sizeof(conn_t) + sizeof(int) + sizeof(size_t)) {
        printf("[ERROR] [UDP_COMM] Buffer too small or NULL\n");
        return NULL;
    }

    const char *ptr = buffer;
    packet_t *pkt = malloc(sizeof(packet_t));
    if (!pkt) {
        printf("[ERROR] [UDP_COMM] Failed to allocate packet");
        return NULL;
    }

    // Allocation et copie des adresses IP
    pkt->src_addr = malloc(sizeof(ip4_addr_t));
    pkt->dest_addr = malloc(sizeof(ip4_addr_t));
    pkt->next_hop_addr = malloc(sizeof(ip4_addr_t));
    if (!pkt->src_addr || !pkt->dest_addr || !pkt->next_hop_addr) {
        printf("[ERROR] [UDP_COMM] Failed to allocate memory for IP addresses\n");
        free(pkt->src_addr); free(pkt->dest_addr); free(pkt->next_hop_addr); free(pkt);
        return NULL;
    }

    memcpy(pkt->src_addr, ptr, sizeof(ip4_addr_t));
    ptr += sizeof(ip4_addr_t);

    memcpy(pkt->dest_addr, ptr, sizeof(ip4_addr_t));
    ptr += sizeof(ip4_addr_t);

    memcpy(pkt->next_hop_addr, ptr, sizeof(ip4_addr_t));
    ptr += sizeof(ip4_addr_t);

    memcpy(&pkt->proto, ptr, sizeof(conn_t));
    ptr += sizeof(conn_t);

    memcpy(&pkt->dest_port, ptr, sizeof(int));
    ptr += sizeof(int);

    memcpy(&pkt->payload_len, ptr, sizeof(size_t));
    ptr += sizeof(size_t);

    if ((size_t)(ptr - buffer + pkt->payload_len) > buf_size) {
        printf("[ERROR] [UDP_COMM] Payload length exceeds buffer size\n");
        free(pkt->src_addr); free(pkt->dest_addr); free(pkt->next_hop_addr); free(pkt);
        return NULL;
    }

    pkt->payload = malloc(pkt->payload_len);
    if (!pkt->payload) {
        perror("[ERROR] [UDP_COMM] Failed to allocate memory for payload");
        free(pkt->src_addr); free(pkt->dest_addr); free(pkt->next_hop_addr); free(pkt);
        return NULL;
    }

    memcpy(pkt->payload, ptr, pkt->payload_len);

    return pkt;
}


static void udp_default_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip4_addr_t *addr, u16_t port) {
    printf("[WARNING] [UDP_COMM] Packet received from %s:%d — %s | No callback defined: Please define a callback\n", ip4addr_ntoa((const ip_addr_t *)addr), port, (char *)p->payload);
    pbuf_free(p);
}

struct udp_pcb *udp_comm_init_receiver(int port) {
    struct udp_pcb *upcb = udp_new();
    if (!upcb) return NULL;

    ip_set_option(upcb, SOF_BROADCAST);

    if (udp_bind(upcb, IP_ADDR_ANY, port) != ERR_OK) return NULL;

    udp_recv(upcb, udp_default_callback, NULL);
    return upcb;
}

struct udp_pcb *udp_comm_init_sender() {
    struct udp_pcb *upcb = udp_new();
    if (!upcb) return NULL;
    ip_set_option(upcb, SOF_BROADCAST);
    udp_bind(upcb, IP_ADDR_ANY, 0);
    return upcb;
}

int udp_comm_send(struct udp_pcb *upcb, packet_t * pkt, const ip_addr_t *addr, uint16_t port) {
    size_t buf_size;
    char *buf = serialize_packet(pkt, &buf_size);
    if (!buf || buf_size == 0) {
        printf("[ERROR] [UDP] Serialization returned empty buffer\n");
        return -1;
    }
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, buf_size, PBUF_RAM);
    if (!p) return -1;
    memcpy(p->payload, buf, buf_size);
    err_t err = udp_sendto(upcb, p, (const ip_addr_t *)addr, port);
    if (err != ERR_OK) {
        printf("[ERROR] UDP send failed with error code: %d\n", err);
        pbuf_free(p);
        return -1;
    }
    pbuf_free(p);
    return 0;
    
}

void udp_comm_set_callback(struct udp_pcb *upcb, udp_recv_fn callback, void *arg) {
    if (!upcb) {
        printf("[ERROR] UDP PCB is NULL, cannot set callback\n");
        return;
    }
    udp_recv(upcb, callback, arg);
}

void udp_comm_remove(struct udp_pcb *upcb) {
    if (upcb) {
        udp_remove(upcb);  // Frees the PCB and associated resources
    } else {
        printf("[WARNING] Attempted to remove a NULL UDP PCB\n");
    }
}