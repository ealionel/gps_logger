#include <Arduino.h>
#include <LiquidCrystal.h>
#include <NMEAGPS.h>

#include "buttons.h"
#include "helper.h"

#define MAX_VIEWS 4
class LCDView {
   public:
    virtual void render(LiquidCrystal& lcd, ProgramContext& context) {}
    virtual ~LCDView() {}
};

class LCDViewManager {
   public:
    LCDViewManager(LiquidCrystal& lcd, ProgramContext& context);
    ~LCDViewManager();

    void setView(LCDView* view);
    void selectView(int viewId);
    void selectNextView();

    void addView(LCDView* view);

    void renderView();

   private:
    LCDView* current;
    LCDView* views[MAX_VIEWS];
    LiquidCrystal* lcd;
    ProgramContext* context;

    uint8_t currentView;
    uint8_t nbViews = 0;
};

class CoordinateView : public LCDView {
   public:
    void render(LiquidCrystal& lcd, ProgramContext& context);
};

class DefaultView : public LCDView {
   public:
    void render(LiquidCrystal& lcd, ProgramContext& context);
    int state = 0;
};
