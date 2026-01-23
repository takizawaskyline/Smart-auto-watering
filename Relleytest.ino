#include <PCA95x5.h>

PCA9555 ioex;

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
     ioex.write(static_cast<PCA95x5::Port::Port>(7), PCA95x5::Level::H);
}
