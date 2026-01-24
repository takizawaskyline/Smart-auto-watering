#include <PCA95x5.h>

PCA9555 ioex;
// Создаем массив с нужными портами
PCA95x5::Port::Port relayPorts[] = {
    PCA95x5::Port::P00, // Реле 1
    PCA95x5::Port::P01, // Реле 2
    PCA95x5::Port::P02, // Реле 3
    PCA95x5::Port::P03, // Реле 4
    PCA95x5::Port::P04, // Реле 5
    PCA95x5::Port::P05, // Реле 6
    PCA95x5::Port::P06, // Реле 7
    PCA95x5::Port::P07  // Реле 8
};

void setup() {
    Serial.begin(115200);
    delay(2000);

    Wire.begin();
    ioex.attach(Wire);
    ioex.polarity(PCA95x5::Polarity::ORIGINAL_ALL);
    ioex.direction(PCA95x5::Direction::OUT_ALL);
    ioex.write(PCA95x5::Level::L_ALL);
}

void loop() {
  for (int i = 0; i < 8; i++) {
    ioex.write(relayPorts[i], PCA95x5::Level::H);
    delay(300);
  }
  for (int i = 0; i < 8; i++) {
    ioex.write(relayPorts[i], PCA95x5::Level::L);
    delay(300);
  }
}