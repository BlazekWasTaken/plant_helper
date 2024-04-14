#include <input.h>

int input::getX(){
    return analogRead(A2);
}

int input::getY(){
    return analogRead(A1);
}

int input::getClick(){
    return digitalRead(2);
}

input::input() {
    pinMode(2, INPUT_PULLUP);
}
