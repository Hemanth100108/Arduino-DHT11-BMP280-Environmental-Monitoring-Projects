#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define DHTPIN 2
#define DHTTYPE DHT11

#define GREEN_LED 8
#define YELLOW_LED 9
#define RED_LED 10

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
}

void loop() {

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    return;
  }

  String status;

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  if (hum > 80) {
    status = "HUMID";
    digitalWrite(RED_LED, HIGH);
  }
  else if (temp > 30) {
    status = "HOT";
    digitalWrite(YELLOW_LED, HIGH);
  }
  else {
    status = "COMFORT";
    digitalWrite(GREEN_LED, HIGH);
  }

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.print("Temp: ");
  display.print(temp);
  display.println(" C");

  display.setCursor(0,15);
  display.print("Humidity: ");
  display.print(hum);
  display.println("%");

  display.setCursor(0,30);
  display.print("Status:");

  display.setCursor(0,42);
  display.setTextSize(2);
  display.println(status);

  // Thermometer-style bar
  int barLength = map(temp, 0, 50, 0, 120);

  display.drawRect(4, 56, 120, 6, SSD1306_WHITE);
  display.fillRect(4, 56, barLength, 6, SSD1306_WHITE);

  display.display();

  delay(2000);
}