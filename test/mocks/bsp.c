#include "bsp.h"

#include <hal/bsp.h>

static inline struct bsp_mock* mock(struct bsp* bsp) { return (struct bsp_mock*)bsp; }

struct clock* bsp_clock(struct bsp* bsp) {
  return (struct clock*)&mock(bsp)->clock;
}

struct endpoint* bsp_endpoint(struct bsp* bsp) {
  return (struct endpoint*)&mock(bsp)->endpoint;
}

struct metrology* bsp_metrology(struct bsp* bsp) {
  return (struct metrology*)&mock(bsp)->metrology;
}