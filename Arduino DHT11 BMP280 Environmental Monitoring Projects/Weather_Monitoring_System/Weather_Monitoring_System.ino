#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP280.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_BMP280 bmp;

// Traffic Light LEDs
#define GREEN_LED 8
#define YELLOW_LED 9
#define RED_LED 10

// Custom LCD Characters

// Sunny Icon
byte sun[8] = {
  B00100,
  B10101,
  B01110,
  B11111,
  B01110,
  B10101,
  B00100,
  B00000
};

// Cloud Icon
byte cloud[8] = {
  B00000,
  B00110,
  B01111,
  B11111,
  B11111,
  B01110,
  B00000,
  B00000
};

// Rain Icon
byte rain[8] = {
  B00110,
  B01111,
  B11111,
  B11111,
  B01110,
  B00100,
  B01010,
  B00000
};

void setup() {
  Serial.begin(9600);

  // LED Setup
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // LCD Setup
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, sun);
  lcd.createChar(1, cloud);
  lcd.createChar(2, rain);

  // BMP280 Setup
  if (!bmp.begin(0x76)) {   // Change to 0x77 if your module uses that address
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BMP280 Error");
    while (1);
  }

  // Splash Screen
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Weather System");

  lcd.setCursor(2, 1);
  lcd.print("HEMANTH A S");

  delay(3000);
}

void loop() {

  float pressure = bmp.readPressure() / 100.0; // hPa

  String weather;
  int icon;

  // Turn off all LEDs first
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  // Educational weather approximation
  if (pressure > 1015) {
    weather = "Sunny";
    icon = 0;
    digitalWrite(GREEN_LED, HIGH);
  }
  else if (pressure >= 1000) {
    weather = "Normal";
    icon = 1;
    digitalWrite(YELLOW_LED, HIGH);
  }
  else {
    weather = "Rain";
    icon = 2;
    digitalWrite(RED_LED, HIGH);
  }

  // Serial Monitor
  Serial.print("Pressure: ");
  Serial.print(pressure, 1);
  Serial.println(" hPa");

  Serial.print("Weather: ");
  Serial.println(weather);

  Serial.println("----------------------");

  // LCD Display
  lcd.clear();

  // First Line: Pressure + Icon
  lcd.setCursor(0, 0);
  lcd.print("P:");
  lcd.print((int)pressure);
  lcd.print("hPa");

  lcd.setCursor(15, 0);
  lcd.write(icon);

  // Second Line: Weather Status
  lcd.setCursor(0, 1);
  lcd.print("Weather:");
  lcd.print(weather);

  delay(1000);
}