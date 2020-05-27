/*
Copyright (c) 2013-2016 Ubidots.
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Made by Mateo Velez - Metavix for Ubidots Inc
*/

#ifndef __UbidotsWiFly_H_
#define __UbidotsWiFly_H_
#include <Arduino.h>
#include <Stream.h>
#include <SoftwareSerial.h>
#include <WiFly.h>

#define WIFLY_AUTH_OPEN        0    // Open (default)  
#define WIFLY_AUTH_WEP         1    // WEP-128
#define WIFLY_AUTH_WPA1        2    // WPA1
#define WIFLY_AUTH_WPA1_2      3    // Mixed-mode WPA1 and WPA2-PSK
#define WIFLY_AUTH_WPA2_PSK    4    // WPA2-PSK
#define WIFLY_AUTH_ADHOC       6    // Ad-hoc, join any Ad-hoc network

#define TX 2
#define RX 3
#define BAUDRATE 9600
#define TCP_CLIENT_DEFAULT_TIMEOUT         30000  // 3s

#define SERVER "industrial.api.ubidots.com"
#define PORT 9010
#define MAX_VALUES 5



typedef struct Value {
  char  *idName;
  float valueName;
  char *ctext;
} Value;

class Ubidots {
 public:
      Ubidots(char* token);
      bool sendAll();
      bool wifiConnection(const char *ssid, const char *phrase, int auth = WIFLY_AUTH_OPEN);
      float getValue(char* id);
      void add(char *variable_id, float value);
      void add(char *variable_id, float value, char* context);
      bool setDatasourceName(char* dsName);
      bool setDatasourceTag(char* dsTag);
 private:
      SoftwareSerial uart = SoftwareSerial(TX, RX);
      char* _token;
      char* _dsTag;
      char* _dsName;
      uint8_t maxValues;
      uint8_t currentValue;
      Value * val;
      float parseValue(String body);
      WiFly _client = WiFly(uart);
};
#endif