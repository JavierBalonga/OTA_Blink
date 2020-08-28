#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

//WiFi config
//your ssid
#define ssid "yourSsid"
//your password
#define pass "yourPassword"

//OTA config
//you have to use your server url
#define OTAHost "http://192.168.1.48/firmware/index.php"
//the name of the compiled arduino sketch
#define Version "OTA_Blink.ino.nodemcu.bin"
//the time between updates expressed in milliseconds
#define OTAPeriodicity 10000

ESP8266WiFiMulti WiFiMulti;
void setup() {
  //serial communication begins
  Serial.begin(115200);
  delay(500);

  //Wifi
  WiFiMulti.addAP(ssid, pass);

  //blink setup
  pinMode(2, OUTPUT);
  
}

void loop() {
  //An easy blink
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);             // wait for a second
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);             // wait for a second

  //The update
  static unsigned long t;
  if (millis() > t) {
    t = millis() + OTAPeriodicity;
    OTA();
  }
}

void OTA() {
  // wait for WiFi connection
  if ((WiFiMulti.run() != WL_CONNECTED)) return;
  Serial.print("Checking updates from ");
  Serial.println(OTAHost);
  t_httpUpdate_return ret = ESPhttpUpdate.update(OTAHost, Version);
  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      Serial.println();
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      break;
  }
}
