#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <time.h>

#define my_ssid "my_ssid"
#define my_password "my_password"

const char* ssid = my_ssid;
const char* password = my_password;

int timezone = -4 * 3600;

int dst = 0;
unsigned int id = 0;

unsigned long int reqTime = millis();

void get_time(); 

DynamicJsonDocument doc(2048);

void setup() {
  
  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }


  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  Serial.println("\nWaiting for Internet time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  Serial.println("\nTime response....OK");   
}

void loop() {
  get_time(); 
}

void get_time(){
  if (millis() - reqTime  > 60000){
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    doc["id"] = id;
    doc["hour"] = p_tm->tm_hour;
    doc["minute"] = p_tm->tm_min;
    doc["second"] = p_tm->tm_sec;
    id++;
    serializeJsonPretty(doc, Serial);
    Serial.println();
    reqTime = millis();
  }
}