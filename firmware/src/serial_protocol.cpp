#include "serial_protocol.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace {
const char* errorName(ProtocolError error) {
  switch (error) {
    case ProtocolError::kMalformed: return "MALFORMED";
    case ProtocolError::kUnknownCommand: return "UNKNOWN_COMMAND";
    case ProtocolError::kOutOfRange: return "OUT_OF_RANGE";
    case ProtocolError::kEstopActive: return "ESTOP_ACTIVE";
    case ProtocolError::kNotCalibrated: return "NOT_CALIBRATED";
    case ProtocolError::kHardwareError: return "HARDWARE_ERROR";
    default: return "MALFORMED";
  }
}

bool parseSequence(const char* token, uint16_t* value) {
  if (token == nullptr || *token == '\0') return false;
  char* end = nullptr;
  const unsigned long parsed = strtoul(token, &end, 10);
  if (*end != '\0' || parsed > 65535UL) return false;
  *value = static_cast<uint16_t>(parsed);
  return true;
}

bool parseChannel(const char* token, uint8_t* value) {
  if (token == nullptr || *token == '\0') return false;
  char* end = nullptr;
  const unsigned long parsed = strtoul(token, &end, 10);
  if (*end != '\0' || parsed > 15UL) return false;
  *value = static_cast<uint8_t>(parsed);
  return true;
}

bool parsePulse(const char* token, uint16_t* value) {
  if (token == nullptr || *token == '\0') return false;
  char* end = nullptr;
  const unsigned long parsed = strtoul(token, &end, 10);
  if (*end != '\0' || parsed < kServoMinPulseUs || parsed > kServoMaxPulseUs) return false;
  *value = static_cast<uint16_t>(parsed);
  return true;
}

bool parseAngle(const char* token, float* value) {
  if (token == nullptr || *token == '\0') return false;
  char* end = nullptr;
  const float parsed = strtof(token, &end);
  if (*end != '\0' || !isfinite(parsed) || parsed < 0.0F || parsed > 180.0F) return false;
  *value = parsed;
  return true;
}

bool noMoreTokens(char* context) { return strtok_r(nullptr, ",", &context) == nullptr; }
}  // namespace

ProtocolError SerialProtocol::parse(char* line, ParsedCommand* command) const {
  if (line == nullptr || command == nullptr || *line == '\0') return ProtocolError::kMalformed;
  char* context = nullptr;
  char* name = strtok_r(line, ",", &context);
  char* sequence = strtok_r(nullptr, ",", &context);
  command->sequence = 0;
  if (name == nullptr || !parseSequence(sequence, &command->sequence)) return ProtocolError::kMalformed;

  if (strcmp(name, "PING") == 0) {
    command->type = ProtocolCommand::kPing;
    return noMoreTokens(context) ? ProtocolError::kNone : ProtocolError::kMalformed;
  }
  if (strcmp(name, "GET_STATE") == 0) {
    command->type = ProtocolCommand::kGetState;
    return noMoreTokens(context) ? ProtocolError::kNone : ProtocolError::kMalformed;
  }
  if (strcmp(name, "ESTOP") == 0) {
    command->type = ProtocolCommand::kEstop;
    return noMoreTokens(context) ? ProtocolError::kNone : ProtocolError::kMalformed;
  }
  if (strcmp(name, "CAL_PULSE") == 0) {
    command->type = ProtocolCommand::kCalPulse;
    if (!parseChannel(strtok_r(nullptr, ",", &context), &command->calChannel)) return ProtocolError::kOutOfRange;
    if (!parsePulse(strtok_r(nullptr, ",", &context), &command->calPulseUs)) return ProtocolError::kOutOfRange;
    return noMoreTokens(context) ? ProtocolError::kNone : ProtocolError::kMalformed;
  }
  if (strcmp(name, "CAL_DISABLE") == 0) {
    command->type = ProtocolCommand::kCalDisable;
    if (!parseChannel(strtok_r(nullptr, ",", &context), &command->calChannel)) return ProtocolError::kOutOfRange;
    return noMoreTokens(context) ? ProtocolError::kNone : ProtocolError::kMalformed;
  }
  if (strcmp(name, "SET_JOINTS") != 0) return ProtocolError::kUnknownCommand;

  command->type = ProtocolCommand::kSetJoints;
  for (uint8_t i = 0; i < kJointCount; ++i) {
    if (!parseAngle(strtok_r(nullptr, ",", &context), &command->angles.value[i])) {
      return ProtocolError::kOutOfRange;
    }
  }
  return noMoreTokens(context) ? ProtocolError::kNone : ProtocolError::kMalformed;
}

void SerialProtocol::formatError(uint16_t sequence, ProtocolError error, char* response,
                                 size_t responseSize) const {
  snprintf(response, responseSize, "ERR,%u,%s", sequence, errorName(error));
}

void SerialProtocol::execute(const ParsedCommand& command, RobotState* state, PCA9685Driver* driver,
                             char* response, size_t responseSize) const {
  if (command.type == ProtocolCommand::kPing) {
    snprintf(response, responseSize, "PONG,%u,v0", command.sequence);
    return;
  }
  if (command.type == ProtocolCommand::kGetState) {
    const JointAngles& target = state->target();
    snprintf(response, responseSize, "STATE,%u,0,%.2f,%.2f,%.2f,%.2f", command.sequence,
             target.value[0], target.value[1], target.value[2], target.value[3]);
    return;
  }
  if (command.type == ProtocolCommand::kEstop) {
    state->engageEstop();
    if (driver != nullptr) {
      driver->disableAll();
    }
    snprintf(response, responseSize, "ACK,%u", command.sequence);
    return;
  }
  if (command.type == ProtocolCommand::kCalPulse) {
    if (state->estopActive()) {
      formatError(command.sequence, ProtocolError::kEstopActive, response, responseSize);
      return;
    }
    if (driver != nullptr && driver->setPulse(command.calChannel, command.calPulseUs)) {
      snprintf(response, responseSize, "ACK,%u", command.sequence);
    } else {
      formatError(command.sequence, ProtocolError::kHardwareError, response, responseSize);
    }
    return;
  }
  if (command.type == ProtocolCommand::kCalDisable) {
    if (driver != nullptr) {
      driver->disableChannel(command.calChannel);
    }
    snprintf(response, responseSize, "ACK,%u", command.sequence);
    return;
  }
  if (command.type == ProtocolCommand::kSetJoints) {
    if (state->estopActive()) {
      formatError(command.sequence, ProtocolError::kEstopActive, response, responseSize);
    } else if (!state->isCalibrated()) {
      formatError(command.sequence, ProtocolError::kNotCalibrated, response, responseSize);
    } else if (state->setTarget(command.angles)) {
      if (driver != nullptr) {
        driver->writeAllJoints(command.angles);
      }
      snprintf(response, responseSize, "ACK,%u", command.sequence);
    }
    return;
  }
  formatError(command.sequence, ProtocolError::kMalformed, response, responseSize);
}
