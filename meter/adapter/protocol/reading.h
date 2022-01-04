#ifndef DEVELOPER_JOYOFENERGY_C_READING_H
#define DEVELOPER_JOYOFENERGY_C_READING_H

#include <domain/electricity_reading.h>
#include <configuration.h>
#include "adapter/protocol/protocol.h"

typedef struct {
  struct electricity_reading_service* service;
} readings_controller;

static inline void readings_controller_init(readings_controller* controller,
                                            struct electricity_reading_service* service) {
  controller->service = service;
}

struct reading_message_response {
  uint32_t readings_count;
  struct electricity_reading readings[MAX_MESSAGE_READING_PAYLOAD_COUNT];
};

message reading_read(readings_controller* controller, const message* request);

#endif  // DEVELOPER_JOYOFENERGY_C_READING_H
