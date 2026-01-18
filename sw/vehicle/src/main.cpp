#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>
#include "MX1508_Simple.h"

// Pins
#define RX_PIN PB12
#define MOTOR1_PIN1 PA0
#define MOTOR1_PIN2 PA1
#define MOTOR2_PIN1 PA2
#define MOTOR2_PIN2 PA3

// Radio
RH_ASK driver(2000, RX_PIN, -1);

// Message struct (must match transmitter!)
struct ControlMessage {
  uint8_t pairing;
  bool forward;
  uint8_t speed;     // 0–255
  int8_t steering;   // -100..100
};

const uint8_t MY_PAIRING = 0x42;

// Deadbands
const int SPEED_DEADBAND = 15;    // joystick rest tolerance for forward/back
const int STEER_DEADBAND = 25;    // larger deadband for steering

// Motors
MX1508_Simple motorLeft(MOTOR1_PIN1, MOTOR1_PIN2, false);
MX1508_Simple motorRight(MOTOR2_PIN1, MOTOR2_PIN2, true); // inverted

void setup() {
  driver.init();
}

void loop() {
  uint8_t buf[sizeof(ControlMessage)];
  uint8_t buflen = sizeof(ControlMessage);

  if (driver.recv(buf, &buflen)) {

    ControlMessage msg;
    memcpy(&msg, buf, sizeof(msg));
    int leftPWM = 0;
    int rightPWM = 0;

    if (msg.pairing == MY_PAIRING) {
      int minPWM = 120; // no movement below this setpoint
      if (msg.speed != 0) {
        msg.speed = map(msg.speed, 0, 255, minPWM, 255);
        int axialSpeedSetpoint = msg.forward ? msg.speed : -msg.speed;

        if (msg.steering == 0) {
          // Pure forward/backward
          leftPWM = axialSpeedSetpoint;
          rightPWM = axialSpeedSetpoint;
        } else {

        }
      } else {
        if (msg.steering != 0) {
          // Pure rotation (ignore forward/backward)
          int spinSpeed = 255;
          if (msg.steering > 0) {
            // rotate right
            leftPWM = spinSpeed;
            rightPWM = -spinSpeed;
          } else {
            // rotate left
            leftPWM = -spinSpeed;
            rightPWM = spinSpeed;
          }
        }
      }

      motorLeft.motorGo(leftPWM);
      motorRight.motorGo(rightPWM);
    }
  }
}
