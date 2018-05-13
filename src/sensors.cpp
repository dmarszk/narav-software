#include <sys/unistd.h>

#include "esp_err.h"
#include "esp_log.h"

#include "MPU.hpp"
#include "mpu/math.hpp"
#include "mpu/types.hpp"
#include "I2Cbus.hpp"

#include "config.h"
#include "sensors.h"
#include "structs.h" 
#include "storage.h"


#define SENSORS_TASK_STACK_SIZE 4096
static const char *TAG = "sensors";

void sensors_task(void *pvParameter);

mpud::MPU MPU;

esp_err_t sensors_init(void)
{
    esp_err_t ret;
    ret = i2c0.begin(PIN_IMU_SDA, PIN_IMU_SCL, IMU_I2C_SPEED);
    if (ret != ESP_OK) return ret;
    // Uncomment the line below to detect and print slave addresses on the bus
    // i2c0.scanner();
    MPU.setBus(i2c0);
    MPU.setAddr(mpud::MPU_I2CADDRESS_AD0_LOW);
    ret = MPU.testConnection();
    if (ret != ESP_OK) return ret;
    ret = MPU.initialize();
    if (ret != ESP_OK) return ret;
    ret = MPU.setSampleRate(100);  // in (Hz)
    if (ret != ESP_OK) return ret;
    ret = MPU.setAccelFullScale(mpud::ACCEL_FS_16G);
    if (ret != ESP_OK) return ret;
    ret = MPU.setGyroFullScale(mpud::GYRO_FS_2000DPS);
    if (ret != ESP_OK) return ret;
    ret = MPU.setDigitalLowPassFilter(mpud::DLPF_188HZ);  // smoother data
    if (ret != ESP_OK) return ret;
    
    ESP_LOGI(TAG, "MPU ID: %d", (int)MPU.whoAmI());
    
    xTaskCreate(&sensors_task, "sensors_task", SENSORS_TASK_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL);
    return ESP_OK;
}

void sensors_task(void *pvParameter)
{
    mpud::raw_axes_t accelRaw, gyroRaw; // Raw data
    mpud::float_axes_t accelG, gyroDPS; // Converted data
    Measurement currentSample;
    //uint32_t timestamp;
    while (1)
    {
      usleep(30*1000);
      mpud::raw_axes_t accelRaw;     // holds x, y, z axes as int16
      mpud::raw_axes_t gyroRaw;      // holds x, y, z axes as int16
      currentSample.header = (((uint32_t)MEAS_TYPE_ACCEL << MEAS_TYPE_SHIFT) | (esp_log_timestamp() & TIMESTAMP_MASK));
      MPU.acceleration(&accelRaw);  // fetch raw data from the registers
      currentSample.data.accel.ax = accelRaw.x;
      currentSample.data.accel.ay = accelRaw.y;
      currentSample.data.accel.az = accelRaw.z;
      storeMeasurement(currentSample);

      currentSample.header = (((uint32_t)MEAS_TYPE_GYRO << MEAS_TYPE_SHIFT) | (esp_log_timestamp() & TIMESTAMP_MASK));
      MPU.rotation(&gyroRaw);       // fetch raw data from the registers
      currentSample.data.gyro.gx = gyroRaw[0];
      currentSample.data.gyro.gy = gyroRaw[1];
      currentSample.data.gyro.gz = gyroRaw[2];
      storeMeasurement(currentSample);

      //printf("accel: %+d %+d %+d\n", accelRaw.x, accelRaw.y, accelRaw.z);
      //printf("gyro: %+d %+d %+d\n", gyroRaw[0], gyroRaw[1], gyroRaw[2]);
      //accelG = mpud::accelGravity(accelRaw, mpud::ACCEL_FS_16G);  // raw data to gravity
      //gyroDPS = mpud::gyroDecPerSec(gyroRaw, mpud::GYRO_FS_500DPS);  // raw data to º/s
      //printf("accel: %+.2f %+.2f %+.2f\n", accelG[0], accelG[1], accelG[2]);
      //printf("gyro: %+.2f %+.2f %+.2f\n", gyroDPS.x, gyroDPS.y, gyroDPS.z);
    }
}

