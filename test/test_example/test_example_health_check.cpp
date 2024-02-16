#include "test_example.h" //* Look in here for definitions

// --------------Set Up------------------

void setUp( void ) { //* Have the default values be the *Ideal Values* for your param (I.E. Default values should pass in your tests)
  // Default values for test suite globals (Runs before every test case)

  Test_Values = Data(); //! MUST INITIALIZE AS EMPTY FIRST OR USE `Data( *All Arguments* )` CONSTRUCTOR

  int16_t r_accel[ 3 ] = { 0, 0, 0 }; //* Not needed for `Check_Systems()`
  float_t n_accel[ 3 ] = { SafeSurfaceAccelX, SafeSurfaceAccelY, SafeSurfaceAccelZ };

  int16_t r_gyro[ 3 ] = { 0, 0, 0 }; //* Not needed for `Check_Systems()`
  float_t n_gyro[ 3 ] = { SafeSurfaceTiltX, SafeSurfaceTiltY, SafeSurfaceTiltZ };

  Test_Values.time = "";

  Test_Values.raw_accel[0] = r_accel[0]; //* Direct brace expansion does not work with pointer objects in manually constructed structs, you have to copy a reference to another pointer
  Test_Values.raw_accel[1] = r_accel[1];
  Test_Values.raw_accel[2] = r_accel[2];

  Test_Values.normalized_accel[0] = n_accel[0];
  Test_Values.normalized_accel[1] = n_accel[1];
  Test_Values.normalized_accel[2] = n_accel[2];

  Test_Values.raw_gyro[0] = r_gyro[0];
  Test_Values.raw_gyro[1] = r_gyro[1];
  Test_Values.raw_gyro[2] = r_gyro[2];

  Test_Values.normalized_gyro[0] = n_gyro[0];
  Test_Values.normalized_gyro[1] = n_gyro[1];
  Test_Values.normalized_gyro[2] = n_gyro[2];

  Test_Values.temperature = 0.0f; //* Not needed for `Check_Systems()`
  Test_Values.pressure = SafeSurfacePressure;
  Test_Values.altitude = 0.0f; //* Not needed for `Check_Systems()`

  Prev_Values = Test_Values; // Default value for previous measurement

  test_result = Result(); // Empty result by default

  raw_input_voltage = 1024;
  nominal_voltage = NOMINAL_INPUT_VOLTAGE;
  min_voltage = MINIMUM_INPUT_VOLTAGE;
  max_voltage = MAXIMUM_INPUT_VOLTAGE;

}

// --------------Tear Down------------------

void tearDown( void ) { //* If you do any manual memory allocation, you would free/reallocate/clean up here (This is generally for anything you do in `setUp()`)
  // Print out `result` from last test case (Runs after every test case)

  print_test_result( test_result );

}

// --------------Supplimentary Functions------------------

void mocks() { //* Ideally no mocking should be necessary, but if needed, you should only need to mock Arduino Functions

  mock_arduino( analogRead, 1024 );
  mock_arduino( digitalRead, 1 );
  mock_arduino( digitalWrite, ); //* No 2nd param means to return nothing (void)

}

// --------------Test Cases------------------

void test_check_good_1( void ) { //* First `valid data` test should generally check with your default values

  mocks();

  test_result = Check_Systems( &Test_Values, &Prev_Values, {}, {}, 0 , 1024, 5.0f, 3.3f, 5.0f );

  TEST_ASSERT_EQUAL( 0, test_result.error ); // 0 means no error

}

void test_check_good_2( void ) { //* Re-initialize test-specific globals in the test case to re-test with different params
  
  mocks();

  

  test_result = Check_Systems( &Test_Values, &Prev_Values, {}, {}, 0 , 1024, 5.0f, 3.3f, 5.0f );

  TEST_ASSERT_EQUAL( 0, test_result.error ); // 0 means no error

}

void test_check_good_3( void ) {

  mocks();

  test_result = Check_Systems( &Test_Values, &Prev_Values, {}, {}, 0 , 1024, 5.0f, 3.3f, 5.0f );

  TEST_ASSERT_EQUAL( 0, test_result.error ); // 0 means no error

}

// ------------

void test_check_bad_1( void ) {

  mocks();

    test_result = Check_Systems( &Test_Values, &Prev_Values, {}, {}, 0 , 1024, 5.0f, 3.3f, 5.0f );


  TEST_ASSERT_EQUAL( -1, test_result.error ); // -1 means error

}

void test_check_bad_2( void ) {

  mocks();

  test_result = Check_Systems( &Test_Values, &Prev_Values, {}, {}, 0 , 1024, 5.0f, 3.3f, 5.0f );

  TEST_ASSERT_EQUAL( -1, test_result.error ); // -1 means error

}

void test_check_bad_3( void ) {

  mocks();

  test_result = Check_Systems( &Test_Values, &Prev_Values, {}, {}, 0 , 1024, 5.0f, 3.3f, 5.0f );

  TEST_ASSERT_EQUAL( -1, test_result.error ); // -1 means error

}

// --------------Test Suite Entrypoint------------------

#ifdef NATIVE_TC

  int main( void ) {

    UNITY_BEGIN();

    for ( int i = 0; i < TEST_NUM; i++ ) RUN_TEST( TESTS[ i ] );

    return UNITY_END();
  }

#endif

#ifdef ARDUINO_TC //* If you have tests that run on an Arduino framework (E.g. the teensy)
  
  void setup() {
  
    UNITY_BEGIN();

    for ( int i = 0; i < TEST_NUM; i++ ) RUN_TEST( TESTS[ i ] );

    UNITY_END();

  }
  void loop();

#endif
