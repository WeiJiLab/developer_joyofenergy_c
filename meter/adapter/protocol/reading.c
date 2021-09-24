#include "reading.h"
#include "protocol.h"

#include <app/service/electricity_reading_service.h>

struct message reading_read(struct readings_controller* controller, const struct message* request) {
  struct message response;
  if (request->head.type == MESSAGE_READINGS_READ) {
    struct electricity_reading readings[1024];
    size_t expected_count = sizeof(readings) / sizeof(readings[0]);
    size_t actual_count = electricity_reading_service_get(controller->service, readings, expected_count);
    response.head.type = MESSAGE_READINGS_READ;
    response.head.size = sizeof(struct message);
    struct reading_message_response* result = (struct reading_message_response*)response.payload;
    result->readings_count = actual_count;
    for (size_t i = 0; i < actual_count; ++i) {
      result->readings[i] = readings[i];
    }
  }
  return response;
}
