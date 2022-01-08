#ifndef DEVELOPER_JOYOFENERGY_C_MOCK_BUS_MESSAGE_QUEUE
#define DEVELOPER_JOYOFENERGY_C_MOCK_BUS_MESSAGE_QUEUE
#include <stdbool.h>
#include <stdint.h>
#include "hal/message.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *(*iterator_next)(void *);

struct iterator {
    iterator_next next;
    uint32_t index;
};

struct bus_message_queue {
    unsigned int size;
    struct message *message;
    unsigned int write_index;
    unsigned int read_index;
    bool is_roll;
    struct iterator iter;
};

struct bus_message_queue *create_bus_message_queue(uint32_t size);
void destory_bus_message_queue(struct bus_message_queue *queue);
bool queue_push(struct bus_message_queue *queue, struct message *message);
bool queue_pop(struct bus_message_queue *queue, struct message *message);
void queue_clear(struct bus_message_queue *queue);
uint32_t queue_size(struct bus_message_queue *queue);
struct iterator *bus_message_queue_iterator(struct bus_message_queue *queue);
void *bus_message_queue_next(void *data);

#ifdef __cplusplus
}
#endif
#endif