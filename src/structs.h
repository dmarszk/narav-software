#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#define MEAS_TYPE_SHIFT 28

#define MEAS_TYPE_START     0
#define MEAS_TYPE_AHRS      1
#define MEAS_TYPE_ACCEL     2
#define MEAS_TYPE_GYRO      3
#define MEAS_TYPE_MAG       4
#define MEAS_TYPE_BAROM     5

#define TIMESTAMP_MASK (0x0FFFFFFF)

struct DataStart {
  uint8_t version; //add whatever needed data, after the version
  float MPU9250testResult[6]; // data provided by MPU9250SelfTest
  float magCalibration[3]; // data provided by initAK8963
  float gyroBias[3]; // data provided by calibrateMPU9250
  float accelBias[3]; // data provided by calibrateMPU9250
  float aRes; // data provided by getAres
  float gRes; // data provided by getGres
  float mRes; // data provided by getMres
} __attribute__((packed));

struct DataAhrs {
  float yaw, pitch, roll;
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t mx, my, mz;
} __attribute__((packed));

struct DataAccel {
  int16_t ax, ay, az;
} __attribute__((packed));

struct DataGyro {
  int16_t gx, gy, gz;
} __attribute__((packed));

struct DataMag {
  int16_t mx, my, mz;
} __attribute__((packed));

struct DataBarom {
  uint32_t pressure; // Pascal
  int32_t temperature; // miliC
} __attribute__((packed));

union DataUnion {
  DataStart start;
  DataAhrs ahrs;
  DataAccel accel;
  DataGyro gyro;
  DataMag mag;
  DataBarom barom;
} __attribute__((packed));

/**
*  Header structure:
*   - 4-bit type [31:28]
*   - 28-bit timestamp [27:0]
*/
struct Measurement {
  uint32_t header;
  DataUnion data;
} __attribute__((packed));

#endif // __STRUCTS_H__