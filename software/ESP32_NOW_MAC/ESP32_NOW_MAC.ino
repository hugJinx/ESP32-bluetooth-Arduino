#include "WiFi.h"
 
void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  Serial.print("MAC地址为：");
  Serial.println(WiFi.macAddress());
}
 
void loop(){

}
