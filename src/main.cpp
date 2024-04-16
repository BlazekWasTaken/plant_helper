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

int temperatureGood = 20;
const int temperatureTolerance = 10;
int airHumidityGood = 50;
const int airHumidityTolerance = 30;
int lightIntensityGood = 200;
const int lightIntensityTolerance = 150;
int soilMoistureGood = 50;
const int soilMositureTolerance = 30;

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
    o->writeToSerial((String)soilMoistureGood + " moisture");
    o->writeToSerial((String)lightIntensityGood + " light");
    o->writeToSerial((String)temperatureGood + " temperature");
    o->writeToSerial((String)airHumidityGood + " humidity");
    o->writeToSerial("-------------------------------------");
    o->writeToSerial("");
#pragma endregion

    o->clear();

    switch (st) {
        case Scrolling:
            display();
            if (d == Moisture) d = Temperature;
            else d = Display(d + 1);

            // If every condition is satisfied, display happy face :)
            if(s->readSoilMoisture() > soilMoistureGood - soilMositureTolerance
                && s->readTemperature() > temperatureGood - temperatureTolerance
                && s->readTemperature() < temperatureGood + temperatureTolerance
                && s->readHumidity() > airHumidityGood - airHumidityTolerance
                && s->readLightIntensity() > lightIntensityGood - lightIntensityTolerance) {
                o->matrixSmile();
            }
            // If only dirt moisture is satisfied, dislpay meh face :|
            else if(s->readSoilMoisture() > soilMoistureGood - soilMositureTolerance) {
                o->matrixMeh();
            }
            // If no condition is satisfied, display sad face and beep :(
            else {
                o->beep();
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
                int a = temperatureGood;
                o->writeFirstLine("Set temperature:");
                o->writeSecondLine((String)a + " C");
                if (i->getY() > 700) {
                    a += 5;
                    o->writeFirstLine("Set temperature:");
                    o->writeSecondLine((String)a + " C");
                }
                if (i->getY() < 300) {
                    a -= 5;
                    o->writeFirstLine("Set temperature:");
                    o->writeSecondLine((String)a + " C");
                }
                temperatureGood = a;
            }
            if (d == Humidity) {
                int a = airHumidityGood;
                o->writeFirstLine("Set humidity:");
                o->writeSecondLine((String)a + " %");
                if (i->getY() > 700) {
                    a += 5;
                    o->writeFirstLine("Set humidity:");
                    o->writeSecondLine((String)a + " %");
                }
                if (i->getY() < 300) {
                    a -= 5;
                    o->writeFirstLine("Set temperature:");
                    o->writeSecondLine((String)a + " %");
                }
                // Set percentage only if it is in boundaries (it can't be negative nor more than 100)
                if (a >= 0 && a <= 100) {
                    airHumidityGood = a;
                }
                else if (a >= 0){
                    a = 100;
                }
                else {
                    a = 0;
                }
            }
            if (d == Light) {
                int a = lightIntensityGood;
                o->writeFirstLine("Set light intensity:");
                o->writeSecondLine((String)a + " lx");
                if (i->getY() > 700) {
                    a += 50;
                    o->writeFirstLine("Set light intensity:");
                    o->writeSecondLine((String)a + " lx");
                }
                if (i->getY() < 300) {
                    a -= 50;
                    o->writeFirstLine("Set light intensity:");
                    o->writeSecondLine((String)a + " lx");
                }
                lightIntensityGood= a;
            }
            if (d == Moisture) {
                int a = soilMoistureGood;
                o->writeFirstLine("Set soil moisture:");
                o->writeSecondLine((String)a + " %");
                if (i->getY() > 700) {
                    a += 10;
                    o->writeFirstLine("Set soil moisture:");
                    o->writeSecondLine((String)a + " %");
                }
                if (i->getY() < 300) {
                    a -= 10;
                    o->writeFirstLine("Set soil moisture:");
                    o->writeSecondLine((String)a + " %");
                }
                // Set percentage only if it is in boundaries (it can't be negative nor more than 100)
                if (a >= 0 && a <= 100) {
                    soilMoistureGood = a;
                }
                else if (a >= 0){
                    a = 100;
                }
                else {
                    a = 0;
                }
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



