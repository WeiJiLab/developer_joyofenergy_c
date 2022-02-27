#include <adapter/protocol/protocol.h>
#include <adapter/protocol/reading.h>
#include <gmock/gmock.h>
#include <mocks/bsp.h>

#include "meter.h"

#define METER_ID_0 "smart-meter-0"
#define METER_ID_1 "smart-meter-1"
#define METER_ID_2 "smart-meter-2"
#define METER_ID_3 "smart-meter-3"
#define METER_ID_4 "smart-meter-4"

class EndpointTest : public ::testing::Test {
  public:
    EndpointTest() {}
    void TearDown() {
        destroy_meter(METER_ID_0);
        destroy_meter(METER_ID_1);
        destroy_meter(METER_ID_2);
        destroy_meter(METER_ID_3);
        destroy_meter(METER_ID_4);
    }
    void SetUp() {
        bsp_mock_init(&mock);
        meter_0 = create_meter(METER_ID_0, (struct bsp *) &mock);
        assert(meter_0 != NULL);
        meter_1 = create_meter(METER_ID_1, (struct bsp *) &mock);
        assert(meter_1 != NULL);
        meter_2 = create_meter(METER_ID_2, (struct bsp *) &mock);
        assert(meter_2 != NULL);
        meter_3 = create_meter(METER_ID_3, (struct bsp *) &mock);
        assert(meter_3 != NULL);
        meter_4 = create_meter(METER_ID_4, (struct bsp *) &mock);
        assert(meter_4 != NULL);
    }

    struct bsp_mock mock;
  private:
    struct meter *meter_0 = NULL;
    struct meter *meter_1 = NULL;
    struct meter *meter_2 = NULL;
    struct meter *meter_3 = NULL;
    struct meter *meter_4 = NULL;
};

message make_request(message_type requestType) {
    message req = {0};
    strncpy(req.head.meter_id, METER_ID_3, METER_ID_LEN);
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
    struct reading_message_response *data = (struct reading_message_response *) response->payload;
    ASSERT_EQ(data->readings_count, 21);
    ASSERT_EQ(data->readings[0].power, 4000);
}

TEST_F(EndpointTest, DoubleMessagesTest) {
    struct message msg = make_request(MESSAGE_READINGS_READ);
    master_send_message(&msg);
    master_send_message(&msg);

    struct message *response = &msg;
    master_receive_message(&msg);
    struct reading_message_response *data = (struct reading_message_response *) response->payload;
    ASSERT_EQ(data->readings_count, 21);
    ASSERT_EQ(data->readings[20].power, 4000);

    master_receive_message(&msg);
    data = (struct reading_message_response *) response->payload;
    ASSERT_EQ(data->readings_count, 21);
    ASSERT_EQ(data->readings[20].power, 4000);
}

TEST_F(EndpointTest, MultipleMessagesTest) {
    int i;
    int result;
    struct message msg = make_request(MESSAGE_READINGS_READ);
    for (i = 0; i < 0x10; i++) {
        result = master_send_message(&msg);
        ASSERT_EQ(result, 0);
    }
    // result = master_send_message(&msg);
    // ASSERT_NE(result, 0);

    struct message *response = &msg;
    for (i = 0; i < 0x10; i++) {
        result = master_receive_message(&msg);
        ASSERT_EQ(result, 0);
        struct reading_message_response *data = (struct reading_message_response *) response->payload;
        ASSERT_EQ(data->readings_count, 21);
        ASSERT_EQ(data->readings[20].power, 4000);
    }
}

TEST_F(EndpointTest, ShouldStoreReadingAfter15Minutes) {
    clock_forward_minutes(&mock.clock, 15);
    const int power = 5000;
    metrology_mock_set_power(&mock.metrology, power);

    struct message msg = make_request(MESSAGE_READINGS_READ);
    master_send_message(&msg);

    struct message *response = &msg;
    master_receive_message(&msg);
    struct reading_message_response *data = (struct reading_message_response *) response->payload;
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
    price_plan_compare_all_response *data = (price_plan_compare_all_response *) response->payload;
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
    price_plan_recommend_response *data = (price_plan_recommend_response *) response->payload;
    ASSERT_EQ(data->plans_count, recommend_request->limit);
    ASSERT_STREQ(data->plans[0].plan, "price-plan-2");
    ASSERT_EQ(data->plans[0].charge, 4 * 100);
}
