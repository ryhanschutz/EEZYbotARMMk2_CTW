#include <unity.h>

#include "robot_state.h"
#include "serial_protocol.h"

void test_ping_is_parsed_and_answered() {
  char line[] = "PING,42";
  ParsedCommand command{};
  SerialProtocol protocol;
  RobotState state;
  char response[kProtocolResponseLength];

  TEST_ASSERT_EQUAL_INT(static_cast<int>(ProtocolError::kNone), static_cast<int>(protocol.parse(line, &command)));
  protocol.execute(command, &state, response, sizeof(response));
  TEST_ASSERT_EQUAL_STRING("PONG,42,v0", response);
}

void test_set_joints_is_blocked_before_calibration() {
  char line[] = "SET_JOINTS,7,90,80,70,60";
  ParsedCommand command{};
  SerialProtocol protocol;
  RobotState state;
  char response[kProtocolResponseLength];

  TEST_ASSERT_EQUAL_INT(static_cast<int>(ProtocolError::kNone), static_cast<int>(protocol.parse(line, &command)));
  protocol.execute(command, &state, response, sizeof(response));
  TEST_ASSERT_EQUAL_STRING("ERR,7,NOT_CALIBRATED", response);
}

void test_out_of_range_angle_is_rejected() {
  char line[] = "SET_JOINTS,9,90,181,70,60";
  ParsedCommand command{};
  SerialProtocol protocol;
  TEST_ASSERT_EQUAL_INT(static_cast<int>(ProtocolError::kOutOfRange), static_cast<int>(protocol.parse(line, &command)));
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_ping_is_parsed_and_answered);
  RUN_TEST(test_set_joints_is_blocked_before_calibration);
  RUN_TEST(test_out_of_range_angle_is_rejected);
  UNITY_END();
}

void loop() {}
