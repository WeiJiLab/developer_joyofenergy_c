#include "price_plan_service.h"

static int calculate_average_reading(struct electricity_reading* readings, size_t readings_count) {
  int sum = 0;
  for (size_t i = 0; i < readings_count; ++i) {
    sum += readings[i].power;
  }
  return (int)(((double)sum + 0.5) / readings_count);
}

static time_t calculate_time_elapsed(struct electricity_reading* readings, size_t readings_count) {
  time_t first = readings[0].at;
  time_t last = readings[readings_count - 1].at;
  return last - first;
}

static int calculate_cost(struct electricity_reading* readings, size_t readings_count, struct price_plan* pricePlan) {
  double average = calculate_average_reading(readings, readings_count);
  size_t seconds_elapsed = calculate_time_elapsed(readings, readings_count);

  int consumed_watt = average * 3600 / seconds_elapsed;
  int cost_in_cents = consumed_watt * pricePlan->centsPerKWH / 10;
  return cost_in_cents;
}

size_t price_plan_service_compare_all(struct price_plan_service* service, struct plan_charge* results, size_t count) {
  struct electricity_reading readings[1024];
  size_t readings_count = sizeof(readings) / sizeof(readings[0]);
  readings_count = electricity_reading_service_get(service->reading_service, readings, readings_count);

  if (readings_count == 0) {
    return 0;
  }

  for (size_t i = 0; i < service->plans_count; ++i) {
    results[i].plan = service->plans[i].name;
    results[i].charge = calculate_cost(readings, readings_count, &service->plans[i]);
  }
  return service->plans_count;
}
