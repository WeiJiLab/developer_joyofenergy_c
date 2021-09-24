#ifndef DEVELOPER_JOYOFENERGY_C_ELECTRICITY_READING_SERVICE_H
#define DEVELOPER_JOYOFENERGY_C_ELECTRICITY_READING_SERVICE_H
#include <domain/electricity_reading.h>
#include <hal/bsp.h>
#include <string.h>

struct electricity_reading_service {
  struct bsp* bsp;
  struct electricity_reading* readings;
  size_t count;
};

static inline void electricity_reading_service_init(struct electricity_reading_service* service, struct bsp* bsp,
                                                    struct electricity_reading* readings, size_t count) {
  service->bsp = bsp;
  service->readings = readings;
  service->count = count;
}

static inline size_t electricity_reading_service_get(struct electricity_reading_service* service,
                                                     struct electricity_reading* reading, size_t expected_count) {
  size_t copy_count = service->count > expected_count ? expected_count : service->count;
  if (copy_count > 0) {
    memcpy(reading, service->readings, sizeof(struct electricity_reading) * copy_count);
  }
  return service->count;
}

static inline void electricity_reading_service_store(struct electricity_reading_service* service) {
  struct clock* clock = bsp_clock(service->bsp);
  struct metrology* metrology = bsp_metrology(service->bsp);
  electricity_reading_init(&service->readings[service->count], clock_now(clock), metrology_get_power(metrology));
  ++service->count;
}

#endif  // DEVELOPER_JOYOFENERGY_C_ELECTRICITY_READING_SERVICE_H
