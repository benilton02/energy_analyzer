#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include <SSD1306Wire.h>
 
 
#define my_ssid "Jacare_2G"
#define my_password "coxinha1"
 
const char* ntp_server = "a.st1.ntp.br"; 
const int time_zone = -14400; 
unsigned long int req_time = millis();

 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntp_server, time_zone, 60000);
 

SSD1306Wire oled(0x3C, 21, 22);
 
 
void setup(){  
  set_oled();
  wifi_connect();
}
  
void loop(){
  show_time();
}

void show_time(){
  
  if(millis() - req_time > 900){
    timeClient.update();
    //Serial.println(timeClient.getFormattedTime());
    String now = timeClient.getFormattedTime();
    oled.clear();
    oled.drawString(63, 19, now);
    oled.drawLine(10, 52, 117, 52);
    oled.display();
    req_time = millis();
  }

}

void wifi_connect(){
  WiFi.begin(my_ssid, my_password);
  unsigned int timeout = millis();
  while ((WiFi.status() != WL_CONNECTED)){
      oled.clear();
      oled.drawString(63, 12, "Connecting \nto wifi.");
      oled.display();
      oled.clear();
      delay(500);
      oled.drawString(63, 12, "Connecting \nto wifi..");
      oled.display();
      oled.clear();
      delay(500);
      oled.drawString(63, 12, "Connecting \nto wifi...");
      oled.display();
      delay(500);
      if((millis() - timeout > 5000)){
        oled.clear();
        oled.drawString(63, 12, "Fail!");
        oled.display();
        delay(5000);
        timeout = millis();
      }
  }
  oled.clear();
  oled.drawString(63, 12, "Connected \nDevice!");
  oled.display();
  delay(5000);
  timeClient.begin();
}

void set_oled(){
  oled.init();
  oled.clear();
  oled.flipScreenVertically();
  oled.setFont(ArialMT_Plain_24);
  oled.setTextAlignment(TEXT_ALIGN_CENTER);

}