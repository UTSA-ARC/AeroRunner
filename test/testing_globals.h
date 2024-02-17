#pragma once

#include <unity.h>

#include "globals.h"

#ifndef ARDUINO_TC

  #include <ArduinoFake.h>
  #define mock_arduino( method, return_value ) When( Method( ArduinoFake(), method ) ).AlwaysReturn( return_value )
  using namespace fakeit;

  #ifdef NATIVE_TC

    int main( void );

  #endif

#else

  #include <Arduino.h>
  #define ARDUINO_TEST_DELAY 2000

  void setup();
  void loop();

#endif

void setUp( void );

void tearDown( void );

void print_test_result( Result test_result ) {

    String message = "Result array: \n\t error: ";
    message += String( test_result.error );
    message += "\n\t message: ";
    message += String( test_result.message );
  
    printf( "\n--------------------------------\n\n" );
    printf( "%s", message.c_str() );
    printf( "\n\n--------------------------------\n" );
  
  }
