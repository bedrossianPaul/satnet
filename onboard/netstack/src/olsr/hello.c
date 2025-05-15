/*
    * OLSR - Hello protocol
    *
    * Date : 14/05/2025
    * 
    * Purpose : Hello protocol for OLSR
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#include <hello.h>
#include <lwip/ip_addr.h>
#include <iru.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void hello_start(int proc_id, table_t *one_hop_neighbors, table_t *two_hop_neighbors, list_t *mprs) {
    pthread_t listen_thread, send_thread;
    proc_args_t *args = malloc(sizeof(proc_args_t));
    args->proc_id = proc_id;
    args->one_hop_neighbors = one_hop_neighbors;
    args->two_hop_neighbors = two_hop_neighbors;
    args->mprs = mprs;

    if (pthread_create(&listen_thread, NULL, listen_hello, (void*)&proc_id) != 0) {
        printf("[ERROR] [OLSR] Failed to create listen_hello thread\n");
    }


    if (pthread_create(&send_thread, NULL, send_hello, (void*)&proc_id) != 0) {
        printf("[ERROR] [OLSR] Failed to create send_hello thread\n");
    }
}

void *send_hello(void *arg) {
    proc_args_t *args = (proc_args_t *)arg;
    ip4_addr_t dest_ip4;
    IP4_ADDR(&dest_ip4, 10, 0, 0, 255);
    ip_addr_t dest;
    ip_addr_copy_from_ip4(dest, dest_ip4);


    while (1) {
        hello_msg_t *msg = malloc(sizeof(hello_msg_t));
        msg->willingness = WILLINGNESS_DEFAULT;
        msg->neighbor_count = args->one_hop_neighbors->size;
        msg->neighbors= malloc(msg->neighbor_count * sizeof(ip_addr_t));

        for (int i = 0; i < msg->neighbor_count; i++) {
                int ip_raw = args->one_hop_neighbors->entries[i].key;
                ip_addr_t addr;
                IP_SET_TYPE_VAL(addr, IPADDR_TYPE_V4);
                addr.addr = (u32_t) ip_raw;
                msg->neighbors[i] = addr;
        }
        msg->mprs_count = args->mprs->size;
        msg->neighbors = malloc(msg->mprs_count * sizeof(ip_addr_t));
        for (int i =0; i < args->mprs->size; i++) {
            int ip_raw = *(int *) list_get(args->mprs, i);
            ip_addr_t addr;
            IP_SET_TYPE_VAL(addr, IPADDR_TYPE_V4);
            addr.addr = (u32_t) ip_raw;
            msg->mprs[i] = addr;
        }
        
        packet_t *packet = malloc(sizeof(packet_t));
        packet->dest_addr = &dest;
        packet->src_addr = NULL;
        packet->proto = UDP_T;
        packet->dest_port = 1698;
        packet->payload_len = sizeof(hello_msg_t);
        packet->payload = serialize_hello_msg(msg);
    
        iru_send(packet);
    
        // Libération
        free_hello_msg(msg);
        free(packet);
    
        sleep(HELLO_INTERVAL / 1000);
    }
}

void *listen_hello(void *arg) {
    proc_args_t *args = (proc_args_t *)arg;
    while (1) { 
        packet_t *p = iru_recv(args->proc_id, 1698, UDP_T);
        if (p == NULL) {
            printf("[ERROR] [OLSR] Failed to receive packet: NULL pointer\n");
        } else {
            hello_msg_t *msg = unserialize_hello_msg(p->payload, p->payload_len);
            if (msg == NULL) {
                printf("[ERROR] [OLSR] Failed to receive packet: NULL payload\n");
            } else {
                int id = (int) ip4_addr_get_u32(p->src_addr);
                olsr_one_hop_neighbor_t new_neighbor;
                new_neighbor.last_seen = time(NULL);
                new_neighbor.willingness = msg->willingness;
                if (table_contains(args->one_hop_neighbors, id)) {
                    table_update(args->one_hop_neighbors, id, &new_neighbor);
                } else {
                    table_insert(args->one_hop_neighbors, id, &new_neighbor);
                }
                // TODO: Update the two hop neighbors and we are good to go
                // TODO: Update the mprs_selectors
            }
            free_hello_msg(msg);
            free(p->payload);
            free(p);
        }
        sleep(1); // TODO: remove that
    }
}

hello_msg_t *unserialize_hello_msg(const char *buff, size_t buff_size) {
    if (buff == NULL || buff_size <= (2*sizeof(int))) {
        printf("[ERROR] [OLSR] Invalid buffer or size\n");
        return NULL;
    }
    hello_msg_t *msg = malloc(sizeof(hello_msg_t));
    if (msg == NULL) {
        printf("[ERROR] [OLSR] Memory allocation failed\n");
        return NULL;
    }
    char *ptr = buff;
    memcpy(&msg->neighbor_count, ptr, sizeof(int));
    ptr += sizeof(int);
    if (msg->neighbor_count > 0) {
        msg->neighbors = malloc(msg->neighbor_count * sizeof(ip_addr_t));
        if (msg->neighbors == NULL) {
            printf("[ERROR] [OLSR] Memory allocation failed\n");
            free(msg);
            return NULL;
        }
        for (int i = 0; i < msg->neighbor_count; i++) {
            memcpy(&msg->neighbors[i], ptr, sizeof(ip_addr_t));
            ptr += sizeof(ip_addr_t);
        }
    }
    memcpy(&msg->mprs_count, ptr, sizeof(int));
    ptr += sizeof(int);
    if (msg->mprs_count > 0) {
        msg->mprs = malloc(msg->mprs_count * sizeof(ip_addr_t));
        if (msg->mprs == NULL) {
            printf("[ERROR] [OLSR] Memory allocation failed\n");
            free(msg->neighbors);
            free(msg);
            return NULL;
        }
        for (int i = 0; i < msg->mprs_count; i++) {
            memcpy(&msg->mprs[i], ptr, sizeof(ip_addr_t));
            ptr += sizeof(ip_addr_t);
        }
    }
    memcpy(&msg->willingness, ptr, sizeof(int));
    return msg;
}

char *serialize_hello_msg(hello_msg_t *msg) {
    if (msg == NULL) {
        printf("[ERROR] [OLSR] Invalid hello message\n");
        return NULL;
    }
    size_t size = msg->neighbor_count * sizeof(ip_addr_t) + msg->mprs_count * sizeof(ip_addr_t) + 3 * sizeof(int);
    char *buffer = malloc(size);
    if (buffer == NULL) {
        printf("[ERROR] [OLSR] Memory allocation failed\n");
        return NULL;
    }
    char *ptr = buffer;
    memcpy(ptr, &msg->neighbor_count, sizeof(int));
    ptr += sizeof(int);
    for (int i = 0; i < msg->neighbor_count; i++) {
        memcpy(ptr, &msg->neighbors[i], sizeof(ip_addr_t));
        ptr += sizeof(ip_addr_t);
    }
    memcpy(ptr, &msg->mprs_count, sizeof(int));
    ptr += sizeof(int);
    for (int i = 0; i < msg->mprs_count; i++) {
        memcpy(ptr, &msg->mprs[i], sizeof(ip_addr_t));
        ptr += sizeof(ip_addr_t);
    }
    memcpy(ptr, &msg->willingness, sizeof(int));
    return buffer;
}

void free_hello_msg(hello_msg_t *msg) {
    if (msg != NULL) {
        if (msg->neighbors != NULL)
            free(msg->neighbors);
        free(msg);
    }
}
