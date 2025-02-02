#include "lcdView.h"

#include <EEPROM.h>

#include "buttons.h"
#include "helper.h"
#include "logger.h"

// --- HOME ---
void DefaultView::render(ProgramContext& context) {
    auto toggleLogging = [&context]() {
        if (!context.logger.isLogging) {
            context.logger.enable();
        } else {
            context.logger.disable();
        }
    };

    auto debug = [&context]() {
        File logs = SD.open(F("/LOGS/"));
        printDirectory(logs, 0);
        printFile(F("/LOGS/INDEX"));
    };
    onButtonPush(SW_2, debug);


    onButtonPush(SW_1, toggleLogging);

    lcd.setCursor(0, 0);
    lcd.print(F("GPS:"));

    lcd.setCursor(7, 0);
    if (context.fix.valid.location) {
        lcd.write(LCD_CUSTOM_CHECKMARK);
    } else {
        lcd.write('x');
    }

    lcd.setCursor(0, 1);

    lcd.print(F("Log:"));

    lcd.setCursor(7, 1);
    if (context.logger.isLogging) {
        lcd.write(LCD_CUSTOM_CHECKMARK);
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
        lcd.setCursor(0, 1);

        if (context.fix.valid.time) {
            char time[9];
            formatTime(time, context.fix);
            lcd.print(time);
        }
    }
}

// --- INDEX ----

void IndexView::onEnter(ProgramContext& context) {
    lineScroll = 0;
    nbEntries = context.logger.getNbIndexEntries();
    entry = context.logger.parseLogEntry(lineScroll);
}

void IndexView::onExit(ProgramContext& context) {}

void IndexView::render(ProgramContext& context) {
    if (nbEntries == 0) {
        lcd.setCursor(0, 0);
        lcd.print(F("-"));
        return;
    }

    auto sendFileCallback = [this, &context]() {
        context.logger.sendFile(this->entry);
    };

    auto scrollDown = [this, &context]() {
        lcd.clear();
        if (this->lineScroll < this->nbEntries - 1) {
            this->lineScroll++;
            this->entry = context.logger.parseLogEntry(lineScroll);
        }

    };

    auto scrollUp = [this, &context]() {
        lcd.clear();
        if (this->lineScroll >= 1) {
            this->lineScroll--;
            this->entry = context.logger.parseLogEntry(lineScroll);
        }

    };

    onButtonPush(SW_2, scrollUp);
    onButtonPush(SW_3, scrollDown);
    onButtonPush(SW_1, sendFileCallback);

    if (nbEntries > 0) {
        lcd.setCursor(0, 0);
        lcd.print(F("RUN #"));
        lcd.print(entry.id);

        lcd.setCursor(7, 0);
        lcd.write(LCD_LEFT_ARROW);
    }
}

// --- NEW LOG FILE VIEW ---

void NewLogView::render(ProgramContext& context) {
    auto startNew = [&context]() {
        context.logger.newLogFile(context.fix);
        views.selectView(0);
    };

    lcd.setCursor(0, 0);
    lcd.print(F("New log?"));
    lcd.setCursor(0, 1);
    lcd.write(LCD_RIGHT_ARROW);
    lcd.print(F("YES"));

    onButtonPush(SW_1, startNew);
}

// --- SETTINGS ---

SettingsView::SettingsView() {
    subViews[0] = &SettingsView::intervalSettings;
    subViews[1] = &SettingsView::clearSettings;
}

void SettingsView::onEnter(ProgramContext& context) { currentSubView = 0; }

void SettingsView::onExit(ProgramContext& context) {
    EEPROM.put(EEPROM_LOGINTERVAL_ADDR, context.logger.logInterval);
}

void SettingsView::render(ProgramContext& context) {
    auto nextView = [this, &context]() {
        lcd.clear();
        this->currentSubView =
            (this->currentSubView + 1) % NB_SETTINGS_SUBVIEWS;
    };

    onButtonPush(SW_3, nextView);

    subViews[currentSubView](context);
}

void SettingsView::intervalSettings(ProgramContext& context) {
    auto incrementInterval = [&context]() {
        lcd.clear();
        context.logger.logInterval += context.logger.logInterval == 1 ? 4 : 5;
    };

    auto decrementInterval = [&context]() {
        lcd.clear();
        if (context.logger.logInterval >= 5) {
            context.logger.logInterval -= 5;
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
    lcd.write(LCD_RIGHT_ARROW);
    lcd.print(F("YES"));

    onButtonPush(SW_1, clearLogs);
}