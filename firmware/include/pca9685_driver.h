#pragma once

#include <Adafruit_PWMServoDriver.h>
#include <stdint.h>
#include "robot_state.h"

// Safe pulse limits for the EEZYbotARM servos, tuned for MG90S/MG90 behavior.
// These values avoid overdriving the servo while still allowing a full 180° sweep.
constexpr uint16_t kServoMinPulseUs = 500;
constexpr uint16_t kServoMaxPulseUs = 2400;
constexpr uint16_t kServoDefaultCenterUs = 1500;
constexpr float kPca9685PwmFrequency = 50.0f;  // 50 Hz = 20ms period

struct JointCalibration {
  uint8_t channel;        // PCA9685 channel (0..15)
  uint16_t minPulseUs;    // Min safe pulse in microseconds
  uint16_t centerPulseUs; // Center reference pulse in microseconds
  uint16_t maxPulseUs;    // Max safe pulse in microseconds
  float minAngleDeg;      // Minimum logical angle in degrees
  float maxAngleDeg;      // Maximum logical angle in degrees
  bool inverted;          // Direction inversion flag
  bool calibrated;        // True only after physical calibration is completed
};

class PCA9685Driver {
 public:
  PCA9685Driver();

  // Initializes I2C (SDA=21, SCL=22 on ESP32) and configures PCA9685 at 50 Hz
  bool begin(uint8_t i2cAddress = 0x40, int sdaPin = 21, int sclPin = 22);

  bool isConnected() const;

  // Raw pulse width output in microseconds (450..2550 us)
  bool setPulse(uint8_t channel, uint16_t pulseUs);

  // Disable PWM on a specific channel (releases motor holding torque)
  void disableChannel(uint8_t channel);

  // Disable PWM on all 16 channels immediately (ESTOP / Safety)
  void disableAll();

  // Configures calibration limits for a specific joint (0..3)
  bool configureJoint(uint8_t jointIndex, const JointCalibration& cal);
  const JointCalibration& getJointConfig(uint8_t jointIndex) const;

  // Converts logical angle (deg) to pulse width (us) based on joint calibration
  uint16_t angleToPulse(uint8_t jointIndex, float angleDeg) const;

  // Writes calibrated angle to the joint's assigned channel
  bool writeJointAngle(uint8_t jointIndex, float angleDeg);

  // Writes all 4 joints atomically
  bool writeAllJoints(const JointAngles& angles);

 private:
  Adafruit_PWMServoDriver pwm_;
  bool initialized_;
  JointCalibration joints_[kJointCount];
};
