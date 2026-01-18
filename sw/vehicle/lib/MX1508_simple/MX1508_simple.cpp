#include "MX1508_Simple.h"

MX1508_Simple::MX1508_Simple(uint8_t pin1, uint8_t pin2, bool inverted) {
  _pin1 = pin1;
  _pin2 = pin2;
  _inverted = inverted;
  pinMode(_pin1, OUTPUT);
  pinMode(_pin2, OUTPUT);
}

void MX1508_Simple::motorGo(int pwm) {
  if (_inverted) pwm = -pwm;
  pwm = constrain(pwm, -255, 255);

  if (pwm > 0) {
    analogWrite(_pin1, pwm);
    analogWrite(_pin2, 0);
  } else if (pwm < 0) {
    analogWrite(_pin2, -pwm);
    analogWrite(_pin1, 0);
  } else {
    stop();
  }
}

void MX1508_Simple::stop() {
  analogWrite(_pin1, 0);
  analogWrite(_pin2, 0);
}
