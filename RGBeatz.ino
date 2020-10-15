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

int R = 10;
int G = 10;
int B = 10;
int lol;

uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;
CRGB leds[NUM_LEDS];



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
  
  // Shows new values
  FastLED.show();
  goto lol;
}



void setup()

{

  Serial.begin(115200);
  pinMode(Erasing_button, INPUT);
  

  for (uint8_t t = 4; t > 0; t--) {
    Serial.println(t);
    delay(1000);
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
    // .  Write the setup part of your code
    //
    
  }



}



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
  
  // process received value
}







void loop()
{
   Blynk.run();
   Serial.println("ulu");
  
  Credentials.server_loops();

  if (connected_to_internet)
  {

   
    lol = R + G + B;
    
    if (lol == 0){
      Serial.println("switch");
      delay(100);
      music_setup();  
      
  }


Serial.println(lol);
Serial.println(R);
Serial.println(G);
Serial.println(B);


for(int i=0;i<NUMPIXELS;i++){

pixels.setPixelColor(i, pixels.Color(R,G,B));

pixels.show();
}
delay(100);

    
  }
}



BLYNK_APP_CONNECTED() {
 Serial.println("Blynk App Connected");
}
