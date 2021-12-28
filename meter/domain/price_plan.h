#ifndef DEVELOPER_JOYOFENERGY_C_PRICE_PLAN_H
#define DEVELOPER_JOYOFENERGY_C_PRICE_PLAN_H
#include <string.h>
//#include "adapter/protocol/protocol.h"

typedef struct {
  char supplier[256];
  char name[256];
  int centsPerKWH;
} price_plan;

static inline void price_plan_init(price_plan* plan, const char* planName, const char* energySupplier, int pricePerKWH) {
  strcpy(plan->name, planName);
  strcpy(plan->supplier, energySupplier);
  plan->centsPerKWH = pricePerKWH;
}

#endif  // DEVELOPER_JOYOFENERGY_C_PRICE_PLAN_H
