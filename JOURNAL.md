---
title: "Spotify Deck"
author: "adr"
description: "A simple spotify player display"
created_at: "2026-10-26"
---

# 2026-10-30: Writing Firmware

**Total time spent: 2h 8m**

After setting up Arduino IDE and connecting Hackatime, i wrote the code in different sessions, whenever i got time in day. The code started with describing the api and client keys and stuff, then setting the display colours and then void setup, which keeps repeating in the code
![image.png](https://cdn.hackclub.com/01a18dab-0acf-76fe-b67a-9403e9f738f2/image.png)

# 2026-10-26: Writing the Code

**Total time spent: 1 hour**

The first step before writing the code for the display is to set up the libraries in arduino IDE. After some research, the libraries needed for this project are: TFT_eSPI, XPT2046_Touchscreen, ArduinoJson, TJpg_Decoder, WiFiManager and one more, which can't be found in the Arduino IDE's preset libraries. This one can be downloaded from this link: <br>
<h3>https://github.com/witnessmenow/spotify-api-arduino/archive/refs/heads/main.zip</h3> <br>
To set this up, go to Sketch → Include Library → Add .ZIP Library → and select the file you just downloaded.
The next step was to configure the display driver. 
On this location, Documents/Arduino/libraries/TFT_eSPI/User_Setup.h
Delete all of its content and replace it with this: <br>

#define ILI9341_DRIVER<br>

#define TFT_MISO 12<br>
#define TFT_MOSI 13<br>
#define TFT_SCLK 14<br>
#define TFT_CS   15<br>
#define TFT_DC   2<br>
#define TFT_RST  -1<br>
<br>
#define TOUCH_CS -1<br>

#define LOAD_GLCD<br>
#define LOAD_FONT2<br>
#define LOAD_FONT4<br>
#define LOAD_FONT6<br>
#define LOAD_FONT7<br>
#define LOAD_FONT8<br>
#define LOAD_GFXFF<br>
#define SMOOTH_FONT<br>

#define SPI_FREQUENCY  40000000<br>
#define SPI_READ_FREQUENCY  20000000<br>
#define SPI_TOUCH_FREQUENCY  2500000<br>
Save it and close it
![Screenshot_2026-10-26_at_2.42.50_PM.png](https://cdn.hackclub.com/01a1777b-f53f-7725-9603-87430b3af8c6/Screenshot_2026-10-26_at_2.42.50_PM.png) <br>
Next, to create the Spotify app, head over to developer.spotify.com/dashboard and log in. <br>
![Screenshot_2026-10-26_at_2.48.21_PM.png](https://cdn.hackclub.com/01a17781-088b-74db-b403-0307e1642519/Screenshot_2026-10-26_at_2.48.21_PM.png)
Click Create app — name it anything <br>
For the Redirect URI, type exactly: <br>
<h3>http://127.0.0.1:8888/callback</h3><br>
Check the 'WEB API' box, then click Save <br>
Copy your Client ID and click "View client secret" to copy the Client Secret too. < br>

<h3>Get your permanent login pass:</h3>
Build this link (swap in your real Client ID), open it in a browser, and click Agree:<br>
<h2>https://accounts.spotify.com/authorize?client_id=YOUR_CLIENT_ID&response_type=code&redirect_uri=http://127.0.0.1:8888/callback&scope=user-read-currently-playing%20user-read-playback-state%20user-modify-playback-state</h2><br>
The page will look "broken" after — that's expected. Copy everything after 'code=' in the address bar.
Then in Terminal, run this (with your own values filled in):<br><h4>
curl -X POST https://accounts.spotify.com/api/token \<br>
-H "Content-Type: application/x-www-form-urlencoded" \<br>
-d grant_type=authorization_code \<br>
-d code=YOUR_CODE_HERE \<br>
-d redirect_uri=http://127.0.0.1:8888/callback \<br>
-d client_id=YOUR_CLIENT_ID \<br>
-d client_secret=YOUR_CLIENT_SECRET</h4>
Copy the refresh_token from the response — (that's your permanent pass.)
<h3>Add your codes to the project</h3>
Open secrets.h and fill in your three values:
#define SPOTIFY_CLIENT_ID "paste it here"<br>
#define SPOTIFY_CLIENT_SECRET "paste it here"<br>
#define SPOTIFY_REFRESH_TOKEN "paste it here"<br>
Keep these values handy for the final upload
After this, i wrote the code and tracked the time in Arduino IDE







