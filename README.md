# B-12 — Arduino Nano Companion

Meet B-12, an Arduino Nano project where I built my own little companion!

## Overview
B-12 is a small, custom companion robot based on an Arduino Nano. This repository contains the firmware written in C++ and documentation for wiring, assembly, and usage.

## Features
- Core control and logic running on an Arduino Nano
- Sensors and actuators to interact with the environment (see BOM)
- Simple behavior routines and user controls
- Small footprint and easy to reproduce

## Bill of Materials (example)
Replace or expand this list with the exact parts you used.
- Arduino Nano (or compatible)
- 1 × I2C OLED display (128x64) or similar
- 1 × Rotary encoder
- Wires

## Wiring

Important: The wiring below contains the mapping I used with my Arduino Nano. Please replace with the exact pin names and connections from your build.

- Power
  - VIN / RAW: Optional input if using external regulator.
  - 5V: Connect to 5V supply (or to USB 5V when programming).
  - GND: Connect all grounds together (Arduino GND, battery negative, sensor grounds).

- I2C device
  - OLED SDA -> A4 (Arduino Nano SDA)
  - OLED SCL -> A5 (Arduino Nano SCL)
  - OLED VCC -> 5V or 3.3V(check module)
  - OLED GND -> GND

- Rotary encoder
  - ROT GND -> GND
  - ROT + -> 5V or 3.3V(check module)
  - ROT CLK -> D6 (Digital 6)
  - ROT DT -> D7 (Digital 7)
  - ROT SW -> D8 (Digital 8)

Notes and safety:
- Check voltage requirements for I2C and others modules (3.3V vs 5V).
- If using a LiPo battery, follow charging and protection best practices.

Wiring (your build):
- Please replace the example above with the exact pin mapping of your project.

Circuit diagram:
![](https://raw.githubusercontent.com/Loufok0/B-12/main/resources/Circuit_diagram.png)

## Photos
I will attach photos of the project here:
*
*
*
*


## Build & Upload
1. Install Arduino IDE.
2. Select the correct board: "Arduino Nano" (choose the correct processor/bootloader, e.g., ATmega328P).
3. Install required libraries:
   - LiquidCrystal_I2C.h (or any other, depending on your display)
4. Open the .ino file in the repo, compile, and upload via USB.
5. If using an external power source, disconnect USB before connecting the external battery (or follow safe powering sequence).

## Usage
- Power on the unit.
- Interact using the rotary encoder.
- Have fun!

## Troubleshooting
- No response: check power, USB connection, and verify board selection in IDE.
- Sensors not read: confirm wiring, common ground, voltage levels, and the rotary encoder quality.

## Contributing
If you want to contribute:
- Open an issue describing the change or feature.
- Send a pull request with code and documentation updates.

## Contact
Creator: Loufok0
