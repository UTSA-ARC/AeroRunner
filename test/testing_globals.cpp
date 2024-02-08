#include "testing_globals.h"

int runUnityTests(UnityTestFunction testFunction) {

  UnityTestFunction* unity_tests = {};
  int numTests = 0;

  UNITY_BEGIN();
  
  for ( int i = 0; i < numTests; i++ );
  RUN_TEST( testFunction );

  return UNITY_END();

}

void print_result( Result result ) { printf( "\n Result array: \n\t error: %d \n\t message: %s  \n", result.error, result.message.c_str() ); }

#ifdef NATIVE_TC

  int main( void ) {

    runUnityTests(TEST);

  }

#endif

#ifdef ARDUINO_TC

  setup(){

    delay( ARDUINO_TEST_DELAY );

    runUnityTests();

  }

  void loop(){};

#endif
