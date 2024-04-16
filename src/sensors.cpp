#include <sensors.h>

sensors::sensors() : aht10(nullptr), lightMeter(nullptr) {
    //Initializing a library for I2C communication
    Wire.begin();

    //Setting pins for the distance sensor
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

sensors::~sensors() {
    delete aht10;
    delete lightMeter;
    Wire.end();
}

void sensors::startAht() {
    if (aht10 != nullptr) return;
    //Initializing a library for the temperature and humidity sensor
    aht10 = new AHTxx();
}

void sensors::startBh() {
    if (lightMeter != nullptr) return;
    //Initializing a library for the light sensor
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
    delay(10);
    double val = analogRead(sensorPin);
    //Calculating the percentage of soil moisture (based on values read by the sensor in dry air and in water)
    double moisture = 100 - (val - 250) / 753 * 100;
    if (moisture > 100) return 100;
    else if (moisture < 0) return 0;
    else return moisture;
}