#include "pca9685_driver.h"
#include <Arduino.h>
#include <Wire.h>

PCA9685Driver::PCA9685Driver() : pwm_(0x40), initialized_(false) {
  // Default channel assignments: J1 -> Ch 0, J2 -> Ch 1, J3 -> Ch 2, J4 -> Ch 3
  for (uint8_t i = 0; i < kJointCount; ++i) {
    joints_[i].channel = i;
    joints_[i].minPulseUs = 500;
    joints_[i].centerPulseUs = 1500;
    joints_[i].maxPulseUs = 2500;
    joints_[i].minAngleDeg = 0.0f;
    joints_[i].maxAngleDeg = 180.0f;
    joints_[i].inverted = false;
    joints_[i].calibrated = false;  // Disabled until calibrated
  }
}

bool PCA9685Driver::begin(uint8_t i2cAddress, int sdaPin, int sclPin) {
  Wire.begin(sdaPin, sclPin);
  
  // Test I2C communication before proceeding
  Wire.beginTransmission(i2cAddress);
  const uint8_t error = Wire.endTransmission();
  if (error != 0) {
    initialized_ = false;
    return false;
  }

  pwm_ = Adafruit_PWMServoDriver(i2cAddress);
  pwm_.begin();
  pwm_.setOscillatorFrequency(27000000);  // 27MHz internal oscillator
  pwm_.setPWMFreq(kPca9685PwmFrequency);
  
  // Disable all channels on startup for safety
  disableAll();
  initialized_ = true;
  return true;
}

bool PCA9685Driver::isConnected() const {
  return initialized_;
}

bool PCA9685Driver::setPulse(uint8_t channel, uint16_t pulseUs) {
  if (!initialized_ || channel > 15) {
    return false;
  }

  // Enforce safety limits to protect servo hardware
  if (pulseUs < kServoMinPulseUs) pulseUs = kServoMinPulseUs;
  if (pulseUs > kServoMaxPulseUs) pulseUs = kServoMaxPulseUs;

  // 12-bit resolution: 4096 ticks in a 20,000 us cycle (50 Hz)
  const uint32_t ticks = (static_cast<uint32_t>(pulseUs) * 4096UL) / 20000UL;
  pwm_.setPWM(channel, 0, static_cast<uint16_t>(ticks));
  return true;
}

void PCA9685Driver::disableChannel(uint8_t channel) {
  if (initialized_ && channel <= 15) {
    pwm_.setPWM(channel, 0, 0);
  }
}

void PCA9685Driver::disableAll() {
  if (!initialized_) return;
  for (uint8_t ch = 0; ch < 16; ++ch) {
    pwm_.setPWM(ch, 0, 0);
  }
}

bool PCA9685Driver::configureJoint(uint8_t jointIndex, const JointCalibration& cal) {
  if (jointIndex >= kJointCount || cal.channel > 15) {
    return false;
  }
  joints_[jointIndex] = cal;
  return true;
}

const JointCalibration& PCA9685Driver::getJointConfig(uint8_t jointIndex) const {
  if (jointIndex >= kJointCount) {
    return joints_[0];
  }
  return joints_[jointIndex];
}

uint16_t PCA9685Driver::angleToPulse(uint8_t jointIndex, float angleDeg) const {
  if (jointIndex >= kJointCount) return kServoDefaultCenterUs;

  const JointCalibration& cal = joints_[jointIndex];
  
  // Constrain angle within calibrated range
  if (angleDeg < cal.minAngleDeg) angleDeg = cal.minAngleDeg;
  if (angleDeg > cal.maxAngleDeg) angleDeg = cal.maxAngleDeg;

  float norm = (angleDeg - cal.minAngleDeg) / (cal.maxAngleDeg - cal.minAngleDeg);
  if (cal.inverted) {
    norm = 1.0f - norm;
  }

  const float pulse = cal.minPulseUs + norm * (cal.maxPulseUs - cal.minPulseUs);
  return static_cast<uint16_t>(pulse + 0.5f);
}

bool PCA9685Driver::writeJointAngle(uint8_t jointIndex, float angleDeg) {
  if (!initialized_ || jointIndex >= kJointCount) return false;
  if (!joints_[jointIndex].calibrated) return false;

  const uint16_t pulse = angleToPulse(jointIndex, angleDeg);
  return setPulse(joints_[jointIndex].channel, pulse);
}

bool PCA9685Driver::writeAllJoints(const JointAngles& angles) {
  if (!initialized_) return false;

  // Check that all joints are calibrated before moving
  for (uint8_t i = 0; i < kJointCount; ++i) {
    if (!joints_[i].calibrated) return false;
  }

  for (uint8_t i = 0; i < kJointCount; ++i) {
    writeJointAngle(i, angles.value[i]);
  }
  return true;
}
