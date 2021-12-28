#include "protocol.h"

#include "reading.h"

message default_handle(struct protocol* controller, const message* request) {
  message message = {0};
  return message;
}

typedef message (*handle_func)(void* controller, const message* request);

static message protocol_handle(struct protocol* router, const message* request) {
  readings_controller readings;
  readings_controller_init(&readings, router->reading_service);
  price_plan_controller price_plan;
  price_plan_controller_init(&price_plan, router->plan_service);

  void* handler = router;
  handle_func func = (handle_func)default_handle;
  switch (request->head.type) {
    case MESSAGE_READINGS_READ:
      handler = &readings;
      func = (handle_func)reading_read;
      break;
    case MESSAGE_PRICE_PLAN_COMPARE_ALL:
      handler = &price_plan;
      func = (handle_func)price_plan_controller_compare_all;
      break;
    case MESSAGE_PRICE_PLAN_RECOMMEND:
      handler = &price_plan;
      func = (handle_func)price_plan_controller_recommend;
    case MESSAGE_INVALID:
    default:
      break;
  }
  return func(handler, request);
}

bool protocol_process(struct protocol* protocol, struct endpoint* endpoint) {
  char buffer[MAX_MESSAGE_READING_PAYLOAD_COUNT];
  int rec = endpoint_receive(endpoint, buffer, sizeof(buffer));
  if (rec == 0) {
    return true;
  }
  message* request = (message*)buffer;
  message response = protocol_handle(protocol, request);
  endpoint_send(endpoint, &response, sizeof(response));
  return false;
}
