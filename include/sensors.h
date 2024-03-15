#pragma once
#include <AHTxx.h>
#include <BH1750.h>

#define sensorPower 7
#define sensorPin A0

class sensors {
private:
    AHTxx *aht10;
    BH1750 *lightMeter;

    const int trigPin = 9, echoPin = 10;
public:
    sensors();
    ~sensors();

    void startAht();
    void startBh();

    double readTemperature();
    double readHumidity();
    double readLightIntensity();
    double readDistance() const;
    double readSoilMoisture();
};