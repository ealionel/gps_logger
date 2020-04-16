#include <Arduino.h>

#define SW_NONE 3 // 011
#define SW_1 4 // 100
#define SW_2 5 // 101
#define SW_3 6 // 110
#define SW_4 7 // 111

// Digital pins for buttons
#define BP1_PIN 15
#define BP0_PIN 16
#define BPEN_PIN 17

// Analog pins for buttons
#define ABP1_PIN A1
#define ABP0_PIN A2
#define ABPEN_PIN A3

// Récupère l'ID du bouton au moment de la lecture
uint8_t readButton();