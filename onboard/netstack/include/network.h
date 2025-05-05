/*
    * Network management for lwIP
    * 
    * Date : 24/04/2025
    * 
    * Purpose : Give utils to initialize the network of lwIP.
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com
*/

#ifndef NETWORK_H
#define NETWORK_H

/*
    * Function  : network_init
    * Purpose   : Initialize the lwIP stack and the tap interface
    * Input     : ip - String representation of the IP address
    *           : subnet - Prefix length for the subnet mask
    * Output    : 0 on success, -1 on failure
    * Note      : The function will set the tap interface as the default netif
*/
int network_init(const char *ip, int subnet);

/*
    * Function  : network_poll
    * Purpose   : Poll the tap interface for incoming packets
    * Input     : None
    * Output    : None
    * Note      : This function should be called in a loop to process incoming packets
*/
void network_poll();

#endif