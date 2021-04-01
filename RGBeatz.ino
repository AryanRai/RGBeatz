//INCLUDE WALA PART

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "OTABlynkCredentials.h"

#ifdef ESP8266
#include <BlynkSimpleEsp8266.h>
#elif defined(ESP32)
#include <BlynkSimpleEsp32.h>
#else
#error "Board not found"
#endif

#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include "FastLED.h"

#include <ESP8266WiFi.h>
#include <FS.h>
#include <CertStoreBearSSL.h>
#include <ESP_OTA_GitHub.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

credentials Credentials;
BearSSL::CertStore certStore;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//VARIABLES

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char auth_token[33];
bool connected_to_internet = 0;
const int Erasing_button = 0;


//Provide credentials for your ESP server
char* esp_ssid = "RGBeatz";
char* esp_pass = "";

String ssid = "";
String pass = "";



#define NUM_LEDS 500
#define DATA_PIN 2
#define PIN D2
#define BLYNK_PRINT Serial

int NUMPIXELS = 60;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int pattern_delay;
int R;
int G;
int B;
int randR;
int randG;
int randB;
int lol = 0;
int bot = 0;

int CURRENTPIXELPLUS;
int CURRENTPIXELMINUS;
int LEDSEGMENTS;

String RGBmode;
String http_blynk;
int responselength;

int numCerts;
String RGBeatz_error;

uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;
CRGB leds[NUM_LEDS];



/* Set up values for your git repository and binary names */
#define GHOTA_USER "AryanRai"
#define GHOTA_REPO "RGBeatz_firmware_beta"
#define GHOTA_CURRENT_TAG "0.0.0"
#define GHOTA_BIN_FILE "RGBeatz.bin"
#define GHOTA_ACCEPT_PRERELEASE 0

ESPOTAGitHub ESPOTAGitHub(&certStore, GHOTA_USER, GHOTA_REPO, GHOTA_CURRENT_TAG, GHOTA_BIN_FILE, GHOTA_ACCEPT_PRERELEASE);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//VOID RESET RGBEATZ

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void RESETRGBEATZ(){
      
      bot = bot + 1;
      Serial.println("my time has come");
      Serial.println("erasing");
      Serial.println("BYE :( its all your fault");
      Serial.println("-WHAT WAS THAT");
      Serial.println("~DEATH");
      Serial.println("-WHAT TYPE?");
      Serial.println("~INSTANT");
      Serial.println("-b-b-but there was mo sound he just died!");
      Serial.println("bot was not the imposter");
      delay(100);
      Credentials.Erase_eeprom();  
      //Credentials.setupAP(esp_ssid, esp_pass);
      Credentials.setupAP(esp_ssid, esp_pass);

      
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//BLYNK FUNCTIONS

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BLYNK_WRITE(V0)
{
//Serial.println("blynk_write");  

R =  param.asInt();
//G = param[1].asInt();
//B = param[2].asInt();
 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BLYNK_WRITE(V1)
{
  G =  param.asInt(); // assigning incoming value from pin V1 to a variable

 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BLYNK_WRITE(V2)
{
  B =  param.asInt(); // assigning incoming value from pin V2 to a variable
 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BLYNK_WRITE(V3)
{
  lol =  param.asInt(); // assigning incoming value from pin V3 to a variable
 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BLYNK_WRITE(V4)
{
bot =  param.asInt(); 
}  

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//BLYNK_APP_CONNECTED
/////////////////////////////////////////////////////////////////////////////////////////////

BLYNK_APP_CONNECTED() {
 Serial.println("Blynk App Connected");
 Serial.println("more like RGBeatz connected");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//MUSICSETUP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void usb_music_setup(){
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
  //music_loop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//USBMUSICLOOP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void usb_music_loop() {
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
               if(bot < 1023) {
                
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// COMPLICATED VARIBLES AND PTA NI KYA KYA
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// byte wheel
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

byte * Wheel(byte WheelPos) {
  static byte c[3];
  //Serial.println("wheel");
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }
    
  //Serial.println(c);
  return c;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//gitupdater

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//STARTUP ANIMATION

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void startup_animation() {

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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// setPixel
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setPixel(int Pixel, byte red, byte green, byte blue) {
   pixels.setPixelColor(Pixel, pixels.Color(red, green, blue)); //use pixels.begin caz strip isnt ours
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// showStrip
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void showpixels() {
   pixels.show();
 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//RGBBLYNKCOLORFADINGOUTWARD

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RGBBLYNKCOLORFADINGOUTWARD(){
      Serial.println("RGBBLYNKCOLORFADINGOUTWARD");
      Serial.println("YEETTT");      
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


      for(CURRENTPIXELPLUS = NUMPIXELS/2;CURRENTPIXELPLUS < NUMPIXELS;CURRENTPIXELPLUS++,CURRENTPIXELMINUS--){
      
        pixels.setPixelColor(CURRENTPIXELPLUS, pixels.Color(R,G,B));// do something repetitive 200 times
        pixels.setPixelColor(CURRENTPIXELMINUS, pixels.Color(R,G,B));
        pixels.show();
        delay(40);
        //Serial.print(CURRENTPIXELMINUS);
        }

      if (CURRENTPIXELPLUS >= NUMPIXELS ) {
        //Serial.print("reset");
        CURRENTPIXELPLUS = NUMPIXELS/2;
        CURRENTPIXELMINUS = CURRENTPIXELPLUS - 1;

        }

      for(CURRENTPIXELPLUS = NUMPIXELS/2;CURRENTPIXELPLUS < NUMPIXELS;CURRENTPIXELPLUS++,CURRENTPIXELMINUS--){
      //Serial.print("bot");
        pixels.setPixelColor(CURRENTPIXELPLUS, pixels.Color(0,0,0));// do something repetitive 200 times
        pixels.setPixelColor(CURRENTPIXELMINUS, pixels.Color(0,0,0));
        pixels.show();
        delay(40);
      }



    if (CURRENTPIXELPLUS >= NUMPIXELS ) {
      Serial.print("reset");
      CURRENTPIXELPLUS = NUMPIXELS/2;
      CURRENTPIXELMINUS = CURRENTPIXELPLUS - 1;

      }
      
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//RGBRANDOMCOLORFADINGOUTWARD

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RGBRANDOMCOLORFADINGOUTWARD(){
      Serial.println("RGBRANDOMCOLORFADINGOUTWARD");
      Serial.println("YEETTT");
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

      randR = random(0, 255);
      randG = random(0, 255);
      randB = random(0, 255);

      for(CURRENTPIXELPLUS = NUMPIXELS/2;CURRENTPIXELPLUS < NUMPIXELS;CURRENTPIXELPLUS++,CURRENTPIXELMINUS--){
      //Serial.print("bot");
        pixels.setPixelColor(CURRENTPIXELPLUS, pixels.Color(randR,randG,randB));// do something repetitive 200 times
        pixels.setPixelColor(CURRENTPIXELMINUS, pixels.Color(randR,randG,randB));
        pixels.show();
        delay(40);     
       }

    if (CURRENTPIXELPLUS >= NUMPIXELS ) {
        CURRENTPIXELPLUS = NUMPIXELS/2;
        CURRENTPIXELMINUS = CURRENTPIXELPLUS - 1;

      }

      for(CURRENTPIXELPLUS = NUMPIXELS/2;CURRENTPIXELPLUS < NUMPIXELS;CURRENTPIXELPLUS++,CURRENTPIXELMINUS--){      
        pixels.setPixelColor(CURRENTPIXELPLUS, pixels.Color(0,0,0));// do something repetitive 200 times
        pixels.setPixelColor(CURRENTPIXELMINUS, pixels.Color(0,0,0));
        pixels.show();
        delay(40);
      }



    if (CURRENTPIXELPLUS >= NUMPIXELS ) {
      Serial.print("reset");
      CURRENTPIXELPLUS = NUMPIXELS/2;
      CURRENTPIXELMINUS = CURRENTPIXELPLUS - 1;
      randR = 0;
      randG = 0;
      randB = 0;

      }
           
   } 
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//VOID RGBFORLIFE

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RGBFORLIFE(int SpeedDelay){
  
  byte *c;
  uint16_t i, j;
  Serial.println("RGBFORLIFE");
  Serial.println("YEETTT");

  for(j=0; j<256*1; j++) { // 1 cycles of all colors on wheel
    for(i=0; i< NUMPIXELS; i++) {                                               /////////////useee NUMPIXELS NOT NUM_LED
      c=Wheel(((i * 256 / NUMPIXELS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showpixels(); 
    delay(SpeedDelay);
  }
 
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//VOID SOLIDBLYNKCOLOR

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void SOLIDBLYNKCOLOR(){
      Serial.println("SOLIDBLYNKCOLOR");
      Serial.println("YEETTT");      
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
      delay(50);

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//VOID SETUP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(){


  
  pinMode(Erasing_button, INPUT);
  lol = 0;

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


  
    // Start serial for debugging (not used by library, just this sketch).
  Serial.begin(115200);

  Serial.println();
  Serial.println("================================================================================");
  Serial.println("|                                                                              |");
  Serial.println("|                Welcome to the ESP GitHub OTA Update Showcase                 |");
  Serial.println("|                =============================================                 |");
  Serial.println("|                                                                              |");
  Serial.println("|    Authur:     Gavin Smalley                                                 |");
  Serial.println("|    Repository: https://github.com/yknivag/ESP_OTA_GitHub_Showcase/           |");
  Serial.println("|    Version:    0.0.0                                                         |");
  Serial.println("|    Date:       17th January 2020                                             |");
  Serial.println("|                                                                              |");
  Serial.println("================================================================================");
  Serial.println();
  Serial.println();

  // Start SPIFFS and retrieve certificates.
  SPIFFS.begin();
  int numCerts = certStore.initCertStore(SPIFFS, PSTR("/certs.idx"), PSTR("/certs.ar"));
  Serial.print(F("Number of CA certs read: "));
  Serial.println(numCerts);
  if (numCerts == 0) {
    Serial.println(F("No certs found. Did you run certs-from-mozill.py and upload the SPIFFS directory before running?"));
    return; // Can't connect to anything w/o certs!
  }

  // Connect to WiFi
  Serial.print("Connecting to WiFi... ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if ((WiFi.status() != WL_CONNECTED)) {
    Serial.print("... ");
  }
  Serial.println();

    /* This is the actual code to check and upgrade */
    Serial.println("Checking for update...");
    if (ESPOTAGitHub.checkUpgrade()) {
    Serial.print("Upgrade found at: ");
    Serial.println(ESPOTAGitHub.getUpgradeURL());
    if (ESPOTAGitHub.doUpgrade()) {
      Serial.println("Upgrade complete."); //This should never be seen as the device should restart on successful upgrade.
    } else {
      Serial.print("Unable to upgrade: ");
      Serial.println(ESPOTAGitHub.getLastError());
    }
    } else {
    Serial.print("Not proceeding to upgrade: ");
    Serial.println(ESPOTAGitHub.getLastError());
    }
    /* End of check and upgrade code */

  // Your setup code goes here
    /* End of check and upgrade code */


  

  if (connected_to_internet)
  {
     Serial.println("connected to internet");   
     pixels.begin();
    
     if (numCerts == 0) {
    Serial.println(F("No certs found. Did you run certs-from-mozill.py and upload the SPIFFS directory before running?"));
     // Can't connect to anything w/o certs!
    Serial.println(F("continuing without certs no updates will be possible without certs"));
    RGBeatz_error = "certz-error";
    }
    
     while (numCerts > 0) {
    Serial.println("certs found");
    Serial.println("starting git updater");
    
    
  


    break;
    }  
    
    
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
                
                      startup_animation();
   
                          }
                                 
                        //Write the setup part of your code
    
                  }

            }

      }

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//VOID LOOP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
   Blynk.run();
   Credentials.server_loops();
   
  if (connected_to_internet)
  {
  


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MODES AND FUNCTIONS AND ANIMATIONS AND ...
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
//RESET RGBEATZ
/////////////////////////////////////////////////////////////////////////////////////////    
if (lol == 10000){
     //RESETRGBEATZ();
     bot = bot + 1;
      Serial.println("my time has come");
      Serial.println("erasing");
      Serial.println("BYE :( its all your fault");
      Serial.println("-WHAT WAS THAT");
      Serial.println("~DEATH");
      Serial.println("-WHAT TYPE?");
      Serial.println("~INSTANT");
      Serial.println("-b-b-but there was mo sound he just died!");
      Serial.println("bot was not the imposter");
      delay(100);
      Credentials.Erase_eeprom();  
      ESP.restart();
      //Credentials.setupAP(esp_ssid, esp_pass);
      
 }

 //////////////////////////////////////////////////////////////////////////////////////////////////
//RESET FROM HARDWARE
/////////////////////////////////////////////////////////////////////////////////////////   

if (digitalRead(Erasing_button) == LOW)
  {
    Credentials.Erase_eeprom();
    ESP.restart();
  }
 

 
/////////////////////////////////////////////////////////////////////////////////////////////
//CHANGE TO SOLIDBLYNKCOLOR 
/////////////////////////////////////////////////////////////////////////////////////////////

if (lol == 0){
SOLIDBLYNKCOLOR();      
}


/////////////////////////////////////////////////////////////////////////////////////////  
//CHANGE TO USBMUSIC
/////////////////////////////////////////////////////////////////////////////////////////      
 if (lol == 1023){
      Serial.println("switch");
      Serial.println("USBMUSIC");
      Serial.println("YEETTT");
      delay(100);
      usb_music_setup();
      usb_music_loop();      
 }


///////////////////////////////////////////////////////////////////////////////////////
//CHANGE TO RGBBLYNKCOLORFADINGOUTWARD
//////////////////////////////////////////////////////////////////////////////////////
if (lol == 30){            
   RGBBLYNKCOLORFADINGOUTWARD();      
} 

 
/////////////////////////////////////////////////////////////////////////////////////////  
//CHANGE TO RGBRANDOMCOLORFADINGOUTWARD
/////////////////////////////////////////////////////////////////////////////////////////      
 if (lol == 60){
      RGBRANDOMCOLORFADINGOUTWARD();       
 }

  
/////////////////////////////////////////////////////////////////////////////////////////  
//CHANGE TO RGBFORLIFE
/////////////////////////////////////////////////////////////////////////////////////////      
 if (lol == 90){
      pattern_delay = 10; // pattern_delay future idea, make pattern_delay controlable using blynk
      RGBFORLIFE(pattern_delay);   
 } 
   
    } 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOOP END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
