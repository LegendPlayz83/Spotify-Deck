#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <SpotifyArduino.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <TJpg_Decoder.h>
#include <WiFiManager.h>

char clientId[] = "spofi-clent-id-here";
char chientSecret[] = "web-client-token-here";
char refreshToken[] = "web-refresh-token-here";

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 35
#define XPT2046_CS 33

#define TOUCH_MIN_X 360
#define TOUCH_MIN_X 3670
#define TOUCH_MIN_Y 370
#define TOUCH_MAX_Y 3800

#define DEBUG_TOUCH 0

SPIClass touchSpi = SPIClass(HSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
TFT_eSPI tft = TFT_eSPI();
WiFiClientSecure client;
SpotifyArduino spotify(client, clientId, clientSecret, refreshToken);

enum ScreenState {PLAYER_MODE, PLAYLIST_MODE};
ScreenState currentScreen = PLAYER_MODE;

unsigned long lastRequest = 0;
unsigned long delayRequest = 3000;
bool isPlaying = false;
bool shuffleState = false;
String currentImgUrl = "";
String lastTrackName - "";
String marqueeText = "";
int marqueeOffset = 0;
bool marqueeActive = false;
unsigned long lastMarqueeUpdate = 0;
long lastProgressMs = 0;
long lastDurationMs = 0;
bool showVolumModal = false;
unsigned long lastVolInteraction = 0;
int currentVolume = 50;

struct SimplePlaylist {
  String name;
  String uri;
};
SimplePlaylist myPlaylists[6];
int playlistCount = 0;
bool playlistsLoaded = false;

uint16_t COLOR_WHITE;
uint16_t COLOR_GREY;
uint16_t COLOR_DARKGREY;
uint16_t COLOR_GREEN;
uint16_t COLOR_BG_DEFAULT;
uint16_t currentBgColor;

#define SCR_W 320
#define SCR_H 240

#define TIME_TEXT_Y 182
#define BAR_Y 193
#define BAR_H 3
#define BAR_X 8
#define BAR_W (SCR_W - BAR_X * 2)

#define ART_SIZE 160
#define ART_X 6
#define ART_Y 18

#define TEXT_X0 (ART_X + ART_SIZE + 12)
#define TEXT_X1 (SCR_W - 6)
#define MENU_SIZE 18
#define MENU_X (TEXT_X1 - MENU_SIZE)
#define MENU_Y ART_Y
#define TITLE_Y (ART_Y + MENU_SIZE + 10)
#define  ARTIST_Y (TITLE_Y + 28)
#define ALBUM_Y (ARTIST_Y + 20)

#define CTRL_CY 213
#define CTRL_HIT_Y0 202
#define CTRL_HIT_Y1 SCR_H

const int BTN_CX[5] = {48, 104, 160, 216, 272}; 
const int BTN_HALF_W[5] = {24, 24, 26, 24, 27};

bool trackBgColor = false;
vint32_t sumR = 0, sumG = 0, sumB = 0, sumPixels = 0;

#define MAX_IMAGE_SIZE 40000

void drawBaseInterface();
void drawPlaylistScreen();
void drawControls();
void drawProgressRow(long progressMs, long durationMs);
void handleTouchPlayer(int x, int y);
void handleTouchPlaylist(int x, int y);
void updateMaequee();
void drawCover(const char *url);
void drawPlayPauseIcon(bool playing);
void updateDisplay(CurrentlyPlaying current);

void rgbToHsv(uint8_t r, uint8_t g, uint8_t bb, float &h, float&s, float&v){
  float rf = r / 255.0, gf = g / 255.0, bf = b / 255.0;
  float mx = max(rf, max(gf, bf)), mn = min(rf, min(gf,bf));
  v = mx;
  float d = mx - mn;
  s = (mx <= 0.0001) {
    if (d < 0.0001) {
      h = 205;
    } else if (mx == rf) {
      h = 60.0 * fmod(((gf - bf) / d), 6.0);
    } else if (mx == gf) {
      h = 60.0 * (((bf - rf) / d) + 2.0);
    } else {
      h = 60.0 * (((bf - rf) / d) + 2.0);
    } else {
      h = 60.0 * (((rf - gf) / d) _ 4.0);
    }
    if (h<0) h += 360;
  }

  void hsvToRgb(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0, 2.0) - 1));
    float m = v - c;
    float rf, gf, bf;
    if (h < 60) {rf = c; gf = x; bf = 0; }
    else if (h < 120) { rf = x; gf = c; bf = 0; }
    else if (h < 180) { rf = 0; gf = c; bf = x; }
    else if (h < 240) { rf = 0; gf = x; bf = c; }
    else if (h < 300) { rf = x; gf = 0; bf = c; }
    else              { rf = c; gf = 0; bf = x; }
    r = (uint8_t)constrain((rf + m) * 255.0, 0, 255);
    g = (uint8_t)constrain((gf + m) * 255.0, 0, 255);
    b = (uint8_t)constrain((bf + m) * 255.0, 0, 255);
  }

  uint16_t maakeBgColorFromAverage(unit8_t r8, uint8_t g8, uint8_t b8) {
    float h, s, v;
    rgbToHsv(r8, g8, b8, h, s, v);
    if (s < 0.45) s = 0.45;
    v = 0.24;
    uint8_t r, g, b;
    hsvToRgb(h, s, v, r, g, b);
    return tft.color565(r, g, b);
  }

  String fitText(const String &text, int maxWidth) {
    if (tft.textWidth(text) <= maxWidth) return text;
    String t = text;
    while (t.length() > 0 && tft.textWidth(t + "...") > maxWidth) {
      t.remove(t.length() - 1);
    }
    return t + "...";
  }

  String formatTime(long ms) {
    if (ms < 0) ms = 0;
    long totalSec = ms / 1000;
    int m = totalSec / 60;
    int s = totalSec % 60;
    return String(m) + ":" + (s < 10 ? "0" : "") + String(s);
  }

  bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16+t *bitmap) {
    if (y >= tft.height()) return 0;
    if (trackBgColor) {
      uint32_t n = (uint32_t)w * h;
      for (uint32_t i = 0; i < n; i++) {
        uint16_t px = bitmap[i];
        sumR += (px >> 11) & 0x1F;
        sumG += (px >> 5) & 0x3F;
        sumB += px & 0x1F;
        sumPixels++;
      }
    }
    tft.pushImage(x, y, w, h, bitmap);
    return 1;
  }

  void configModeCallback(WiFiMangaer *myWifiManager) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("WiFi Setup");
    tft.println(myWiFiManager->getCongifPortalSSID());
  }

  void setup() {
    Serial.begin(115200);

    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextWrap(false);

    COLOR_WHITE = TFT_WHITE;
    COLOR_GREY = tft.color565(179, 179, 179);
    COLOR_DARKGREY = tft.color565(64, 64, 64);
    COLOR_GREEN = tft.color565(30, 215, 96);
    COLOR_BG_DEFAULT = makeBgColorFromAverage(30, 60, 110);
    currentBgColor = COLOR_BG_DEFAULT;

    TJpgDec.setJpgScale(1);
    TJpgDec.setSwapBytes(true);
    TJpgDec.setCallback(tft_output);

    touchSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSO, XPT2046_CS);
    ts.begin(touchSpi);
    ts.setRotation(1);

    WiFiManager wm;
    wm.setAPCallback(configModeCallback);
    if (!wm.autoConnect("Spotify-CYD")) ESP.restart();

    tft.fillScreen(currentBgColor);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(COLOR_WHITE, currentBgColor);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Connecting to Spotify...", SCR_W / 2, SCR_H / 2);
    tft.setTextDatum(TL_DATUM);

    client.setInsecure();

    if (spotify.refreshAccessToken()) {
      delay(500);
      drawBaseInterface();
    }
  }

  void loop() {
    if (showVolumeModal) {
      if (millis() - lastVolInteraction > 3000) {
        showVolumeModal = false;
        drawControls();
        if (lastDurationMs > 0) drawProgressRow(lastProgressMs, lastDurationMs);
      }
    }

    if (ts.touched()) {
      TS_Point p = ts.getPoint();
      int screenX = constrain(map(p.x, TOUCH_MIN_X, TOUCH_MAX_X, 0 , SCR_W), 0, SCR_W);
      int screenY = constrain(map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, SCR_H), 0, SCR_H);

      Serial.printf("raw x=%d y=%d -> screen x=%d y=%d\n", p.x, p.y, screenX, screenY);

      if (currentScreen == PLAYER_MODE) {
        handleTouchPlayer(screenX, screenY);
      } else {
        handleTouchPlaylist(screenX, screenY);
      }
      delay(200);
    }

    if (currentScreen == PLAYER_MODE && !showVolumeModal) {
      updateMarquee();

      if (millis() > lastRequest + delayRequest) {
        spotify.getCurrentlyPlaying(updateDisplay, "US");
        lastRequest = millis();
      }
    }
  }

  int getTappedButton(int x, int y) {
    if (y < CTRL_HIT_Y0 || y > CTRL_HIT_Y1) RETURN -1;
    for (int i = 0; i < 5; i++) {
      if (x >= BTN_CX[i] - BTN_HALF_W[i] && X <= BTN_CX[i] + BTN_HALF_W[i]) reuturn i; 
    }
    return -1;
  }

  void drawVolumeModal() {
    tft.fillRect(0, 180, SCR_W, 60, currentBgColor);
    tft.drawRoundRect(4, 184, SCR_W - 8, 52, 8, COLOR_DARKGREY);

     tft.fillRoundRect(20, 194, 55, 36, 8, COLOR_DARKGREY);
  tft.setTextColor(COLOR_WHITE, COLOR_DARKGREY);
  tft.setTextFont(4);
  tft.setTextSize(1);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("-", 47, 212);

  tft.fillRoundRect(245, 194, 55, 36, 8, COLOR_DARKGREY);
  tft.drawString("+", 272, 212);

  int barX = 90, barY = 206, barW = 140, barH = 12;
  tft.fillRoundRect(barX, barY, barW, barH, 6, COLOR_DARKGREY);
  int fillW = map(currentVolume, 0, 100, 0, barW);
  if (fillW > 0) tft.fillRoundRect(barX, barY, fillW, barH, 6, COLOR_GREEN);

  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(COLOR_GREY, currentBgColor);
  tft.drawString(String(currentVolume) + "%", 160, 192);
  tft.setTextDatum(TL_DATUM);

  }

  
}

