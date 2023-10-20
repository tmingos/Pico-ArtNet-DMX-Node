# Pi Pico Art-Net/DMX Node

[A DIY Art-Net to DMX node running on the Raspberry Pi Pico W (Step by Step)](https://www.techandtransit.com/lighting/pi-pico-artnet-to-dmx-node/)

This homemade device converts wireless Art-Net DMX packets into two wired DMX512 ouputs.

It uses the PIOs on the Pico's RP2040 to implement the DMX protocol.

This node has two DMX outputs (A and B) each supporting a separate universe (configurable).

The device has an OLED screen (128x64) for visual debugging. It displays WiFi connection status and some configuration settings. See: OLED.h. This code runs whether or not an OLED display is connected.

## Arduino IDE Setup

Add the following url to the Additional Board Manager URLs field in the Arduino IDE Settings/Preferences:
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json


Next, install the RP2040 board in the Arduino IDE. Go to Tools → Board → Boards Manager. Scroll down until you find Raspberry Pi Pico/RP2040 and click on Install.

Be sure to install the following dependencies:

Dependencies:
  - ArtnetWifi by Nathanaël Lécaudé, Stephan Ruloff (Tested with 1.5.1)
  - Pico-DMX by Jostein (Tested with 3.1.0)
  - Adafruit SSD1306 (Tested with 2.5.7)
  - Adafruit GFX Library (Tested with 1.11.9)
  - Adafruit BusIO (Tested with 1.14.5)

---

Created 10/18/2023 by Taylor Mingos