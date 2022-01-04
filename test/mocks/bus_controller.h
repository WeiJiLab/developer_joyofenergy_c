#ifndef DEVELOPER_JOYOFENERGY_C_MOCK_BUS_CONTROLLER_H
#define DEVELOPER_JOYOFENERGY_C_MOCK_BUS_CONTROLLER_H

#include "adapter/protocol/protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

int master_receive_message(struct message *msg);
int master_send_message(struct message *msg);

#ifdef __cplusplus
}
#endif
#endif  // DEVELOPER_JOYOFENERGY_C_MOCK_BUS_CONTROLLER_H
