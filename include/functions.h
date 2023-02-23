
#include "setup.h"

// -------------------------Vector Functions-----------------------

Vector<uint> Get_Raw_Accel() { // Returns an uint vector of the raw acceleration Values from the MPU

  Wire.beginTransmission( MPU );
  Wire.write( 0x3B );                   // Start with register 0x3B ( ACCEL_XOUT_H )
  Wire.endTransmission( false );
  Wire.requestFrom( MPU, 6, true );     // Read 6 registers total, each axis value is stored in 2 registers

  Vector<uint> result;

  result.push_back( ( Wire.read() << 8 | Wire.read() ) * 1000 ); // Raw X-axis value
  result.push_back( ( Wire.read() << 8 | Wire.read() ) * 1000 ); // Raw Y-axis value
  result.push_back( ( Wire.read() << 8 | Wire.read() ) * 1000 ); // Raw Z-axis value

  return result;

}

Vector<uint> Get_Normalized_Accel( const Vector<uint>& raw_accel ) { // Returns the normalized acceleration Values from the MPU
  
  Vector<uint> normalized_accel;

  normalized_accel.push_back( raw_accel.at( 0 ) / ALSB_Sensitivity ); 
  normalized_accel.push_back( raw_accel.at( 1 ) / ALSB_Sensitivity );
  normalized_accel.push_back( raw_accel.at( 2 ) / ALSB_Sensitivity );

  return normalized_accel;

}

Vector<uint> Get_Raw_Gyro() { // Returns an uint vector containing the raw gyrospocic Values from the MPU

  Wire.beginTransmission( MPU );
  Wire.write( 0x43 );                 // Gyro data first register address 0x43
  Wire.endTransmission( false );
  Wire.requestFrom( MPU, 6, true );   // Read 4 registers total, each axis value is stored in 2 registers

  Vector<uint> raw_gyro;

  raw_gyro.push_back( ( Wire.read() << 8 | Wire.read() ) * 10000 );
  raw_gyro.push_back( ( Wire.read() << 8 | Wire.read() ) * 10000 );
  raw_gyro.push_back( ( Wire.read() << 8 | Wire.read() ) * 10000 );

  return raw_gyro;

}

Vector<uint> Get_Normalized_Gyro( const Vector<uint>& raw_gyro ) { // Returns an uint vector containing the normalized gyrospocic Values from the MPU

  Vector<uint> normalized_gyro;

  normalized_gyro.push_back( raw_gyro.at( 0 ) / GLSB_Sensitivity );
  normalized_gyro.push_back( raw_gyro.at( 1 ) / GLSB_Sensitivity );
  normalized_gyro.push_back( raw_gyro.at( 2 ) / GLSB_Sensitivity );

  return normalized_gyro;

}

// -----------------------Struct Functions------------------------------------------

INTData Get_All_Values_INT() {

  INTData data;

  // Get Values from Accelorometer
  data.raw_accel = Get_Raw_Accel();
  data.normalized_accel = Get_Normalized_Accel( data.raw_accel );

  // Get Values from Gyroscope
  data.raw_gyro = Get_Raw_Gyro();
  data.normalized_gyro = Get_Normalized_Gyro( data.raw_gyro );

  data.time = millis();

  data.temperature = static_cast<int>( bmp.temperature * 1000 );
  data.pressure = static_cast<int>( bmp.pressure * 1000 );
  data.altitude = static_cast<uint>( bmp.readAltitude( SEALEVELPRESSURE_HPA ) * 1000 );

  return data;

}

// -----------------------Data Handling Functions-----------------------------------

void Print_All_Values( INTData& Values ) { // Print the Values on the serial monitor

  Serial.begin( 115200 ); // Open Serial Port

  String output = ""; // init output string

  output.append( "Time ( S ): " + String( Values.time / 1000.0f ) + '\n' );
  output.append( "Raw Acceleration ( X, Y, Z ): " + String( Values.raw_accel.at( 0 ) / 1000.0f, 2 ) + ',' + String( Values.raw_accel.at( 1 ) / 1000.0f, 2 ) + ',' + String( Values.raw_accel.at( 2 )  / 1000.0f, 2 ) + '\n' );
  output.append( "Normalized Acceleration ( X, Y, Z ): " + String( Values.normalized_accel.at( 0 )  / 1000.0f, 2 ) + ',' + String( Values.normalized_accel.at( 1 ) / 1000.0f, 2 ) + ',' + String( Values.normalized_accel.at( 2 ) / 1000.0f, 2 ) + '\n' );
  output.append( "Raw GyroRange ( X, Y, Z ): " + String( Values.raw_gyro.at( 0 ) / 1000.0f, 2 ) + ',' + String( Values.raw_gyro.at( 1 ) / 1000.0f, 2 ) + ',' + String( Values.raw_gyro.at( 2 ) / 1000.0f, 2 ) + '\n' );
  output.append( "Normalized Gyro Range ( X, Y, Z ): " + String( Values.normalized_gyro.at( 0 ) / 1000.0f, 2 ) + ',' + String( Values.normalized_gyro.at( 1 ) / 1000.0f, 2 ) + ',' + String( Values.normalized_gyro.at( 2 ) / 1000.0f, 2 ) + '\n' );

  output.append( "\n\n Now reading BMP390...\n" );
  output.append( "Tempurature ( C ): " + String( Values.temperature / 1000.0f ) + '\n' );
  output.append( "Pressure ( kPa ): " + String( Values.pressure / 1000.0f ) + '\n' );
  output.append( "Altitude ( m ): " + String( Values.altitude / 1000.0f ) + '\n' );

  Serial.println( output ); // Print output to screen

  Serial.end(); // End Serial Transmission

}

void Write_All_Values_To_SD( INTData& Values ) { // Records values to Sd card

  File myFile = SD.open( "Raw_V05.csv", FILE_WRITE );

  //* Time ( seconds ),Raw Ax ( g ),Raw Ay ( g ),Raw Az ( g ),Ax ( g ),Ay ( g ),Az ( g ),Raw Gx ( deg/s ),Raw Gy ( deg/s ),Raw Gz ( deg/s ),Gx ( deg/s ),Gy ( deg/s ),Gz ( deg/s ),Temperature ( *C ),Pressure ( kpA ),Altitude ( m ),Message

  myFile.print( String( Values.time / 1000.0f ) + ',');

  int i;

  for ( i = 0; i < 3; i++ ) myFile.print( String( Values.raw_accel.at( i ) / 1000.0f ) + ',' );

  for ( i = 0; i < 3; i++ ) myFile.print( String( Values.normalized_accel.at( i ) / 1000.0f ) + ',' );

  for ( i = 0; i < 3; i++ ) myFile.print( String( Values.raw_gyro.at( i ) / 1000.0f ) + ',' );

  for ( i = 0; i < 3; i++ ) myFile.print( String( Values.normalized_gyro.at( i ) / 1000.0f ) + ',' );

  myFile.print( String( Values.temperature / 1000.0f ) + ',' );

  myFile.print( String( Values.pressure / 1000.0f ) + ',' );

  myFile.print( String( Values.altitude / 1000.0f ) + ',' );

  myFile.print( Values.message );

  myFile.close();

}

void Record_Data( INTData& Values ) { // Prints data to screen and saves it to file

  Print_All_Values( Values );
  Write_All_Values_To_SD( Values );

}

// -----------------------Parachute Functions---------------------------------------

void Deploy_Parachute( uint pin ) { // Arms Parachute

  Para_Armed = 1;

}

void Launch_Parachute( uint schute ) { // Launches Parachute

  switch ( schute ) {

    case 0: // Main Schute
      digitalWrite( pinMain, HIGH );
    
    case 1: // Drouge Schute
      digitalWrite( pinDrouge, HIGH );

  }

}

// -----------------------Internal Trigger Functions--------------------------------

Result Check_Altitude( uint altitude ) { // Checks if altitude is safe

  if ( altitude > safeAltitude ) return { .error = 0, .msg = "Safe Altitude"  }; // Safe

  return { .error = 1, .msg = "Dangerous Altitude" }; // Unsafe

}

Result Check_Pressure( uint pressure ) { // Checks if pressure is safe

  if ( pressure > safePressure ) return { .error = 0, .msg = "Safe Pressure" }; // Safe

  return { .error = 0, .msg = "Dangerous Pressure" }; // Unsafe

}

Result Check_Tilt( Vector<uint> gyro ) { // Checks if tilt is safe

  if ( gyro.at( 0 ) - 10 > 0 ) return { .error = 1, .msg = "X axis tilt is Dangerous" }; // Unsafe
  if ( gyro.at( 1 ) - 10 > 0 ) return { .error = 2, .msg = "Y axis tilt is Dangerous" }; // Unsafe
  if ( gyro.at( 2 ) - 10 > 0 ) return { .error = 3, .msg = "Z axis tilt is Dangerous" }; // Unsafe

  return { .error = 0, .msg = "Safe All-Axis Tilt" }; // Safe

}

Result Check_Accel( Vector<uint> old_accel, Vector<uint> new_accel ) { // Checks if accel is correct

  if ( old_accel.at( 0 ) - new_accel.at( 0 ) > 0 ) return { .error = 1, .msg = "Dangerous X axis acceleration" }; // Unsafe
  if ( old_accel.at( 1 ) - new_accel.at( 1 ) > 0 ) return { .error = 2, .msg = "Dangerous Y axis acceleration" }; // Unsafe
  if ( old_accel.at( 2 ) - new_accel.at( 2 ) > 0 ) return { .error = 3, .msg = "Dangerous Z axis acceleration" }; // Unsafe

  return { .error = 0, .msg = "Safe acceleration direction" }; // Safe

}

// -----------------------External Trigger Functions--------------------------------

int Check_Systems( INTData& Values ) { // Checks if systems are safe

  //TODO: Implement
  
  //* Will trigger LED based on error code

  return 0; // Safe

}
