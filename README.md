# Spotify-Deck

This is an IoT-based Spotify Display that can be built using an ESP32 CYD touchscreen board. The project uses the small touchscreen to create a personalised Spotify controller that displays real-time music information, including album artwork, track name, artist, and playback progress.

<img width="1223" height="814" alt="spotify-deck" src="https://github.com/user-attachments/assets/40b48e41-a89f-4702-8c7d-6afd6d286021" />

<h3>Why ESP32 CYD?</h3>
This board has touchscreen, along with Bluetooth and WIFI, which cancels out any effort spent in soldering boards and wires. 

<h3>Some features:</h3>

- <b>Dynamic Album Artwork:</b> Displays the currently playing track's album cover with automatically generated background colours based on the artwork.
- <b>Real-Time Music Information:</b> Shows track title, artist name, album name, and playback progress.
- <b> Touchscreen Controls:</b> Supports play, pause, next track, previous track, and shuffle.

<h3>BOM:</h3>

| Name | Quantity | Cost | Link |
|---|---:|---:|---|
| ESP32 Dev Board | 1 | $18.20 | [Buy](https://techtonics.in/product/esp32-dev-board-wifi-bluetooth-with-2-8-inch-240x320-display-tft-touchscreen-cyd/) |
| Spotify Premium | 1 | $15.42 | [Buy](https://open.spotify.com/premium) |
| **Total** | | **$33.62** | |
| **Total Funding Required** | | **$33.62** | |

<h3>Arduino IDE libraries needed:</h3>  

- ArduinoJson
- TFT_eSPI
- XPT2046_Touchscreen
- TJpg_Decoder
- WiFiManager

<h3>Github repo format:</h3>  

```text
Firmware/
└── Spotify-Deck/
    ├── Spotify-Deck.ino
    ├── JOURNAL.md
    ├── LICENSE
    ├── README.md
    └── bom.csv
```

