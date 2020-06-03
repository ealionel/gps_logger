#include "lcdView.h"

#include "buttons.h"
#include "logger.h"
#include "helper.h"
#include <EEPROM.h>

// --- HOME ---
void DefaultView::render(ProgramContext& context) {
    auto toggleLogging = [&context]() {
        if (!context.logger.isLogging) {
            context.logger.enable();
        } else {
            context.logger.disable();
        }
    };

    onButtonPush(SW_1, toggleLogging);

    lcd.setCursor(0, 0);
    lcd.print(F("GPS: "));

    lcd.setCursor(7, 0);
    if (context.fix.valid.location) {
        lcd.write(byte(0));
    } else {
        lcd.write('x');
    }

    lcd.setCursor(0, 1);

    lcd.print(F("Log: "));

    lcd.setCursor(7, 1);
    if (context.logger.isLogging) {
        lcd.write(byte(0));
    } else {
        lcd.write('x');
    }
}

// --- COORDINATES VIEW ---

void CoordinateView::onEnter(ProgramContext& context) {}

void CoordinateView::onExit(ProgramContext& context) {}

void CoordinateView::render(ProgramContext& context) {
    lcd.setCursor(0, 0);
    if (context.fix.valid.location) {
        lcd.print(context.fix.longitude(), 6);
        lcd.setCursor(0, 1);
        lcd.print(context.fix.latitude(), 6);
    } else {
        lcd.print(F("No GPS"));
        lcd.setCursor(0,1);

        lcd.print(formatTime(context.fix));
        // lcd.print(F("signal."));
    }
}

// --- INDEX ----

void IndexView::onEnter(ProgramContext& context) {
    lineScroll = 0;
    nbEntries = context.logger.getNbIndexEntries();
    entries = context.logger.loadIndexFile();
}

void IndexView::onExit(ProgramContext& context) {
    Serial.println("debug clear");
    delete[] entries;
    entries = NULL;
}

void IndexView::render(ProgramContext& context) {
    if (nbEntries == 0) {
        lcd.setCursor(0, 0);
        lcd.print(F("Empty"));
        return;
    }

    onButtonPush<IndexView>(SW_3, this, [](IndexView& v) -> void {
        lcd.clear();
        if (v.lineScroll < v.nbEntries - 1) {
            v.lineScroll++;
        }
    });

    onButtonPush<IndexView>(SW_2, this, [](IndexView& v) -> void {
        lcd.clear();
        if (v.lineScroll >= 1) {
            v.lineScroll--;
        }
    });

    if (nbEntries > 0) {
        lcd.setCursor(0, 0);
        lcd.print(entries[lineScroll].fileName);

        lcd.setCursor(7, 0);
        lcd.write(byte(LEFT_ARROW_LCD));
    }

    if (lineScroll < nbEntries - 1) {
        lcd.setCursor(0, 1);
        lcd.print(entries[lineScroll + 1].fileName);
    }
}

// --- NEW LOG FILE VIEW ---

void NewLogView::render(ProgramContext& context) {
    auto startNew = [&context]() {
        context.logger.newLogFile(context.fix);

        views.selectView(0);
    };

    lcd.setCursor(0,0);
    lcd.print(F("New log?"));
    lcd.print(F("1 = YES"));

    onButtonPush(SW_1, startNew);
}

// --- SETTINGS ---

SettingsView::SettingsView() {
    subViews[0] = &SettingsView::intervalSettings;
    subViews[1] = &SettingsView::clearSettings;

    // subViews = {&intervalSettings, &clearSettings};
}

void SettingsView::onEnter(ProgramContext& context) {
    currentSubView = 0;
}

void SettingsView::onExit(ProgramContext& context) {
    EEPROM.put(EEPROM_INTERVAL_ADDR, context.logger.logInterval);
}


void SettingsView::render(ProgramContext& context) {
    auto nextView = [this, &context]() {
        lcd.clear();
        this->currentSubView = (this->currentSubView + 1) % NB_SETTINGS_SUBVIEWS;
    };

    onButtonPush(SW_3, nextView);

    subViews[currentSubView](context);
}

void SettingsView::intervalSettings(ProgramContext& context) {
    auto incrementInterval = [&context]() {
        lcd.clear();
        context.logger.logInterval += context.logger.logInterval == 1? 4:5;
    };

    auto decrementInterval = [&context]() {
        lcd.clear();
        if (context.logger.logInterval >= 5) {
            context.logger.logInterval-=5;
        }

        if (context.logger.logInterval < 1) {
            context.logger.logInterval = 1;
        }
    };

    onButtonPush(SW_1, decrementInterval);
    onButtonPush(SW_2, incrementInterval);

    lcd.setCursor(0, 0);
    lcd.print("Interval");

    lcd.setCursor(0, 1);
    lcd.print(secToMin(context.logger.logInterval));
}

void SettingsView::clearSettings(ProgramContext& context) {
    auto clearLogs = [&context]() {
        context.logger.clearDirectory();
        views.selectView(0);
    };


    lcd.setCursor(0, 0);
    lcd.print(F("Reset?"));
    lcd.setCursor(0, 1);
    lcd.print(F("1 = YES"));

    onButtonPush(SW_1, clearLogs);
}