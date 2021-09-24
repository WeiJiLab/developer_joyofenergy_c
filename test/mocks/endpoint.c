#include "endpoint.h"

#include <hal/endpoint.h>
#include <string.h>

const struct buffer endpoint_mock_receive(struct endpoint_mock* mock) {
  struct buffer result;
  if (mock->out_buffers_length > 0) {
    memcpy(&result, &mock->out_buffers[0], sizeof(result));
    --mock->out_buffers_length;
    memcpy(mock->out_buffers, mock->out_buffers + 1, sizeof(mock->out_buffers[0]) * mock->out_buffers_length);
  }
  return result;
}

bool endpoint_mock_send(struct endpoint_mock* endpoint, const void* data, size_t size) {
  size_t capacity = sizeof(endpoint->in_buffers) / sizeof(endpoint->in_buffers[0]);
  if (endpoint->in_buffers_length >= capacity) {
    return false;
  }

  struct buffer* in = &endpoint->in_buffers[endpoint->in_buffers_length];
  memcpy(in->data, data, size);
  in->size = size;

  ++endpoint->in_buffers_length;
  return true;
}

int endpoint_open(struct endpoint* endpoint) { return 0; }

int endpoint_close(struct endpoint* endpoint) { return 0; }

size_t endpoint_send(struct endpoint* endpoint, const void* data, size_t size) {
  struct endpoint_mock* mock = (struct endpoint_mock*)endpoint;
  size_t capacity = sizeof(mock->out_buffers) / sizeof(mock->out_buffers[0]);
  if (mock->out_buffers_length >= capacity) {
    return 0;
  }

  struct buffer* out = &mock->out_buffers[mock->out_buffers_length];
  size_t sent_size = size > sizeof(out->data) ? sizeof(out->data) : size;
  memcpy(out->data, data, sent_size);
  out->size = sent_size;
  ++mock->out_buffers_length;
  return sent_size;
}

size_t endpoint_receive(struct endpoint* endpoint, void* data, size_t expected_size) {
  struct endpoint_mock* mock = (struct endpoint_mock*)endpoint;
  if (mock->in_buffers_length <= 0) {
    return 0;
  }

  const struct buffer* in = &mock->in_buffers[0];
  size_t received_size = in->size > expected_size ? expected_size : in->size;
  memcpy(data, in->data, received_size);

  --mock->in_buffers_length;
  memcpy(mock->in_buffers, mock->in_buffers + 1, sizeof(mock->in_buffers[0]) * mock->in_buffers_length);
  return received_size;
}