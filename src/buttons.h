#ifndef _H_BUTTONS
#define _H_BUTTONS

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "globalState.h"

// Digital pins for buttons
#define BP1_PIN 15
#define BP0_PIN 16
#define BPEN_PIN 17

// Analog pins for buttons
#define ABP1_PIN A1
#define ABP0_PIN A2
#define ABPEN_PIN A3


// Physical buttons state definition
#define SW_NONE 0 // 011
#define SW_1 1 // 100
#define SW_2 2 // 101
#define SW_3 3 // 110
#define SW_4 4 // 111

typedef uint8_t ButtonId;

// Récupère l'indice du bouton appuyé au moment de l'appel de la fonction
// 0 = SW_NONE
// 1 = SW_1
// 2 = SW_2
// 3 = SW_3
// 4 = SW_4
ButtonId readButton();

void onButtonPush(ButtonId target, void (*f)(void) );

template <typename T>
void onButtonPush(ButtonId target, T* obj, void (*callback)(T&)) {
    if (buttonState != lastButtonState) {
        if (buttonState == target) {
            callback(*obj);
        }
    }
}

#endif