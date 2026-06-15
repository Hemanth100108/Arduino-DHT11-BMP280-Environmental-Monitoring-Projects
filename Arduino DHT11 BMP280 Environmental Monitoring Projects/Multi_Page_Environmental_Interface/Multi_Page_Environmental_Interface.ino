#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define BUTTON_PIN 3

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int page = 1;
bool lastButtonState = HIGH;

void setup() {

  Serial.begin(9600);

  dht.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  if (!bmp.begin(0x76)) {
    lcd.print("BMP280 Error");
    while (1);
  }

  // Splash Screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor Interface");

  lcd.setCursor(2, 1);
  lcd.print("HEMANTH A S");

  delay(3000);
}

void loop() {

  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    page++;

    if (page > 3)
      page = 1;

    delay(200); // debounce
  }

  lastButtonState = currentButtonState;

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  float pressure = bmp.readPressure() / 100.0;
  float altitude = bmp.readAltitude(1013.25);

  lcd.clear();

  // PAGE 1
  if (page == 1) {

    lcd.setCursor(0, 0);
    lcd.print("Pg1 T:");
    lcd.print(temp, 1);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print(hum, 0);
    lcd.print("%");
  }

  // PAGE 2
  else if (page == 2) {

    lcd.setCursor(0, 0);
    lcd.print("Pg2 P:");
    lcd.print((int)pressure);

    lcd.setCursor(0, 1);
    lcd.print("A:");
    lcd.print((int)altitude);
    lcd.print("m");
  }

  // PAGE 3 - Combined Dashboard
  else {

    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print((int)temp);

    lcd.setCursor(7, 0);
    lcd.print("H:");
    lcd.print((int)hum);

    lcd.setCursor(0, 1);
    lcd.print("P:");
    lcd.print((int)pressure);

    lcd.setCursor(9, 1);
    lcd.print("A:");
    lcd.print((int)altitude);
  }

  delay(300);
}