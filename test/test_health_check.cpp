#include <unity.h>
#include <Arduino.h>

#include "setup.h"

Data Values;
Data Prev_Values;

void setUp(void) {
  // Default values for structs

  Values = {
      
    .time = "0",

    .raw_accel = { 0, -1024, 0 },
    .normalized_accel = { 0, -9.8, 0 },

    .temperature = 20.5,
    .pressure = 1013.25,
    .altitude = 300,

    .message = "Default Testing Values"

  };

  Prev_Values = Values; // Default value for previous measurement

}

void tearDown(void) {
  // clean stuff up here
}

// --------------------------------

void test_check_good_1( void ) {

  Result result = Check_Systems( &Values, &Prev_Values );

  TEST_ASSERT_EQUAL( 0, result.error ); // 0 means no error

}

// --------------------------------

int runUnityTests( void ) {

  UNITY_BEGIN();
  RUN_TEST( test_check_good_1 );
  return UNITY_END();

}

#ifdef NATIVE_TC
  int main( void ) {

    return runUnityTests();

  }

#endif

#ifdef ARDUINO_TC
  /**
    * For Arduino framework
    */
  void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();

  }
  void loop() {}

#endif