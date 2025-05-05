/*
    * Process Queue 
    *
    * Purpose : Manage the processes and their listeners in IRU.
    * 
    * Date : 28/04/2025
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com> 
*/

#ifndef PROC_QUEUE_H
#define PROC_QUEUE_H

#include <pthread.h>
#include <custom_type.h>

typedef struct listener {
    int port;                  // Port for the listener
    pthread_mutex_t lock;      // Mutex for thread-safety
    pthread_cond_t cond;       // Condition variable for synchronization
    packet_t *recv_packet;     // Pointer to the received packet
    struct listener* next;     // Pointer to the next listener
} listener_t;

typedef struct proc {
    int id;                     // Process ID
    listener_t* listeners;      // List of listeners associated with the process
    struct proc* next;          // Pointer to the next process
} proc_t;

typedef struct {
    proc_t* head;              // Pointer to the first process in the queue
    pthread_mutex_t lock;      // Mutex for thread-safety
    int size;                  // Number of processes in the queue
} proc_queue_t;

/*
    * Function : create_proc_queue
    * Purpose : Initialize the process queue.
    * Parameters : None
    * Return : A pointer to the created proc_queue_t, or NULL if error.
    * Note : This function should be called before using any other proc_queue functions.
*/
proc_queue_t* create_proc_queue(void);

/*
    * Function : destroy_proc_queue
    * Purpose : Free the memory used by the process queue and its resources.
    * Parameters : proc_queue_t* queue : The queue to destroy
    * Return : None
*/
void destroy_proc_queue(proc_queue_t* queue);

/*
    * Function : add_process
    * Purpose : Add a new process to the process queue.
    * Parameters : proc_queue_t* queue : The queue to add the process to
    *             int id : The process ID
    * Return : 0 if success, -1 if error
*/
int add_process(proc_queue_t* queue, int id);

/*
    * Function : remove_process
    * Purpose : Remove a process from the queue by its ID.
    * Parameters : proc_queue_t* queue : The queue to remove the process from
    *             int id : The process ID
    * Return : 0 if success, -1 if error
*/
int remove_process(proc_queue_t* queue, int id);

/*
    * Function : add_listener
    * Purpose : Add a listener to a specific process in the queue.
    * Parameters : proc_queue_t* queue : The queue containing the process
    *             int proc_id : The ID of the process to add the listener to
    *             int port : The port to be listened on
    * Return : 0 if success, -1 if error
*/
int add_listener(proc_queue_t* queue, int proc_id, int port);

/*
    * Function : remove_listener
    * Purpose : Remove a listener from a specific process in the queue.
    * Parameters : proc_queue_t* queue : The queue containing the process
    *             int proc_id : The ID of the process to remove the listener from
    *             int port : The port of the listener to remove
    * Return : 0 if success, -1 if error
*/
int remove_listener(proc_queue_t* queue, int proc_id, int port);

/*
    * Function : get_listener
    * Purpose : Retrieve a listener associated with a specific process and port.
    * Parameters : proc_queue_t* queue : The queue containing the process
    *             int proc_id : The ID of the process
    *             int port : The port of the listener
    * Return : 0 if listener found, -1 if error
*/
listener_t *get_listener(proc_queue_t* queue, int proc_id, int port);

/*
    * Function : get_process
    * Purpose : Retrieve a process by its ID from the queue.
    * Parameters : proc_queue_t* queue : The queue containing the process
    *             int id : The ID of the process to retrieve
    * Return : Pointer to the process, or NULL if not found
*/
proc_t* get_process(proc_queue_t* queue, int id);

#endif /* PROC_QUEUE_H */
