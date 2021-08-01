#include "ACS712.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include <SSD1306Wire.h>
#include "EmonLib.h"
#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>
#include <stdlib.h>


 
#define VOLT_CAL 211.6 
#define my_ssid "Jacare_2G"
#define my_password "coxinha1"
 
const char* ntp_server = "a.st1.ntp.br"; 
const int time_zone = -14400; 
unsigned long int req_time = millis(),
                  timeCurrent = millis();

DynamicJsonDocument doc(2048); 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntp_server, time_zone, 60000);
ACS712  ACS(34,2.6, 4095, 185);
SSD1306Wire oled(0x3C, 21, 22);
EnergyMonitor emon1;
File dataFile;
 
 
void setup(){  
  emon1.voltage(2, VOLT_CAL, 1.7);
  ACS.autoMidPoint();
  set_oled();
  wifi_connect();

}
  
void loop(){
  show_time();
  if(millis() - timeCurrent > 1000){ 
    show_data();
    timeCurrent = millis();
  }
  
}


void save_data(){
  struct tm* p_tm = localtime(&now);
  doc["id"] = id;
  doc["hour"] = p_tm->tm_hour;
  doc["minute"] = p_tm->tm_min;
  doc["second"] = p_tm->tm_sec;
  doc["amperage"] = amperage();
  doc["voltage"] = voltage();
  id++;

  if (dataFile = SD.open("data.txt", FILE_WRITE)) {    
    dataFile.print(doc["id"]);
    dataFile.print(doc["minute"]);
    dataFile.print(doc["second"]);
    dataFile.print(doc["voltage"]);
    dataFile.print(doc["amperage"]);
    
    dataFile.close();
  } 
 
}

void show_data(){
  int current = amperage(), 
      volt = voltage();
   
  
  float watt = (current * volt)/1000.0 * 0;
  current = random(200, 300);
  oled.setFont(ArialMT_Plain_16); 
  oled.clear();
  //char msg[256];
  //sprintf(msg, "Amp: %d, %.3d", ampere/1000, ampere%1000); 
  
  oled.drawString(60, 5, "Current: " + String(current) + " mA" );
  oled.drawString(55, 25, "Voltage: " + String(volt) + " V" );
  oled.drawString(55, 45, "KHW: " + String(watt) );
  //oled.drawString(63, 19, String(msg));
  //oled.drawLine(10, 52, 117, 52);
  oled.display();
  delay(5000);
}

int amperage(){
  return ACS.mA_AC(60);
}

int voltage(){
  float volt = emon1.calcVI(17,2000); 
  float supplyVoltage = emon1.Vrms;
  return supplyVoltage;
}

void show_time(){
  
  if(millis() - req_time > 900){
    oled.setFont(ArialMT_Plain_24);
    timeClient.update();
    //Serial.println(timeClient.getFormattedTime());
    String now = timeClient.getFormattedTime();
    oled.clear();
    oled.drawString(63, 19, now);
    oled.drawLine(10, 52, 117, 52);
    oled.display();
    
    if (now >= "10:00:00" and now <= "18:00:00")
      save_data();
    
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
  oled.setFont(ArialMT_Plain_16);
  oled.setTextAlignment(TEXT_ALIGN_CENTER);

}
