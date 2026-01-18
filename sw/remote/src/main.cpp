#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

// Radio
RH_ASK driver(2000, -1, 12); // bitrate, rx_pin (not used), tx_pin
// Note: Uno TX = D11, RX not needed

// Joystick pins
#define JOY_X A0
#define JOY_Y A1

// Deadbands
const int X_DEADBAND = 200; // left/right
const int Y_DEADBAND = 200; // forward/backward

// Message struct
struct ControlMessage {
  uint8_t pairing;
  bool forward;
  uint8_t speed;     // 0–255
  int8_t steering;   // -100..100
};

const uint8_t MY_PAIRING = 0x42;

void setup() {
  // Serial.begin(9600);
  driver.init();
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);
}

void loop() {
  int rawX = analogRead(JOY_X); // 0-1023
  int rawY = analogRead(JOY_Y);

  // Center around 0
  int x = rawX - 512;
  int y = rawY - 512;

  // Apply deadbands
  if (abs(x) < X_DEADBAND) x = 0;
  if (abs(y) < Y_DEADBAND) y = 0;

  ControlMessage msg;
  msg.pairing = MY_PAIRING;

  // Forward/backward
  if (y >= 0) {
    msg.forward = true;
    msg.speed = map(y, 0, 511, 0, 255);
  } else {
    msg.forward = false;
    msg.speed = map(-y, 0, 512, 0, 255);
  }

  // Steering
  msg.steering = map(x, -512, 511, -100, 100);

  // Debug
  // Serial.print("Y=");
  // Serial.print(y);
  // Serial.print(" -> speed=");
  // Serial.print(msg.speed);
  // Serial.print(" forward=");
  // Serial.print(msg.forward);
  // Serial.print(" | X=");
  // Serial.print(x);
  // Serial.print(" -> steer=");
  // Serial.println(msg.steering);

  // Send message
  driver.send((uint8_t*)&msg, sizeof(msg));
  driver.waitPacketSent();

  delay(200);
}
