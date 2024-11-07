
---

# RGBeatz - Smart RGB Strip Controller with Music-Reactive Features

RGBeatz is an open-source project that enables remote control of addressable RGB LED strips through a **NodeMCU** microcontroller connected to the internet. It supports a variety of lighting patterns and can react to audio analytics from Spotify, creating dynamic, music-synchronized lighting. Additionally, RGBeatz uses **Firebase Realtime Database** for seamless communication between a mobile app and the MCU, and it supports user authentication via Google accounts.

## Key Features
- **Remote Control of RGB Strips**: Manage RGB patterns over WiFi from a mobile app.
- **Music-Reactive Lighting**: Synchronize the RGB patterns to beats in music tracks using Spotify's Audio Analytics API (no microphones required).
- **Firebase Realtime Database Integration**: Uses Google Firebase for communication between the mobile app and the MCU.
- **Multiple RGB Patterns**: Supports 5 RGB patterns, with plans to add 20 more.
- **Google Account Authentication**: Secure login with Google to sync preferences and settings.
- **OTA Updates (Upcoming)**: Over-the-air update support will be added soon.

## Hardware Requirements
- **NodeMCU (ESP8266)**: Used as the main controller for the RGB strip.
- **Addressable RGB LED Strip (WS2812B)**: Ensure the strip is connected with the **data pin on digital pin D2** of the NodeMCU.

## Software Requirements
- **Arduino IDE**: For uploading code to the NodeMCU. Download the latest `RGBeatz.ino` from the GitHub repository.
- **RGBeatz Mobile App**: Available on Android and iOS. Download from the GitHub repository [here](https://github.com/AryanRai/RGBeatz_app).
- **Firebase Realtime Database**: The mobile app communicates with the MCU via Firebase, requiring an active internet connection.

## Installation Guide

### Step 1: Hardware Setup
1. Connect the **WS2812B RGB strip** to the **NodeMCU**. Ensure the data pin is connected to **digital pin D2**.
2. Power up the NodeMCU with a compatible power source.

### Step 2: Arduino Code Upload
1. Download the latest version of the `RGBeatz.ino` file.
2. Open `RGBeatz.ino` in the Arduino IDE.
3. Configure your Firebase and WiFi credentials in the code, if necessary.
4. Connect your NodeMCU to your computer and select the appropriate board and port in the Arduino IDE.
5. Upload the code to the NodeMCU.

### Step 3: RGBeatz App Setup
1. Download and install the RGBeatz mobile app from [GitHub](https://github.com/AryanRai/RGBeatz_app).
2. Log in using your **Google account**. This will synchronize your settings and data across devices.
3. Follow the steps in the app to pair your NodeMCU with your account and control your RGB strip.

### Step 4: Running RGBeatz.exe
- If you prefer to control the RGB setup from a desktop, run `RGBeatz.exe`. Note that **Google Chrome** is required.
- This desktop controller is an optional companion tool that complements the mobile app.

## Usage Instructions
1. Once logged in to the app, you’ll be able to control the RGB strip through the mobile interface.
2. To enable **music-reactive features**, ensure that Spotify is playing on your device. The app will analyze the music beats in real-time and adjust the lighting accordingly.
3. Experiment with the 5 available RGB patterns, with more patterns planned in future updates.

## Notes
- **Supported Platforms**: The RGBeatz app is compatible with Android and iOS.
- **Firebase Configuration**: Ensure that the Firebase Realtime Database is set up correctly to allow the mobile app and NodeMCU to communicate.
- **Additional Patterns & OTA Updates**: More patterns and OTA functionality are in development. Check back for updates on GitHub.

## Support and Contributions
RGBeatz is an evolving project, and community contributions are welcome. For issues, suggestions, or feature requests, please visit the [GitHub repository](https://github.com/AryanRai/RGBeatz_app) and submit a pull request or an issue.

--- 

----------



Here's a polished version of the **RGBeatz Arduino Documentation**:

---

--------

## Table of Contents
1. [Overview](#overview)
2. [Dependencies](#dependencies)
3. [Configuration](#configuration)
4. [Core Features](#core-features)
   - LED Patterns
   - WiFi & Firebase Setup
   - System Reset Functions
5. [Usage Example](#usage-example)

---

## Overview

RGBeatz is powered by an **ESP8266/NodeMCU** controller, which manages WS2812B LED strips through WiFi. By connecting to Firebase, it allows remote control via a mobile app, with support for multiple dynamic lighting modes including music reactivity.

## Dependencies

The following libraries are required for RGBeatz:

```cpp
// WiFi & Web Server
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

// Time
#include <NTPClient.h>
#include <WiFiUdp.h>

// LED Control
#include <Adafruit_NeoPixel.h>
#include <FastLED.h>

// Firebase
#include <Firebase_ESP_Client.h>
#include "addons/RTDBHelper.h"

// File System & JSON
#include <FS.h>
#include <ArduinoJson.h>
```

## Configuration

### Hardware Configuration
- **LED Data Pin**: D2 on NodeMCU
- **Default LED Count**: 60 (customizable)

### Firebase Configuration
Define the Firebase URL and authentication key in the code:
```cpp
#define DATABASE_URL "your_database_url"
#define DATABASE_SECRET "your_database_secret"
```

## Core Features

### 1. WiFi Setup
- **Initial Configuration**: Configures through AP mode.
- **Credential Storage**: Saves WiFi credentials in **SPIFFS** for persistence.
- **Auto-Reconnect**: Automatically reconnects if WiFi connection drops.

```cpp
void wifisetup() {
  WiFi.softAP(apssid, appassword);
  server.on("/", handleRoot);
  server.on("/wifiset", getwififromportalwrite);
  server.begin();
}
```

### 2. Firebase Integration
- **Real-Time Synchronization**: Updates LED settings instantly via Firebase.
- **Separate Streams**: Manages color parameters (R, G, B) and mode using the `lol` parameter.

```cpp
void setup() {
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;
  Firebase.begin(&config, &auth);
}
```

### 3. LED Control Modes

#### Music Reactive Mode (`lol = 1023`)
Creates a music-synchronized effect with LEDs lighting outward from the center based on music amplitude.

```cpp
void wifi_music() {
  // Implements music visualization based on amplitude
  // Center-outward effect synchronized with music
}
```

#### Solid Color Mode (`lol = 0`)
Sets the entire LED strip to a single RGB color based on provided R, G, B values.

```cpp
void SOLIDBLYNKCOLOR() {
  // Sets entire strip to single RGB color
}
```

## LED Patterns

The available patterns and modes include:
1. **Solid Color (Mode 0)**: Displays a static color across the strip.
2. **Music Reactive (Mode 1023)**: Creates dynamic, center-outward lighting based on music amplitude.
3. **Color Fading Outward (Mode 30)**: Animates colors from the center outward using configured RGB values.
4. **Random Color Fading (Mode 60)**: Fades to random colors across the strip.
5. **RGB Cycle (Mode 90)**: Continuously cycles through rainbow colors.

## WiFi & Firebase Setup

### Initial WiFi Configuration
1. The device starts in AP mode, creating a WiFi network called "RGBeatz."
2. Connect to the "RGBeatz" AP to set WiFi credentials.
3. Credentials are saved in **SPIFFS**, allowing the device to reconnect after reset.

### Firebase Connection
The Firebase configuration includes a database URL and secret key, which are authenticated during the initial setup.

```cpp
void setup() {
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;
  Firebase.begin(&config, &auth);
}
```

## System Reset Functions

### Soft Reset (`lol = 10022`)
Restarts the device, keeping saved configurations intact.

### Factory Reset (`lol = 10032`)
Resets device settings, clearing WiFi and Firebase credentials from SPIFFS.

```cpp
void RESETRGBEATZ() {
  SPIFFS.remove("/wififirebase.txt");
  ESP.restart();
}
```

## Error Handling
RGBeatz includes checks to ensure stable operation:
- **WiFi Monitoring**: Regular checks for WiFi connectivity.
- **Firebase Connection Validation**: Ensures connection to Firebase is active.
- **SPIFFS Operations**: Verifies file system access for storing/retrieving configurations.

## File System Structure

1. `wififirebase.txt`: Stores WiFi and Firebase credentials.
2. `sysconffile.txt`: Holds system configurations and device-specific UUID.

## App Images

# RGBeatz_app
-Apps for https://github.com/AryanRai/RGBeatz

# Python App
Used to look like this back in the day
# Login UI
![image](https://github.com/user-attachments/assets/939bff89-98f9-4e88-873a-e6be029ba4c9)

Sorry about the background
# Google Login
![Screenshot 2024-11-07 205030](https://github.com/user-attachments/assets/859ad1b8-4b6b-4f83-b708-6e978f828528)
![Screenshot 2024-11-07 204919](https://github.com/user-attachments/assets/5e5a9725-db35-4b9e-8b90-ee8e37a81dd3)
![Screenshot 2024-11-07 205042](https://github.com/user-attachments/assets/b42489e0-8c2b-4cd2-84fe-080126bde355)

# Spotify Login
![Screenshot 2024-11-07 205011](https://github.com/user-attachments/assets/73aa390c-657a-4a1f-801b-1ec9f046daa8)
![Screenshot 2024-11-07 205019](https://github.com/user-attachments/assets/d6873535-25bb-4dd9-b228-8751d69db254)

# Device Pairing Wizard (incomplete)
![Screenshot 2024-11-07 205516](https://github.com/user-attachments/assets/86add7a7-c37f-42e4-a3ab-79ca8b879e56)

# Control Dashboard UI
![Screenshot 2024-11-07 205531](https://github.com/user-attachments/assets/a9eb8df3-4656-4e20-8235-3c78e7fd537a)
![Screenshot 2024-11-07 205535](https://github.com/user-attachments/assets/4fcc28cc-ac8d-403e-9ef4-2efde672e162)
![Screenshot 2024-11-07 205541](https://github.com/user-attachments/assets/22e24f88-417c-429c-b7e7-bc2f3488aee8)

# Firebase Realtime DB struc
![Screenshot 2024-11-07 205634](https://github.com/user-attachments/assets/29e76f8c-717a-4632-8768-3e06df5593a1)

# Realtime value change demo 
![Screenshot 2024-11-07 205644](https://github.com/user-attachments/assets/69004dfe-adc9-4fa0-8e88-88a83545455a)

## Usage Example

Basic setup and loop functions for a 60-LED strip:

```cpp
void setup() {
  Serial.begin(115200);
  checklocalwificreds();
  // WiFi and Firebase setup follows
}

void loop() {
  // Select pattern based on 'lol' value
  if (lol == 0) SOLIDBLYNKCOLOR();
  else if (lol == 1023) wifi_music();
  // Additional patterns...
}
```

---

This documentation outlines the main functionalities of the **RGBeatz Arduino code**, covering the setup, patterns, and modes available. For further setup instructions and troubleshooting, please visit the [RGBeatz GitHub repository](https://github.com/AryanRai/RGBeatz_app).
