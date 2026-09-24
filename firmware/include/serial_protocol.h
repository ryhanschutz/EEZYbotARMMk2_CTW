#pragma once

#include "robot_state.h"

#include <stddef.h>
#include <stdint.h>

constexpr size_t kProtocolMaxLineLength = 96;
constexpr size_t kProtocolResponseLength = 96;

enum class ProtocolCommand : uint8_t { kPing, kGetState, kSetJoints, kEstop, kInvalid };

enum class ProtocolError : uint8_t {
  kNone,
  kMalformed,
  kUnknownCommand,
  kOutOfRange,
  kEstopActive,
  kNotCalibrated,
};

struct ParsedCommand {
  ProtocolCommand type;
  uint16_t sequence;
  JointAngles angles;
};

// Pure command parser/executor. The caller owns the input and output buffers.
// Keeping it independent from Serial makes it unit-testable and reusable with
// USB serial, Bluetooth, or a simulator transport.
class SerialProtocol {
 public:
  ProtocolError parse(char* line, ParsedCommand* command) const;
  void execute(const ParsedCommand& command, RobotState* state, char* response,
               size_t responseSize) const;
  void formatError(uint16_t sequence, ProtocolError error, char* response,
                   size_t responseSize) const;
};
