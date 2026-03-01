#define GH_INCLUDE_PORTAL
#define dht11_sens 32


#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Drewduino_I2CRelay_PCA95x5.h>
#include <GyverDS18.h>
#include "Cl_timestamp.h"

bool mqttConnected = false;  // Добавлена переменная

int hum_sens_pin[5] = { 32, 33, 34, 35, 36 };
int temp_sens_pin[3] = { 13, 12, 14 };


// Настройки Wi-Fi
const char* ssid = "CPOD";
const char* password = "ApoX51s42wR7FDK8";

// Настройки MQTT
const char* mqtt_server = "m5.wqtt.ru";
const int mqtt_port = 14182;
const char* mqtt_user = "Rasbery";
const char* mqtt_pass = "154321";
const char* mqtt_topic_sens_pub = "kvant/R22/BV/auto_poliv/sensor/data";

unsigned long previousMillis = 0;  // время последней отправки
const long interval = 5000;        // интервал 5 секунд (в миллисекундах)


WiFiClient espClient;
PubSubClient client(espClient);
timeSt test_send_time;

// Инициализация датчиков температуры
GyverDS18Single ds1(temp_sens_pin[0]);
GyverDS18Single ds2(temp_sens_pin[1]);
GyverDS18Single ds3(temp_sens_pin[2]);

void setup() {
  Serial.begin(115200);

  ds1.setResolution(12);
  ds2.setResolution(12);
  ds3.setResolution(12);

  test_send_time.timeSetting("pool.ntp.org", 3 * 3600, 0);  // для timestamp | GMT+3 (Москва) = 3 * 3600 секунд, Летнее время (0, если не используется)

  for (int i = 0; i < 5; i++) {
    pinMode(hum_sens_pin[i], INPUT);
  }

  client.setServer(mqtt_server, mqtt_port);
  client.setBufferSize(256);  // Увеличиваем буфер сообщений

  wifi();
  //client.setCallback(callback);  // Функция обработки входящих сообщений
}

void loop() {

  ds1.tick();
  ds2.tick();
  ds3.tick();

  unsigned long currentMillis = millis();
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
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    String data_json = sens_val();
    client.publish(mqtt_topic_sens_pub, data_json.c_str());
    Serial.println(data_json);
  }
}

void wifi() {
  Serial.println();
  Serial.print("Подключение к ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  delay(1000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi подключён");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  delay(1000);
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
  if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
    Serial.println("Успешно!");
    //ВОТ тут подписка на топик
    //delay(500);
    //client.subscribe(mqtt_topic_sub);
  } else {
    Serial.print("Ошибка, rc=");
    Serial.println(client.state());
  }
}

String sens_val() {
  int val_h[5];
  int val_temp[3];

  for (int i = 0; i < 5; i++) {
    val_h[i] = analogRead(hum_sens_pin[i]);
  }

  val_temp[0] = ds1.getTemp();
  val_temp[1] = ds1.getTemp();
  val_temp[2] = ds2.getTemp();

  String json = json_file(val_h, val_temp);

  return json;
}


String json_file(int* val_h, int* val_temp) {
  test_send_time.timeStam();
  String jsonchik = "{";
  jsonchik += "\"key\":\"info_about_sost\",";
  jsonchik += "\"timestamp\":" + String(test_send_time.timeS) + ",";
  jsonchik += "\"humidity\": {";
  jsonchik += "\"sensor_1\":" + String(val_h[0]) + ",";
  jsonchik += "\"sensor_2\":" + String(val_h[1]) + ",";
  jsonchik += "\"sensor_3\":" + String(val_h[2]) + ",";
  jsonchik += "\"sensor_4\":" + String(val_h[3]) + ",";
  jsonchik += "\"sensor_5\":" + String(val_h[4]) + "},";
  jsonchik += "\"temperature\": {";
  jsonchik += "\"sensor_1\":" + String(val_temp[0]) + ",";
  jsonchik += "\"sensor_2\":" + String(val_temp[1]) + ",";
  jsonchik += "\"sensor_3\":" + String(val_temp[2]);
  jsonchik += "}}";

  return jsonchik;
}

// void callback(char* topic, byte* payload, unsigned int length) {
//   //Печатем топик с которого получено сообщение
//   Serial.print("Получено сообщение [");
//   Serial.print(topic);
//   Serial.print("]: ");

//   // Сохраняем сообщение в переменную  "message"
//   String message;
//   for (int i = 0; i < length; i++) {
//     message += (char)payload[i];
//   }
//   //Печатаем само сообщение в сериал
//   Serial.println(message);

//   //Если получено сообщение с топика mqtt_topic_sub
//   if (String(topic) == mqtt_topic_sub) {
//     if (message == "ТОМУ ЧТО НУЖНО") {
//       // делаем одно
//     }
//     if (message == "ТОМУ ЧТО НУЖНО2") {
//       // делаем другое
//     }
//   }
// }