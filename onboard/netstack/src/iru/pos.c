/*
    * Pos module
    *
    * Date : 05/05/2025
    * 
    * Purpose : Thread to get position of a nanosat based on the config file (config/orbits.conf)
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#include <pos.h>

/*
    * Function to set the receiver position (must be threaded)
    * 
    * @param pos the position of the receiver
    * 
    * 
*/
position_t * get_receiver() {
    position_t *pos = malloc(sizeof(position_t));
    pos->x = 0;
    pos->y = 0;
    pos->z = 0;
    return pos;
}

double get_range() {
    // TODO : Implement the function to get the range of the nanosat based on the config file (config/range.conf)
    // For the moment, it will just return a static range
    return 10;
}

position_t* get_position(ip4_addr_t *ip_addr) {
    // TODO : Implement the function to get the position of a nanosat based on the config file (config/orbits.conf)
    // For the moment, it will just return a static position
    position_t *pos = malloc(sizeof(position_t));
    pos->x = 0;
    pos->y = 0;
    pos->z = 0;
    return pos;
}

bool is_in_range(position_t *sender) {
    position_t * receiver = get_receiver();
    double d_x = sender->x - receiver->x;
    double d_y = sender->y - receiver->y;
    double d_z = sender->z - receiver->z;
    double distance = sqrt(d_x*d_x + d_y*d_y + d_z*d_z);
    return distance <= get_range();
}

