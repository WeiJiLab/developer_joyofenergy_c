#ifndef DEVELOPER_JOYOFENERGY_C_PROTOCOL_PRICE_PLAN_H
#define DEVELOPER_JOYOFENERGY_C_PROTOCOL_PRICE_PLAN_H
#include <app/service/price_plan_service.h>
#include "message.h"

typedef struct {
  struct price_plan_service* service;
} price_plan_controller;

typedef struct price_plan_compare_all_response {
  uint32_t plans_count;
  struct {
    char plan[256];
    uint32_t charge;
  } plans[5];
} price_plan_compare_all_response;

typedef struct price_plan_recommend_request {
  uint32_t limit;
} price_plan_recommend_request;

typedef struct price_plan_recommend_response {
  uint32_t plans_count;
  struct {
    char plan[256];
    uint32_t charge;
  } plans[5];
} price_plan_recommend_response;

static inline void price_plan_controller_init(price_plan_controller* controller, struct price_plan_service* service) {
  controller->service = service;
}

message price_plan_controller_compare_all(price_plan_controller* controller, const message* request);

message price_plan_controller_recommend(price_plan_controller* controller, const message* request);

#endif  // DEVELOPER_JOYOFENERGY_C_PROTOCOL_PRICE_PLAN_H
