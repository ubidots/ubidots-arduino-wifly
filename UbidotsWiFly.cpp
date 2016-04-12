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

#include "UbidotsWiFly.h"
#include <string.h>
#include <SoftwareSerial.h>
#include "WiFly.h"
/**
 * Constructor.
 */
Ubidots::Ubidots(char* token) {
    _token = token;
    _dsName = "WiFly";
    _dsTag = "WiFly_TAG/1.0.0";
    uart.begin(BAUDRATE);         // WiFly UART Baud Rate: 9600
    currentValue = 0;
    val = (Value *)malloc(MAX_VALUES*sizeof(Value));
}
bool Ubidots::setDatasourceName(char* dsName) {
    _dsName = dsName;
    return true;
}
bool Ubidots::setDatasourceTag(char* dsTag) {
    _dsTag = dsTag;
    return true;
}
bool Ubidots::wifiConnection(const char *ssid, const char *phrase, int auth) {
    if (!_client.isAssociated(ssid)) {
        while (!_client.join(ssid, phrase, auth)) {
            Serial.println("Failed to join ");
            Serial.println(F("Wait 0.1 second and try again..."));
            delay(100);
        }
        _client.save();    // save configuration, 
    }
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::parseValue(String body){
  /*String rawvalue;
  float num;
  char reply[15];
  uint8_t bodyPosinit;
  uint8_t bodyPosend;
  bodyPosinit = 4 + body.indexOf("\r\n\r\n");
  rawvalue = body.substring(bodyPosinit);
  bodyPosinit = 9 + rawvalue.indexOf("\"value\": ");
  bodyPosend = rawvalue.indexOf(", \"timestamp\"");
  rawvalue.substring(bodyPosinit,bodyPosend).toCharArray(reply, 10);
  num = atof(reply); 
  return num;*/
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValue(char* id){
  float num;
  int i = 0;
  String raw;
  uint8_t bodyPosinit;
  uint8_t bodyPosend;
  char reply[15];
  if (!_client.connect("things.ubidots.com", 80)) {
        Serial.println("Failed to connect.\r\n");
        return -2;
  }
  Serial.println(F("Geting your variable"));
  // Make a HTTP request:
  _client.send("GET /api/v1.6/variables/");
  _client.send(id);
  _client.send("/values?page_size=1 HTTP/1.1");
  _client.send("\r\n");
  _client.send("Host: things.ubidots.com");
  _client.send("\r\n");
  _client.send("User-Agent: Arduino-Ethernet/1.0"); 
  _client.send("\r\n");
  _client.send("X-Auth-Token: ");
  _client.send(_token);
  _client.send("\r\n");
  _client.send("Connection: close");
  _client.send("\r\n");
  _client.send("\r\n");
  
  while (!_client.available());
  while (_client.available()){
      char c = _client.read();
      raw += c;
      //Serial.write(c);
  }
  Serial.println(raw);
  num = parseValue(raw);     
  return num;
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, float value) {
  add(variable_id, value, NULL);
}
void Ubidots::add(char *variable_id, float value, char* context) {
  (val+currentValue)->idName = variable_id;
  (val+currentValue)->valueName = value;
  (val+currentValue)->ctext = context;
  currentValue++;
  if(currentValue>MAX_VALUES){
    currentValue = MAX_VALUES;
  }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll() {
    int i;
    String all;
    String str;
    all = "WiFly/1.0|POST|";
    all += _token;
    all += "|";
    all += _dsTag;
    all += "=>";
    for (i = 0; i < currentValue; ) {
        str = String(((val + i)->valueName), 5);
        all += String((val + i)->idName);
        all += ":";
        all += str;
        if ((val + i)->ctext != NULL) {
            all += "$";
            all += String((val + i)->ctext);
        }
        i++;
        if(i < currentValue){
            all += ","; 
        }
    }
    all += "|end";
    Serial.println(all.c_str());
    i = 0;
    while (!_client.connected() && i < 6) {
        i++;
        _client.connect(SERVER, PORT);
    }
    if (_client.connected()) {  // Connect to the server
        _client.send(all.c_str());
        currentValue = 0;
    }    
    int c;
    while (!_client.available());
    while (_client.available()) {
      c = _client.read();
      if (c > 0) {
        Serial.write((char)c);
      }
    }
    currentValue = 0;
    return true;
}
