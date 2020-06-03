#include "lcdView.h"
#include "globalState.h"

LCDViewManager::LCDViewManager(ProgramContext &ctxt) {
    context = &ctxt;
}

LCDViewManager::~LCDViewManager() {}

void LCDViewManager::renderView() {
    if (nbViews == 0) {
        return;
    }
    current->render(*context); 
}

void LCDViewManager::setView(LCDView *view) {
    if(current) {
        current->onExit(*context);
    }
    lcd.clear();
    view->onEnter(*context);
    current = view;
};

void LCDViewManager::addView(LCDView *view) {
    views[nbViews] = view;
    nbViews++;
}

void LCDViewManager::selectView(int id) {
    if (id > nbViews || id > MAX_VIEWS - 1 || id < 0) {
        return;
    }

    currentView = id;
    setView(views[id]);
}

void LCDViewManager::selectNextView() {
    selectView((currentView + 1) % nbViews);
}