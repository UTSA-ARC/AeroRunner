#pragma once

#include <Adafruit_BMP3XX.h>
#include <Wire.h>
#include <TimeLib.h>
#include <Arduino.h>

Adafruit_BMP3XX bmp;
uint16_t ALSB_Sensitivity, GLSB_Sensitivity;
// Structs
typedef struct Data { //* All measurements

    String time;

    int16_t raw_accel[3];
    float_t normalized_accel[3];

    int16_t raw_gyro[3];
    float_t normalized_gyro[3];

    float_t temperature;
    float_t pressure;
    float_t altitude;

    String message = "-";

} Data;

typedef struct SampleData : Data { String timeEnd; } SampleData;

typedef struct Result {

    int8_t error;
    String message;

} Result;

typedef struct TimeData {

    uint16_t timeNow = 0;
    uint16_t timePrev = 0;
    uint16_t dt = 0; // loop time used to update integral
    
} TimeData;