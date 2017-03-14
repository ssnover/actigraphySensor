# Overview
This repository holds source code and files for my AVR-based actigraphy sensor. This embedded system samples an LIS3DH accelerometer to record magnitude of movement while I sleep. The data is then sent to a receiving node connected to my PC via a CAN bus.

# Contents
* Embedded C firmware for Atmel's 8-bit AVR microcontroller family.
  * State machine for application control.
  * Simple C libraries for Adafruit's LIS3DH breakout and Microchip's MCP2515 CAN controller.
  * Simple C libraries for a handful of AVR peripherals, especially SPI.
* Hardware design files from KiCad.
  * Electrical schematics.
  * Printed circuit board designs.
