#ifndef DEVELOPER_JOYOFENERGY_C_MOCK_BSP_H
#define DEVELOPER_JOYOFENERGY_C_MOCK_BSP_H

#include "clock.h"
#include "endpoint.h"
#include "metrology.h"

struct bsp_mock {
  struct clock_mock clock;
  struct endpoint_mock endpoint;
  struct metrology_mock metrology;
};

void bsp_mock_init(struct bsp_mock* mock) {
  clock_mock_init(&mock->clock);
  endpoint_mock_init(&mock->endpoint);
  metrology_mock_init(&mock->metrology);
}

#endif  // DEVELOPER_JOYOFENERGY_C_MOCK_BSP_H
