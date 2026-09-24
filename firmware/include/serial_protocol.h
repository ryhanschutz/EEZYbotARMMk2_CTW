#pragma once

#include "pca9685_driver.h"
#include "robot_state.h"

#include <stddef.h>
#include <stdint.h>

constexpr size_t kProtocolMaxLineLength = 96;
constexpr size_t kProtocolResponseLength = 96;

enum class ProtocolCommand : uint8_t {
  kPing,
  kGetState,
  kSetJoints,
  kEstop,
  kCalPulse,
  kCalDisable,
  kInvalid
};

enum class ProtocolError : uint8_t {
  kNone,
  kMalformed,
  kUnknownCommand,
  kOutOfRange,
  kEstopActive,
  kNotCalibrated,
  kHardwareError,
};

struct ParsedCommand {
  ProtocolCommand type;
  uint16_t sequence;
  JointAngles angles;
  uint8_t calChannel;
  uint16_t calPulseUs;
};

// Pure command parser/executor. The caller owns the input and output buffers.
class SerialProtocol {
 public:
  ProtocolError parse(char* line, ParsedCommand* command) const;
  void execute(const ParsedCommand& command, RobotState* state, PCA9685Driver* driver,
               char* response, size_t responseSize) const;
  void formatError(uint16_t sequence, ProtocolError error, char* response,
                   size_t responseSize) const;
};
