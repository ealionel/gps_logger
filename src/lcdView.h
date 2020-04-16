#include <Arduino.h>
#include <LiquidCrystal.h>
#include "buttons.h"


class LCDView {
    public:
        virtual void render(LiquidCrystal& lcd) {}
};

class TestView : public LCDView {
    public:
        void render(LiquidCrystal& lcd) {
            lcd.setCursor(0,0);
            lcd.print("vue 1");
        }

};

class TestView2 : public LCDView {
    public:
        void render(LiquidCrystal& lcd) {
            lcd.setCursor(0,0);
            lcd.print("vue 2 !");
        }
};

class CoordinateView : public LCDView {
    public:
        void render(LiquidCrystal& lcd);
};

