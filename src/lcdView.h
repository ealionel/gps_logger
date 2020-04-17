#include <Arduino.h>
#include <LiquidCrystal.h>

#include "buttons.h"

#define MAX_VIEWS 4
class LCDView {
   public:
    virtual void render(LiquidCrystal& lcd) {}
    virtual ~LCDView() {}
};

class LCDViewManager {
   public:
    LCDViewManager(LiquidCrystal& lcd);
    ~LCDViewManager();

    void setView(LCDView* view);
    void selectView(int viewId);

    void addView(LCDView* view);

    void renderView();

   private:
    LCDView* current;
    LCDView* views[MAX_VIEWS];
    LiquidCrystal* lcd;

    uint8_t nbViews = 0;
};

class TestView : public LCDView {
   public:
    void render(LiquidCrystal& lcd) {
        lcd.setCursor(0, 0);
        lcd.print("vue 1");
    }
};

class TestView2 : public LCDView {
   public:
    void render(LiquidCrystal& lcd) {
        lcd.setCursor(0, 0);
        lcd.print("vue 2 !");
    }
};

class CoordinateView : public LCDView {
   public:
    void render(LiquidCrystal& lcd);
};

void setView(LCDView& view);