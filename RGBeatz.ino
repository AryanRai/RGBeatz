#include "OTABlynkCredentials.h"

#ifdef ESP8266
#include <BlynkSimpleEsp8266.h>
#elif defined(ESP32)
#include <BlynkSimpleEsp32.h>
#else
#error "Board not found"
#endif

credentials Credentials;



//Variables
char auth_token[33];
bool connected_to_internet = 0;
const int Erasing_button = 0;


//Provide credentials for your ESP server
char* esp_ssid = "RGBeatz";
char* esp_pass = "";

//
//
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
//#include <BlynkSimpleEsp8266.h>
//#include <ESP8266WiFi.h>
#include "FastLED.h"


#define NUM_LEDS 500
#define DATA_PIN 2
#define PIN D2
#define BLYNK_PRINT Serial

int NUMPIXELS = 60;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int R;
int G;
int B;
int lol = 0;
int bot = 0;

String RGBmode;
String http_blynk;
int responselength;

uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;
CRGB leds[NUM_LEDS];

BLYNK_WRITE(V0)
{
//Serial.println("blynk_write");  

R =  param.asInt();
//G = param[1].asInt();
//B = param[2].asInt();
//lol = R + G + B;

//if (lol == 0){
//Serial.println("switch");
//music_setup();  
}

BLYNK_WRITE(V1)
{
  G =  param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
}

BLYNK_WRITE(V2)
{
  B =  param.asInt(); // assigning incoming value from pin V1 to a variable
 // Serial.println("ulu");
  // process received value
}

BLYNK_WRITE(V3)
{
  lol =  param.asInt(); // assigning incoming value from pin V1 to a variable
 // Serial.println("ulu");
  // process received value
}

BLYNK_WRITE(V4)
{
bot =  param.asInt(); 
}  //lol =  param.asInt(); // assigning incoming value from pin V1 to a variable
    

  // process received value
//}


//BLYNK_WRITE(V3)
//{
  //bot = param.asInt(); // assigning incoming value from pin V1 to a variable
  //Serial.println("ulu");
  // process received value
//}

void music_setup(){
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUMPIXELS);
  
  // Initial RGB flash
  LEDS.showColor(CRGB(255, 0, 0));
  delay(500);
  LEDS.showColor(CRGB(0, 255, 0));
  delay(500);
  LEDS.showColor(CRGB(0, 0, 255));
  delay(500);
  LEDS.showColor(CRGB(0, 0, 0));
  
  
  // Send "Magic Word" string to host
  Serial.print("Ada\n");
  music_loop();

}



void music_loop() {
    lol:
    //Blynk.run();
    //LEDS.showColor(CRGB(0, 255, 0));
    //delay(500);
    //LEDS.showColor(CRGB(0, 0, 0));
   for(i = 0; i < sizeof prefix; ++i) {
    waitLoop: while (!Serial.available()) ;;
    // Check next byte in Magic Word
    if(prefix[i] == Serial.read()) continue;
    // otherwise, start over
    i = 0;
    goto waitLoop;
  }
  
  // Hi, Lo, Checksum  
  while (!Serial.available()) ;;
  hi=Serial.read();
  while (!Serial.available()) ;;
  lo=Serial.read();
  while (!Serial.available()) ;;
  chk=Serial.read();
  
  // If checksum does not match go back to wait
  if (chk != (hi ^ lo ^ 0x55)) {
    i=0;
    goto waitLoop;
  }
  
  memset(leds, 0, NUMPIXELS * sizeof(struct CRGB));              
  // Read the transmission data and set LED values
  for (uint8_t i = 0; i < NUMPIXELS; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
   // blynkloop();
  }
     bot = 1; 
     String auth;
     auth = String(auth_token);
     HTTPClient http;
     http_blynk = "http://blynk-cloud.com/" + auth + "/get/V3";
     http.begin(http_blynk);
     int httpCode = http.GET();
     

    if(httpCode > 0) {
           if(httpCode == HTTP_CODE_OK) {
             //HTTP_CODE_OK means code == 200
               RGBmode = http.getString();// gives us the message received by the GET Request
               RGBmode.remove(0, 2);
               responselength = RGBmode.length();
               RGBmode.remove(responselength - 2, 2);
               bot = RGBmode.toInt();
               http.end();
               if(bot == 0) {
                
                HTTPClient http;
            http.begin("http://blynk-cloud.com/" + auth + "/update/V3?value=1");  //Specify request destination
            httpCode = http.GET(); 

            if (httpCode > 0) {
            
            String payload = http.getString();   //Get the request response payload
            Serial.println(payload);                     //Print the response payload
            Serial.println("LUL"); 
            }
 
            http.end();
            
            
            Serial.println("transmitting nigga cat");
          /*
           
           for (int i = 0; i < botst.length(); ++i)
          {
            EEPROM.write(128 + i, botst[i]);
            Serial.print("Wrote: ");
            Serial.println(botst[i]);
          }
          EEPROM.commit();
          */
    ESP.restart();
   // blynkloop();
  

              
              }

           }

    }
            
  
  // Shows new values
  FastLED.show();
  goto lol;
}


//void blynkloop()
//{
//Blynk.run();  
//music_loop();
//}




void setup(){

  Serial.begin(115200);
  pinMode(Erasing_button, INPUT);
  

  for (uint8_t t = 4; t > 0; t--) {
    Serial.println(t);
    delay(500);
  }

  // Press and hold the button to erase all the credentials
  if (digitalRead(Erasing_button) == LOW)
  {
    Credentials.Erase_eeprom();

  }

  String auth_string = Credentials.EEPROM_Config();
  auth_string.toCharArray(auth_token, 33);

  if (Credentials.credentials_get())
  {

    Blynk.config(auth_token);
    connected_to_internet = 1;

  }
  else
  {
    Credentials.setupAP(esp_ssid, esp_pass);
    connected_to_internet = 0;
  }


  if (connected_to_internet)
  {
    pixels.begin();
    delay(100);
    
    String auth;
     auth = String(auth_token);
     HTTPClient http;
     http_blynk = "http://blynk-cloud.com/" + auth + "/get/V3";
     http.begin(http_blynk);
     int httpCode = http.GET();
     

    if(httpCode > 0) {
           if(httpCode == HTTP_CODE_OK) {
             //HTTP_CODE_OK means code == 200
               RGBmode = http.getString();// gives us the message received by the GET Request
               RGBmode.remove(0, 2);
               responselength = RGBmode.length();
               RGBmode.remove(responselength - 2, 2);
               bot = RGBmode.toInt();
               http.end();
               
               if(bot == 1) {
                Serial.println("not a restart u khuttu");

                HTTPClient http;
                http.begin("http://blynk-cloud.com/" + auth + "/update/V3?value=0");  //Specify request destination
                httpCode = http.GET(); 

                if (httpCode > 0) {
            
                String payload = http.getString();   //Get the request response payload
                Serial.println(payload);                     //Print the response payload
                Serial.println("LUL"); 
                }
 
                http.end();

                
               }
    
               else {
                Serial.println("a restart u khuttu");
               
    for(int j=0;j<NUMPIXELS;j++){

    pixels.setPixelColor(j, pixels.Color(255,0,0));

    pixels.show();
      
     delay(30);
      }





    for(int j=0;j<NUMPIXELS;j++){

    pixels.setPixelColor(j, pixels.Color(0,255,0));

    pixels.show();
      
     delay(30);
      }


    

    for(int j=0;j<NUMPIXELS;j++){

    pixels.setPixelColor(j, pixels.Color(0,0,255));

    pixels.show();
      
     delay(30);
      }


    for(int j=0;j<NUMPIXELS;j++){

    pixels.setPixelColor(j, pixels.Color(255,255,0));

    pixels.show();
      
     delay(30);
      }





    for(int j=0;j<NUMPIXELS;j++){

    pixels.setPixelColor(j, pixels.Color(0,255,255));

    pixels.show();
      
     delay(30);
      }




    for(int j=0;j<NUMPIXELS;j++){

    pixels.setPixelColor(j, pixels.Color(255,0,255));

    pixels.show();
      
     delay(30);
      }


       for(int j=0;j<NUMPIXELS;j++){

    pixels.setPixelColor(j, pixels.Color(0,0,0));

    pixels.show();
      
     delay(30);
      }

   
     }
    // .  Write the setup part of your code
    //
    
  }



}

  }  

}

void loop()
{
   Blynk.run();
   
  
  Credentials.server_loops();

  if (connected_to_internet)
  {

      
 if (lol == 1023){
      Serial.println("switch");
      Serial.println("to op");
      Serial.println("YEETTT");
      delay(100);
      music_setup();  
      
  } 

if (bot == 69){
      bot = bot + 1;
      Serial.println("my time has come");
      Serial.println("I MUST GO");
      Serial.println("erasing");
      Serial.println("BYE :( its all your fault");
      Serial.println("bot was not the imposter");
      delay(100);
      Credentials.Erase_eeprom();  
      Credentials.setupAP(esp_ssid, esp_pass);
      
  } 
Serial.print("R="); 
Serial.print(R);
Serial.print("G="); 
Serial.print(G);
Serial.print("B="); 
Serial.print(B);
Serial.print("mode="); 
Serial.print(lol);
Serial.print("others="); 
Serial.print(bot);
Serial.println("");

for(int i=0;i<NUMPIXELS;i++){

pixels.setPixelColor(i, pixels.Color(R,G,B));

pixels.show();
}
delay(100);

    
  }
}



BLYNK_APP_CONNECTED() {
 Serial.println("Blynk App Connected");
 Serial.println("more like RGBeatz connected");
}
