#include "clock.h"

time_t clock_now(struct clock *clock) {
  struct clock_mock *mock = (struct clock_mock *)clock;
  return mock->now;
}
