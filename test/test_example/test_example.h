#pragma once //* Or use compiler guards
#include "../testing_globals.h"

#include "setup.h" // Include the file containing the function to test

//* setUp(), tearDown(), main(), setup(), loop(), "globals.h", "Arduino.h/AruinoFake.h", `test_func` type, print_test_result(), and mock_arduino() are pre-defined in testing_globals.h *//

// --------------Test Definitions------------------

void test_check_good_1( void ); // void inside () explicitly states no params
void test_check_good_2( void );
void test_check_good_3( void );
void test_check_bad_pressure_movement( void );
void test_check_bad_2( void );
void test_check_bad_3( void );

const uint8_t TEST_NUM = 6; // Number of tests
test_func TESTS[6] = { test_check_good_1, test_check_good_2, test_check_good_3, test_check_bad_pressure_movement, test_check_bad_2, test_check_bad_3 }; // Array of Test Function names

// --------------Test Suite Globals Definitions------------------

Data Test_Values;
Data Prev_Test_Values;
Result test_result;

uint8_t* src_pins;
uint8_t* gnd_pins;
uint8_t continuity_pins_amount;

uint16_t raw_input_voltage;
float_t nominal_voltage;
float_t min_voltage;
float_t max_voltage;
