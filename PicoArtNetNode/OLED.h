#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// SSD1306 display connected to I2C (SDA, SCL pins)
// On a RP2040:                 4(SDA), 5(SCL)
#define OLED_RESET -1        // Reset pin # 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define SCREEN_ADDRESS 0x3C  // Note: Some 128x64 displays start at 0x3D

#define WIFI_ICON_WIDTH     16
#define WIFI_ICON_HEIGHT    16
const unsigned char wifi_bmp [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x3e, 0x7c, 0x70, 0x0e, 0xc7, 0xe3, 0x1f, 0xf8, 0x38, 0x1c, 
	0x33, 0xcc, 0x0f, 0xf0, 0x0c, 0x30, 0x01, 0x80, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00
};

class OLED {
  private:
    Adafruit_SSD1306& displayRef;
    int universeA;
    int universeB;
    const char* wifiNetwork;

    void _drawWifiLogo(void) {
      displayRef.drawBitmap(
          (displayRef.width() - WIFI_ICON_WIDTH - 5), 5,
          wifi_bmp, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, 1);
    }

    void _addNetworkName() {
      displayRef.println(wifiNetwork);
      displayRef.println();
    }

    void _displayUniverseConfig() {
      char univA[4];
      char univB[4];
      String str;
      str = String(universeA);
      str.toCharArray(univA, 4);
      str = String(universeB);
      str.toCharArray(univB, 4);

      displayRef.setCursor(0, 42);
      displayRef.write("Output A - Univ ");
      displayRef.write(univA);
      displayRef.setCursor(0, 55);
      displayRef.write("Output B - Univ ");
      displayRef.write(univB);
      displayRef.display();
    }

    void _displayCommonInfo() {
      displayRef.clearDisplay();
      _drawWifiLogo();
      displayRef.drawLine(0, 35, displayRef.width(), 35, SSD1306_WHITE);
      _displayUniverseConfig();
      displayRef.setCursor(0, 5);
    }
  
  public:
    OLED(Adafruit_SSD1306& display, int univA, int univB, const char* ssid) 
      : displayRef(display), universeA(univA), universeB(univB), wifiNetwork(ssid) { }

    void initDisplay() {
      // Start up SSD1306 display
      if(!displayRef.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed
      }
      
      // Configure display
      displayRef.setTextSize(1);
      displayRef.setTextColor(SSD1306_WHITE);
    }

    void displayWifiConnecting() {
      _displayCommonInfo();
      displayRef.write("Connecting...\n\n");
      _addNetworkName();
      displayRef.display();
    }

    void displayWifiError() {
      _displayCommonInfo();
      displayRef.write("Could not connect\n\n");
      _addNetworkName();
      displayRef.display();
    }

    void displayWifiConnected(const char *ipAddress) {
      _displayCommonInfo();
      _addNetworkName();
      displayRef.write(ipAddress);
      displayRef.display();
    }
};


