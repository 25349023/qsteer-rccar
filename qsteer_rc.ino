#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>

// Uncomment the following line if you are using a MCU with an 8MHz clock (e.g., 3.3V Arduino Pro Mini).
// See the README.md for details.
// #define CLK_RATE_8MHZ

#ifdef CLK_RATE_8MHZ
#include "command_8mhz.h"
#else
#include "command_16mhz.h"
#endif

enum class Band {
    A, B, C, D
};

enum class Command {
    NOOP = -1, FORWARD, BACKWARD, LEFT, RIGHT, FWD_LEFT, FWD_RIGHT, BACK_LEFT, BACK_RIGHT,
    TURBO_FWD, TURBO_BACK, TURBO_FWD_LEFT, TURBO_FWD_RIGHT, TURBO_BACK_LEFT, TURBO_BACK_RIGHT
};

Band band;

const int X_AXIS = 0;
const int Y_AXIS = 1;
const int TURBO_SW = 4;

void setup() {
    pinMode(TURBO_SW, INPUT_PULLUP);
    Serial.begin(115200);
    band = Band::A;
}

void loop() {
    int x = to_digital(analogRead(X_AXIS)); 
    int y = to_digital(analogRead(Y_AXIS));
    bool turbo = !digitalRead(TURBO_SW);

    Command c = Command::NOOP;

    if (turbo) {
        if (x == 0) {
            if (y == -1) {
                c = Command::TURBO_FWD;
            } else if (y == 1) {
                c = Command::TURBO_BACK;
            }
        } else if (x == 1) {
            if (y == -1) {
                c = Command::TURBO_FWD_RIGHT;
            } else if (y == 1) {
                c = Command::TURBO_BACK_RIGHT;
            } else {
                c = Command::RIGHT;
            }
        } else if (x == -1) {
            if (y == -1) {
                c = Command::TURBO_FWD_LEFT;
            } else if (y == 1) {
                c = Command::TURBO_BACK_LEFT;
            } else {
                c = Command::LEFT;
            }
        }
    } else {
        if (x == 0) {
            if (y == -1) {
                c = Command::FORWARD;
            } else if (y == 1) {
                c = Command::BACKWARD;
            }
        } else if (x == 1) {
            if (y == -1) {
                c = Command::FWD_RIGHT;
            } else if (y == 1) {
                c = Command::BACK_RIGHT;
            } else {
                c = Command::RIGHT;
            }
        } else if (x == -1) {
            if (y == -1) {
                c = Command::FWD_LEFT;
            } else if (y == 1) {
                c = Command::BACK_LEFT;
            } else {
                c = Command::LEFT;
            }
        }
    }

    if (c != Command::NOOP) {
        IrSender.sendRaw(START, sizeof(START) / sizeof(START[0]), 38 * FREQ_RATIO);
        IrSender.sendRaw(BAND_PREFIX[(int)band], sizeof(BAND_PREFIX[0]) / sizeof(BAND_PREFIX[0][0]), 38 * FREQ_RATIO);
        IrSender.sendRaw(COMMANDS[(int)c], sizeof(COMMANDS[0]) / sizeof(COMMANDS[0][0]), 38 * FREQ_RATIO);
    }

    delay(20);
}

int to_digital(int analog) {
    if (analog < 300) {
        return -1;
    } 
    if (analog > 700) {
        return 1;
    } 
    return 0;
}
