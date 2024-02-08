#pragma once

#include "toolchain.h"
// Structs
typedef struct Data { //* All measurements

    String time;

    int16_t* raw_accel;
    float_t* normalized_accel;

    int16_t* raw_gyro;
    float_t* normalized_gyro;

    float_t temperature;
    float_t pressure;
    float_t altitude;

    String message;

    Data(): message( "-" ) {}
    Data(
        
        String _time, 

        int16_t* _raw_accel, 
        float_t* _normalized_accel, 

        int16_t* _raw_gyro, 
        float_t* _normalized_gyro, 

        float_t _temperature, 
        float_t _pressure, 
        float_t _altitude, 
        
        String _message 
        
        ):

        time( _time ),

        raw_accel( _raw_accel ),
        normalized_accel( _normalized_accel ),

        raw_gyro( _raw_gyro ),
        normalized_gyro( _normalized_gyro ),

        temperature( _temperature ),
        pressure( _pressure ),
        altitude( _altitude ), 
        
        message( _message ) {}

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
