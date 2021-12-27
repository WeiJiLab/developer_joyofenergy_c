#ifndef DEVELOPER_JOYOFENERGY_C_READING_H
#define DEVELOPER_JOYOFENERGY_C_READING_H

#include <hal/endpoint.h>
#include <domain/electricity_reading.h>
#include <configuration.h>

struct readings_controller {
  struct electricity_reading_service* service;
};

static inline void readings_controller_init(struct readings_controller* controller,
                                            struct electricity_reading_service* service) {
  controller->service = service;
}

struct reading_message_response {
  uint32_t readings_count;
  struct electricity_reading readings[MAX_MESSAGE_READING_PAYLOAD_COUNT];
};

struct message reading_read(struct readings_controller* controller, const struct message* request);

#endif  // DEVELOPER_JOYOFENERGY_C_READING_H
