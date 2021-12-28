#ifndef DEVELOPER_JOYOFENERGY_C_CONFIGURATION_H
#define DEVELOPER_JOYOFENERGY_C_CONFIGURATION_H
#include <assert.h>
#include <domain/price_plan.h>

#define DR_EVILS_DARK_ENERGY_ENERGY_SUPPLIER "Dr Evil's Dark Energy"
#define THE_GREEN_ECO_ENERGY_SUPPLIER "The Green Eco"
#define POWER_FOR_EVERYONE_ENERGY_SUPPLIER "Power for Everyone"

#define MOST_EVIL_PRICE_PLAN_ID "price-plan-0"
#define RENEWABLES_PRICE_PLAN_ID "price-plan-1"
#define STANDARD_PRICE_PLAN_ID "price-plan-2"

// default schedule process cycle time
#define DEFAULT_SCHEDULE_CYCLE_TIME_MINUTE  60 * 15

static inline size_t price_plans(price_plan* plans, size_t count) {
  assert(count >= 3);
  strcpy(plans[0].name, MOST_EVIL_PRICE_PLAN_ID);
  strcpy(plans[0].supplier, DR_EVILS_DARK_ENERGY_ENERGY_SUPPLIER);
  plans[0].centsPerKWH = 10;
  strcpy(plans[1].name, RENEWABLES_PRICE_PLAN_ID);
  strcpy(plans[1].supplier, THE_GREEN_ECO_ENERGY_SUPPLIER);
  plans[1].centsPerKWH = 2;
  strcpy(plans[2].name, STANDARD_PRICE_PLAN_ID);
  strcpy(plans[2].supplier, POWER_FOR_EVERYONE_ENERGY_SUPPLIER);
  plans[2].centsPerKWH = 1;
  return 3;
}

#endif  // DEVELOPER_JOYOFENERGY_C_CONFIGURATION_H
