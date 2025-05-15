/*
    * OLSR - Hello protocol
    *
    * Date : 14/05/2025
    * 
    * Purpose : Hello protocol for OLSR
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#ifndef OLSR_HELLO_H
#define OLSR_HELLO_H

#include <table.h>
#include <olsropts.h>
#include <list.h>
#include <lwip/ip_addr.h>

typedef struct {
    int proc_id;
    table_t *one_hop_neighbors;
    table_t *two_hop_neighbors;
    list_t *mprs;
} proc_args_t;

void hello_start(int proc_id, table_t *one_hop_neighbors, table_t *two_hop_neighbors, list_t *mprs);

void *send_hello(void *arg);
void *listen_hello(void *arg);
hello_msg_t *unserialize_hello_msg(const char *buff, size_t buff_size);
char *serialize_hello_msg(hello_msg_t *msg);
void free_hello_msg(hello_msg_t *msg);

#endif