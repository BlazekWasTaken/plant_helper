// ---------------- //
//  buzzer          //
//      Gnd -> Gnd  //
//      S -> 8      //
//  Led matrix      //
//      Vcc -> 5V   //
//      Gnd -> Gnd  //
//      DIN -> 11   //
//      CS -> 12    //
//      CLK -> 13   //
// ---------------- //

#pragma once
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>
#include <LedControl.h>

class output {
private:
    LedControl *matrix;
    LiquidCrystal_I2C *lcd;

    int buzzer = 8;
    int DIN = 11;
    int CS = 12;
    int CLK = 13;

public:
    output();
    ~output();

    void startLcd();
    void startMatrix();

    void writeFirstLine(const String& input);
    void writeSecondLine(const String& input);

    void writeToSerial(const String& input);

    void matrixSmile();
    void matrixMeh();
    void matrixSad();

    void beep() const;
};
