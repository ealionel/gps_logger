#ifndef LCDVIEW_H_INCLUDED
#define LCDVIEW_H_INCLUDED

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <NMEAGPS.h>

#include "buttons.h"
#include "helper.h"
#include "logger.h"

#define MAX_VIEWS 4
class LCDView {
   public:
    // Méthode principale pour faire rendu sur l'écran
    virtual void render(ProgramContext& context) {}

    // Méthode appelée lorsque la vue est chargée
    virtual void onEnter(ProgramContext& context) {}

    // Méthode appelée lorsque la vue est déchargée
    virtual void onExit(ProgramContext& context) {}
    virtual ~LCDView() {}
};

class LCDViewManager {
   public:
    LCDViewManager(ProgramContext& context);
    ~LCDViewManager();

    void setView(LCDView* view);
    void selectView(int viewId);
    void selectNextView();

    void addView(LCDView* view);

    void renderView();

   private:
    LCDView* current;
    LCDView* views[MAX_VIEWS];
    ProgramContext* context;

    uint8_t currentView;
    uint8_t nbViews = 0;
};

class CoordinateView : public LCDView {
   public:
    void render(ProgramContext& context);
};

class DefaultView : public LCDView {
   public:
    void render(ProgramContext& context);
    int state = 0;
};

/**
 * Vue qui affiche les différents parcours enregistrés
 */
class IndexView : public LCDView {
   public:
    void render(ProgramContext& context);
    void onEnter(ProgramContext& context);
    void onExit(ProgramContext& context);

    int lineScroll = 0;
    int nbEntries = 0;
    LogIndexEntry* entries;
};

class SettingsView : public LCDView {
   public:
    void render(ProgramContext& context);
    void onEnter(ProgramContext& context);
    void onExit(ProgramContext& context);
};

#endif