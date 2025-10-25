#define GH_INCLUDE_PORTAL
#include "Arduino.h"
#include "GyverHub.h"
#include <Pairs.h>
#include <PairsExt.h>
#include <PairsStatic.h>
GyverHub hub;
bool flag = 0; 
#define ssid "CPOD"
#define password "ApoX51s42wR7FDK8"
Pairs zahar;
String litra = "Литература";
String himia = "Химия";


void setup() {
  wifi();
  hub.config(F("Azrielshka"), F("Andrey"), F(""));
  hub.begin();
  hub.onBuild(build);
  pinMode(4, OUTPUT);
  zahar[litra] = 2;
  zahar[himia] = 5;

}

void loop() {
  hub.tick();
}

void build(gh::Builder& b) {
  if (b.beginRow()) {

      if(b.Switch(&flag).size(1).click()) {
        digitalWrite(4, flag);
      }
   
   }
    b.endRow();

  if(b.beginRow()) {
    b.Text("Состаяние светодиода");
    b.widget.label("Данные с датчиков");
    b.widget.rows(2);
  }
   b.endRow();

  if (b.beginRow()) {
    b.Text_("ocenki", zahar);
    b.widget.label("Тест peirs");
    b.widget.rows(2);
  }   
  b.endRow();
}

void wifi() {
  delay(2000);
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
  Serial.println(WiFi.localIP());
}