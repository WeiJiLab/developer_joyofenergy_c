#ifndef DEVELOPER_JOYOFENERGY_C_MOCK_METROLOGY_H
#define DEVELOPER_JOYOFENERGY_C_MOCK_METROLOGY_H

struct metrology_mock {
  int power;
};

static inline void metrology_mock_init(struct metrology_mock* mock) { mock->power = 0; }

static inline void metrology_mock_set_power(struct metrology_mock* mock, int power) { mock->power = power; }

#endif  // DEVELOPER_JOYOFENERGY_C_MOCK_METROLOGY_H
