#pragma once

#include <Adafruit_BMP3XX.h>
#include <Wire.h>
#include <TimeLib.h>
#include <Arduino.h>

Adafruit_BMP3XX bmp;

// Structs
typedef struct Data { //* All data in integer form 0.0X accuracy (Divide by MULT for dec)

    String time;

    int raw_accel[3] = {0,0,0};
    float normalized_accel[3] = {0,0,0};

    int raw_gyro[3] = {0,0,0};
    float normalized_gyro[3] = {0,0,0};

    float temperature = 0;
    float pressure = 0;
    float altitude = 0;

    String message = "-";

} Data;

typedef struct SampleData : Data { String timeEnd; } SampleData;

typedef struct Result {

    int error;
    String message;

} Result;