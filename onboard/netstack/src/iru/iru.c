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

#include <iru.h>
#include <udp_comm.h>
#include <forwarder.h>
#include <network.h>
#include <lwip/sys.h>
#include <lwip/timeouts.h>
#include <lwip/ip_addr.h>
#include <string.h>
#include <filter.h>
#include <lwip/pbuf.h>

static pthread_mutex_t send_mutex = PTHREAD_MUTEX_INITIALIZER;
static struct udp_pcb *send_pcb = NULL;
static proc_queue_t *proc_queue = NULL;
static ip_addr_t * src_ip = NULL;


ip4_addr_t * get_next_hop_addr(packet_t *packet) {
    // TODO : Implement the logic to get the next hop address
    return packet->dest_addr;
}

int init_iru(ip_addr_t * ip) {
    src_ip = malloc(sizeof(ip_addr_t));
    if (src_ip == NULL) {
        printf("[ERROR] [IRU] Memory allocation failed for source IP\n");
        return -1;
    }
    ip_addr_copy(*src_ip, *ip);
    proc_queue = create_proc_queue();
    if (network_init("192.168.1.2", 24) != 0) {
        printf("[ERROR] [IRU] Network initialization failed\n");
        return -1;
    }
    if (proc_queue == NULL) {
        printf("[ERROR] [IRU] Processus queue initialization failed\n");
        return -1;
    }
    send_pcb = udp_comm_init_sender();
    if (send_pcb == NULL) {
        printf("[ERROR] [IRU] Failed to initialize UDP PCB\n");
        return -1;
    }
    printf("[LOG] [IRU] IRU initialized successfully\n");
    return 0;
}

int register_process_iru() {
    int timestamp = (int)time(NULL);
    int random_part = (int)rand();
    int proc_uid = timestamp ^ random_part; 
    if (add_process(proc_queue, proc_uid) == -1) {
        printf("[ERROR] [IRU] Processus registration failed\n");
        return -1;
    };
    return proc_uid;
}

void iru_udp_callback(
    void *arg,
    struct udp_pcb *upcb,
    struct pbuf *p,
    const ip_addr_t *addr,
    u16_t port
) {
    if (!p) return;
    callback_args_t *cb_args = (callback_args_t *)arg;
    proc_queue_t *queue = cb_args->queue;
    int proc_id = cb_args->proc_id;
    int dest_port = cb_args->port;

    listener_t *listener = get_listener(queue, proc_id, port);

    if (listener == NULL) {
        printf("[ERROR] [IRU] Listener not found in callback\n");
        pbuf_free(p);
        return;
    }

    packet_t *recv_packet = deserialize_packet(p->payload, p->len);
    // Filter the packet (discard if not in range)
    packet_t *packet = filter(recv_packet);
    if (packet == NULL) {
        pbuf_free(p);
        return;
    }
    if (!ip_addr_cmp(packet->src_addr, src_ip)){
        listener->recv_packet = packet;
        udp_comm_remove(upcb);
        free(arg); // Free the callback args after use
        pthread_cond_signal(&listener->cond);
        remove_listener(queue, proc_id, dest_port);
    } else {
        free(packet->src_addr);
        free(packet->dest_addr);
        free(packet->next_hop_addr);
        free(packet->payload);
        free(packet);
    }
    pbuf_free(p);
}

packet_t *iru_recv(int proc_id, int port, conn_t protocol) {
    proc_t *proc = get_process(proc_queue, proc_id);
    if (proc == NULL) {
        printf("[ERROR] [IRU] Processus not found\n");
        return NULL;
    }
    if (add_listener(proc_queue, proc_id, port) == -1) {
        printf("[ERROR] [IRU] Listener registration failed\n");
        return NULL;
    }
    listener_t *listener = get_listener(proc_queue, proc_id, port);
    if (listener == NULL) {
        printf("[ERROR] [IRU] Listener not found\n");
        return NULL;
    }
    switch (protocol) {
        case UDP_T:
            struct udp_pcb *upcb = udp_comm_init_receiver(port);
            if (upcb == NULL) {
                printf("[ERROR] [IRU] Failed to initialize UDP PCB\n");
                return NULL;
            }
            callback_args_t *args = malloc(sizeof(callback_args_t));
            if (args == NULL) {
                printf("[ERROR] [IRU] Failed to allocate memory for callback args\n");
                return NULL;
            }
            args->queue = proc_queue;
            args->proc_id = proc_id;
            args->port = port;
            udp_comm_set_callback(upcb, iru_udp_callback, args);
            break;
        case TCP_T:
            printf("[ERROR] [IRU] TCP protocol not supported yet\n");
            break;
        default:
            printf("[ERROR] [IRU] Protocol not supported\n");
            return NULL;
    }
    
    pthread_mutex_lock(&listener->lock);
    while (listener->recv_packet == NULL) {
        pthread_cond_wait(&listener->cond, &listener->lock);
    }
    
    packet_t *packet = listener->recv_packet;
    pthread_mutex_unlock(&listener->lock);
    
    return packet;
}

int iru_send(packet_t *packet) {
    // Verrouille le mutex pour s'assurer que seul un thread à la fois exécute cette fonction
    pthread_mutex_lock(&send_mutex);

    packet->next_hop_addr = get_next_hop_addr(packet);

    if (packet->src_addr == NULL) {
        packet->src_addr = src_ip;
    }

    if (send_pcb == NULL) {
        printf("[ERROR] [IRU] Failed to initialize UDP PCB\n");
        pthread_mutex_unlock(&send_mutex);  // Déverrouille avant de quitter
        return -1;
    }

    // Adresse de destination
    ip_addr_t * dest_addr = packet->next_hop_addr;


    // Envoi du paquet
    if (udp_comm_send(send_pcb, packet, dest_addr, packet->dest_port) == -1) {
        printf("[ERROR] [IRU] Failed to send packet\n");
        pthread_mutex_unlock(&send_mutex);  // Déverrouille avant de quitter
        return -1;
    }

    // Déverrouille le mutex avant de quitter la fonction
    pthread_mutex_unlock(&send_mutex);

    return 0;
}