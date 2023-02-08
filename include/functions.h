
#include "setup.h"

void Print_All_Values() { // Print the values on the serial monitor

  Serial.begin( 115200 ); // Open Serial Port

  // Get Values from Accelorometer
  const Vector<int> raw_accel = raw_accel;
  const Vector<int> normalized_accel = Normalized_Accel( raw_accel );

  // Get Values from Gyroscope
  const Vector<int> raw_gyro = raw_gyro;
  const Vector<int> normalized_gyro = Normalized_Gyro( raw_gyro );

  // Print all relevent values
  Serial.println( "Time ( S ): " + String( millis() / 1000.0 ) );

  Serial.println( "Raw Acceleration ( X, Y, Z ): " + String( raw_accel.at( 0 ) / 1000.0, 2 ) + ',' + String( raw_accel.at( 1 ) / 1000.0, 2 ) + ',' + String( raw_accel.at( 0 )  / 1000.0, 2 ) ); // Arduino is really dumb
  
  Serial.println( "Normalized Acceleration ( X, Y, Z ): " + String( normalized_accel.at( 0 )  / 1000.0, 2 ) + ',' + String( normalized_accel.at( 1 ) / 1000.0, 2 ) + ',' + String( normalized_accel.at( 2 ) / 1000.0, 2 ) ); // Arduino is really dumb again
  
  Serial.println( "Raw GyroRange ( X, Y, Z ): " + String( raw_gyro.at( 0 ) / 1000.0, 2 ) + ',' + String( raw_gyro.at( 1 ) / 1000.0, 2 ) + ',' + String( raw_gyro.at( 2 ) / 1000.0, 2 ) ); // Arduino is really dumb again,...again

  Serial.println( "Normalized Gyro Range ( X, Y, Z ): " + String( normalized_gyro.at( 0 ) / 1000.0, 2 ) + ',' + String( normalized_gyro.at( 1 ) / 1000.0, 2 ) + ',' + String( normalized_gyro.at( 2 ) / 1000.0, 2 ) + '\n' ); // Arduino is really dumb again,...again, and again

  Serial.println( "Now reading BMP390..." );
  
  Serial.println( "Tempurature ( C ): " + String( bmp.temperature ) );
  
  Serial.println( "Pressure ( kPa ): " + String( bmp.pressure / 1000.0 ) );

  Serial.println( "Altitude ( m ): " + String( bmp.readAltitude( SEALEVELPRESSURE_HPA ) ) + '\n' );

  Serial.end(); // End Serial Transmission

}