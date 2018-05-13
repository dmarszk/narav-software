#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <driver/gpio.h>

#define PIN_SD_MISO     GPIO_NUM_32
#define PIN_SD_MOSI     GPIO_NUM_33
#define PIN_SD_CLK      GPIO_NUM_12
#define PIN_SD_CS       GPIO_NUM_26

#define PIN_IMU_SCL     GPIO_NUM_17
#define PIN_IMU_SDA     GPIO_NUM_16

#define PIN_BUZZER      GPIO_NUM_13
#define PIN_EXT_LED     GPIO_NUM_22
#define PIN_BOARD_LED   GPIO_NUM_5

// Set LED drive strength (0 - 5mA, 1 - 10mA, 2 - 20mA, 3 - 40mA)
#define PIN_EXT_LED_DRIVE_STRENGTH GPIO_DRIVE_CAP_2

#define EXT_LED_ACTIVE_LOW (0)

#define BUZZER_MAX_BEEP_TIME (2000)

// Run SCL at 400 kHz
#define IMU_I2C_SPEED (400000)


#define MAX_SAMPLE_FILE_INDEX 100000


#define SOFTWARE_VERSION 2
#define STORAGE_BUFFER_SIZE 512

#endif //__CONFIG_H__