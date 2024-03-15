#include <output.h>

output::output() : matrix(nullptr), lcd(nullptr) {
    Serial.begin(9600);


}

output::~output() {
    delete matrix;
    delete lcd;
}

void output::startLcd() {

}

void output::startMatrix() {

}

void output::writeFirstLine(String input) {

}

void output::writeSecondLine(String input) {

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