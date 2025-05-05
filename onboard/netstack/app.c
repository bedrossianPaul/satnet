#include <stdio.h>
#include <lwip/ip4_addr.h>
#include <network.h>
#include <olsr_hello.h>
#include <iru.h>
#include <forwarder.h>
#include <custom_type.h>
#include <unistd.h>

int main() {

    ip_addr_t ip;
    IP_ADDR4(&ip, 10, 0, 0, 1); // Set the source IP address
    
    init_forwarder(1698, UDP_T); //for forwarding packets to the lwIP stack [OLSR]
    if (init_iru(&ip) != 0) {
        printf("[ERROR] [IRU] Initialization failed\n");
        return -1;
    }

    int olsr_id = register_process_iru();
    sleep(1); // Wait for the process to be registered
    init_olsr_hello(olsr_id);


    while (1) {
        network_poll();  // poll tap interface
    }

    return 0;
}
