#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "src/ESP2SOTA/src/ESP2SOTA.h"
//#include "src/TFT_eSPI/TFT_eSPI.h"
#include <WiFiAP.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite buf = TFT_eSprite(&tft);
TFT_eSprite back = TFT_eSprite(&tft);
TFT_eSprite nidle = TFT_eSprite(&tft);

char AP_SSID[32] = "TestTest";
#define AP_PASS "12345678"

IPAddress Actual_IP;

IPAddress PageIP(192,168,1,1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;

WebServer server(80);


int led = 2;
int up = 0;
void setup() {
  pinMode(led, OUTPUT);
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig(PageIP, gateway, subnet);
  delay(100);
  Actual_IP = WiFi.softAPIP();
  ESP2SOTA.begin(&server);
  server.begin();
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(0);
  buf.createSprite(240,230);
  back.createSprite(240,140);
  nidle.createSprite(120,20);
  nidle.fillCircle(100,10,10,TFT_RED);
  nidle.fillTriangle(20,10,120,14,120,6,TFT_WHITE);
  nidle.fillCircle(100,10,4,TFT_PINK);
  tft.fillScreen(rgb(255,0,255));
  Serial.begin(9600);  
  back.fillCircle(120, 120, 120, TFT_BLACK);
  for(float i = 0; i<360; i++){
    float x = 120+(cos(i*(PI/180))*-120);
    float y = 120+(sin(i*(PI/180))*-120);
    float x1 = 120+(cos((i+1)*(PI/180))*-120);
    float y1 = 120+(sin((i+1)*(PI/180))*-120);
    if(i<=120)
    back.fillTriangle(120,120,x,y,x1,y1,rgb(map(i,0,120,0,255),map(i,0,120,255,0),0));
    else if(i<=240)
    back.fillTriangle(120,120,x,y,x1,y1,rgb(map(i,120,0,0,255),0,map(i,120,0,255,0)));
    else
    back.fillTriangle(120,120,x,y,x1,y1,rgb(0,map(i,0,120,0,255),map(i,0,120,255,0)));
  }
}
int x = 0;
int delta=0;
void loop() {
  x=0;
  delta = millis(); 

  if(up+16<millis()){
    tft.setPivot(0, 0);
//    back.setPivot(120, 120);
//    back.pushRotated(&buf,0,TFT_BLACK);
    buf.fillRect(0,0,240,240,TFT_BLACK);
    nidle.setPivot(100, 10);
    nidle.pushRotated(&buf,millis()/10,TFT_BLACK);
    buf.pushSprite(0,0);
    up = millis();
    delta = millis() - delta; 
    //server.handleClient();
    Serial.print(x);
    Serial.print(",");
    Serial.println(delta);
  }
}

int rgb(unsigned char r, unsigned char g, unsigned char b) {
    if (r < 0 || 255 < r || g < 0 || 255 < g || b < 0 || b > 255)
        return -1;

    int result;

    int red = r * 31 / 255;
    int green = g * 63/ 255;
    int blue = b * 31 / 255;


    //int result = (red << 11) | (green << 5) | blue;

    green = green << 5;
    red = red << 11;

    result = red | green | blue;


    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}
