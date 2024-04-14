#include <Arduino.h>
#include <sensors.h>
#include <output.h>
#include <input.h>

sensors *s;
output *o;
input *i;
int buzzer = 8;
enum Display { Temperature = 0, Humidity = 1, Light = 2, Moisture = 3 };
Display d = Temperature;
enum Stage { Scrolling = 0, Setting = 1, Changing = 2 };
Stage st = Scrolling;

int goodTemp = 23;
int goodHum = 10;
int goodLigh = 100;
int goodMoi = 50;

void display();

void setup() {
    s = new sensors;
    s->startAht();
    s->startBh();

    o = new output;
    o->startLcd();
    o->startMatrix();

    i = new input;

    pinMode(buzzer,OUTPUT);
}

void loop() {
#pragma region debug
    o->writeToSerial((String)s->readTemperature() + " C");
    o->writeToSerial((String)s->readHumidity() + " %");
    o->writeToSerial((String)s->readLightIntensity() + " lx");
    o->writeToSerial((String)s->readDistance() + " cm");
    o->writeToSerial((String)s->readSoilMoisture() + " %");
    o->writeToSerial((String)i->getX() + " - x");
    o->writeToSerial((String)i->getY() + " - y");
    o->writeToSerial((String)i->getClick() + " - click status");
    o->writeToSerial((String)goodMoi + " moisture");
    o->writeToSerial((String)goodLigh + " light");
    o->writeToSerial((String)goodTemp + " temperature");
    o->writeToSerial((String)goodHum + " humidity");
    o->writeToSerial("-------------------------------------");
    o->writeToSerial("");
#pragma endregion

    o->clear();

    switch (st) {
        case Scrolling:
            display();
            if (d == Moisture) d = Temperature;
            else d = Display(d + 1);

            if(s->readSoilMoisture() > goodMoi - 20 && s->readTemperature() < goodTemp + 1.5  && s->readTemperature() > goodTemp - 1.5 && s->readHumidity() > goodHum - 5 && s->readHumidity() < goodHum + 5 && s->readLightIntensity() < goodLigh + 10 && s->readLightIntensity() > goodLigh - 10) {
                o->matrixSmile();
            }
            else if(s->readSoilMoisture() < goodMoi - 20 || s->readSoilMoisture() > goodMoi - 70) {
                o->matrixMeh();
            }
            else {
                o->matrixSad();
            }

            delay(3000);
            if (i->getClick() == 0) {
                st = Setting;
            }
            break;
        case Setting:
            if (i->getY() > 700) {
                if (d == Moisture) d = Temperature;
                else d = Display(d + 1);
            }
            if (i->getY() < 300) {
                if (d == Temperature) d = Moisture;
                else d = Display(d - 1);
            }
            display();
            delay(500);
            if (i->getClick() == 0) st = Changing;
            break;
        case Changing:
            if (d == Temperature) {
                int a = goodTemp;
                o->writeFirstLine("Set temperature:");
                o->writeSecondLine((String)a + " C");
                if (i->getY() > 700) {
                    a += 2;
                    o->writeFirstLine("Set temperature:");
                    o->writeSecondLine((String)a + " C");
                }
                if (i->getY() < 300) {
                    a -= 2;
                    o->writeFirstLine("Set temperature:");
                    o->writeSecondLine((String)a + " C");
                }
                goodTemp = a;
            }
            if (d == Humidity) {
                int a = goodHum;
                o->writeFirstLine("Set humidity:");
                o->writeSecondLine((String)a + " %");
                if (i->getY() > 700) {
                    a += 1;
                    o->writeFirstLine("Set humidity:");
                    o->writeSecondLine((String)a + " %");
                }
                if (i->getY() < 300) {
                    a -= 1;
                    o->writeFirstLine("Set temperature:");
                    o->writeSecondLine((String)a + " %");
                }
                goodHum = a;
            }
            if (d == Light) {
                int a = goodLigh;
                o->writeFirstLine("Set light intensity:");
                o->writeSecondLine((String)a + " lx");
                if (i->getY() > 700) {
                    a += 5;
                    o->writeFirstLine("Set light intensity:");
                    o->writeSecondLine((String)a + " lx");
                }
                if (i->getY() < 300) {
                    a -= 5;
                    o->writeFirstLine("Set light intensity:");
                    o->writeSecondLine((String)a + " lx");
                }
                goodLigh= a;
            }
            if (d == Moisture) {
                int a = goodMoi;
                o->writeFirstLine("Set soil moisture:");
                o->writeSecondLine((String)a + " %");
                if (i->getY() > 700) {
                    a += 1;
                    o->writeFirstLine("Set soil moisture:");
                    o->writeSecondLine((String)a + " %");
                }
                if (i->getY() < 300) {
                    a -= 1;
                    o->writeFirstLine("Set soil moisture:");
                    o->writeSecondLine((String)a + " %");
                }
                goodMoi = a;
            }
            delay(500);
            if (i->getClick() == 0) {
                st = Scrolling;
            }
            break;
        default:
            o->writeFirstLine("Error");
            break;
    }
}

void display(){
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
}



