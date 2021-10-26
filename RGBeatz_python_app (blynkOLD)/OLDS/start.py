import eel
x = 0

eel.init('web')

@eel.expose
def get_oauth(raw_auth, access, refresh):    
    raw_auth_token = raw_auth
    access_token = access
    refresh_token = refresh
    print(raw_auth_token)
    print(access_token)
    print(refresh_token)
    
    

def my_other_thread():
    while True:
        print("I'm a thread")
        
        global x
        
        if (x == 0):
            x = 10
            eel.sleep(5)
            eel.req_oauth()
            
        
            
        eel.sleep(0.1)
        
                # Use eel.sleep(), not time.sleep()

eel.spawn(my_other_thread)
    

eel.start('start.html', block=False)
while True:
    print("I'm a main loop")
    eel.sleep(0.1)
    



