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
  protocol.execute(command, &state, nullptr, response, sizeof(response));
  TEST_ASSERT_EQUAL_STRING("PONG,42,v0", response);
}

void test_set_joints_is_blocked_before_calibration() {
  char line[] = "SET_JOINTS,7,90,80,70,60";
  ParsedCommand command{};
  SerialProtocol protocol;
  RobotState state;
  char response[kProtocolResponseLength];

  TEST_ASSERT_EQUAL_INT(static_cast<int>(ProtocolError::kNone), static_cast<int>(protocol.parse(line, &command)));
  protocol.execute(command, &state, nullptr, response, sizeof(response));
  TEST_ASSERT_EQUAL_STRING("ERR,7,NOT_CALIBRATED", response);
}

void test_out_of_range_angle_is_rejected() {
  char line[] = "SET_JOINTS,9,90,181,70,60";
  ParsedCommand command{};
  SerialProtocol protocol;
  TEST_ASSERT_EQUAL_INT(static_cast<int>(ProtocolError::kOutOfRange), static_cast<int>(protocol.parse(line, &command)));
}

void test_cal_pulse_command_parsed() {
  char line[] = "CAL_PULSE,10,0,1500";
  ParsedCommand command{};
  SerialProtocol protocol;
  TEST_ASSERT_EQUAL_INT(static_cast<int>(ProtocolError::kNone), static_cast<int>(protocol.parse(line, &command)));
  TEST_ASSERT_EQUAL_UINT16(10, command.sequence);
  TEST_ASSERT_EQUAL_UINT8(0, command.calChannel);
  TEST_ASSERT_EQUAL_UINT16(1500, command.calPulseUs);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_ping_is_parsed_and_answered);
  RUN_TEST(test_set_joints_is_blocked_before_calibration);
  RUN_TEST(test_out_of_range_angle_is_rejected);
  RUN_TEST(test_cal_pulse_command_parsed);
  UNITY_END();
}

void loop() {}
