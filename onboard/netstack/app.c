#include <stdio.h>
#include <lwip/ip4_addr.h>
#include <network.h>
#include <iru.h>
#include <forwarder.h>
#include <custom_type.h>
#include <unistd.h>

// TODO : memory leak :/

int main() {

    ip_addr_t ip;
    IP_ADDR4(&ip, 10, 0, 0, 1); // Set the source IP address
    
    init_forwarder(1698, UDP_T); //for forwarding packets to the lwIP stack [OLSR]
    if (init_iru(&ip) != 0) {
        printf("[ERROR] [IRU] Initialization failed\n");
        return -1;
    }



    while (1) {
        network_poll();  // poll tap interface
    }

    return 0;
}
