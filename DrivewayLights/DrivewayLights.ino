#include <TLC59108.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>

//#define HW_RESET_PIN 16
#define LED_SCL_PIN 5
#define LED_SDA_PIN 4
//#define IRLED1_PIN  12
//#define IRLED2_PIN  13
//#define IRIN1_PIN   16
//#define IRIN2_PIN   5
#define BLUE_LED  2
#define RED_LED  16

#define I2C_ADDR TLC59108::I2C_ADDR::BASE
#define LED_CURRENT_GAIN_VALUE  0xD5

TLC59108 leds(0x40);
WiFiUDP Udp;
unsigned int localUdpPort = 4210;
char incomingPacket[255];
char  replyPacekt[] = "Hi there! Got the message :-)";
int lastTime = 0;
bool blinkOn = false;
int cycleCount = 0;
const uint8_t allOn[8] = {255,255,255,255,255,255,255,255};
const uint8_t allOff[8] = {0,0,0,0,0,0,0,0};


void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println();
    Wire.begin(LED_SDA_PIN, LED_SCL_PIN);
    Wire.setClock(50000L);
    pinMode(RED_LED,OUTPUT);
    pinMode(BLUE_LED,OUTPUT);
    digitalWrite(RED_LED,1);
    digitalWrite(BLUE_LED,0);
    leds.init();
    leds.setLedOutputMode(TLC59108::LED_MODE::PWM_IND);
    leds.setRegister(TLC59108::REGISTER::IREF::ADDR, LED_CURRENT_GAIN_VALUE);
    leds.setAllBrightness((uint8_t)0);
    WiFi.begin("<SSID>","<password>");
    
      while(WiFi.status() != WL_CONNECTED)
      {
          delay(250);
          if(blinkOn)
        {
          digitalWrite(BLUE_LED,1);
        }
        else
        {
          digitalWrite(BLUE_LED,0);
        }
        blinkOn = !blinkOn;
      }
    Udp.begin(localUdpPort);
    Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

void loop() {
    // put your main code here, to run repeatedly:
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
      //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
        if ((incomingPacket[0] == 0x70) && (len > 2)) {
          leds.setBrightness(incomingPacket[1],incomingPacket[2]);
        }
      }
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(replyPacekt);
        Udp.endPacket();
    }
    if (millis()-lastTime > 1000)
    {
      lastTime = millis();
//      Serial.print("S:");
//      Serial.println(WiFi.RSSI());
//      Udp.beginPacket("10.33.33.255", localUdpPort);
//        Udp.write("RSSI=");
//        Udp.write(WiFi.RSSI());
        
//           if(cycleCount == 0) {
//              leds.setBrightness(1,10);
//           } else if (cycleCount == 1) {
//              leds.setBrightness(2,10);
//           } else if (cycleCount == 2) {
//              leds.setBrightness(3,10);
//           } else if (cycleCount == 3) {
//              leds.setBrightness(1,0);
//           } else if (cycleCount == 4) {
//              leds.setBrightness(2,0);
//           } else if (cycleCount == 5) {
//              leds.setBrightness(3,0);
//           } 
//           cycleCount++;
//           if (cycleCount > 5) {cycleCount = 0;}
        if(blinkOn)
        {
          digitalWrite(RED_LED,1);
        }
        else
        {
          digitalWrite(RED_LED,0);
        }
        blinkOn = !blinkOn;
//        Udp.endPacket();
    }
}
