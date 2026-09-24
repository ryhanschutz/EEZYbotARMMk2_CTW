#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define I2C_SDA 21
#define I2C_SCL 22
#define PCA9685_ADDR 0x40
#define SERVO 0

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(PCA9685_ADDR);

void setPulse(uint16_t us) {
  pwm.writeMicroseconds(SERVO, us);
  Serial.print("PWM = ");
  Serial.print(us);
  Serial.println(" us");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Wire.begin(I2C_SDA, I2C_SCL);

  if (!pwm.begin()) {
    Serial.println("[ERRO] PCA9685 nao detectado!");
    while (1) {
      delay(1000);
    }
  }

  pwm.setPWMFreq(50);
  delay(100);

  Serial.println("TESTE MG996R - rotacao continua");
  setPulse(1500);
  delay(1000);
}

void loop() {
  const uint16_t values[] = {1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800,
    0 1900, 2000};

  for (uint8_t i = 0; i < 11; ++i) {
    Serial.print("Valor: ");
    Serial.print(values[i]);
    Serial.println(" us");
    setPulse(values[i]);
    delay(1500);
  }

  Serial.println("Fim do sweep. Reiniciando...");
  delay(1000);
}
