/*
    * OLSR - Optimized Link State Routing Protocol
    *
    * Date : 14/05/2025
    * 
    * Purpose : Routing protocol for mobile ad-hoc networks
    * 
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>
*/

#include <iru.h>
#include <table.h>
#include <list.h>
#include <olsr.h>
#include <hello.h>
#include <olsropts.h>
#include <stdbool.h>

bool is_MPR;

void olsr_init() {
    is_MPR = false; // For the moment, we are not MPR

    // Instanciate the data structures
    table_t *one_hop_neighbors = table_new(sizeof(olsr_one_hop_neighbor_t));
    table_t *two_hop_neighbors = table_new(sizeof(olsr_two_hop_neighbors_t));

    list_t *mprs = list_new(sizeof(int)); // Current node's MPRs
    list_t *mprs_selectors = list_new(sizeof(int)); // Node that selected the current node as MPR

    // Launch the OLSR process
    int proc_hello_id = register_process_iru();
    if (proc_hello_id < 0) {
        printf("[ERROR] [OLSR] Failed to register OLSR process\n");
        return;
    }
    hello_start(proc_hello_id, one_hop_neighbors, two_hop_neighbors, mprs); // Start the hello process

    // TODO: start a thread to select the MPRs

    // int proc_tc_id = register_process_iru();
    // if (proc_tc_id < 0) {
    //     printf("[ERROR] [OLSR] Failed to register OLSR process\n");
    //     return;
    // }
    // tc_start(proc_tc_id); // Start the TC process

}

// TODO: penser a free les two_hop infos