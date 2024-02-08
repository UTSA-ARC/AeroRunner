#pragma once
#include "testing_globals.h"

#include "setup.h"

//* test type, runUnityTests(), print_result(), and mock_arduino() are pre-defined in testing_globals.h *//

//* Define your test functions here *//
void test_check_good_1( void ); // void inside () explicitly states no params
void test_check_good_2( void );
void test_check_good_3( void );
void test_check_bad_1( void );
void test_check_bad_2( void );
void test_check_bad_3( void );

TESTS = { test_check_good_1, test_check_good_2, test_check_good_3, test_check_bad_1, test_check_bad_2, test_check_bad_3 };
uint8_t TEST_NUM = 6;

//static void* tests = { test_check_good_1 };
uint8_t test_count = 1;

Data Values;
Data Prev_Values;

