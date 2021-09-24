#ifndef DEVELOPER_JOYOFENERGY_C_HAL_ENDPOINT_H
#define DEVELOPER_JOYOFENERGY_C_HAL_ENDPOINT_H

#include <stdlib.h>

struct endpoint;

int endpoint_open(struct endpoint*);
int endpoint_close(struct endpoint*);
size_t endpoint_send(struct endpoint* endpoint, const void* data, size_t size);
size_t endpoint_receive(struct endpoint* endpoint, void* data, size_t expected_size);

#endif  // DEVELOPER_JOYOFENERGY_C_HAL_ENDPOINT_H
