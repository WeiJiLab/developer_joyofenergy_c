#ifndef DEVELOPER_JOYOFENERGY_C_PROTOCOL_PRICE_PLAN_H
#define DEVELOPER_JOYOFENERGY_C_PROTOCOL_PRICE_PLAN_H
#include <app/service/price_plan_service.h>

struct price_plan_controller {
  struct price_plan_service* service;
};

struct price_plan_compare_all_response {
  size_t plans_count;
  struct {
    char plan[256];
    int charge;
  } plans[5];
};

static inline void price_plan_controller_init(struct price_plan_controller* controller, struct price_plan_service* service) {
  controller->service = service;
}

struct message price_plan_controller_compare_all(struct price_plan_controller* controller, const struct message* request);

struct message price_plan_controller_recommend(struct price_plan_controller*controller, const struct message* request);

#endif  // DEVELOPER_JOYOFENERGY_C_PROTOCOL_PRICE_PLAN_H
