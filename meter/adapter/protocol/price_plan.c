#include "price_plan.h"

#include <app/service/price_plan_service.h>

#include "protocol.h"

struct message price_plan_controller_compare_all(struct price_plan_controller* controller, const struct message* request) {
  struct plan_charge charges[5];
  size_t charges_count = sizeof(charges) / sizeof(charges[0]);
  size_t actual_count = price_plan_service_compare_all(controller->service, charges, charges_count);

  struct message response;
  response.head.type = MESSAGE_PRICE_PLAN_COMPARE_ALL;
  struct price_plan_compare_all_response* result = (struct price_plan_compare_all_response*)response.payload;
  result->plans_count = actual_count;
  for (size_t i = 0; i < actual_count; ++i) {
    strcpy(result->plans[i].plan, charges[i].plan);
    result->plans[i].charge = charges[i].charge;
  }
  return response;
}

struct message price_plan_controller_recommend(struct price_plan_controller* controller, const struct message* request) {
  struct plan_charge charges[5];
  size_t charges_count = sizeof(charges) / sizeof(charges[0]);
  size_t actual_count = price_plan_service_recommend(controller->service, charges, charges_count);

  struct message response;
  response.head.type = MESSAGE_PRICE_PLAN_RECOMMEND;
  struct price_plan_recommend_response* result = (struct price_plan_recommend_response*)response.payload;
  struct price_plan_recommend_request* recommend_request = (struct price_plan_recommend_request*)request->payload;
  result->plans_count = actual_count > recommend_request->limit ? recommend_request->limit : actual_count;
  for (size_t i = 0; i < actual_count; ++i) {
    strcpy(result->plans[i].plan, charges[i].plan);
    result->plans[i].charge = charges[i].charge;
  }
  return response;
}
