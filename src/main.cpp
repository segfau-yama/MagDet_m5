#include <WiFi.h>
#include <M5Stack.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define MY_SSID "yamamoto"
#define MY_PASS "suiki32"
#define URL "http://192.168.0.219:54321"
#define PIN1 19

StaticJsonDocument<200> availability;

bool old_lock = NULL;

void setup()
{
  pinMode(PIN1, INPUT);
  M5.begin();
  M5.Lcd.setTextSize(2);
  WiFi.begin(MY_SSID, MY_PASS);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.drawCentreString("connecting", 160, 100, 4);

  while (WiFi.status() != WL_CONNECTED)
  {
    // M5.Lcd.print(".");
    delay(100);
  }
  M5.Lcd.clear(BLACK);
  M5.Lcd.drawCentreString("connected", 160, 100, 4);
  delay(1000);
}

void loop()
{
  String output = "";
  bool new_lock = old_lock;
  HTTPClient http;
  http.begin(URL);
  M5.Lcd.setTextSize(3);

  new_lock = digitalRead(PIN1);
  if (old_lock != new_lock)
  {
    availability["room"] = 1;
    availability["lock"] = new_lock;
    serializeJson(availability, output);
    if (availability["lock"])
    {
      M5.Lcd.clear(BLACK);
      M5.Lcd.setTextColor(RED);
      M5.Lcd.drawCentreString("CLOSE", 160, 95, 4);
    }
    else
    {
      M5.Lcd.clear(BLACK);
      M5.Lcd.setTextColor(BLUE);
      M5.Lcd.drawCentreString("OPEN", 160, 95, 4);
    }
    http.POST(output);
  }
  old_lock = new_lock;
  delay(100);
}