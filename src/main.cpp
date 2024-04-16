#include <Arduino.h>
#include <sensors.h>
#include <output.h>
#include <input.h>

sensors *s;
output *o;
input *i;

//Enumerators for variables and display modes
enum Display { Temperature = 0, Humidity = 1, Light = 2, Moisture = 3 };
Display d = Temperature;
enum Stage { Scrolling = 0, Setting = 1, Changing = 2 };
Stage st = Scrolling;

//Variables for controlling the plant's environment
int temperatureGood = 20;
const int temperatureTolerance = 10;
int airHumidityGood = 50;
const int airHumidityTolerance = 30;
int lightIntensityGood = 200;
const int lightIntensityTolerance = 150;
int soilMoistureGood = 50;
const int soilMoistureTolerance = 30;

//Variables for replacing the delay function
unsigned long timeWait;
unsigned long timeButton;
unsigned long timeSleep;

//A function triggered by an interrupt (implemented below)
void changeStage();
//A function handling the "power saving" (turning displays on and off)
void powerSaving();
//A function displaying all sensor readings to the lcd screen
void displayList();
//A function handling the change of "good" values
void displayAndChange();
void scrollForward();
void scrollBackward();
void handleLedsAndBuzzer();

void setup() {
    //Creating a new instance of sensors class
    s = new sensors;
    //Starting light sensor and humidity and temp sensors
    s->startAht();
    s->startBh();

    //Creating a new instance of output class
    o = new output;
    //Initializing the lcd and led matrix
    o->startLcd();
    o->startMatrix();

    //Creating a new instance of input class
    i = new input;
    //Creating an interrupt for joystick button
    attachInterrupt(digitalPinToInterrupt(i->getButtonPin()), changeStage, FALLING);

    //Setting the wait time so that the countdown will start immediately
    timeSleep = millis() + 30000;
}

void loop() {
    powerSaving();

    //delay function replacement
    if (timeWait > millis()) return;

    o->clearLcd();

    switch (st) {
        //Scrolling displays the monitored values one by one in 2 second intervals
        case Scrolling:
            timeWait = millis() + 2000;

            displayList();

            //Change the value displayed on the screen
            scrollForward();

            handleLedsAndBuzzer();
            break;
        //Setting allows the user to manually scroll the displayed values using the joystick
        case Setting:
            timeWait = millis() + 500;

            if (i->getX() > 700) {
                scrollForward();
            }
            if (i->getX() < 300) {
                scrollBackward();
            }

            displayList();
            break;
        //Changing allows the user to set the desired "Good" values
        case Changing:
            timeWait = millis() + 200;

            displayAndChange();
            break;
        default:
            o->writeFirstLine("Error");
            break;
    }
}

void changeStage() {
    //Delay function replacement
    if (timeButton > millis()) return;

    //Sound indication of getting input from pressing the button
    o->beep();
    //Changing the stage of the program according to order
    if (st == Changing) st = Scrolling;
    else st = Stage(st + 1);

    //Delay function replacement
    timeButton = millis() + 200;
}

void powerSaving() {
    //Reading the distance and starting the countdown if distance read is equal or greater than 40 cm
    if (s->readDistance() < 40) timeSleep = millis() + 30000;

    //Handling turning on and off
    if (timeSleep > millis()) {
        o->wakeLcd();
        o->wakeMatrix();
    }
    else {
        o->sleepLcd();
        o->sleepMatrix();
    }
}

void displayList() {
    String prefix = "";
    if (st == Setting) {
        prefix = "*";
    }

    switch (d) {
        case Temperature:
            o->writeFirstLine(prefix + "Temperature:");
            o->writeSecondLine((String)s->readTemperature() + " C");
            break;
        case Humidity:
            o->writeFirstLine(prefix + "Humidity:");
            o->writeSecondLine((String)s->readHumidity() + "%");
            break;
        case Light:
            o->writeFirstLine(prefix + "Light intensity:");
            o->writeSecondLine((String)s->readLightIntensity() + " lx");
            break;
        case Moisture:
            o->writeFirstLine(prefix + "Soil moisture:");
            o->writeSecondLine((String)s->readSoilMoisture() + "%");
            break;
        default:
            o->writeFirstLine( prefix + "Error");
            break;
    }
}

void scrollForward() {
    if (d == Moisture) d = Temperature;
    else d = Display(d + 1);
}

void scrollBackward() {
    if (d == Temperature) d = Moisture;
    else d = Display(d - 1);
}

void handleLedsAndBuzzer() {
    // If every condition is satisfied, display happy face :)
    if(s->readSoilMoisture() > soilMoistureGood - soilMoistureTolerance
       && s->readTemperature() > temperatureGood - temperatureTolerance
       && s->readTemperature() < temperatureGood + temperatureTolerance
       && s->readHumidity() > airHumidityGood - airHumidityTolerance
       && s->readLightIntensity() > lightIntensityGood - lightIntensityTolerance) {
        o->matrixSmile();
    }
    // If only soil moisture is satisfied, display meh face :|
    else if(s->readSoilMoisture() > soilMoistureGood - soilMoistureTolerance) {
        o->matrixMeh();
    }
    // If no condition is satisfied, display sad face and beep :(
    else {
        o->beep();
        o->matrixSad();
    }
}

void displayAndChange() {
    int tempValue;

    switch (d) {
        case Temperature:
            tempValue = temperatureGood;

            if (i->getY() > 700) tempValue -= 5;
            else if (i->getY() < 300) tempValue += 5;
            o->writeFirstLine("Set temperature:");
            o->writeSecondLine((String)tempValue + " C");

            //Set temperature values only in a range that makes more or less sense
            if (tempValue > 50) tempValue = 50;
            else if (tempValue < 5) tempValue = 5;
            temperatureGood = tempValue;
            break;
        case Humidity:
            tempValue = airHumidityGood;

            if (i->getY() > 700) tempValue -= 5;
            else if (i->getY() < 300) tempValue += 5;
            o->writeFirstLine("Set humidity:");
            o->writeSecondLine((String)tempValue + " %");

            // Set percentage only if it is in boundaries (it can't be negative nor more than 100)
            if (tempValue > 100) tempValue = 100;
            else if (tempValue < 0) tempValue = 0;
            airHumidityGood = tempValue;
            break;
        case Light:
            tempValue = lightIntensityGood;

            if (i->getY() > 700) tempValue -= 50;
            else if (i->getY() < 300) tempValue += 50;
            o->writeFirstLine("Set light intensity:");
            o->writeSecondLine((String)tempValue + " lx");

            // Set value only if it is in boundaries (it can't be negative)
            if (tempValue < 0) tempValue = 0;
            lightIntensityGood = tempValue;
            break;
        case Moisture:
            tempValue = soilMoistureGood;

            if (i->getY() > 700) tempValue -= 10;
            else if (i->getY() < 300) tempValue += 10;
            o->writeFirstLine("Set soil moisture:");
            o->writeSecondLine((String)tempValue + " %");

            // Set percentage only if it is in boundaries (it can't be negative nor more than 100)
            if (tempValue > 100) tempValue = 100;
            else if (tempValue < 0) tempValue = 0;
            soilMoistureGood = tempValue;
            break;
    }
}