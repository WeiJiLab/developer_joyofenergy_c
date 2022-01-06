#include "protocol.h"
#include "reading.h"

struct message default_handle(struct protocol *controller, const struct message *request)
{
    struct message message = { 0 };
    return message;
}

typedef struct message (*handle_func)(void *controller, const struct message *request);

static struct message protocol_handle(struct protocol *router, const struct message *request)
{
    struct readings_controller readings;
    readings_controller_init(&readings, router->reading_service);
    struct price_plan_controller price_plan;
    price_plan_controller_init(&price_plan, router->plan_service);

    void *handler = router;
    handle_func func = (handle_func) default_handle;
    switch (request->head.type) {
    case MESSAGE_READINGS_READ:
        handler = &readings;
        func = (handle_func) reading_read;
        break;
    case MESSAGE_PRICE_PLAN_COMPARE_ALL:
        handler = &price_plan;
        func = (handle_func) price_plan_controller_compare_all;
        break;
    case MESSAGE_PRICE_PLAN_RECOMMEND:
        handler = &price_plan;
        func = (handle_func) price_plan_controller_recommend;
    case MESSAGE_INVALID:
    default:
        break;
    }
    return func(handler, request);
}

bool protocol_process(struct protocol *protocol)
{
  struct message msg;

  int ret = slave_receive_message(&msg);
  if (0 != ret) {
      return true;
  }

  struct message response = protocol_handle(protocol, &msg);
  slave_send_message(&response);
  return false;
}
