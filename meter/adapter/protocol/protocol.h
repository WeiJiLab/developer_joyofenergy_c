#ifndef DEVELOPER_JOYOFENERGY_C_PROTOCOL_H
#define DEVELOPER_JOYOFENERGY_C_PROTOCOL_H

#include <app/service/electricity_reading_service.h>
#include <stdbool.h>
#include <configuration.h>
#include <hal/message.h>
#include <hal/endpoint.h>
#include "price_plan.h"

struct protocol {
  struct electricity_reading_service* reading_service;
  struct price_plan_service* plan_service;
};

static inline void protocol_init(struct protocol* router, struct electricity_reading_service* reading_service,
                                 struct price_plan_service* plan_service) {
  router->reading_service = reading_service;
  router->plan_service = plan_service;
}

bool protocol_process(struct protocol* protocol);
#endif  // DEVELOPER_JOYOFENERGY_C_PROTOCOL_H
