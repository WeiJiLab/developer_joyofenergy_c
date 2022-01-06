#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bus_message_queue.h"

struct bus_message_queue *create_bus_message_queue(uint32_t size)
{
    if (size == 0) {
        return NULL;
    }

    struct bus_message_queue *queue = (struct bus_message_queue *)malloc(sizeof(struct bus_message_queue));
    queue->message = (struct message *)malloc(size * sizeof(struct message));
    queue->read_index = 0;
    queue->write_index = 0;
    queue->is_roll = false;
    return queue;
}

bool queue_push(struct bus_message_queue *queue, struct message *message)
{
    if ((queue == NULL) || (message == NULL)) {
        return false;
    }

    if (!(queue->is_roll) && (queue->write_index >= queue->size)) {
        queue->is_roll = 1;
        queue->write_index = 0;
    }
    if ((queue->is_roll) && (queue->write_index >= queue->read_index)) {
        return false;
    }
    memcpy(&queue->message[queue->write_index++], message, sizeof(struct message));
    return true;
}

bool queue_pop(struct bus_message_queue *queue, struct message *message)
{
    if (queue == NULL) {
        return false;
    }

    if ((queue->is_roll) && (queue->read_index >= queue->size)) {
        queue->is_roll = 0;
        queue->read_index = 0;
    }
    if (!(queue->is_roll) && (queue->read_index >= queue->write_index)) {
        return false;
    }
    memcpy(message, &queue->message[queue->read_index++], sizeof(struct message));
    return true;
}

void destory_bus_message_queue(struct bus_message_queue *queue)
{
    if ((queue == NULL) || (queue->message== NULL))
        return;
    
    free(queue->message);
    free(queue);
    queue->message= NULL;
    queue = NULL;
}