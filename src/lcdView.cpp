#include "lcdView.h"

void CoordinateView::render(LiquidCrystal& lcd) {
    lcd.setCursor(0,0);
    lcd.print("Coords:");
    lcd.setCursor(0,0);
}

void DefaultView::render(LiquidCrystal& lcd) {
    lcd.setCursor(0,0);
    lcd.print("Hello !");
}

