#include "robot_state.h"

RobotState::RobotState() : target_{{90.0F, 90.0F, 90.0F, 90.0F}}, estopActive_(false), calibrated_(false) {}

const JointAngles& RobotState::target() const { return target_; }

bool RobotState::estopActive() const { return estopActive_; }

bool RobotState::isCalibrated() const { return calibrated_; }

void RobotState::engageEstop() { estopActive_ = true; }

void RobotState::setCalibrated(bool calibrated) { calibrated_ = calibrated; }

bool RobotState::setTarget(const JointAngles& target) {
  if (estopActive_ || !calibrated_) {
    return false;
  }
  target_ = target;
  return true;
}
