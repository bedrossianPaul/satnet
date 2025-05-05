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
#include "custom_netif.h"


/* 
----------------------------------------------------------------
                        Utility functions 
----------------------------------------------------------------
*/
/*
    * Function : str_to_ip4addr
    * Purpose : Convert a string representation of an IP address to an ip4_addr_t structure
    * Input   : ip_str - String representation of the IP address 
    *         : ipaddr - Pointer to the ip4_addr_t structure to store the result
    * Output  : 0 on success, -1 on failure
    * Note    : The input string should be in the format "x.x.x.x" where x is a number between 0 and 255
*/
int str_to_ip4addr(const char* ip_str, ip4_addr_t* ipaddr)
{
    unsigned int ip1, ip2, ip3, ip4;
    if (sscanf(ip_str, "%u.%u.%u.%u", &ip1, &ip2, &ip3, &ip4) != 4)
    {
        return -1;
    }
    if (ip1 > 255 || ip2 > 255 || ip3 > 255 || ip4 > 255)
    {
        return -1;
    }
    IP4_ADDR(ipaddr, ip1, ip2, ip3, ip4);
    return 0;
}

/*
    * Function : prefixlen_to_mask
    * Purpose : Convert a prefix length to a subnet mask
    * Input   : prefixlen - Prefix length (0-32)
    *         : mask - Pointer to the ip4_addr_t structure to store the result
    * Output  : 0 on success, -1 on failure
*/
int prefixlen_to_mask(int prefixlen, ip4_addr_t* mask)
{
    if (prefixlen < 0 || prefixlen > 32)
    {
        return -1;
    }
    unsigned int mask_value = ~((1 << (32 - prefixlen)) - 1);
    IP4_ADDR(mask, (mask_value >> 24) & 0xFF, (mask_value >> 16) & 0xFF, (mask_value >> 8) & 0xFF, mask_value & 0xFF);
    return 0;
}

/* 
----------------------------------------------------------------
                        Exposed functions 
----------------------------------------------------------------
*/

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
netif_status_t init_custom_netif(struct netif* netif_out, const char* ip_str, int prefixlen) 
{
    if (!netif_out || !ip_str) return NETIF_ERR_ADD_FAIL;

    ip4_addr_t ipaddr, netmask, gateway;

    if (str_to_ip4addr(ip_str, &ipaddr) != 0)
        return NETIF_ERR_BAD_IP;

    if (str_to_ip4addr("192.168.1.1", &gateway) != 0)
        return NETIF_ERR_BAD_IP;

    if (prefixlen_to_mask(prefixlen, &netmask) != 0)
        return NETIF_ERR_BAD_MASK;

    if (!netif_add(netif_out, &ipaddr, &netmask, &gateway, NULL, tapif_init, netif_input))
        return NETIF_ERR_ADD_FAIL;

    netif_set_default(netif_out);
    netif_set_up(netif_out);
    return NETIF_OK;
}


