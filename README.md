# OTA_Blink
This is a project dedicated to making the simplest program that can make OTA (Over The Air) with an esp8266 and a server programmed in php. OTA means that the device can be updated via WiFi.

I did the project because at least for me the examples of the `ESP8266httpUpdate` library did not work.

---
## The project has two sides:
* Arduino Sketch
* Server
---
## Arduino sketch:
### libraries used:
```cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
```
### WiFi configuration
```cpp
//your ssid
#define ssid "yourSsid"
//your password
#define pass "yourPassword"
```
### OTA configuration
```cpp
//you have to use your server url
#define OTAHost "http://192.168.1.48/firmware/index.php"
//the name of the compiled arduino sketch
#define Version "OTA_Blink.ino.nodemcu.bin"
```
The following is optional and depends on how you decide to build your code, if it updates every certain time this may work for you.
```cpp
//the time between updates expressed in milliseconds
#define OTAPeriodicity 10000
```
### The function that updates
```cpp
void OTA()
```
### The Sample code (blink)
```cpp
  //An easy blink
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);             // wait for a second
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);             // wait for a second
```
### And the update logic
```cpp
  //The update
  static unsigned long t;
  if (millis() > t) {
    t = millis() + OTAPeriodicity;
    OTA();
  }
```
---
## Server
### Basic checks
The server does some checks to confirm that the device that is requesting the sketch is adequate:
```php
if(
    $_SERVER['HTTP_USER_AGENT'] != 'ESP8266-http-Update' ||
    !isset($_SERVER['HTTP_X_ESP8266_STA_MAC']) ||
    !isset($_SERVER['HTTP_X_ESP8266_VERSION']) ||
    !isset($_SERVER['HTTP_X_ESP8266_FREE_SPACE']) ||
    !isset($_SERVER['HTTP_X_ESP8266_SKETCH_MD5'])
) {
    header($_SERVER["SERVER_PROTOCOL"].' 403 Forbidden', true, 403);
    echo "only for ESP8266 updater! (header)\n";
    exit();
}
```
### Memory check
A check to see if the device has enough memory
```php
$path = "bin/".$_SERVER['HTTP_X_ESP8266_VERSION'];
if($_SERVER['HTTP_X_ESP8266_FREE_SPACE'] < filesize($path)) {
    header($_SERVER["SERVER_PROTOCOL"].' -100 too less space', true, -100);
    exit();
}
```
note: the compiled sketches have to be in the same folder as the .php file
### Modification check
And a check to see if the sketch that the device already has installed is the latest version of it.
```php
if($_SERVER['HTTP_X_ESP8266_SKETCH_MD5'] == md5_file($path)) {
    header($_SERVER["SERVER_PROTOCOL"].' 304 Not Modified', true, 304);
    exit();
}
```
This is done with the MD5 code.
### The sending the compiled sketch
```php
header($_SERVER["SERVER_PROTOCOL"].' 200 OK', true, 200);
header('Content-Type: application/octet-stream', true);
header('Content-Disposition: attachment; filename='.basename($path));
header('Content-Length: '.filesize($path), true);
header('x-MD5: '.md5_file($path), true);
readfile($path);
```
---
