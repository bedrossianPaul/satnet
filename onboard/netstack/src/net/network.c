/*
    * Network management for lwIP
    * 
    * Date : 24/04/2025
    * 
    * Purpose : Give utils to initialize the network of lwIP.
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/
#include "network.h"
#include <lwip/init.h>
#include <netif/tapif.h>
#include "custom_netif.h"

static struct netif tap;

/*
    * Function  : network_init
    * Purpose   : Initialize the lwIP stack and the tap interface
    * Input     : ip - String representation of the IP address
    *           : subnet - Prefix length for the subnet mask
    * Output    : 0 on success, -1 on failure
    * Note      : The function will set the tap interface as the default netif
*/
int network_init(const char *ip, int subnet) {
    lwip_init();

    netif_status_t status = init_custom_netif(&tap, ip, subnet);
    if (status != NETIF_OK) return -1;

    return 0;
}

/*
    * Function  : network_poll
    * Purpose   : Poll the tap interface for incoming packets
    * Input     : None
    * Output    : None
    * Note      : This function should be called in a loop to process incoming packets
*/
void network_poll() {
    tapif_poll(&tap);
}
