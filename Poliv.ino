#define GH_INCLUDE_PORTAL

#include <WiFi.h>
#include <PubSubClient.h>

bool flag = 0;
bool mqttConnected = false;  // Добавлена переменная

#define hum_sen1 15
#define hum_sen2 2
#define hum_sen3 4
#define hum_sen4 5
#define hum_sen5 18
#define hum_sen6 19
#define hum_sen7 23

int hum_sens_pin[7] = {32, 33, 34, 35, 36, 37, 38};

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

const char* user_mqtt = "Poliv";
const char* password_mqtt = "Iz8GUHz8";
const int port_mqtt = 19778;
const char* server_mqtt = "m9.wqtt.ru";
const char* mqtt_topic_pub = "epstains_file/delo_na_ostrove/vecherinki/Andrey_Grygoriev";
const char* mqtt_topic_sub = "electro/wqtt/esp32/led"; 
const char* mqtt_topic_hum = "poliv/hum_sens_val";


#include <Wire.h>
#include <Drewduino_I2CRelay_PCA95x5.h>

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < 6; i++) {
    pinMode(hum_sens_pin[i], INPUT);
  }

  client.setServer(server_mqtt, port_mqtt);
  client.setBufferSize(256);  // Увеличиваем буфер сообщений

  wifi();
  client.setCallback(callback);  // Функция обработки входящих сообщений
}

void loop() {

 // delay(2000);
  if (!client.connected()) {
    mqttConnected = false;
    reconnectMQTT();
  } else {
    if (!mqttConnected) {
      mqttConnected = true;
      Serial.println("MQTT подключён стабильно!");
    }
  }

  client.loop();

   hum_sens();
  
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

  

void reconnectMQTT() {
  static unsigned long lastAttempt = 0;
  
  // Пытаемся переподключаться не чаще 1 раза в 5 секунд
  if (millis() - lastAttempt < 5000) {
    return;
  }
  lastAttempt = millis();
  
  Serial.print("Попытка подключения к MQTT...");
  
  // Случайный ID клиента для избежания конфликтов
  String clientId = "ESP32-" + String(random(0xFFFF), HEX);
   if (client.connect(clientId.c_str(), user_mqtt, password_mqtt)) {
    Serial.println("Успешно!");
    // ВОТ тут подписка на топик
    client.subscribe(mqtt_topic_sub);
  } 
  else {
    Serial.print("Ошибка, rc=");
    Serial.println(client.state());
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Печатем топик с которого получено сообщение
  Serial.print("Получено сообщение [");
  Serial.print(topic);
  Serial.print("]: ");
  
  // Сохраняем сообщение в переменную  "message"
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  //Печатаем само сообщение в сериал
  Serial.println(message);

  //Если получено сообщение с топика mqtt_topic_sub
  if (String(topic) == mqtt_topic_sub) {
    if (message == "ТОМУ ЧТО НУЖНО") {
      // делаем одно
    }
    if (message == "ТОМУ ЧТО НУЖНО2") {
       // делаем другое
    }  
  }
}


  void hum_sens() {
    delay(2000);
    int hum_sens_pin[7] = {32, 33, 34, 35, 36};
    int val_h[7];
    for (int i=0; i < 7; i++) {
       val_h[i] = analogRead(hum_sens_pin[i]);
       Serial.println(val_h[i]);
    }
      String json = json_file(val_h);
    
      client.publish(mqtt_topic_hum, json.c_str());
  }

  String json_file (int* val_h) {

  String jsonchik = "{";
  jsonchik += "\"key\":\"info_about_AC\",";
  jsonchik += "\"hum1\":" + String(val_h[0]) + ",";
  jsonchik += "\"hum2\":" + String(val_h[1]) + ",";
  jsonchik += "\"hum3\":" + String(val_h[2]) + ",";
  jsonchik += "\"hum4\":" + String(val_h[3]) + ",";
  jsonchik += "\"hum5\":" + String(val_h[4]);
  jsonchik += "}";

  return jsonchik;
  }
