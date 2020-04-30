#include "lcdView.h"

LCDViewManager::LCDViewManager(LiquidCrystal &targetLcd, ProgramContext &ctxt) {
    lcd = &targetLcd;
    context = &ctxt;
}

LCDViewManager::~LCDViewManager() {
    for (int i = 0; i < nbViews; i++) {
        delete views[i];
    }
}

void LCDViewManager::renderView() {
    if (nbViews == 0) {
        return;
    }
    current->render(*lcd, *context); 
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
    
    currentView = id;
    setView(views[id]);
}

void LCDViewManager::selectNextView() {
    selectView((currentView + 1) % nbViews);
}