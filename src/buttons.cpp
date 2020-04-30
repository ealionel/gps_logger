#include "buttons.h"

// #include "globalState.h"

ButtonId readButton() {
    uint8_t bpen = digitalRead(BPEN_PIN), bp1 = digitalRead(BP1_PIN),
            bp0 = digitalRead(BP0_PIN);

    // Concatène les 3 valeurs binaires en un entier et retire l'offset qui est
    // de 3 (commence à 011)
    return (bpen << 2 | bp1 << 1 | bp0) - 3;
}

void onButtonPush(ButtonId target, void (*callback)(void)) {
    if (buttonState != lastButtonState) {
        if (buttonState == target) {
            callback();
        }
    }
}

void onButtonPush(ButtonId target, LiquidCrystal &lcd, void (*callback)(LiquidCrystal&)) {
    if (buttonState != lastButtonState) {
        if (buttonState == target) {
            callback(lcd);
        }
    }
}

