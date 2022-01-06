#ifndef DEVELOPER_JOYOFENERGY_C_MOCK_BUS_MESSAGE_QUEUE
#define DEVELOPER_JOYOFENERGY_C_MOCK_BUS_MESSAGE_QUEUE
#include <stdbool.h>
#include <stdint.h>
#include "hal/message.h"

struct bus_message_queue {
    unsigned int size;
    struct message *message;
    unsigned int write_index;
    unsigned int read_index;
    bool is_roll;
};

struct bus_message_queue *create_bus_message_queue(uint32_t size);
bool queue_push(struct bus_message_queue *queue, struct message *message);
bool queue_pop(struct bus_message_queue *queue, struct message *message);
void destory_bus_message_queue(struct bus_message_queue *queue);

#endif