#pragma once

#include <Adafruit_BMP3XX.h>
#include <Wire.h>
#include <TimeLib.h>
#include <Arduino.h>

Adafruit_BMP3XX bmp;

// Structs
typedef struct Data { //* All data in integer form 0.0X accuracy (Divide by MULT for dec)

    String time;

    int raw_accel[3];
    float normalized_accel[3];

    int raw_gyro[3];
    float normalized_gyro[3];

    float temperature;
    float pressure;
    float altitude;

    String message = "-";

} Data;

typedef struct SampleData : Data { String timeEnd; } SampleData;

typedef struct Result {

    int error;
    String message;

} Result;

typedef struct TimeData {
    unsigned int timeNow = 0UL;
    unsigned int timePrev = 0UL;
    unsigned int dt = 0UL; // loop time used to update integral
} TimeData;