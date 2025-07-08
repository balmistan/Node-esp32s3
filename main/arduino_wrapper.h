#ifndef ARDUINO_WRAPPER_H
#define ARDUINO_WRAPPER_H

#include "driver/gpio.h"

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

void pinMode(gpio_num_t pin, int mode);
void digitalWrite(gpio_num_t pin, uint8_t level);
void analogWrite(gpio_num_t pin, uint8_t value);

#endif // ARDUINO_WRAPPER_H