#pragma once

#include "toolchain.h"
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

    String message;

    Data(): message( "-" ) {}

} Data;

typedef struct SampleData : Data { 
    
    String timeEnd; 
    
} SampleData;

typedef struct Result {

    int8_t error;
    String message;

    Result(): error( 0 ), message( "-" ) {};
    Result( int8_t _error, String _message ): error( _error ), message( _message ) {};

} Result;


typedef struct TimeData {

    uint16_t timeNow;
    uint16_t timePrev;
    uint16_t dt; // loop time used to update integral
    
    TimeData() {}
    TimeData( uint16_t _timeNow, uint16_t _timePrev, uint16_t _dt ): timeNow( _timeNow ), timePrev( _timePrev ), dt( _dt ) {}

} TimeData;
