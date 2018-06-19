# Narav - Nano Rocket Avionics

Software for avionics package based on WEMOS LOLIN32 board.

## Hardware

LOLIN32 is a board based on ESP32 WROOM module.

This software has been developed with couple of peripheral modules connected to LOLIN32:
* uSD card,
* BMP280 (pressure altimeter),
* MPU9250 (inertial measurement unit consisting of AK8963 magnetometer and MPU6500 accelerometer+gyro),
* Buzzer (optional).

In the example design, GY-91 sensor board has been used. It holds BMP280 and MPU9250.

The example design uses the following mapping (most of it is unnecessary for basic functionality):

ESP32 pin   |Peripheral pin | Notes
------------|---------------|------------
GPIO32      |SD card MISO   |
GPIO33      |SD card MOSI   |
GPIO26      |SD card CS     |
GPIO12      |SD card SCK    |
GPIO17      |IMU+baro SCL   |
GPIO16      |IMU+baro SDA   |
GPIO4       |IMU+baro SDO   |unused in I2C mode, leave in hi-Z pull-down mode
GPIO5       |LED0 -         |blue LED on WEMOS LOLIN32, anode connected to VCC through 2k resistor
GPIO22      |LED1 +         |low power green LED
GPIO21      |LED1 -         |connected through resistor to GND - leave in hi-Z mode
GPIOxx      |LED2 -         |bright blue LED, anode connected to VCC without resistor
GPIO13      |Buzzer +       |use with low duty cycle

Running sensors in SPI mode is possible, but would require wiring 2 more pins: CSB and nCS.

I2C slave addresses of the peripherals are as follows:
* MPU9250 - 0x68
* BMP280 - 0x76
