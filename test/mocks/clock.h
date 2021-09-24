#ifndef DEVELOPER_JOYOFENERGY_C_MOCK_CLOCK_H
#define DEVELOPER_JOYOFENERGY_C_MOCK_CLOCK_H

#include <hal/clock.h>

struct clock_mock {
  time_t now;
};

static inline void clock_mock_init(struct clock_mock* mock) { mock->now = time(NULL); }

static inline void clock_forward_seconds(struct clock_mock* mock, size_t seconds) { mock->now += seconds; }
static inline void clock_forward_minutes(struct clock_mock* mock, size_t minutes) { mock->now += minutes * 60; }

#endif  // DEVELOPER_JOYOFENERGY_C_MOCK_CLOCK_H
