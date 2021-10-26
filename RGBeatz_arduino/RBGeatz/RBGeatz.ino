//INCLUDE WALA PART

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <FS.h>

#ifdef ESP8266

//#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>


#elif defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#else
#error "Board not found"
#endif

#include <Arduino.h>
//

//WIFI
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include "ESP8266WiFi.h"
#include "FS.h"


//TIME

#include <NTPClient.h>
#include <WiFiUdp.h>


//LED
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include "FastLED.h"

#include <Firebase_ESP_Client.h>

//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

 
const char *apssid = "RGBeatz";
const char *appassword = "timepass";

const long utcOffsetInSeconds = 0;

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

bool firsttimeconf = false;

StaticJsonDocument<200> localwificonfig;
StaticJsonDocument<200> sysconf;

ESP8266WebServer server(80);


//AsyncWebServer server(80);
WiFiManager wifiManager;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


#define DATABASE_URL "a" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "s"

/* 3. Define the Firebase Data object */
FirebaseData Rfbdo;
FirebaseData Gfbdo;
FirebaseData Bfbdo;
FirebaseData lolfbdo;
//FirebaseData sysconffbdo;
/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

int count = 0;

uint32_t idleTimeForStream = 15000;

unsigned long dataMillis = 0;

const char* devuuid;



#define PIN D2
#define BLYNK_PRINT Serial

int NUMPIXELS = 60;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel();

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

//VOID SETUP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(){

  
  Serial.begin(115200);

  Serial.println();

  
  checklocalwificreds();
  
  const char* wifissid = localwificonfig["wifi"];
  const char* wifipassword = localwificonfig["password"];
  const char* usrfirebaseuuid = localwificonfig["firebaseuuid"];
  
  WiFi.begin(wifissid, wifipassword);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(wifissid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());   
   if (WiFi.status() == WL_CONNECTED)
        {

        
        // Define NTP Client to get time

        timeClient.begin();
        

        Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
      
          /* Assign the certificate file (optional) */
          //config.cert.file = "/cert.cer";
          //config.cert.file_storage = StorageType::FLASH;
      
          /* Assign the database URL and database secret(required) */
        config.database_url = DATABASE_URL;
        config.signer.tokens.legacy_token = DATABASE_SECRET;
      
          //Firebase.reconnectWiFi(true);
      
          /* Initialize the library with the Firebase authen and config */
        Firebase.begin(&config, &auth);


        checksysconf();

        devuuid = sysconf["uuid"];
        Serial.println(devuuid);

        std::string devicedatapath = std::string("/users/") + usrfirebaseuuid + "/devices/" + devuuid + "/";

        if (firsttimeconf == true) {
        Serial.println("abc");
        Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&lolfbdo, (devicedatapath + std::string("data/R")).data(), 0) ? "ok" : lolfbdo.errorReason().c_str());
        Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&lolfbdo, (devicedatapath + std::string("data/G")).data(), 0) ? "ok" : lolfbdo.errorReason().c_str());
        Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&lolfbdo, (devicedatapath + std::string("data/B")).data(), 0) ? "ok" : lolfbdo.errorReason().c_str());
        Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&lolfbdo, (devicedatapath + std::string("data/lol")).data(), 0) ? "ok" : lolfbdo.errorReason().c_str());
        Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&lolfbdo, (devicedatapath + std::string("leds")).data(), NUMPIXELS) ? "ok" : lolfbdo.errorReason().c_str());
        }

        delay(1000);
        Serial.printf("Get int... %s\n", Firebase.RTDB.getInt(&lolfbdo, (devicedatapath + std::string("leds")).data()) ? String(lolfbdo.intData()).c_str() : lolfbdo.errorReason().c_str());
       

        NUMPIXELS = lolfbdo.intData();
        Serial.println("number of leds =");
        Serial.print(NUMPIXELS);
        pixels.updateType(NEO_GRB + NEO_KHZ800);
        pixels.updateLength(NUMPIXELS);
        pixels.setPin(PIN);
        pixels.begin();  
        //pinMode(Erasing_button, INPUT);

        
        if (!Firebase.RTDB.beginStream(&Rfbdo, (devicedatapath + std::string("data/R")).data()))
        Serial.printf("sream begin error, %s\n\n", Rfbdo.errorReason().c_str());

        Firebase.RTDB.setStreamCallback(&Rfbdo, RstreamCallback, streamTimeoutCallback);

        if (!Firebase.RTDB.beginStream(&Gfbdo, (devicedatapath + std::string("data/G")).data()))
        Serial.printf("sream begin error, %s\n\n", Gfbdo.errorReason().c_str());

        Firebase.RTDB.setStreamCallback(&Gfbdo, GstreamCallback, streamTimeoutCallback);

        if (!Firebase.RTDB.beginStream(&Bfbdo, (devicedatapath + std::string("data/B")).data()))
        Serial.printf("sream begin error, %s\n\n", Bfbdo.errorReason().c_str());

        Firebase.RTDB.setStreamCallback(&Bfbdo, BstreamCallback, streamTimeoutCallback);
        
        
        if (!Firebase.RTDB.beginStream(&lolfbdo, (devicedatapath + std::string("data/lol")).data()))
        Serial.printf("sream begin error, %s\n\n", lolfbdo.errorReason().c_str());

        Firebase.RTDB.setStreamCallback(&lolfbdo, lolstreamCallback, streamTimeoutCallback);

        

        /*
        
         //Or use legacy authenticate method
        //Firebase.begin(DATABASE_URL, DATABASE_SECRET);
        if (!Firebase.RTDB.beginStream(&Rfbdo, (devicedatapath + std::string("data/R")).data()))
        Serial.printf("sream begin error, %s\n\n", Rfbdo.errorReason().c_str());
      
        if (!Firebase.RTDB.beginStream(&Gfbdo, (devicedatapath + std::string("data/G")).data()))
        Serial.printf("sream begin error, %s\n\n", Gfbdo.errorReason().c_str());
      
        if (!Firebase.RTDB.beginStream(&Bfbdo, (devicedatapath + std::string("data/B")).data()))
        Serial.printf("sream begin error, %s\n\n", Bfbdo.errorReason().c_str());
      
        if (!Firebase.RTDB.beginStream(&lolfbdo, (devicedatapath + std::string("data/lol")).data()))
        Serial.printf("sream begin error, %s\n\n", lolfbdo.errorReason().c_str());
        
        */
        
        


        lol = 0;
        for (uint8_t t = 4; t > 0; t--) {
          Serial.println(t);
          delay(500);
        }
        Serial.println();         
        startup_animation();
   
        }

        else {
          RESETRGBEATZ();
        }

     

}

void RstreamCallback(FirebaseStream data)
{
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data); //see addons/RTDBHelper.h
  Serial.println();
  R = data.intData();
  Serial.println(R);
}

void GstreamCallback(FirebaseStream data)
{
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data); //see addons/RTDBHelper.h
  Serial.println();
  G = data.intData();
  Serial.println(G);
}

void BstreamCallback(FirebaseStream data)
{
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data); //see addons/RTDBHelper.h
  Serial.println();
  B = data.intData();
  Serial.println(B);
}

void lolstreamCallback(FirebaseStream data)
{
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data); //see addons/RTDBHelper.h
  Serial.println();
  lol = data.intData();
  Serial.println(lol);
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timeout, resuming...\n");
}


void checksysconf() {

  bool result = SPIFFS.begin();
  Serial.println("SPIFFS opened: " + result);
 
  // this opens the file "myfile.txt" in read-mode
  File sysconffile = SPIFFS.open("/sysconffile.txt", "r");
 
  if (!sysconffile) 
  {
    Serial.println("File doesn't exist yet. Creating it");
    sysconfwrite();
    // open the file in write mode
    
  } 
  else 
  {
    // we could open the file
    //int linenumb = 0;
    
    while(sysconffile.available()) 
    {
      //char *myStrings[] = {};
      //read line by line from the file
      String line = sysconffile.readStringUntil('\n');
      String orgline = line;
      Serial.println(line);
      //message = message + "//" + String(linenumb) + "//" + line;
      line.remove(line.indexOf("= "), line.length() - line.indexOf("= "));
      orgline.remove(0, (orgline.length() - (orgline.length() - orgline.indexOf("= "))) + 2);
      orgline.remove(orgline.length() - 1, 1);
      sysconf[line] = orgline;
      //myStrings[linenumb] =  line;
      //linenumb = linenumb + 1;
      
    }
    
 
  }
  sysconffile.close();
  
}



void sysconfwrite() {
  
  while(!timeClient.update()) {
  timeClient.forceUpdate();
  }

  randomSeed(analogRead(0));
  SPIFFS.remove("/sysconf.txt");
  delay(100);
  File sysconffile = SPIFFS.open("/sysconffile.txt", "w");
    if (!sysconffile) 
    {
      Serial.println("file creation failed");
    }
    //write two lines
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime);
    int monthDay = ptm->tm_mday;
    int currentMonth = ptm->tm_mon+1;
    String currentMonthName = months[currentMonth-1];
    int currentYear = ptm->tm_year+1900;
    String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
    
    String randomness = String(random(10000, 99999));
    String creationtimewritable = timeClient.getFormattedTime();
    String uuidwritable = "--t-" + creationtimewritable + "--d-" + currentDate + "--r-" + randomness;
    sysconffile.println("creationtime= " + creationtimewritable);
    sysconffile.println("creationdate= " + currentDate);
    sysconffile.println("uuid= " + uuidwritable);
    sysconffile.close();
    delay(1000);
    firsttimeconf = true;
    
    //devuuid = sysconf["uuid"];
    //const char* usrfirebaseuuid = localwificonfig["firebaseuuid"];
    //Serial.println(devuuid);
    //std::string devicedatapath = std::string("/users/") + usrfirebaseuuid + "/devices/" + devuuid + "/";


        
    delay(1000);

    checksysconf();

}



void checklocalwificreds() {

  bool result = SPIFFS.begin();
  Serial.println("SPIFFS opened: " + result);
 
  // this opens the file "myfile.txt" in read-mode
  File testFile = SPIFFS.open("/wififirebase.txt", "r");
 
  if (!testFile) 
  {
    Serial.println("File doesn't exist yet. Creating it");
    wifisetup();
    // open the file in write mode
    
  } 
  else 
  {
    // we could open the file
    //int linenumb = 0;
    
    while(testFile.available()) 
    {
      //char *myStrings[] = {};
      //read line by line from the file
      String line = testFile.readStringUntil('\n');
      String orgline = line;
      Serial.println(line);
      //message = message + "//" + String(linenumb) + "//" + line;
      line.remove(line.indexOf("= "), line.length() - line.indexOf("= "));
      orgline.remove(0, (orgline.length() - (orgline.length() - orgline.indexOf("= "))) + 2);
      orgline.remove(orgline.length() - 1, 1);
      localwificonfig[line] = orgline;
      //myStrings[linenumb] =  line;
      //linenumb = linenumb + 1;
      
    }
 
  }
  testFile.close();
  
}



void wifisetup() {
  
  WiFi.softAP(apssid, appassword);
 
  Serial.println();
  Serial.print("Server IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("Server MAC address: ");
  Serial.println(WiFi.softAPmacAddress());
 
  server.on("/", handleRoot);
  server.on("/wifiset", getwififromportalwrite);
  server.begin();
 
  Serial.println("Server listening");
  
  while (true){
    server.handleClient();
   }

}

void handleRoot() {

    DynamicJsonDocument doc(512);
    JsonArray data = doc.createNestedArray("wifilist");
    String wifilist = "";
    
    int n = WiFi.scanNetworks();
    Serial.println("Wifi scan ended");
    if (n == 0) {
    Serial.println("no networks found");
    } 
    else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(") ");
      Serial.print(WiFi.SSID(i));// SS
      data.add(WiFi.SSID(i));
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
      
  serializeJson(doc, wifilist);
  server.send(200, "application/json", wifilist);
}


void getwififromportalwrite() {
  SPIFFS.remove("/wififirebase.txt");
  delay(100);
  String ssid = server.arg("ssid");
  String password = server.arg("password");//this lets you access a query param (http://x.x.x.x/action1?value=1)
  String firebaseuuid = server.arg("firebaseuuid");//this lets you access a query param (http://x.x.x.x/action1?value=1)
  Serial.println("recieved wifi");
  Serial.println(ssid);
  Serial.println(password);
  Serial.println(firebaseuuid);
  if (ssid != "" && password.length() >= 8) {
  File testFile = SPIFFS.open("/wififirebase.txt", "w");
    if (!testFile) 
    {
      Serial.println("file creation failed");
    }
    //write two lines
    testFile.println("wifi= " + ssid);
    testFile.println("password= " + password);
    testFile.println("firebaseuuid= " + firebaseuuid);
    testFile.close();
    server.send(200, "text/html", "<h1>Hello from ESP8266 AP!</h1>");
    delay(5000);
  
    ESP.restart();
  }

  else{
  server.send(403, "text/html", "<h1>no hello from ESP8266 AP!</h1>");
  }
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
  Serial.println("-b-b-but there was mo sound he just died!");
  Serial.println("bot was not the imposter");
  delay(100);
  SPIFFS.remove("/wififirebase.txt");
  Serial.println("formatted");
  delay(5000);
  
  ESP.restart();

      
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
      //delay(50);

}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//VOID LOOP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MODES AND FUNCTIONS AND ANIMATIONS AND ...
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



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


//////////////////////////////////////////////////////////////////////////////////////////////////
//RESET RGBEATZ
/////////////////////////////////////////////////////////////////////////////////////////    

  if (lol == 10022){
  ESP.restart();
  }

  if (lol == 10032){
  RESETRGBEATZ();
  }

  if (lol == 10042){
  Serial.println("potato");
  SPIFFS.remove("/sysconffile.txt");
  Serial.println("formatted");
  delay(5000);
  
  ESP.restart();
  }

  if (lol == 10052){
  Serial.println("potato");
  SPIFFS.remove("/sysconffile.txt");
  Serial.println("formatted");
  delay(5000);
  RESETRGBEATZ();
  
  }

 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOOP END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  

  

/*

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
      RESETRGBEATZ();
      
 }

 //////////////////////////////////////////////////////////////////////////////////////////////////
//RESET FROM HARDWARE
/////////////////////////////////////////////////////////////////////////////////////////   

  if (Serial.parseInt() == 22){
  ESP.restart();
  }

  if (Serial.parseInt() == 32){
  RESETRGBEATZ();
  }

  if (Serial.parseInt() == 42){
  Serial.println("potato");
  SPIFFS.remove("/sysconffile.txt");
  Serial.println("formatted");
  delay(5000);
  
  ESP.restart();
  }


/////////////////////////////////////////////////////////////////////////////////////////////
//CHANGE TO SOLIDBLYNKCOLOR 
/////////////////////////////////////////////////////////////////////////////////////////////

  if (Serial.parseInt() == 2){
    FirebaseJson json;
    json.add("lol", 70);
    json.add("num", count);
    Serial.printf("Set json... %s\n\n", Firebase.RTDB.setJSON(&lolfbdo, "/users/XvHP9fovBiZojhxmKiDID3Fcd5V2/devices/--t-06:21:42--d-3932650-2-23--r-32397/data", &json) ? "ok" : lolfbdo.errorReason().c_str());

  }


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
   */
  
