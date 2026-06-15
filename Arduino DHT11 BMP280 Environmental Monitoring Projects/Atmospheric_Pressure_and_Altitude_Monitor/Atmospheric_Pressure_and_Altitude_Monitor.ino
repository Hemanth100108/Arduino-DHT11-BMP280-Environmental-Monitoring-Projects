#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_BMP280 bmp;  // I2C BMP280

void setup() {
  Serial.begin(9600);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed!");
    while (1);
  }

  // Initialize BMP280
  if (!bmp.begin(0x76)) {   // Change to 0x77 if required
    Serial.println("BMP280 not found!");
    while (1);
  }

  display.clearDisplay();
  display.display();

  Serial.println("BMP280 Altitude & Pressure Monitor");
  Serial.println("--------------------------------");
}

void loop() {
  // Read sensor values
  float pressure = bmp.readPressure() / 100.0;   // Convert Pa to hPa
  float altitude = bmp.readAltitude(1013.25);    // Sea-level pressure

  // ----- Serial Monitor -----
  Serial.print("Pressure: ");
  Serial.print(pressure, 1);
  Serial.println(" hPa");

  Serial.print("Altitude: ");
  Serial.print(altitude, 1);
  Serial.println(" m");

  Serial.println("--------------------");

  // ----- OLED Display -----
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Title
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("BMP280 Monitor");

  // Pressure
  display.setCursor(0, 16);
  display.print("P:");
  display.print(pressure, 1);
  display.println(" hPa");

  // Altitude
  display.setCursor(0, 32);
  display.print("A:");
  display.print(altitude, 1);
  display.println(" m");

  // Vertical altitude indicator
  int barHeight = map((int)altitude, 0, 200, 0, 40);
  barHeight = constrain(barHeight, 0, 40);

  display.drawRect(112, 12, 12, 40, SSD1306_WHITE);
  display.fillRect(113, 52 - barHeight, 10, barHeight, SSD1306_WHITE);

  display.display();

  delay(1000); // Update every second
}