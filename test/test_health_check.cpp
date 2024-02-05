#include <unity.h>
#include <Arduino.h>

#include "setup.h"

void setUp(void) {
  // set stuff up here
    
    

}

void tearDown(void) {
  // clean stuff up here
}

void test_check_systems(void) {
  // test stuff



}

int runUnityTests(void) {

  UNITY_BEGIN();
  RUN_TEST(test_check_systems);
  return UNITY_END();

}

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
