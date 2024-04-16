#include <output.h>

output::output() : matrix(nullptr), lcd(nullptr) {
    //Initializing a library for I2C communication
    Wire.begin();
    //Setting the buzzer pin as an output
    pinMode(buzzer,OUTPUT);
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
    if (matrix != nullptr) return;
    matrix = new LedControl(DIN, CLK, CS, 0);
    matrix->shutdown(0,false);
    matrix->setIntensity(0,0);
    matrix->clearDisplay(0);
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

void output::clearLcd() {
    if (lcd == nullptr) return;
    lcd->clear();
}

void output::matrixSmile() {
    int smileFace[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0}
        };
    for(int j=0;j<8;j++){
        for(int i=0;i<8;i++){
            if(smileFace[j][i] == 1) {
                matrix->setLed(0,j,i,true);
            }
            else {
                matrix->setLed(0,j,i,false);
            }
        }
    }
}

void output::matrixMeh() {
    int mehFace[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    for(int j=0;j<8;j++){
        for(int i=0;i<8;i++){
            if(mehFace[j][i] == 1) {
                matrix->setLed(0,j,i,true);
            }
            else {
                matrix->setLed(0,j,i,false);
            }
        }
    }
}

void output::matrixSad() {
    int sadFace[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    for(int j=0;j<8;j++){
        for(int i=0;i<8;i++){
            if(sadFace[j][i] == 1) {
                matrix->setLed(0,j,i,true);
            }
            else {
                matrix->setLed(0,j,i,false);
            }
        }
    }

}

void output::beep() const {
    tone(buzzer, 261, 200);
}

void output::sleepLcd() {
    lcd->noBacklight();
}

void output::wakeLcd() {
    lcd->backlight();
}

void output::sleepMatrix() {
    matrix->shutdown(0, true);
}

void output::wakeMatrix() {
    matrix->shutdown(0, false);
}
