#include "metrology.h"

#include <hal/metrology.h>

// int metrology_open(struct metrology*);
// int metrology_close(struct metrology*);
int metrology_get_power(struct metrology* metrology) {
  struct metrology_mock* mock = (struct metrology_mock*)metrology;
  return mock->power;
}
