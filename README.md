# Node-esp32s3

Node-esp32s3 implements a communication interface between Node-RED and ESP32s3 microcontrollers.  
It works by sending JSON-formatted messages that trigger predefined functions in the firmware loaded onto the ESPs.  
The MQTT protocol is used to transmit these messages.

## Firmware and Logic Control

On the ESP side, a firmware is installed that handles the interpretation of received messages.  
The entire program logic is managed through JSON-formatted code sent to the ESP via MQTT.

The following example shows a simple LED-on program:  
first, using the standard method (Arduino-style programming), and then how it would be written for Node-esp32s3.

## Example Arduino: Set D0 Pin High to Turn On an LED

Let's suppose we want to set the D0 pin of the NodeMCU to a high logic level in order to turn on a connected LED.

A possible ESP sketch Arduino  to achieve this is the following:

```cpp
void setup() {
  pinMode(D0, OUTPUT);    // set pin D0 as output
  digitalWrite(D0, HIGH); // set pin D0 to 3.3V (turn on the LED)
}

void loop() {
  // nothing to do here
}
```

## Example Raspi-Esp: Set D0 Pin High to Turn On an LED

```json
[
    {
        "cmd": "PM",
        "gpio": 5,
        "mode": 1
    },
    {
        "cmd": "DW",
        "gpio": 5,
        "value": 1
    }
]
```

## Explanation

The first block corresponds to:

pinMode(D1, OUTPUT);

cmd: stands for command.

PM is the abbreviation for PinMode.


gpio: stands for General Purpose I/O.

The value corresponding to pin D1 can be found in the NodeMCU pinout diagram (see Figure 1).


mode: defines the I/O mode of the pin. Possible values are:

| [mode] | [value] | Description                          |
|:--------------|:-----:|--------------------------------------|
| INPUT         | 0     | Sets the pin as input                |
| OUTPUT        | 1     | Sets the pin as output               |
| INPUT_PULLUP  | 2     | Sets the pin as input with pull-up   |

The second block corresponds to:

digitalWrite(D1, HIGH);

cmd: DW stands for digitalWrite.

value: defines the logical state of the pin. Possible values are:

| [value] | State | Description                          |
|:------|:-----:|--------------------------------------|
| 0     | LOW   | Sets the pin to logical LOW (0 Volts) |
| 1     | HIGH  | Sets the pin to logical HIGH (3.3 Volts) |




|functions in NodeMCU|description|msg length (byte)|
|:-------------------|:---:|----------------|
|pinMode(pin, mode)|Configures the pin usage. E.g. input/output, etc.|3|
|digitalRead(pin)|Read the logic state a pin configured as input|2|
|digitalWrite(pin, value)|primitive|2|
|analogWrite(pin, value)|primitive|3|
|attachInterrupt(pin,mode)|primitive|3|
|detachInterrupt(pin)|primitive|2|
|digitalToggle(pin)|user defined|2|
|ping()|user defined |1|
|attachDebounceInterrupt(pin,mode)|user defined|3|

