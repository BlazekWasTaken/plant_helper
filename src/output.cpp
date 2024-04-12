#include <output.h>

output::output() : matrix(nullptr), lcd(nullptr) {
    Serial.begin(9600);
    Wire.begin();
}

output::~output() {
    delete matrix;
    delete lcd;
}

void output::startLcd() {
    if (lcd != nullptr) return;
    lcd = new LiquidCrystal_I2C(0x27, 16, 2);
    lcd->init();
    lcd->clear();
    lcd->backlight();
}

void output::startMatrix() {

}


void output::writeFirstLine(const String& input) {
    if (lcd == nullptr) return;
    lcd->setCursor(0,0);
    lcd->print(input);
}

void output::writeSecondLine(const String& input) {
    if (lcd == nullptr) return;
    lcd->setCursor(0,1);
    lcd->print(input);
}

void output::writeToSerial(const String& input) {
    Serial.println(input);
}

void output::matrixSmile() {

}

void output::matrixMeh() {

}

void output::matrixSad() {

}

void output::beep() const {
    tone(buzzer, 261, 200);
}