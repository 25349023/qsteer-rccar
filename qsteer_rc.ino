#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>

const unsigned int START[2] = {1600, 400};

enum class Band {
    A, B, C, D
};

const unsigned int BAND_PREFIX[4][4] = {
    {400, 400, 400, 400},
    {400, 400, 800, 400},
    {800, 400, 400, 400},
    {800, 400, 800, 400},
};

enum class Command {
    NOOP = -1, FORWARD, BACKWARD, LEFT, RIGHT, FWD_LEFT, FWD_RIGHT, BACK_LEFT, BACK_RIGHT,
    TURBO_FWD, TURBO_BACK, TURBO_FWD_LEFT, TURBO_FWD_RIGHT, TURBO_BACK_LEFT, TURBO_BACK_RIGHT
};

const unsigned int COMMANDS[][8] = {
    {400, 400, 400, 400, 400, 400, 800, 400},  // FWD
    {400, 400, 400, 400, 800, 400, 400, 400},  // BACK
    {400, 400, 400, 400, 800, 400, 800, 400},  // LEFT
    {400, 400, 800, 400, 400, 400, 400, 400},  // RIGHT
    {400, 400, 800, 400, 800, 400, 400, 400},  // FWD_LEFT
    {400, 400, 800, 400, 800, 400, 800, 400},  // FWD_RIGHT
    {800, 400, 400, 400, 800, 400, 400, 400},  // BACK_LEFT
    {800, 400, 400, 400, 800, 400, 800, 400},  // BACK_RIGHT

    {400, 400, 800, 400, 400, 400, 800, 400},  // TURBO_FWD
    {800, 400, 800, 400, 400, 400, 400, 400},  // TURBO_BACK
    {800, 400, 400, 400, 400, 400, 400, 400},  // TURBO_FWD_LEFT
    {800, 400, 400, 400, 400, 400, 800, 400},  // TURBO_FWD_RIGHT
    {800, 400, 800, 400, 400, 400, 800, 400},  // TURBO_BACK_LEFT
    {800, 400, 800, 400, 800, 400, 400, 400},  // TURBO_BACK_RIGHT
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
        IrSender.sendRaw(START, sizeof(START) / sizeof(START[0]), 38);
        IrSender.sendRaw(BAND_PREFIX[(int)band], sizeof(BAND_PREFIX[0]) / sizeof(BAND_PREFIX[0][0]), 38);
        IrSender.sendRaw(COMMANDS[(int)c], sizeof(COMMANDS[0]) / sizeof(COMMANDS[0][0]), 38);
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
