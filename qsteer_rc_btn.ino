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

const int TURBO_SW = 4;
const int FWD_BTN = 5;
const int BACK_BTN = 6;
const int LEFT_BTN = 7;
const int RIGHT_BTN = 8;

void setup() {
    pinMode(TURBO_SW, INPUT_PULLUP);
    pinMode(FWD_BTN, INPUT_PULLUP);
    pinMode(BACK_BTN, INPUT_PULLUP);
    pinMode(LEFT_BTN, INPUT_PULLUP);
    pinMode(RIGHT_BTN, INPUT_PULLUP);
    Serial.begin(115200);
    band = Band::A;
}

void loop() {
    int x = !digitalRead(RIGHT_BTN) - !digitalRead(LEFT_BTN);
    int y = !digitalRead(BACK_BTN) - !digitalRead(FWD_BTN);
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
        Serial.print(x);
        Serial.print(" ");
        Serial.print(y);
        Serial.println();
    }

    delay(20);
}
