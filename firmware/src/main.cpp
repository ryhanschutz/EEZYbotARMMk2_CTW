#include <Arduino.h>

#include "robot_state.h"
#include "serial_protocol.h"

namespace {
RobotState robotState;
SerialProtocol protocol;
char line[kProtocolMaxLineLength + 1];
size_t lineLength = 0;
bool discardLine = false;

void processLine() {
  line[lineLength] = '\0';
  ParsedCommand command{};
  char response[kProtocolResponseLength];
  const ProtocolError error = protocol.parse(line, &command);
  if (error == ProtocolError::kNone) {
    protocol.execute(command, &robotState, response, sizeof(response));
  } else {
    protocol.formatError(command.sequence, error, response, sizeof(response));
  }
  Serial.println(response);
}
}  // namespace

void setup() {
  // Safe skeleton: servo outputs stay disabled until calibration is recorded.
  Serial.begin(115200);
  Serial.println("EEZYbotARM Mk2 firmware: boot");
}

void loop() {
  while (Serial.available() > 0) {
    const char received = static_cast<char>(Serial.read());
    if (received == '\r') continue;
    if (received == '\n') {
      if (!discardLine && lineLength > 0) processLine();
      lineLength = 0;
      discardLine = false;
    } else if (!discardLine && lineLength < kProtocolMaxLineLength) {
      line[lineLength++] = received;
    } else {
      // Once a line is oversized, discard all of it until its newline so a
      // valid-looking suffix can never be interpreted as a fresh command.
      discardLine = true;
    }
  }
}
