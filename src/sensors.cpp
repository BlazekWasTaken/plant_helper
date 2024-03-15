#include <sensors.h>

sensors::sensors() : aht10(nullptr), lightMeter(nullptr) {
    Wire.begin();

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    pinMode(sensorPower, OUTPUT);
    digitalWrite(sensorPower, LOW);
}

sensors::~sensors() {
    delete aht10;
    delete lightMeter;
    Wire.end();
}

void sensors::startAht() {
    if (aht10 != nullptr) return;
    aht10 = new AHTxx();
}

void sensors::startBh() {
    if (lightMeter != nullptr) return;
    lightMeter = new BH1750();
    lightMeter->begin();
}

double sensors::readTemperature() {
    if (aht10 == nullptr) return -1;
    return aht10->readTemperature();
}

double sensors::readHumidity() {
    if (aht10 == nullptr) return -1;
    return aht10->readHumidity();
}

double sensors::readLightIntensity() {
    if (lightMeter == nullptr) return -1;
    return lightMeter->readLightLevel();
}

double sensors::readDistance() const {
    double duration;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = (double)pulseIn(echoPin, HIGH);
    return duration * 0.034 / 2;
}

double sensors::readSoilMoisture() {
    digitalWrite(sensorPower, HIGH);
    delay(10);
    double val = analogRead(sensorPin);
    digitalWrite(sensorPower, LOW);
    return 100 - (val - 250) / 753 * 100;
}