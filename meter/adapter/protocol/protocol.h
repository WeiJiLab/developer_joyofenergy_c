//
// Created by Lijun Lin on 2021/9/1.
//

#ifndef DEVELOPER_JOYOFENERGY_C_PROTOCOL_H
#define DEVELOPER_JOYOFENERGY_C_PROTOCOL_H

#include <app/service/electricity_reading_service.h>
#include <hal/endpoint.h>
#include <stdbool.h>

#include "price_plan.h"

enum message_type {
  MESSAGE_INVALID = 0,
  MESSAGE_READINGS_READ,
  MESSAGE_PRICE_PLAN_COMPARE_ALL,
  MESSAGE_PRICE_PLAN_RECOMMEND,
};

struct message_head {
  size_t size;
  enum message_type type;
  char payload[0];
};

struct message {
  struct message_head head;
  char payload[1024];
};

struct protocol {
  struct electricity_reading_service* reading_service;
  struct price_plan_service* plan_service;
};

static inline void protocol_init(struct protocol* router, struct electricity_reading_service* reading_service,
                                 struct price_plan_service* plan_service) {
  router->reading_service = reading_service;
  router->plan_service = plan_service;
}

bool protocol_process(struct protocol* protocol, struct endpoint* endpoint);
#endif  // DEVELOPER_JOYOFENERGY_C_PROTOCOL_H
