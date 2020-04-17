#include "lcdView.h"

void CoordinateView::render(LiquidCrystal& lcd) {
}


LCDViewManager::LCDViewManager(LiquidCrystal &targetLcd) {
    lcd = &targetLcd;
}

LCDViewManager::~LCDViewManager() {
    for (int i = 0; i < nbViews; i++) {
        delete views[i];
    }
}

void LCDViewManager::renderView() {
    current->render(*lcd);
}

void LCDViewManager::setView(LCDView *view) {
    lcd->clear();
    current = view;
};

void LCDViewManager::addView(LCDView *view) {
    views[nbViews] = view;
    nbViews++;
}

void LCDViewManager::selectView(int id) {
    if (id > nbViews || id > MAX_VIEWS - 1 || id < 0) {
        return;
        Serial.println("Incorrect view id");
    }

    setView(views[id]);
}