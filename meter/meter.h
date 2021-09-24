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
  struct bsp* bsp;
  struct electricity_reading readings[1024];
  size_t readings_count;
  struct price_plan plans[5];
  size_t plans_count;
  struct electricity_reading_service reading_service;
  struct price_plan_service price_plan_service;
  struct schedule schedule;
  struct protocol protocol;
};

int meter_init(struct meter* meter, struct bsp* bsp);
bool meter_process(struct meter* meter);

#ifdef __cplusplus
}
#endif
#endif  // DEVELOPER_JOYOFENERGY_C_METER_H
