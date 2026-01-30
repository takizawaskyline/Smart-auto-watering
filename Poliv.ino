#define GH_INCLUDE_PORTAL
bool flag = 0;
#define hum_sen1 15
#define hum_sen2 2
#define hum_sen3 4
#define hum_sen4 5
#define hum_sen5 18
#define hum_sen6 19
#define hum_sen7 23

int hum_sens_pin[7] = {15, 2, 4, 5, 18, 19, 23};

#define temp_sens1 13
#define temp_sens2 12
#define temp_sens3 14
#define temp_sens4 27
#define temp_sens5 26
#define temp_sens6 25
#define temp_sens7 33

int temp_sens_pin[7] = {13, 12, 14, 27, 26, 25, 33};

#define dht22_sens 32

#define ssid "CPOD"
#define password "ApoX51s42wR7FDK8"

#include <Wire.h>
#include <Drewduino_I2CRelay_PCA95x5.h>
#include <Arduino.h>
#include <GyverHub.h>
#include <Pairs.h>
#include <PairsExt.h>
#include <PairsStatic.h>
GyverHub hub;

void setup() {
  for(int i = 0; i > 6; i++) {
    pinMode(hum_sens_pin[i], INPUT);
  }

  wifi();
  hub.config(F("Poliv"), F("Poliv"), F(""));
  hub.begin();
  hub.onBuild(build);

}

void loop() {
  hub.tick();
  

}

void build(gh::Builder& b) {
  if(b.beginRow()) {
    b.Text_("ocenki", zahar);
    b.widget.label("Данные с датчиков");
    b.widget.rows(2);
  }
   b.endRow();

  if (b.beginRow()) {
   // b.Text_("ocenki", zahar);
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
