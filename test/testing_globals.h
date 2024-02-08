#pragma once

#include <unity.h>

#include "globals.h"

#ifndef ARDUINO_TC

  #include <ArduinoFake.h>
  #define mock_arduino( method, return_value ) When( Method( ArduinoFake(), method ) ).AlwaysReturn( return_value )
  using namespace fakeit;

#else

  #include <Arduino.h>
  #define ARDUINO_TEST_DELAY 2000

#endif

extern void(*TESTS[])();
extern uint8_t TEST_NUM;

void print_result( Result res );

void setUp( void );
void tearDown( void );
int runUnityTests( const void* tests, const uint8_t numTests );
