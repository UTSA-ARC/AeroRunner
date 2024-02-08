#include "test_example.h" //* Look in here for definitions

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

// --------------------------------

void tearDown( void ) {
  // clean stuff up 

}

// --------------------------------

void mocks() {

  mock_arduino(analogRead, 1024);
  mock_arduino(digitalRead, 1);
  mock_arduino(digitalWrite,);

}

// --------------------------------

void test_check_good_1( void ) {

  mocks();

  Result result = Check_Systems( &Values, &Prev_Values, {}, {}, 0, 700 );

  print_result( result );

  TEST_ASSERT_EQUAL( 0, result.error ); // 0 means no error

}

void test_check_good_2( void ) {
  
  mocks();

  Result result = Check_Systems( &Values, &Prev_Values, {}, {}, 0, 700 );

  print_result( result );

  TEST_ASSERT_EQUAL( 0, result.error ); // 0 means no error

}

void test_check_good_3( void ) {

  mocks();

  Result result = Check_Systems( &Values, &Prev_Values, {}, {}, 0, 700 );

  print_result( result );

  TEST_ASSERT_EQUAL( 0, result.error ); // 0 means no error

}

void test_check_bad_1( void ) {

  mocks();

  Result result = Check_Systems( &Values, &Prev_Values, {}, {}, 0, 700 );

  print_result( result );

  TEST_ASSERT_EQUAL( 0, result.error ); // 0 means no error

}

void test_check_bad_2( void ) {

  mocks();

  Result result = Check_Systems( &Values, &Prev_Values, {}, {}, 0, 700 );

  print_result( result );

  TEST_ASSERT_EQUAL( 0, result.error ); // 0 means no error

}

void test_check_bad_3( void ) {

  mocks();

  Result result = Check_Systems( &Values, &Prev_Values, {}, {}, 0, 700 );

  print_result( result );

  TEST_ASSERT_EQUAL( 0, result.error ); // 0 means no error

}


#ifdef NATIVE_TC

  int main( void );

#endif

#ifdef ARDUINO_TC
  
  void setup();
  void loop();

#endif
