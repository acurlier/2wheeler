#include <RH_ASK.h>

// Radio : TX sur PA8 (exemple fiable sur Bluepill)
RH_ASK driver(2000, -1, PA8);

// Message struct (packed pour STM32)
struct __attribute__((packed)) ControlMessage {
  uint8_t pairing;
  uint8_t forward;   // 1 = avant, 0 = arrière
  uint8_t speed;     // 0–255
  int8_t steering;   // -100..100
};

const uint8_t MY_PAIRING = 0x42;

// Durées (ms)
const unsigned long MOVE_TIME = 2000;
const unsigned long PAUSE_TIME = 300;

void setup() {
  if (!driver.init()) {
    while (1); // stop si radio KO
  }
}

void loop() {
  ControlMessage msg;
  msg.pairing = MY_PAIRING;
  msg.steering = 0;
  msg.speed = 255;

  // ===== AVANCE =====
  msg.forward = 1;
  driver.send((uint8_t*)&msg, sizeof(msg));
  driver.waitPacketSent();
  delay(MOVE_TIME);

  // ===== PAUSE =====
  msg.speed = 0;
  driver.send((uint8_t*)&msg, sizeof(msg));
  driver.waitPacketSent();
  delay(PAUSE_TIME);

  // ===== RECULE =====
  msg.speed = 255;
  msg.forward = 0;
  driver.send((uint8_t*)&msg, sizeof(msg));
  driver.waitPacketSent();
  delay(MOVE_TIME);

  // ===== PAUSE =====
  msg.speed = 0;
  driver.send((uint8_t*)&msg, sizeof(msg));
  driver.waitPacketSent();
  delay(PAUSE_TIME);
}
