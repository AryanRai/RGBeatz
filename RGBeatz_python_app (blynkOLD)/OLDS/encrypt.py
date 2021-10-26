from cryptography.fernet import Fernet
import os
import sys
import json
import base64
key_confirmed = False
param_val = ""

def handle_config_creds():
    
    global key
    global json_cred
    global key_confirmed
    global confirmation_key
    global client_id
    global client_secret
    
    key = "B?E(H+Mb8x/A?D(Gr4u7x!A%WmZq4t7weThWmYq3KbPeShVm*G-KaPdSz%C*F-Ja6w9z$C&Fp3s6v9y$"    
    try:
        with open(os.path.join(os.environ['USERPROFILE'], "RGBconf.bin"), 'rb') as file_object:
            for line in file_object:
                key = base64.b64decode(line)
            file_object.close()
            cipher_suite = Fernet(key)
        with open(os.path.join(sys.path[0], "RGBeatz.bin"), 'rb') as file_object:
            for line in file_object:
                encrypted_data = line
            file_object.close()
            
        uncipher_data = (cipher_suite.decrypt(encrypted_data))
        json_cred = json.loads(bytes(uncipher_data).decode("utf-8")) #convert to string
        #print(json_cred)
        confirmation_key = json_cred["cipher_key"]
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
        #########################
        print("getting login data")
        ########################
        print("encrypting")
        key = Fernet.generate_key()
        with open(os.path.join(os.environ['USERPROFILE'], "RGBconf.bin"), 'wb') as file_object:
            file_object.write(base64.b64encode(key))
            file_object.close()
            client_id = base64.b64decode("NGMzOGE1MzQ1MzliNDUxMGI4YmRjNzliNGVjODZmMDA=")
            client_secret = base64.b64decode("MmRiNDFmMmVlNDljNDlhZjg0ZDAxOGRjMWI3NDRlMTc=")
        config_data = json.dumps({
          "name": "Aryan",
          "client_id": str(client_id),
          "client_secret": str(client_secret),
          "cipher_key": str(key),
        })
        encrypt_write(config_data)
    
def encrypt_write(writable_data):    
    global key
    cipher_suite = Fernet(key)
    ciphered_text = cipher_suite.encrypt(bytes(writable_data,'utf-8'))
    with open(os.path.join(sys.path[0], "RGBeatz.bin"), 'wb') as file_object:
        file_object.write(ciphered_text)
        file_object.close()

def get_param_local(param):
    global param_val
    global json_cred
    
    
    try:
        param_val = json_cred[param]
            
    except KeyError:
            param_val = "error 404 keyword not found"
            print(param_val)

def get_spotify_app_id():
    global client_id
    global client_secret
    
    get_param_local("client_id")
    if(param_val == "error 404 keyword not found"):
        handle_config_creds()
    client_id = (param_val.replace("b", "", 1)).replace("'", "")

    get_param_local("client_secret")
    if(param_val == "error 404 keyword not found"):
        handle_config_creds()
    client_secret = (param_val.replace("b", "", 1)).replace("'", "")

    print(client_id)
    print(client_secret)
            
handle_config_creds()
get_spotify_app_id()

