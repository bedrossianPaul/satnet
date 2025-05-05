/*
    * Forward packet receive by an interface to the lwIP stack
    *
    * Date : 25/04/2025
    * 
    * Purpose : Forward packets receive by an interface to the lwIP stack
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/
#ifndef FORWARDER_H
#define FORWARDER_H

#include <custom_type.h>

#define BUFFER_SIZE 1024



int init_forwarder(int port, conn_t type);

#endif