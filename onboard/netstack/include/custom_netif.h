/*
    * Custom Interface operations for lwIP 
    * 
    * Date : 23/04/2025
    * 
    * Purpose : Give utils to manage netif of lwIP (tap0).
    *  
    * Note : This file is used only if you have an OS, if not you have to rewrite this file.
    *        Also, you will need to manage the redirection and NAT yourself
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/

#ifndef CUSTOM_NETIF_H
#define CUSTOM_NETIF_H
#include "lwip/netif.h"
#include "netif/tapif.h"

typedef enum {
    NETIF_OK = 0,
    NETIF_ERR_BAD_IP,
    NETIF_ERR_BAD_MASK,
    NETIF_ERR_ADD_FAIL
} netif_status_t;

/*
    * Function : init_custom_netif
    * Purpose : Initialize a custom netif with the given IP address and prefix length
    * Input   : netif_out - Pointer to the netif structure to be initialized
    *         : ip_str - String representation of the IP address
    *         : prefixlen - Prefix length for the subnet mask
    * Output  : netif_status_t - Status of the initialization
    * Note    : The function will set the netif as the default netif
    *         : and will add it to the list of netifs.
*/
netif_status_t init_custom_netif(
    struct netif* netif_out, 
    const char* ip_str, 
    int prefixlen
);

#endif // CUSTOM_NETIF_H