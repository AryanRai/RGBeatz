//INCLUDE WALA PART

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ESP8266

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>


#elif defined(ESP32)
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#else
#error "Board not found"
#endif

#include <Arduino.h>
//

//ELEGANT WIFI
//#include <ESPConnect.h>
#include <EEPROM.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

//LED
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include "FastLED.h"


WiFiManager wifiManager;

//AsyncWebServer server(80);


bool shouldSaveConfig = false;

const unsigned long CONNECT_TIMEOUT = 10; // Wait 10 Seconds  to connect to the real AP before trying to boot the local AP
const unsigned long AP_TIMEOUT = 20; // Wait 20 Seconds in the config portal before trying again the original WiFi creds



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
int amplitude;
int prev_amplitude;

int bar_decrease_factor = 1;
int bar_decrease_counter = 0;

String RGBmode;
String http_blynk;
int responselength;

String RGBeatz_error;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//WIFIMANAGER SETUP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}


#define EEPROM_SALT 12664
typedef struct
{
  int   salt = EEPROM_SALT;
  char blynk_token[33]  = "";
}

WMSettings;
WMSettings blynk;

void eeprom_read()
{
  EEPROM.begin(512);
  EEPROM.get(0, blynk);
  EEPROM.end();
}


void eeprom_saveconfig()
{
  EEPROM.begin(512);
  EEPROM.put(0, blynk);
  EEPROM.commit();
  EEPROM.end();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//VOID RESET RGBEATZ

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void RESETRGBEATZ(){
      
      
      Serial.println("my time has come");
      Serial.println("erasing");
      Serial.println("BYE :( its all your fault");
      Serial.println("-WHAT WAS THAT");
      Serial.println("~DEATH");
      Serial.println("-WHAT TYPE?");
      Serial.println("~INSTANT");
      Serial.println("-b-b-but there was no sound he just died!");
      Serial.println("bot was not the imposter");
      delay(100);
      WiFi.disconnect(true);
      delay(2000);
      ESP.reset();
      delay(2000);
      ESP.restart();
      //Credentials.Erase_eeprom();  
      //Credentials.setupAP(esp_ssid, esp_pass);
      //Credentials.setupAP(esp_ssid, esp_pass);

      
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

//WIFIMUSIC

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void wifi_music() {
  
      
      amplitude = bot/2;
      CURRENTPIXELPLUS = NUMPIXELS/2;
      CURRENTPIXELMINUS = (NUMPIXELS/2) - 1;
      
      
      //Serial.print(amplitude);
      //Serial.println("");


     
      if (amplitude != prev_amplitude) {
      
      if (prev_amplitude < amplitude) {
      //Serial.println("unequal");  
      /*
      for (int x = 0; x <= 60; x++) {
        pixels.setPixelColor(x, pixels.Color(0,0,0));// do something repetitive 200 times
        pixels.show();
        delay(2);
      }
      */
      bar_decrease_counter = 0;
      for (int x = 0; x < amplitude; x++) {
        //Serial.println(x);       
        //Serial.println(CURRENTPIXELPLUS);
        //Serial.println(CURRENTPIXELMINUS);
        pixels.setPixelColor(CURRENTPIXELPLUS, pixels.Color(R,G,B));// do something repetitive 200 times
        pixels.setPixelColor(CURRENTPIXELMINUS, pixels.Color(R,G,B));
        pixels.show();
        CURRENTPIXELPLUS = CURRENTPIXELPLUS + 1;
        CURRENTPIXELMINUS = CURRENTPIXELMINUS - 1;
        delay(5);
        }
      
      
      }
      if (prev_amplitude > amplitude) {
      bar_decrease_counter = 0;
      CURRENTPIXELPLUS = CURRENTPIXELPLUS + prev_amplitude;
      CURRENTPIXELMINUS = CURRENTPIXELMINUS - prev_amplitude;
      //Serial.println("unequal");
      //Serial.println("unequal"); 
      /* 
      for (int x = 0; x <= 60; x++) {
        pixels.setPixelColor(x, pixels.Color(0,0,0));// do something repetitive 200 times
        pixels.show();
        delay(2);
      }
      */
      for (int x = 0; x < prev_amplitude - amplitude; x++) {
        //Serial.println(x);       
        //Serial.println(CURRENTPIXELPLUS);
        //Serial.println(CURRENTPIXELMINUS);
        pixels.setPixelColor(CURRENTPIXELPLUS, pixels.Color(0,0,0));// do something repetitive 200 times
        pixels.setPixelColor(CURRENTPIXELMINUS, pixels.Color(0,0,0));
        pixels.show();
        CURRENTPIXELPLUS = CURRENTPIXELPLUS - 1;
        CURRENTPIXELMINUS = CURRENTPIXELMINUS + 1;
        delay(7);
        }
      }
     }

       if (prev_amplitude == amplitude) {
        //Serial.println("equal");

        CURRENTPIXELPLUS = CURRENTPIXELPLUS + prev_amplitude;
        CURRENTPIXELMINUS = CURRENTPIXELMINUS - prev_amplitude;
        
        //amplitude = amplitude - 1
        bar_decrease_counter = bar_decrease_counter + 1;
        
        if (bar_decrease_counter >= 60) {
          bar_decrease_counter = 60;
        }
        
        for (int x = 0; x < bar_decrease_counter * bar_decrease_factor; x++) {
        //Serial.println(x);       
        //Serial.println(CURRENTPIXELPLUS);
        //Serial.println(CURRENTPIXELMINUS);
        pixels.setPixelColor(CURRENTPIXELPLUS, pixels.Color(0,0,0));// do something repetitive 200 times
        pixels.setPixelColor(CURRENTPIXELMINUS, pixels.Color(0,0,0));
        pixels.show();
        CURRENTPIXELPLUS = CURRENTPIXELPLUS - 1;
        CURRENTPIXELMINUS = CURRENTPIXELMINUS + 1;
        delay(1);
        }
      }
      
      prev_amplitude = amplitude; 
        
  
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
    delay(15);       
      }

    for(int j=0;j<NUMPIXELS;j++){
    pixels.setPixelColor(j, pixels.Color(0,255,0));
    pixels.show();
    delay(15);
      }

    for(int j=0;j<NUMPIXELS;j++){
    pixels.setPixelColor(j, pixels.Color(0,0,255));
    pixels.show();
    delay(15);
      }

    for(int j=0;j<NUMPIXELS;j++){
    pixels.setPixelColor(j, pixels.Color(255,255,0));
    pixels.show();      
    delay(15);
      }

    for(int j=0;j<NUMPIXELS;j++){
    pixels.setPixelColor(j, pixels.Color(0,255,255));
    pixels.show();
    delay(15);
      }

    for(int j=0;j<NUMPIXELS;j++){
    pixels.setPixelColor(j, pixels.Color(255,0,255));
    pixels.show();
    delay(15);
      }

    for(int j=0;j<NUMPIXELS;j++){
    pixels.setPixelColor(j, pixels.Color(0,0,0));
    pixels.show();
    delay(15);
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
      
        pixels.setPixelColor(CURRENTPIXELPLUS, pixels.Color(R,G,B));// do something repetitive 150 times
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

  
  Serial.begin(115200);

  
  //read wifi and blynk and connect
  /****************************************************************/

  eeprom_read();
  if (blynk.salt != EEPROM_SALT)
  {
    Serial.println("Invalid settings in EEPROM, trying with defaults");
    WMSettings defaults;
    blynk = defaults;
  }

  WiFiManagerParameter custom_blynk_token("blynk-token", "Blynk Token", blynk.blynk_token, 33);

  WiFiManager wifiManager;
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.addParameter(&custom_blynk_token);

  //reset settings - for testing
  //wifiManager.resetSettings();
  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  wifiManager.setMinimumSignalQuality();
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setConnectTimeout(CONNECT_TIMEOUT);
  wifiManager.setTimeout(AP_TIMEOUT);

  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("RGBeatz")) {
    Serial.println("failed to connect and hit timeout");
    Serial.println("Reboot Your Device");
    delay(1000);
    ESP.restart();
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  strcpy(blynk.blynk_token, custom_blynk_token.getValue());

  eeprom_saveconfig();

  //end save

  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  Blynk.config(blynk.blynk_token);
  bool result = Blynk.connect();

  if (result != true) {
    Serial.println("Failed To Connect BLYNK Server");
  } else {
    Serial.println("BLYNK Connected");
    Serial.println(blynk.blynk_token);
  }

  
  pixels.begin();
  //Blynk.config(auth_token);
  
  
  
  //pinMode(Erasing_button, INPUT);
  lol = 0;

  for (uint8_t t = 4; t > 0; t--) {
    Serial.println(t);
    delay(500);
  }

  // Press and hold the button to erase all the credentials
  

  

  
    // Start serial for debugging (not used by library, just this sketch).

  

  // Connect to WiFi
  
  if ((WiFi.status() != WL_CONNECTED)) {
    Serial.print("... ");
  }
  Serial.println();

    /* This is the actual code to check and upgrade */
   
  startup_animation();
   
              
      

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//VOID LOOP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
   Blynk.run();
   
  
  


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MODES AND FUNCTIONS AND ANIMATIONS AND ...
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
//RESET RGBEATZ
/////////////////////////////////////////////////////////////////////////////////////////    
if (lol == 10000){
     //RESETRGBEATZ();
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
      //Credentials.Erase_eeprom();  
      WiFi.disconnect(true);
      delay(2000);
      ESP.reset();
      //Credentials.setupAP(esp_ssid, esp_pass);
      
 }

 //////////////////////////////////////////////////////////////////////////////////////////////////
//RESET FROM HARDWARE
/////////////////////////////////////////////////////////////////////////////////////////   
if (Serial.available() > 0) {
    // read the incoming byte:
    int incomingByte;
    
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);

    if (incomingByte == 10){
     //RESETRGBEATZ();
      Serial.println("my time has come");
      Serial.println("erasing");
      Serial.println("BYE :( its all your fault");
      Serial.println("-WHAT WAS THAT");
      Serial.println("~DEATH");
      Serial.println("-WHAT TYPE?");
      Serial.println("~INSTANT");
      Serial.println("-b-b-but there was mo sound he just died!");
      Serial.println("bot was not the imposter");
      WiFi.disconnect(true);

      
      delay(2000);
      ESP.reset();
      delay(1000);
      //Credentials.Erase_eeprom();  
      ESP.restart();
      ;
}
}
/////////////////////////////////////////////////////////////////////////////////////////////
//CHANGE TO SOLIDBLYNKCOLOR 
/////////////////////////////////////////////////////////////////////////////////////////////

if (lol == 0){
SOLIDBLYNKCOLOR();      
}


/////////////////////////////////////////////////////////////////////////////////////////  
//CHANGE TO wifimusic
/////////////////////////////////////////////////////////////////////////////////////////      
 if (lol == 1023){      
      wifi_music(); 
      //Serial.println("music");     
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOOP END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
