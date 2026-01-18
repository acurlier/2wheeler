#pragma once
#include <Arduino.h>

class MX1508_Simple {
  public:
    MX1508_Simple(uint8_t pin1, uint8_t pin2, bool inverted = false);
    void motorGo(int pwm);
    void stop();

  private:
    uint8_t _pin1, _pin2;
    bool _inverted;
};
