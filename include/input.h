// ---------------- //
//  joystick        //
//      Gnd -> Gnd  //
//      SW -> 2     //
//      VRX -> A2   //
//      VRY -> A1   //
//      5V -> 5V    //
// ---------------- //

#pragma once
#include <Arduino.h>

class input {
private:
    int buttonPin = 2;
public:
    input();
    int getX();
    int getY();
    int getButtonPin() const;
};