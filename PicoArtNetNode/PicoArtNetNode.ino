/*
  PicoArtNetNode.ino

  An Art-Net DMX node running on the Raspberry Pi Pico W. This device converts wireless Art-Net DMX packets into wired DMX512 ouputs.

  It uses the PIOs on the Pico's RP2040 to implement the DMX protocol.

  This node has two DMX outputs (A and B) each supporting a separate universe (configurable).

  The device has an OLED screen (128x64) for visual debugging. It displays WiFi connection status 
  and some configuration settings. See: OLED.h. This code runs whether or not an OLED display is 
  connected.

  Dependencies:
    - ArtnetWifi by Nathanaël Lécaudé, Stephan Ruloff (Tested with 1.5.1)
    - Pico-DMX by Jostein (Tested with 3.1.0)
    - Adafruit SSD1306 (Tested with 2.5.7)
    - Adafruit GFX Library (Tested with 1.11.9)
    - Adafruit BusIO (Tested with 1.14.5)

  Created 10/18/2023 by Taylor Mingos
*/

#include <Arduino.h>
#include <ArtnetWifi.h>
#include <DmxOutput.h>
#include "OLED.h"

// Configure outputs
#define OUTPUT_A_UNIVERSE 1 // Which universe maps to output A
#define OUTPUT_B_UNIVERSE 2 // Which universe maps to output B
#define OUTPUT_A_GPIO 0
#define OUTPUT_B_GPIO 1

// Configure WiFi
const char* ssid = "your-wifi-ssid";
const char* password = "your-wifi-password";

// Create a universe
#define UNIVERSE_LENGTH 512
uint8_t channels[UNIVERSE_LENGTH + 1];

// Init Art-Net
ArtnetWifi artnet;

// Init two DMX outputs
DmxOutput dmx[2];

// Init OLED display for displaying WiFi status and configuration info
OLED oled(display, OUTPUT_A_UNIVERSE, OUTPUT_B_UNIVERSE, ssid);

// Connect to Wifi
bool connectToWifi(void)
{
  bool connected = true;
  int attempts = 0;

  // Connect to WiFi  
  oled.displayWifiConnecting();
  Serial.print("Connecting");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
    if (attempts > 30){
      connected = false;
      break;
    }
    attempts++;
  }

  // Display WiFi connection result
  if (connected) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP: ");
    Serial.println(IPAddress(WiFi.localIP()));
    oled.displayWifiConnected(WiFi.localIP().toString().c_str());
  } else {
    Serial.println("WiFi Connection Failed");
    oled.displayWifiError();
    delay(3000);
  }
  
  return connected;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  Serial.print("DMX: Univ: ");
  Serial.print(universe, DEC);
  Serial.print(", Data Length: ");
  Serial.print(length, DEC);
  Serial.print(", Sequence: ");
  Serial.print(sequence, DEC);
  Serial.print(", Data: ");

  for (int i = 0; i < length; i++)
  {
    if (i < 16) {
      Serial.print(data[i], DEC);
      Serial.print(" ");
    }
    channels[i + 1] = data[i];
  }

  Serial.println();

  // Send out the universe if it's a universe we care about
  if (universe == OUTPUT_A_UNIVERSE) {
    dmx[0].write(channels, UNIVERSE_LENGTH);
  } else if (universe == OUTPUT_B_UNIVERSE) {
    dmx[1].write(channels, UNIVERSE_LENGTH);
  }

  while (dmx[0].busy() || dmx[1].busy())
  {
    // Do nothing while the DMX frame transmits
  }
}

void setup()
{
  // Set up serial for debug
  Serial.begin(115200);

  oled.initDisplay();

  // Turn on Pico's on-board LED
  // This shows that the Pico is turned on
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Start the DMX outputs
  dmx[0].begin(OUTPUT_A_GPIO);
  dmx[1].begin(OUTPUT_B_GPIO);

  // Set all channels to zero
  for (int i = 1; i < UNIVERSE_LENGTH + 1; i++)
  {
    channels[i] = 0;
  }

  // Connect to WiFi
  while(!connectToWifi()) { }
  
  // This will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
  artnet.begin();
}

void loop()
{
  artnet.read();
}