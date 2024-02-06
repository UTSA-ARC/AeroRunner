#include <unity.h>
#include <Arduino.h>

#include "setup.h"

Data Values;
Data Prev_Values;

void setUp( void ) {
  // Default values for structs

  Values = Data();

  int16_t r_accel[] = { 0, 1024, 0 };
  float_t n_accel[] = { 0, -9.8, 0 };

  int16_t r_gyro[] = { 0, 0, 0 };
  float_t n_gyro[] = { 0, 0, 0 };

  Values.time = "";
  Values.raw_accel = r_accel;
  Values.normalized_accel = n_accel;
  Values.raw_gyro = r_gyro;
  Values.normalized_gyro = n_gyro;
  Values.temperature = 20.0f;
  Values.pressure = 1013.0f;
  Values.altitude = 300.0f;

  Prev_Values = Values; // Default value for previous measurement

}

void tearDown( void ) {
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