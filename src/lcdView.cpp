#include "lcdView.h"

void CoordinateView::render(LiquidCrystal& lcd, ProgramContext& context) {
    lcd.setCursor(0, 0);
    lcd.print("Coords:");
    if (context.fix.valid.location) {
        lcd.setCursor(0, 1);
        lcd.print(context.fix.latitude());
    }
}

void DefaultView::render(LiquidCrystal& lcd, ProgramContext& context) {
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
