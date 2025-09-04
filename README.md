# Q-Steer RC Car Controller Emulator

This Arduino project emulates the infrared (IR) controller for the Choro Q Q-Steer RC Car Series. It allows you to control a Q-Steer car using an Arduino board and an IR LED, replicating the original remote's commands.

## Features

- Emulates all standard and turbo movement commands (forward, backward, left, right, diagonals, turbo modes)
- Supports multiple frequency bands (A, B, C, D)
- Uses analog inputs for joystick-style control
- Turbo mode switch for speed boost

## Hardware Requirements

- Arduino board (Tested with Uno R3 and Nano)
- IR LED
- Joystick Module (2-axis analog input device, for X and Y axes)
- Push button or switch for turbo mode

## Pinout

- **A0**: Analog X input (left/right)
- **A1**: Analog Y input (forward/backward)
- **D4**: Turbo mode switch (active LOW)
- **IR LED**: Connect according to the IRremote library documentation. For example, D3 is used for both Uno R3 and Nano boards.

## Usage

1. Connect the hardware as described above.
2. Upload `qsteer_rc.ino` to your Arduino.
3. Use the joystick to control direction.
4. Point the IR LED at your Q-Steer car and control it like the original remote.
5. Press the turbo switch to enable turbo mode.

## Code Overview

- `qsteer_rc.ino` contains all logic for reading inputs, mapping them to Q-Steer commands, and sending IR signals.
- Uses the [IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote) library for IR transmission.

## Customization

- Adjust analog thresholds in `to_digital()` if needed for your input devices.
- Select the frequency band by changing the `band` variable in `setup()`.

## References

- [Q-Steer Wikipedia](https://en.wikipedia.org/wiki/Q-steer)
- [IRremote Arduino Library](https://github.com/Arduino-IRremote/Arduino-IRremote)

## License

This project is provided as-is for educational and hobbyist use.
