#ifndef DEVELOPER_JOYOFENERGY_C_MESSAGE_H
#define DEVELOPER_JOYOFENERGY_C_MESSAGE_H

// default max message reading payload count
#define MAX_MESSAGE_READING_PAYLOAD_COUNT 1024

enum message_type {
  MESSAGE_INVALID = 0,
  MESSAGE_READINGS_READ,
  MESSAGE_PRICE_PLAN_COMPARE_ALL,
  MESSAGE_PRICE_PLAN_RECOMMEND,
};

struct message_head {
  uint32_t meter_id;
  uint32_t size;
  uint32_t type;
  char payload[0];
};

typedef struct message {
  struct message_head head;
  char payload[MAX_MESSAGE_READING_PAYLOAD_COUNT];
} message;
#endif