#include "lcdView.h"

#include "globalState.h"
#include "logger.h"

// --- HOME ---
void DefaultView::render(ProgramContext& context) {
    lcd.setCursor(0, 0);
    lcd.print("Hello !");

    onButtonPush<DefaultView>(SW_3, this, [](DefaultView &r)->void{
        r.state++;
        Serial.print("State : ");
        Serial.println(r.state);
    });

    if (context.fix.valid.time) {
        lcd.setCursor(0, 1);

        lcd.print(context.fix.dateTime.hours);
        lcd.print(F(":"));
        lcd.print(context.fix.dateTime.minutes);
        lcd.print(F(":"));
        lcd.print(context.fix.dateTime.seconds);
    }
}

// --- COORDINATES VIEW ---

void CoordinateView::render(ProgramContext& context) {
    lcd.setCursor(0, 0);
    lcd.print("Coords:");
    if (context.fix.valid.location) {
        lcd.setCursor(0, 1);
        lcd.print(context.fix.latitude());
    }
}


// --- INDEX ----

void IndexView::onEnter(ProgramContext& context) {
    lineScroll = 0;
    nbEntries = context.logger.getNbIndexEntries();
    entries = context.logger.loadIndexFile();
}

void IndexView::onExit(ProgramContext& context) {
    delete [] entries;
}

void IndexView::render(ProgramContext& context) {

    if (nbEntries == 0) {
        lcd.setCursor(0, 0);
        lcd.print("Empty");
        return;
    }

    onButtonPush<IndexView>(SW_3, this, [](IndexView &v)->void{
        lcd.clear();
        if (v.lineScroll < v.nbEntries - 1) {
            v.lineScroll++;
        }
    });

    onButtonPush<IndexView>(SW_2, this, [](IndexView &v)->void{
        lcd.clear();
        if (v.lineScroll >= 1) {
            v.lineScroll--;
        }
    });

    onButtonPush<ProgramContext>(SW_1, &context, [](ProgramContext &c)->void{
        c.logger.clearDirectory();
    });

    // onButtonPush<IndexView>(SW_1, this, [](IndexView &v)->void{
    //     v.onEnter();
    //     lcd.clear();
    // });

    if (nbEntries > 0) {
        lcd.setCursor(0, 0);
        lcd.print(entries[lineScroll].fileName);
    }

    if (lineScroll < nbEntries - 1) {
        lcd.setCursor(0, 1);
        lcd.print(entries[lineScroll+1].fileName);
    }
}

// --- SETTINGS ---

// void SettingsView::onEnter(ProgramContext& context) {
    
// }

// void SettingsView::onExit(ProgramContext& context) {
    
// }

void SettingsView::render(ProgramContext& context) {
    onButtonPush<ProgramContext>(SW_2, &context, [](ProgramContext &c)->void {
        lcd.clear();
        c.logger.logInterval++;
    });

    onButtonPush<ProgramContext>(SW_3, &context, [](ProgramContext &c)->void {
        lcd.clear();
        if (c.logger.logInterval > 1)  {
            c.logger.logInterval--;
        }
    });

    lcd.setCursor(0, 0);
    lcd.print("Interval");
    lcd.setCursor(0, 1);

    lcd.print(context.logger.logInterval);
}


