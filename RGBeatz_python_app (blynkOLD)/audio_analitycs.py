import eel
import spotipy
import json
from spotipy.oauth2 import SpotifyClientCredentials
import requests
import numpy as np
import time
import asyncio
import aiohttp
from cryptography.fernet import Fernet
import os
import sys
import base64
import random

hostport = 6969

key_confirmed = False
param_val = ""

try     :

    import win32gui, win32con;

    frgrnd_wndw = win32gui.GetForegroundWindow();
    wndw_title  = win32gui.GetWindowText(frgrnd_wndw);
    if wndw_title.endswith("py.exe"):
        win32gui.ShowWindow(frgrnd_wndw, win32con.SW_HIDE);

except  :
    pass

client_id = ""
client_secret = ""
google_uid = ""
email = ""
display_name = ""
phone_no = ""
last_spotify_refresh_token = ""
blynk_token = ""

once = 0
prev_track_uri = ""
current_track_uri = ""
numb_leds = 60
#blynk_token = "06dktWNcOZRTF9weMK3siNzB3OirDeJC"
current_sync = False
sync = False
prev_sync = False
key_confirmed = False
token_pg_confirmation = False
access_token = ""
refresh_token = ""
r = ""
g = ""
b = ""

token_refresh_delay = 58 #mins


eel.init('web')



@eel.expose
def get_oauth(raw_auth, access, refresh):    
    global raw_auth_token
    global access_token
    global refresh_token
    global refresh_timer_start
    raw_auth_token = raw_auth
    access_token = access
    refresh_token = refresh
    print(raw_auth_token)
    print("access", access_token)
    print("refersh", refresh_token)
    refresh_timer_start = time.perf_counter()
    #refresh_access_token()
    eel.blynk_tab()


@eel.expose
def confirmation():    
    global confirmation
    confirmation = True
    print(confirmation)


@eel.expose
def creds_from_login_portal(google_uid_param, blynk_token_param, last_spotify_refresh_token_param, email_param, display_name_param, phone_no_param, default_device_param):
    global blynk_token
    global google_uid
    global email
    global display_name
    global phone_no
    global last_spotify_refresh_token
    global refresh_timer_start
    global refresh_token
    global default_device
    
    blynk_token = blynk_token_param
    google_uid = google_uid_param
    email = email_param
    display_name = display_name_param
    phone_no = phone_no_param
    last_spotify_refresh_token = last_spotify_refresh_token_param
    refresh_token = last_spotify_refresh_token
    default_device = default_device_param 
    refresh_timer_start = time.perf_counter()
    

    


def handle_config_creds():
    
    global key
    global key_confirmed
    global confirmation_key
    global client_id
    global client_secret
    global blynk_token
    global google_uid
    global email
    global display_name
    global phone_no
    global last_spotify_refresh_token
    global local_data
    global json_cred
    global default_device
    
    
    
    key = "B?E(H+Mb8x/A?D(Gr4u7x!A%WmZq4t7weThWmYq3KbPeShVm*G-KaPdSz%C*F-Ja6w9z$C&Fp3s6v9y$"    
    try:

        with open(os.path.join(os.environ['USERPROFILE'], "AppData", "Local", "Rgbeatz", "RGBconf.bin"), 'rb') as file_object:
            for line in file_object:
                key = base64.b64decode(line)
            file_object.close()
            cipher_suite = Fernet(key)
        with open(os.path.join(os.environ['USERPROFILE'], "AppData", "Local", "Rgbeatz", "RGBeatz.bin"), 'rb') as file_object:
            for line in file_object:
                encrypted_data = line
            file_object.close()
            
        uncipher_data = (cipher_suite.decrypt(encrypted_data))
        
        json_cred = json.loads(bytes(uncipher_data).decode("utf-8")) #convert to string
        #print(json_cred)
        confirmation_key = json_cred["cipher_key"]
        local_data = json_cred
        #print(confirmation_key, str(key))
        if (confirmation_key != str(key)):
            print("wrong key")
            key = ""

        if (confirmation_key == str(key)):
            print("yay")
            

        
    except IOError:
        print("File not accessible")
        ########################
        print("first time setup")
        eel.sleep(0.5)
        eel.login_portal()
        
        while(len(blynk_token) < 30) and (len(google_uid) < 10):
            eel.sleep(0.5)
            print("ululu")
            
        #########################
        print("getting login data")
        ########################
        print("encrypting")
        key = Fernet.generate_key()

        try:
            os.mkdir(os.path.join(os.environ['USERPROFILE'], "AppData", "Local", "Rgbeatz"))

        except IOError:
            print("folder there")
            
        with open(os.path.join(os.environ['USERPROFILE'], "AppData", "Local", "Rgbeatz", "RGBconf.bin"), 'wb') as file_object:
            file_object.write(base64.b64encode(key))
            file_object.close()
            client_id = base64.b64decode("NGMzOGE1MzQ1MzliNDUxMGI4YmRjNzliNGVjODZmMDA=")
            client_secret = base64.b64decode("MmRiNDFmMmVlNDljNDlhZjg0ZDAxOGRjMWI3NDRlMTc=")
        config_data = json.dumps({
          "name": "Aryan",
          "client_id": str(client_id),
          "client_secret": str(client_secret),
          "cipher_key": str(key),
          "blynk_token": blynk_token,
          "google_uid": google_uid,
          "email": email,
          "display_name": display_name,
          "phone_no": phone_no,
          "last_spotify_refresh_token": last_spotify_refresh_token,
          "default_device": default_device,
        })

        local_data = json.loads(config_data)
        print(client_id, client_secret)
        print(blynk_token)
        encrypt_write(config_data)




def encrypt_write(writable_data):    
    global key
    cipher_suite = Fernet(key)
    ciphered_text = cipher_suite.encrypt(bytes(writable_data,'utf-8'))
    with open(os.path.join(os.environ['USERPROFILE'], "AppData", "Local", "Rgbeatz", "RGBeatz.bin"), 'wb') as file_object:
        file_object.write(ciphered_text)
        file_object.close()

def get_param_local(param):
    global param_val
    global json_cred
    global local_data
    
    param_key = param
    
    try:
        print(param_key)
        param_val = local_data[param_key]
        print(param_val)
        
            
    except KeyError:
            param_val = "error 404 keyword not found"
            print(param_val)


def get_spotify_app_id():
    global client_id
    global client_secret
    global param_val
    global json_cred
    global local_data
    
    try:
        #print(param_key)
        param_val = local_data['client_id']
        print(param_val)
        
            
    except KeyError:
            param_val = "error 404 keyword not found"
            print(param_val)
    
    if(param_val == "error 404 keyword not found"):
        handle_config_creds()
    client_id = (param_val.replace("b", "", 1)).replace("'", "")
    print(client_id)

    try:
        #print(param_key)
        param_val = local_data['client_secret']
        print(param_val)
        
            
    except KeyError:
            param_val = "error 404 keyword not found"
            print(param_val)
            
    if(param_val == "error 404 keyword not found"):
        handle_config_creds()
    client_secret = (param_val.replace("b", "", 1)).replace("'", "")
    print(client_secret)


@eel.expose
def user_creds_main_send():
    global local_data    
    eel.user_creds_main(local_data)

@eel.expose
def get_token_pg_confirmation():
    global token_pg_confirmation
    token_pg_confirmation = True



def spotify_auth():
    global hostname
    global default_device
    global param_val
    global blynk_token
    global token_pg_confirmation
    
    while True:
        print("I'm a thread")
        handle_config_creds()
        get_spotify_app_id()      
        global once
        
        if (once == 0):
            once = 10
            eel.sleep(0.5)
            #eel.get_hostname()            

            #todo redirect eel to get token

            if(len(refresh_token) > 30):
                refresh_access_token()
                eel.sleep(0.5)
                eel.blynk_tab()
            
            if(len(refresh_token) < 7):   
                eel.redirect_for_token()
                eel.sleep(2)
                
                get_param_local('blynk_token')
                blynk_token = param_val
                print("blynks", blynk_token)
                get_param_local('default_device')
                default_device = int(param_val)
                print("dev", default_device)

                while (token_pg_confirmation == False):
                    eel.sleep(0.1)

                eel.req_oauth()
                
            
            eel.sleep(1)
            
            
            break
                
                    
        eel.sleep(0.1)
        
                # Use eel.sleep(), not time.sleep()

async def color_to_blynk():
    global blynk_pin_data
    global pin
    global default_device
    global r
    global g
    global b
    #print(r)
    #print(g)
    #print(b)
    
    
    async with aiohttp.ClientSession() as session:
            
            url = 'http://blynk-cloud.com/' + blynk_token[default_device] + '/update/V' + "0" + '?value=' + str(r)
            async with session.get(url) as resp:
                x = 0
            url = 'http://blynk-cloud.com/' + blynk_token[default_device] + '/update/V' + "1" + '?value=' + str(g)
            async with session.get(url) as resp:
                x = 0
            url = 'http://blynk-cloud.com/' + blynk_token[default_device] + '/update/V' + "2" + '?value=' + str(b)
            async with session.get(url) as resp:
                x = 0
                

async def data_to_blynk():
    global blynk_pin_data
    global pin
    global default_device
    global r
    global g
    global b
    #print(r)
    #print(g)
    #print(b)
    
    
    async with aiohttp.ClientSession() as session:
            url = 'http://blynk-cloud.com/' + blynk_token[default_device] + '/update/V' + str(pin) + '?value=' + str(blynk_pin_data)
            async with session.get(url) as resp:
                x = 0
           
    
def refresh_access_token():
    global access_token
    global refresh_token
    global raw_auth_token
    global client_id
    global client_secret
    global sp 
    

    url = "https://accounts.spotify.com/api/token"

    payload="grant_type=refresh_token&client_id=" + client_id + "&client_secret=" + client_secret + "&refresh_token=" + refresh_token
    headers = {
      'Content-Type': 'application/x-www-form-urlencoded',      
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    raw_auth_token = json.loads(response.text)
    access_token = raw_auth_token["access_token"]
    sp = spotipy.Spotify(auth=access_token)
    print("refreshing token", raw_auth_token)
    print("new", access_token)
    
    
    
def spotify_main():
    #print("spotify_main")
    global current_track_uri
    global current_track_progress
    global prev_track_progress
    global current_track_duration
    global prev_track_uri
    global raw_current_track_data    
    global raw_current_audio_features
    global current_audio_features_segments
    global current_audio_features_segments_array
    global current_segment
    global current_segment_time
    global numb_segments
    global min_loudness_segment
    global min_loudness
    global max_loudness
    global current_loudness
    global current_loudness_time
    global min_loudness_positive
    global max_loudness_positive
    global current_loudness_positive
    global numb_leds
    global bar_percentage
    global r
    global g
    global b
    global pin
    global blynk_pin_data
    global sp
    global once
    global loop
    global current_sync
    global prev_sync
    global sync
    global loop
    global refresh_timer_start
    global refresh_timer_current
    global token_refresh_delay
    
    refresh_timer_current = time.perf_counter()

    if ((refresh_timer_current - refresh_timer_start) >= (token_refresh_delay * 60)):
        refresh_timer_start = refresh_timer_current
        refresh_access_token()
                                                  
    sp = spotipy.Spotify(auth=access_token)
    raw_current_track_data = sp.current_playback()

    if (raw_current_track_data is not None) and ((raw_current_track_data.get("item")) is not None):
        #print(raw_current_track_data)
        #print(type(raw_current_track_data.get("item")))
        current_track_uri = (raw_current_track_data.get("item")).get("uri")
        current_track_progress = (raw_current_track_data.get("progress_ms"))/1000
        current_track_playing_status = raw_current_track_data.get("is_playing")
        #print(current_track_progress)
        #print(current_track_playing_status)
        
        
        if (current_track_uri != prev_track_uri):
            print("song switched! mehnat starts")
            print(current_track_uri)
            pin = 3
            blynk_pin_data = 1023
            loop = asyncio.get_event_loop()
            loop.run_until_complete(data_to_blynk())

            r = random.randint(0, 255)
            g = random.randint(0, 255)
            b = random.randint(0, 255)
            print(r,g,b)
            
            loop = asyncio.get_event_loop()
            loop.run_until_complete(color_to_blynk())

            current_track_duration = (raw_current_track_data.get("item")).get("duration_ms")/100            
            raw_current_audio_features = sp.audio_analysis(current_track_uri)
            current_audio_features_segments = raw_current_audio_features.get("segments")
            current_audio_features_segments_array = np.array(current_audio_features_segments)
            numb_segments = len(current_audio_features_segments_array)
            current_segment = 0
            current_segment_time = 0
            prev_track_progress = 0
            min_loudness_segment = 0
            min_loudness = 0
            max_loudness = 0
            count = 0
            while (count < numb_segments):
                if((current_audio_features_segments_array[count]["loudness_max"]) < min_loudness):
                    min_loudness_segment = count
                    min_loudness = current_audio_features_segments_array[count]["loudness_max"]

                count = count + 1
                
            max_loudness_positive = max_loudness - min_loudness
            min_loudness_positive = min_loudness - min_loudness
            print(min_loudness_segment)
            print(min_loudness)
            print(current_track_duration)
        
        if (raw_current_track_data.get("is_playing") == True):
            if (prev_track_progress > current_track_progress):
                current_segment = 0
                current_segment_time = 0
                print(">"*1000)
            
        while (current_segment_time < current_track_progress):            
            if (current_segment < numb_segments):
                #print(current_segment_time)
                current_loudness = current_audio_features_segments_array[current_segment]["loudness_max"]
                current_loudness_time = current_audio_features_segments_array[current_segment]["loudness_max_time"]
                current_confidence = current_audio_features_segments_array[current_segment]["confidence"]
                current_loudness_positive = current_loudness - min_loudness
                #print(current_loudness)
                #print(current_loudness_positive)
                #print(current_loudness_time)

                if(current_segment_time - current_track_progress < 1) and (current_segment_time - current_track_progress >= -1):
                    prev_sync = current_sync
                    current_sync = True
                    #print(current_sync)
                    if (prev_sync == current_sync == True):
                        sync = True
                        #print(sync)
                        

                else:
                    current_sync = False
                    sync = False
                    #print(sync)
                if (sync == True):
                    bar_percentage = ((current_loudness_positive/max_loudness_positive) * 100)* current_confidence
                    print (("#" * round(bar_percentage)))
                    blynk_pin_data = round((bar_percentage/100) * numb_leds)
                    r = 0
                    g = 125
                    b = 125
                    pin = 4
                    loop = asyncio.get_event_loop()
                    loop.run_until_complete(data_to_blynk())

                    #eel.spawn(data_to_blynk)
                
                
                current_segment = current_segment + 1
                #print(current_segment)
                if (current_segment < numb_segments):
                    current_segment_time = current_audio_features_segments_array[current_segment]["start"]

            else:
                print("song data end")
                break
            
        prev_track_uri = current_track_uri
        prev_track_progress = current_track_progress
            
def blynk_main():
    #print("blynk_main")
    jjjj = 1
    
eel.spawn(spotify_auth)
eel.start('start/start.html', block=False, port=6969)


while True:
    #print("I'm a main loop")
    if (confirmation == True):
            eel.spawn(blynk_main)
            eel.spawn(spotify_main)    


    eel.sleep(0.001)
    

k=input("press close to exit") 



