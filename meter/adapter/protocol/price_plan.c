#include "price_plan.h"

#include <app/service/price_plan_service.h>

#include "protocol.h"

message price_plan_controller_compare_all(price_plan_controller* controller, const message* request) {
  struct plan_charge charges[5];
  size_t charges_count = sizeof(charges) / sizeof(charges[0]);
  size_t actual_count = price_plan_service_compare_all(controller->service, charges, charges_count);

  message response;
  response.head.type = MESSAGE_PRICE_PLAN_COMPARE_ALL;
  price_plan_compare_all_response* result = (price_plan_compare_all_response*)response.payload;
  result->plans_count = actual_count;
  for (size_t i = 0; i < actual_count; ++i) {
    strcpy(result->plans[i].plan, charges[i].plan);
    result->plans[i].charge = charges[i].charge;
  }
  return response;
}

message price_plan_controller_recommend(price_plan_controller* controller, const message* request) {
  struct plan_charge charges[5];
  size_t charges_count = sizeof(charges) / sizeof(charges[0]);
  size_t actual_count = price_plan_service_recommend(controller->service, charges, charges_count);

  message response;
  response.head.type = MESSAGE_PRICE_PLAN_RECOMMEND;
  price_plan_recommend_response* result = (price_plan_recommend_response*)response.payload;
  price_plan_recommend_request* recommend_request = (price_plan_recommend_request*)request->payload;
  result->plans_count = actual_count > recommend_request->limit ? recommend_request->limit : actual_count;
  for (size_t i = 0; i < actual_count; ++i) {
    strcpy(result->plans[i].plan, charges[i].plan);
    result->plans[i].charge = charges[i].charge;
  }
  return response;
}
