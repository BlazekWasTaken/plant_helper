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
public:
    input();
    int getX();
    int getY();
    int getClick();
};