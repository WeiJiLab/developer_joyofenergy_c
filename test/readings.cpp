#include <adapter/protocol/protocol.h>
#include <adapter/protocol/reading.h>
#include <gmock/gmock.h>
#include <mocks/bsp.h>

#include "meter.h"

class EndpointTest : public ::testing::Test {};

struct message make_request(message_type requestType) {
  struct message req = {0};
  req.head.size = sizeof(req);
  req.head.type = requestType;
  return req;
}

TEST_F(EndpointTest, ShouldReadReading) {
  struct bsp_mock mock;
  bsp_mock_init(&mock);
  struct meter meter;
  meter_init(&meter, (struct bsp *)&mock);
  struct message request = make_request(MESSAGE_READINGS_READ);
  endpoint_mock_send(&mock.endpoint, &request, sizeof(request));

  meter_process(&meter);

  const struct buffer received = endpoint_mock_receive(&mock.endpoint);
  ASSERT_GT(received.size, 0);
  struct message *response = (struct message *)received.data;
  ASSERT_EQ(response->head.type, MESSAGE_READINGS_READ);
  ASSERT_GT(response->head.size, sizeof(response->head));
  struct reading_message_response *data = (struct reading_message_response *)response->payload;
  ASSERT_EQ(data->readings_count, 21);
  ASSERT_EQ(data->readings[0].power, 4000);
}

TEST_F(EndpointTest, ShouldStoreReadingAfter15Minutes) {
  struct bsp_mock mock;
  bsp_mock_init(&mock);
  struct meter meter;
  meter_init(&meter, (struct bsp *)&mock);
  clock_forward_minutes(&mock.clock, 15);
  const int power = 5000;
  metrology_mock_set_power(&mock.metrology, power);

  meter_process(&meter);

  struct message request = make_request(MESSAGE_READINGS_READ);
  endpoint_mock_send(&mock.endpoint, &request, sizeof(request));
  meter_process(&meter);
  const struct buffer received = endpoint_mock_receive(&mock.endpoint);
  ASSERT_GT(received.size, 0);
  struct message *response = (struct message *)received.data;
  struct reading_message_response *data = (struct reading_message_response *)response->payload;
  ASSERT_EQ(data->readings_count, 22);
  ASSERT_EQ(data->readings[21].power, power);
  ASSERT_EQ(data->readings[21].at, mock.clock.now);
}

TEST_F(EndpointTest, ShouldCompareAllPricePlan) {
  struct bsp_mock mock;
  bsp_mock_init(&mock);
  struct meter meter;
  meter_init(&meter, (struct bsp *)&mock);
  struct message request = make_request(MESSAGE_PRICE_PLAN_COMPARE_ALL);
  endpoint_mock_send(&mock.endpoint, &request, sizeof(request));

  meter_process(&meter);

  const struct buffer received = endpoint_mock_receive(&mock.endpoint);
  ASSERT_GT(received.size, 0);
  struct message *response = (struct message *)received.data;
  ASSERT_EQ(response->head.type, MESSAGE_PRICE_PLAN_COMPARE_ALL);
  struct price_plan_compare_all_response *data = (struct price_plan_compare_all_response *)response->payload;
  ASSERT_EQ(data->plans_count, 3);
  ASSERT_STREQ(data->plans[0].plan, "price-plan-0");
  ASSERT_EQ(data->plans[0].charge, 40 * 100);
}