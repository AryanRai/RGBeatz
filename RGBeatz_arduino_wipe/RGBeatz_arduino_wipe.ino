#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 

void setup(){
Serial.begin(115200);

WiFiManager wifiManager;
wifiManager.setBreakAfterConfig(true);

WiFi.disconnect(true);
delay(2000);
ESP.reset();
  
}


void loop(){


}
