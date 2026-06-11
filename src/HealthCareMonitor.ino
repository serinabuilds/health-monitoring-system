#define BLYNK_TEMPLATE_ID "TMPL3aSIIi4kL"
#define BLYNK_TEMPLATE_NAME "MAX30100 Pulse Oximeter"
#define BLYNK_AUTH_TOKEN "AxcwaUWe76e7-5-rECZ5lfMJwcJYwLfk"

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// WiFi credentials
char ssid[] = "Serina";
char pass[] = "12345678";

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pulse oximeter configuration
PulseOximeter pox;
#define HR_REPORT_INTERVAL 1000     // 1 second for HR/SpO2
#define ENV_REPORT_INTERVAL 5000    // 5 seconds for temp/humidity
uint32_t lastHRUpdate = 0;
uint32_t lastEnvUpdate = 0;

// Sensor variables
float heartRate = 0;
float spo2 = 0;
float roomTempC = 0;
float humidity = 0;

// Callback for heartbeat detection
void onBeatDetected() {
  Serial.println("Beat!");
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi and Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

  // Initialize MAX30100 sensor
  if (!pox.begin()) {
    Serial.println("MAX30100 FAILED");
    display.setCursor(0, 10);
    display.println("MAX30100 FAILED");
    display.display();
    while (true);
  } else {
    Serial.println("MAX30100 SUCCESS");
    display.setCursor(0, 10);
    display.println("MAX30100 SUCCESS");
    display.display();
  }

  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  Blynk.run();
  pox.update();

  uint32_t now = millis();

  // Update HR and SpO2 every 1 second
  if (now - lastHRUpdate >= HR_REPORT_INTERVAL) {
    lastHRUpdate = now;

    heartRate = pox.getHeartRate();
    spo2 = pox.getSpO2();

    Blynk.virtualWrite(V0, heartRate);
    Blynk.virtualWrite(V1, spo2);
  }

  // Update dummy temp/humidity every 5 seconds
  if (now - lastEnvUpdate >= ENV_REPORT_INTERVAL) {
    lastEnvUpdate = now;

    roomTempC = random(250, 301) / 10.0;  
    humidity = random(600, 701) / 10.0;   

    Blynk.virtualWrite(V2, roomTempC);
    Blynk.virtualWrite(V3, humidity);
  }

  // Update OLED every loop with latest values
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Pulse Oximeter");

  display.setCursor(0, 12);
  display.print("HR: ");
  display.print((int)heartRate);
  display.println(" bpm");

  display.setCursor(0, 24);
  display.print("SpO2: ");
  display.print((int)spo2);
  display.println(" %");

  display.setCursor(0, 36);
  display.print("Temp: ");
  display.print(roomTempC);
  display.println(" C");

  display.setCursor(0, 48);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  display.display();
}
