#ifndef DEVELOPER_JOYOFENERGY_C_ELECTRICITY_READING_H
#define DEVELOPER_JOYOFENERGY_C_ELECTRICITY_READING_H
#include <time.h>
#include <stdint.h>

struct electricity_reading {
    time_t at;
    uint32_t power;
};

static inline void electricity_reading_init(struct electricity_reading *reading, time_t at, int power)
{
    reading->at = at;
    reading->power = power;
}

#endif  // DEVELOPER_JOYOFENERGY_C_ELECTRICITY_READING_H
