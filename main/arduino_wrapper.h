#ifndef ARDUINO_WRAPPER_H
#define ARDUINO_WRAPPER_H

#include "driver/gpio.h"

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define PWM_LED 0
#define PWM_SERVO 1

void pinMode(gpio_num_t pin, uint8_t mode);
void digitalWrite(gpio_num_t pin, uint8_t level);
void analogWrite20KHz(gpio_num_t pin, uint16_t value); for DC motors
void analogWrite50Hz(gpio_num_t pin, uint16_t value);
void pwmWrite(gpio_num_t pin, uint16_t value, uint8_t pwmtype);

#endif // ARDUINO_WRAPPER_H