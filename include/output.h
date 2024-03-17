// ---------------- //
//  buzzer          //
//      Gnd -> Gnd  //
//      S -> 8      //
// ---------------- //

#pragma once
#include <Max72xxPanel.h>
#include <LiquidCrystal_I2C.h>

class output {
private:
    Max72xxPanel *matrix;
    LiquidCrystal_I2C *lcd;

    int buzzer = 8;

public:
    output();
    ~output();

    void startLcd();
    void startMatrix();

    void writeFirstLine(String input);
    void writeSecondLine(String input);

    void writeToSerial(const String& input);

    void matrixSmile();
    void matrixMeh();
    void matrixSad();

    void beep() const;
};
