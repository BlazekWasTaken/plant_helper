#include <input.h>

int input::getX(){
    return analogRead(A2);
}

int input::getY(){
    return analogRead(A1);
}

int input::getClick(){
    return digitalRead(buttonPin);
}

int input::getButtonPin(){
    return buttonPin;
}

input::input() {
    pinMode(buttonPin, INPUT_PULLUP);
}
