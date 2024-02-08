#pragma once
#include "testing_globals.h"

#include "setup.h"

//* test type, runUnityTests(), print_result(), and mock_arduino() are pre-defined in testing_globals.h *//

//* Define your test functions here *//
void test_check_good_1( void ); // void inside () explicitly states no params

TEST = (UnityTestFunction) test_check_good_1;

//static void* tests = { test_check_good_1 };
uint8_t test_count = 1;

Data Values;
Data Prev_Values;

