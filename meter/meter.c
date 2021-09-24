#include "meter.h"

#include <stdlib.h>

#include "adapter/protocol/protocol.h"
#include "adapter/schedule/schedule.h"
#include "configuration.h"

static void generate(struct electricity_reading* readings, size_t number) {
  time_t now = time(NULL);
  for (size_t i = 0; i < number; ++i) {
    readings[i].at = now - (number - i) * 3 * 60;
    readings[i].power = 4000;
  }
}

int meter_init(struct meter* meter, struct bsp* bsp) {
  meter->bsp = bsp;
  meter->readings_count = 21;
  generate(meter->readings, meter->readings_count);
  meter->plans_count = price_plans(meter->plans, sizeof(meter->plans) / sizeof(meter->plans[0]));
  electricity_reading_service_init(&meter->reading_service, bsp, meter->readings, meter->readings_count);
  price_plan_service_init(&meter->price_plan_service, &meter->reading_service, meter->plans, meter->plans_count);
  schedule_init(&meter->schedule, &meter->reading_service, bsp);
  protocol_init(&meter->protocol, &meter->reading_service, &meter->price_plan_service);
  return 0;
}

bool meter_process(struct meter* meter) {
  while (!protocol_process(&meter->protocol, bsp_endpoint(meter->bsp)))
    ;
  return schedule_process(&meter->schedule);
}
