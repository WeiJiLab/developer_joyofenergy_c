#ifndef DEVELOPER_JOYOFENERGY_C_HAL_ENDPOINT_H
#define DEVELOPER_JOYOFENERGY_C_HAL_ENDPOINT_H

#include "message.h"

#ifdef __cplusplus
extern "C" {
#endif

int slave_send_message(struct message *msg);
int slave_receive_message(struct message *msg);

#ifdef __cplusplus
}
#endif
#endif  // DEVELOPER_JOYOFENERGY_C_HAL_ENDPOINT_H
