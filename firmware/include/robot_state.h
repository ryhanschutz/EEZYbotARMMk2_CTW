#pragma once

#include <stdint.h>

constexpr uint8_t kJointCount = 4;

struct JointAngles {
  float value[kJointCount];
};

// This is the firmware's commanded state, not measured position. Hobby servos
// have no encoder feedback in this project.
class RobotState {
 public:
  RobotState();

  const JointAngles& target() const;
  bool estopActive() const;
  bool isCalibrated() const;

  void engageEstop();
  void setCalibrated(bool calibrated);
  bool setTarget(const JointAngles& target);

 private:
  JointAngles target_;
  bool estopActive_;
  bool calibrated_;
};
