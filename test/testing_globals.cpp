#include "testing_globals.h"

int runUnityTests( UnityTestFunction* tests, uint8_t numTests ) {

  UNITY_BEGIN();
  
  for ( int i = 0; i < numTests; i++ ) RUN_TEST( tests[ i ] );

  return UNITY_END();

}

void print_result( Result result ) { printf( "\n Result array: \n\t error: %d \n\t message: %s  \n", result.error, result.message.c_str() ); }

int main( void ) {

  runUnityTests( TESTS, TEST_NUM );

}
