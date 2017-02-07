#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <ESP8266HTTPClient.h>    //Local HTTP Client to load external url
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include <EEPROM.h>

//Username: vncW9MBkPuGAc0Uy7hR4fAvW--V01PNGfMnCkDfr
//Lights:   4, 5, 6, 7, 8, 9, 10, 11, 12
//IP:       192.168.1.179

String url = "http://192.168.1.179/api/vncW9MBkPuGAc0Uy7hR4fAvW--V01PNGfMnCkDfr/lights/7/state";

String json_on_b = "{\"on\": true, \"hue\": ";
String json_on_e = "}";
String json_off = "{\"on\": false}";

boolean state;

void setup() {
    Serial.begin(115200);

    // put your setup code here, to run once:
    WiFiManager wifiManager;
    wifiManager.autoConnect("KVE-ACCESS", "asdf1239");
  
    Serial.println("Connected to wifi");

    state = false;
}

void loop() {
  // put your main code here, to run repeatedly:

  HTTPClient http;

  Serial.println("Updating light 7: " + state);
  http.begin(url);

  int code = -1;

  if(state)
    code = http.sendRequest("PUT", json_off);
  else
  {
    int rnd = random(0, 65535);
    code = http.sendRequest("PUT", (json_on_b + rnd + json_on_e));
  }

  state = !state;
  

  if(code == 200)
  {
    Serial.println("HTTP Success");
    String payload = http.getString();

    Serial.println("Payload: " + payload);
  }
  else
     Serial.println("HTTP Failure");

  http.end();

  delay(3000);
}
