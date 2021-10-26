from spotipy.oauth2 import SpotifyOAuth
SPOTIPY_CLIENT_ID= 'c1d938d90ab342eba143414bcea37487'
SPOTIPY_CLIENT_SECRET= '5719ab413ef94825a2c1a2839bbccb67'
SPOTIPY_REDIRECT_URI= 'http://localhost:8001/callback'
scope = "user-library-read"

sp = spotipy.oauth2.SpotifyOAuth(client_id = SPOTIPY_CLIENT_ID, client_secret = SPOTIPY_CLIENT_SECRET, redirect_uri = SPOTIPY_REDIRECT_URI, scope = scope)

print(sp.get_access_token())

refresh_access_token(refresh_token)
