#include <Arduino.h>
#include <sensors.h>
#include <output.h>

sensors *s;
output *o;
int buzzer = 8;
enum Display { Temperature = 0, Humidity = 1, Light = 2, Moisture = 3 };
Display d = Temperature;

void setup() {
    s = new sensors;
    s->startAht();
    s->startBh();

    o = new output;
    o->startLcd();
    o->startMatrix();

    pinMode(buzzer,OUTPUT);
}

void loop() {
#pragma region debug
    o->writeToSerial((String)s->readTemperature() + " C");
    o->writeToSerial((String)s->readHumidity() + " %");
    o->writeToSerial((String)s->readLightIntensity() + " lx");
    o->writeToSerial((String)s->readDistance() + " cm");
    o->writeToSerial((String)s->readSoilMoisture() + " %");
    o->writeToSerial("-------------------------------------");
    o->writeToSerial("");
#pragma endregion

    o->clear();
    switch (d) {
        case Temperature:
            o->writeFirstLine("Temperature:");
            o->writeSecondLine((String)s->readTemperature() + " C");
            break;
        case Humidity:
            o->writeFirstLine("Humidity:");
            o->writeSecondLine((String)s->readHumidity() + "%");
            break;
        case Light:
            o->writeFirstLine("Light intensity:");
            o->writeSecondLine((String)s->readLightIntensity() + " lx");
            break;
        case Moisture:
            o->writeFirstLine("Soil moisture:");
            o->writeSecondLine((String)s->readSoilMoisture() + "%");
            break;
        default:
            o->writeFirstLine("Error");
            break;
    }
    if (d == Moisture) d = Temperature;
    else d = Display(d + 1);

    if(s->readSoilMoisture() > 50) {
        o->matrixSmile();
    }
    else if(s->readSoilMoisture() > 30) {
        o->matrixMeh();
    }
    else {
        o->matrixSad();
    }

    delay(3000);
}

