#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

// Traffic Light LEDs
#define GREEN_LED 8
#define YELLOW_LED 9
#define RED_LED 10

// Buzzer
#define BUZZER 7

// Thresholds
#define TEMP_THRESHOLD 30
#define HUM_THRESHOLD 70

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  Serial.begin(9600);

  dht.begin();

  lcd.init();
  lcd.backlight();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  // Splash Screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENV WARNING SYS");

  lcd.setCursor(2, 1);
  lcd.print("HEMANTH A S");

  delay(3000);
  lcd.clear();
}

void loop() {

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DHT11 Error");
    noTone(BUZZER);
    return;
  }

  // Serial Monitor Output
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.println("--------------------");

  // Reset Outputs
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  noTone(BUZZER);

  lcd.clear();

  // HIGH TEMPERATURE ALERT
  if (temp > TEMP_THRESHOLD) {

    digitalWrite(RED_LED, HIGH);

    // High-pitched continuous tone
    tone(BUZZER, 2000);

    lcd.setCursor(0, 0);
    lcd.print("HIGH TEMP");

    lcd.setCursor(0, 1);
    lcd.print("ALERT!");
  }

  // HIGH HUMIDITY ALERT
  else if (hum > HUM_THRESHOLD) {

    digitalWrite(YELLOW_LED, HIGH);

    // Medium-pitched continuous tone
    tone(BUZZER, 1000);

    lcd.setCursor(0, 0);
    lcd.print("HIGH");

    lcd.setCursor(0, 1);
    lcd.print("HUMIDITY!");
  }

  // NORMAL CONDITION
  else {

    digitalWrite(GREEN_LED, HIGH);

    noTone(BUZZER);

    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temp, 1);
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(9, 0);
    lcd.print("NORMAL");

    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print(hum, 0);
    lcd.print("%");
  }

  delay(1000);
}