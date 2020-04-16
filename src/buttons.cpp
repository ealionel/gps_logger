#include "buttons.h"

uint8_t readButton() {
    uint8_t bpen = digitalRead(BPEN_PIN), bp1 = digitalRead(BP1_PIN),
            bp0 = digitalRead(BP0_PIN);

    if (!bpen && bp1 && bp0) {
        // 011
        return SW_NONE;
    }
    if (bpen && !bp1 && !bp0) {
        // 100
        return SW_1;
    }
    if (bpen && !bp1 && bp0) {
        // 101
        return SW_2;
    }
    if (bpen && bp1 && !bp0) {
        // 110
        return SW_3;
    }
    if (bpen && bp1 && bp0) {
        // 111
        return SW_4;
    }

    return SW_NONE;
}
