#include <adapter/protocol/protocol.h>
#include <adapter/protocol/reading.h>
#include <gmock/gmock.h>
#include <mocks/bsp.h>

#include "meter.h"

class EndpointTest : public ::testing::Test {
 public:
  EndpointTest() {}
  void TearDown() {  }
  void SetUp() {
    bsp_mock_init(&mock);
    meter_init(&meter, (struct bsp *)&mock);
  }
  struct bsp_mock mock;
  struct meter meter;
};

message make_request(message_type requestType) {
  message req = {0};
  req.head.size = sizeof(req);
  req.head.type = requestType;
  return req;
}

TEST_F(EndpointTest, ShouldReadReading) {
  struct message msg = make_request(MESSAGE_READINGS_READ);
  master_send_message(&msg);

  struct message *response = &msg;
  master_receive_message(response);
  ASSERT_EQ(response->head.type, MESSAGE_READINGS_READ);
  ASSERT_GT(response->head.size, sizeof(response->head));
  struct reading_message_response *data = (struct reading_message_response *)response->payload;
  ASSERT_EQ(data->readings_count, 21);
  ASSERT_EQ(data->readings[0].power, 4000);
}

TEST_F(EndpointTest, ShouldStoreReadingAfter15Minutes) {
  clock_forward_minutes(&mock.clock, 15);
  const int power = 5000;
  metrology_mock_set_power(&mock.metrology, power);

  struct message msg = make_request(MESSAGE_READINGS_READ);
  master_send_message(&msg);

  struct message *response = &msg;
  master_receive_message(&msg);
  struct reading_message_response *data = (struct reading_message_response *)response->payload;
  ASSERT_EQ(data->readings_count, 22);
  ASSERT_EQ(data->readings[21].power, power);
  ASSERT_EQ(data->readings[21].at, mock.clock.now);
}

TEST_F(EndpointTest, ShouldCompareAllPricePlan) {
  struct message msg = make_request(MESSAGE_PRICE_PLAN_COMPARE_ALL);
  master_send_message(&msg);

  struct message *response = &msg;
  master_receive_message(response);
  ASSERT_EQ(response->head.type, MESSAGE_PRICE_PLAN_COMPARE_ALL);
  price_plan_compare_all_response *data = (price_plan_compare_all_response *)response->payload;
  ASSERT_EQ(data->plans_count, 3);
  ASSERT_STREQ(data->plans[0].plan, "price-plan-0");
  ASSERT_EQ(data->plans[0].charge, 40 * 100);
}

TEST_F(EndpointTest, ShouldRecommendPricePlan) {
  struct message msg = make_request(MESSAGE_PRICE_PLAN_RECOMMEND);
  struct price_plan_recommend_request *recommend_request = (struct price_plan_recommend_request *) msg.payload;
  recommend_request->limit = 2;
  master_send_message(&msg);

  struct message *response = &msg;
  master_receive_message(response);
  ASSERT_EQ(response->head.type, MESSAGE_PRICE_PLAN_RECOMMEND);
  price_plan_recommend_response *data = (price_plan_recommend_response *)response->payload;
  ASSERT_EQ(data->plans_count, recommend_request->limit);
  ASSERT_STREQ(data->plans[0].plan, "price-plan-2");
  ASSERT_EQ(data->plans[0].charge, 4 * 100);
}
