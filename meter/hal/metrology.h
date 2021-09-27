#ifndef DEVELOPER_JOYOFENERGY_C_METROLOGY_H
#define DEVELOPER_JOYOFENERGY_C_METROLOGY_H

#include <stdlib.h>

struct metrology;

int metrology_open(struct metrology*);
int metrology_close(struct metrology*);
int metrology_get_power(struct metrology* metrology);

#endif  // DEVELOPER_JOYOFENERGY_C_METROLOGY_H
