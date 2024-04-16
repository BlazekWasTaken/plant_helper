#include <input.h>

//Self documenting coe

int input::getX(){
    return analogRead(A2);
}

int input::getY(){
    return analogRead(A1);
}

int input::getButtonPin() const{
    return buttonPin;
}

input::input() {
    pinMode(buttonPin, INPUT_PULLUP);
}
