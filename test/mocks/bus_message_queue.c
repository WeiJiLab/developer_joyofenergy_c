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
    queue->size = size;
    queue->iter.next = NULL;
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
    if ((queue == NULL) || (message == NULL)) {
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

uint32_t queue_size(struct bus_message_queue *queue)
{
    if (queue->is_roll) {
        return (queue->size - queue->read_index + queue->write_index);
    } else {
        return (queue->write_index - queue->read_index);
    }
}

void queue_clear(struct bus_message_queue *queue)
{
    queue->is_roll = 0;
    queue->iter.index = 0;
    queue->read_index = 0;
    queue->write_index = 0;
    memset(queue->message, 0, queue->size * sizeof(struct message));
}

void *bus_message_queue_next(void *data)
{
    uint32_t read_pos = 0;
    bool read_roll = false;
    struct bus_message_queue *queue = NULL;

    if (data == NULL) {
        return NULL;
    }
    queue = (struct bus_message_queue *)data;
    if (queue->is_roll) {
        if ((queue->read_index + queue->iter.index) >= queue->size) {
            read_pos = (queue->read_index + queue->iter.index - queue->size);
            read_roll = true;
        } else {
            read_pos = (queue->read_index + queue->iter.index);
            queue->iter.index++;
            return &queue->message[read_pos];
        }
        if ((read_roll) && (read_pos < queue->write_index)) {
            queue->iter.index++;
            return &queue->message[read_pos];
        }
        return NULL;
    } else {
        if ((queue->read_index + queue->iter.index) < queue->write_index) {
            return &queue->message[queue->read_index + queue->iter.index++];
        }
        return NULL;
    }
}

struct iterator *bus_message_queue_iterator(struct bus_message_queue *queue)
{
    queue->iter.index = 0;
    if (queue->iter.next == NULL) {
        queue->iter.next = bus_message_queue_next;
    }
    return &queue->iter;
}