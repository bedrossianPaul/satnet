/*
    * Process Queue 
    *
    * Purpose : Manage the processes and their listeners in IRU.
    * 
    * Date : 28/04/2025
    * Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com> 
*/
#include "proc_queue.h"
#include <stdlib.h>
#include <stdio.h>

proc_queue_t* create_proc_queue(void) {
    proc_queue_t* queue = malloc(sizeof(proc_queue_t));
    if (!queue) return NULL;

    queue->head = NULL;
    queue->size = 0;
    pthread_mutex_init(&queue->lock, NULL);

    return queue;
}

void destroy_proc_queue(proc_queue_t* queue) {
    if (!queue) return;

    pthread_mutex_lock(&queue->lock);

    proc_t* proc = queue->head;
    while (proc) {
        listener_t* listener = proc->listeners;
        while (listener) {
            listener_t* next_listener = listener->next;
            pthread_mutex_destroy(&listener->lock);
            pthread_cond_destroy(&listener->cond);
            free(listener);
            listener = next_listener;
        }
        proc_t* next_proc = proc->next;
        free(proc);
        proc = next_proc;
    }

    pthread_mutex_unlock(&queue->lock);
    pthread_mutex_destroy(&queue->lock);
    free(queue);
}

int add_process(proc_queue_t* queue, int id) {
    proc_t* new_proc = malloc(sizeof(proc_t));
    if (!new_proc) return -1;

    new_proc->id = id;
    new_proc->listeners = NULL;
    new_proc->next = NULL;

    pthread_mutex_lock(&queue->lock);
    new_proc->next = queue->head;
    queue->head = new_proc;
    queue->size++;
    pthread_mutex_unlock(&queue->lock);

    return 0;
}

int remove_process(proc_queue_t* queue, int id) {
    pthread_mutex_lock(&queue->lock);

    proc_t* prev = NULL;
    proc_t* curr = queue->head;

    while (curr) {
        if (curr->id == id) {
            if (prev) {
                prev->next = curr->next;
            } else {
                queue->head = curr->next;
            }
            listener_t* listener = curr->listeners;
            while (listener) {
                listener_t* next_listener = listener->next;
                pthread_mutex_destroy(&listener->lock);
                pthread_cond_destroy(&listener->cond);
                free(listener);
                listener = next_listener;
            }
            free(curr);
            queue->size--;
            pthread_mutex_unlock(&queue->lock);
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }

    pthread_mutex_unlock(&queue->lock);
    return -1; 
}

int add_listener(proc_queue_t* queue, int proc_id, int port) {
    proc_t* proc = get_process(queue, proc_id);
    if (!proc) return -1;

    listener_t* new_listener = malloc(sizeof(listener_t));
    if (!new_listener) return -1;

    new_listener->port = port;
    new_listener->recv_packet = NULL;
    new_listener->next = proc->listeners;
    pthread_mutex_init(&new_listener->lock, NULL);
    pthread_cond_init(&new_listener->cond, NULL);

    proc->listeners = new_listener;
    return 0;
}

int remove_listener(proc_queue_t* queue, int proc_id, int port) {
    proc_t* proc = get_process(queue, proc_id);
    if (!proc) return -1;

    listener_t* prev = NULL;
    listener_t* curr = proc->listeners;

    while (curr) {
        if (curr->port == port) {
            if (prev) {
                prev->next = curr->next;
            } else {
                proc->listeners = curr->next;
            }
            pthread_mutex_destroy(&curr->lock);
            pthread_cond_destroy(&curr->cond);
            free(curr);
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }

    return -1; 
}


listener_t* get_listener(proc_queue_t* queue, int proc_id, int port) {
    proc_t* proc = get_process(queue, proc_id);
    if (!proc) return NULL;

    listener_t* listener = proc->listeners;
    while (listener) {
        if (listener->port == port) {
            return listener;
        }
        listener = listener->next;
    }

    return NULL;
}

proc_t* get_process(proc_queue_t* queue, int id) {
    pthread_mutex_lock(&queue->lock);

    proc_t* curr = queue->head;
    while (curr) {
        if (curr->id == id) {
            pthread_mutex_unlock(&queue->lock);
            return curr;
        }
        curr = curr->next;
    }

    pthread_mutex_unlock(&queue->lock);
    return NULL;
}
