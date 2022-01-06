#include "meter.h"

#include <stdlib.h>

#include "adapter/protocol/protocol.h"
#include "adapter/schedule/schedule.h"
#include "configuration.h"

#define MAX_METER_COUNT 0x10

static struct meter meter_info[MAX_METER_COUNT] = { 0 };

static void generate(struct electricity_reading* readings, size_t number)
{
  time_t now = time(NULL);

  for (size_t i = 0; i < number; ++i) {
      readings[i].at = now - (number - i) * 3 * 60;
      readings[i].power = 4000;
  }
}

static bool meter_process(struct meter *meter)
{
  do {
      schedule_process(&meter->schedule);
  } while (!protocol_process(&meter->protocol));

  return false;
}

static struct meter *id_to_meter_obj(const char *meter_id)
{
    int i;

    for (i = 0; i < MAX_METER_COUNT; i++) {
        if (0 == strncmp(meter_info[i].meter_id, meter_id, METER_ID_LEN)) {
            return &meter_info[i];
        }
    }
    return NULL;
}

void device_interrupt_proc(char *meter_id)
{
    if (meter_id) {
        struct meter *meter;
        meter = id_to_meter_obj(meter_id);
        if (NULL == meter) {
            return;
        }
        meter_process(meter);
    }
}

struct meter *create_meter(const char *meter_id, struct bsp *bsp)
{
    int i;

    if (NULL == meter_id) {
        return NULL;
    }

    for (i = 0; i < MAX_METER_COUNT; i++) {
        if (('\0' == meter_info[i].meter_id[0])
         || (0 == strncmp(meter_info[i].meter_id, meter_id, METER_ID_LEN))) {
            break;
        }
    }
    if (MAX_METER_COUNT <= i) {
        return NULL;
    }

    struct meter *meter = &meter_info[i];
    memcpy(meter->meter_id, meter_id, METER_ID_LEN);
    meter->bsp = bsp;
    meter->readings_count = 21;
    generate(meter->readings, meter->readings_count);
    meter->plans_count = price_plans(meter->plans, sizeof(meter->plans) / sizeof(meter->plans[0]));
    electricity_reading_service_init(&meter->reading_service, bsp, meter->readings, meter->readings_count);
    price_plan_service_init(&meter->price_plan_service, &meter->reading_service, meter->plans, meter->plans_count);
    schedule_init(&meter->schedule, &meter->reading_service, bsp);
    protocol_init(&meter->protocol, &meter->reading_service, &meter->price_plan_service);

    return meter;
}

void destroy_meter(const char *meter_id)
{
    int i;

    if (meter_id) {
        for (i = 0; i < MAX_METER_COUNT; i++) {
            if (0 == strncmp(meter_info[i].meter_id, meter_id, METER_ID_LEN)) {
                meter_info[i].meter_id[0] = '\0';
                return;
            }
        }
    }
}
