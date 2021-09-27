#include "metrology.h"

#include <hal/metrology.h>

int metrology_get_power(struct metrology* metrology) {
  struct metrology_mock* mock = (struct metrology_mock*)metrology;
  return mock->power;
}
