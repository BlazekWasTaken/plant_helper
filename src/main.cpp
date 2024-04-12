#include <Arduino.h>
#include <sensors.h>
#include <output.h>

sensors *s;
output *o;
int buzzer = 8;

void setup() {
    s = new sensors;
    s->startAht();
    s->startBh();

    o = new output;
    o->startLcd();

    pinMode(buzzer,OUTPUT);
}

void loop() {
//    o->writeToSerial((String)s->readTemperature() + " C");
//    o->writeToSerial((String)s->readHumidity() + " %");
//    o->writeToSerial((String)s->readLightIntensity() + " lx");
//    o->writeToSerial((String)s->readDistance() + " cm");
//    o->writeToSerial((String)s->readSoilMoisture() + " %");
//    o->writeToSerial("-------------------------------------");
//    o->writeToSerial("");
    o->writeFirstLine((String)s->readTemperature() + " C");
    delay(5000);
}

