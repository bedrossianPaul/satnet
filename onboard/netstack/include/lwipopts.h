/*
    * lwIP Options Header File 
    *
    * Date : 23/04/2025
    *
    * Purpose : override et configure lwIP options
    * The list of all option is available in lwip/src/include/lwip/opts.h
    * In our case, we want to enable IPv4, UDP, TCP and use the raw style API
    * 
    * Note : UDP is enabled by default so don't need to be specified 
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#ifndef LWIP_LWIPOPTS_H
#define LWIP_LWIPOPTS_H

#define NO_SYS       1 // Raw API
#define LWIP_SOCKET  0 // No BSD socket API
#define LWIP_NETCONN 0 // No netconn API

#define MEMP_NUM_SYS_TIMEOUT 10 // Number of sys timeouts (it took it large)

#define LWIP_IPV4 1 // Enable IPv4
#define LWIP_IPV6 0 // Enable IPv4
#define LWIP_UDP 1 // Enable UDP
#define LWIP_ARP 1 // Enable ARP
#define DEVTAP_IF "tap0" // Default tap interface name
#define SSIZE_MAX 1 // ssize_t type already defined (you may change that if you don't have it on your system)

#define IP_SOF_BROADCAST        1
#define IP_SOF_BROADCAST_RECV   1
#define UDP_FLAGS_BROADCAST 0x04


#define LWIP_NETIF_STATUS_CALLBACK  1 // Enable netif status callback
#define LWIP_NETIF_LINK_CALLBACK    1 // Enable netif link callback

#endif /* LWIP_LWIPOPTS_H */