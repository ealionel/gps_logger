#ifndef LCDVIEW_H_INCLUDED
#define LCDVIEW_H_INCLUDED

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <NMEAGPS.h>

#include "helper.h"
#include "logger.h"

#define MAX_VIEWS 5


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

    ProgramContext* context;
   private:
    LCDView* current;
    LCDView* views[MAX_VIEWS];

    uint8_t currentView;
    uint8_t nbViews = 0;
};

class CoordinateView : public LCDView {
   public:
    void render(ProgramContext& context);
    void onEnter(ProgramContext& context);
    void onExit(ProgramContext& context);
};

class DefaultView : public LCDView {
   public:
    void render(ProgramContext& context);
};

/**
 * Vue qui affiche les différents parcours enregistrés
 */
class IndexView : public LCDView {
   public:
    void render(ProgramContext& context);
    void onEnter(ProgramContext& context);
    void onExit(ProgramContext& context);

    uint8_t lineScroll = 0;
    uint8_t nbEntries = 0;
    LogIndexEntry* entries;
};

class NewLogView : public LCDView {
    public:
    void render(ProgramContext& context);
};

#define NB_SETTINGS_SUBVIEWS 2
class SettingsView : public LCDView {
   public:
   typedef void (*SubViewRenderer)(ProgramContext& context);

    SettingsView();
    void render(ProgramContext& context);
    void onEnter(ProgramContext& context);
    void onExit(ProgramContext& context);

    static void intervalSettings(ProgramContext& context);
    static void clearSettings(ProgramContext& context);

    SubViewRenderer subViews[NB_SETTINGS_SUBVIEWS];
    uint8_t currentSubView = 0;
    // void (SettingsView::*subViews[NB_SETTINGS_SUBVIEWS])(ProgramContext& context);
};


#endif