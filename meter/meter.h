#ifndef DEVELOPER_JOYOFENERGY_C_METER_H
#define DEVELOPER_JOYOFENERGY_C_METER_H

#include <stdbool.h>

#include "adapter/schedule/schedule.h"
#include "adapter/protocol/protocol.h"
#include "app/service/electricity_reading_service.h"
#include "hal/bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

struct meter {
  char meter_id[METER_ID_LEN];
  struct bsp *bsp;
  struct electricity_reading readings[MAX_MESSAGE_READING_PAYLOAD_COUNT];
  size_t readings_count;
  price_plan plans[5];
  size_t plans_count;
  struct electricity_reading_service reading_service;
  struct price_plan_service price_plan_service;
  struct schedule schedule;
  struct protocol protocol;
};

struct meter *create_meter(const char *meter_id, struct bsp *bsp);
void destroy_meter(const char *meter_id);

void device_interrupt_proc(char *meter_id);

#ifdef __cplusplus
}
#endif
#endif  // DEVELOPER_JOYOFENERGY_C_METER_H
