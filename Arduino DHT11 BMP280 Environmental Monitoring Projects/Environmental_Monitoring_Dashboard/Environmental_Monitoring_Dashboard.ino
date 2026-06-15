#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;

// Common LCD addresses: 0x27 or 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);

  dht.begin();

  lcd.init();
  lcd.backlight();

  if (!bmp.begin(0x76)) {   // Change to 0x77 if needed
    lcd.clear();
    lcd.print("BMP280 Error");
    while (1);
  }

  // Splash Screen
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("HEMANTH A S");

  lcd.setCursor(0, 1);
  lcd.print("ENV DASHBOARD");

  delay(3000);
}

void loop() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  float pressure = bmp.readPressure() / 100.0;
  float altitude = bmp.readAltitude(1013.25);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT11 Error");
    return;
  }

  // Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  Serial.println("----------------");

  // Screen 1
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity, 0);
  lcd.print("%");

  delay(2000);

  // Screen 2
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("P:");
  lcd.print(pressure, 0);
  lcd.print("hPa");

  lcd.setCursor(0, 1);
  lcd.print("A:");
  lcd.print(altitude, 1);
  lcd.print("m");

  delay(2000);
}