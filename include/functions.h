
#include "setup.h"

// -------------------------Vector Functions-----------------------

int* Get_Raw_Accel() { // Returns an int vector of the raw acceleration Values from the MPU

  Wire.beginTransmission( MPU );
  Wire.write( 0x3B );                   // Start with register 0x3B ( ACCEL_XOUT_H )
  Wire.endTransmission( false );
  Wire.requestFrom( MPU, 6, true );     // Read 6 registers total, each axis value is stored in 2 registers
   
  static int* result;

  for ( int i = 0; i < 3; i++ ) result[ i ] = ( ( Wire.read() << 8 | Wire.read() ) * 1000 ); // Raw values

  return result;

}

int* Get_Normalized_Accel( int* raw_accel ) { // Returns the normalized acceleration Values from the MPU
  
  static int* normalized_accel;

  for ( int i = 0; i < 3; i++ ) normalized_accel[ i ] = raw_accel[ i ] / ALSB_Sensitivity ;

  return normalized_accel;

}

int* Get_Raw_Gyro() { // Returns an int vector containing the raw gyrospocic Values from the MPU

  Wire.beginTransmission( MPU );
  Wire.write( 0x43 );                 // Gyro data first register address 0x43
  Wire.endTransmission( false );
  Wire.requestFrom( MPU, 6, true );   // Read 4 registers total, each axis value is stored in 2 registers

  static int* raw_gyro;
  
  for ( int i = 0; i < 3; i++ ) raw_gyro[ i ] = ( Wire.read() << 8 | Wire.read() ) * 1000;

  return raw_gyro;

}

int* Get_Normalized_Gyro( int* raw_gyro ) { // Returns an int vector containing the normalized gyrospocic Values from the MPU

  static int* normalized_gyro;

  for ( int i = 0; i < 3; i++ ) normalized_gyro[ i ] = raw_gyro[ 0 ] / GLSB_Sensitivity;

  return normalized_gyro;

}

// -----------------------Struct Functions------------------------------------------

INTData Get_All_Values_INT() {

  INTData data;

  int i = 0;
  // Get Values from Accelorometer
  int *raw_a_ptr = Get_Raw_Accel();
  int *raw_g_ptr = Get_Raw_Gyro();
  int *norm_a_ptr = Get_Normalized_Accel( raw_a_ptr );
  int *norm_g_ptr = Get_Normalized_Gyro( raw_g_ptr );

  for ( i = 0; i < 3; i++ ) {

    data.raw_accel[ i ] = raw_a_ptr[ i ];
    data.raw_gyro[ i ] = raw_g_ptr[ i ];
    data.normalized_accel[ i ] = norm_a_ptr[ i ];
    data.normalized_gyro[ i ] = norm_g_ptr[ i ];

  }

  data.time = millis();

  data.temperature = static_cast<int>( bmp.temperature * 1000 );
  data.pressure = static_cast<int>( bmp.pressure * 1000 );
  data.altitude = static_cast<int>( bmp.readAltitude( SEALEVELPRESSURE_HPA ) * 1000 );

  return data;

}

// -----------------------Data Handling Functions-----------------------------------

void Print_All_Values( INTData& Values ) { // Print the Values on the serial monitor

  Serial.begin( 115200 ); // Open Serial Port

  String output = ""; // init output string

  output.append( "Time ( S ): " + String( Values.time / 1000.0f ) + '\n' );
  output.append( "Raw Acceleration ( X, Y, Z ): " + String( Values.raw_accel[ 0 ] / 1000.0f, 2 ) + ',' + String( Values.raw_accel[ 1 ] / 1000.0f, 2 ) + ',' + String( Values.raw_accel[ 2 ]  / 1000.0f, 2 ) + '\n' );
  output.append( "Normalized Acceleration ( X, Y, Z ): " + String( Values.normalized_accel[ 0 ] / 1000.0f, 2 ) + ',' + String( Values.normalized_accel[ 1 ] / 1000.0f, 2 ) + ',' + String( Values.normalized_accel[ 2 ] / 1000.0f, 2 ) + '\n' );
  output.append( "Raw GyroRange ( X, Y, Z ): " + String( Values.raw_gyro[ 0 ] / 1000.0f, 2 ) + ',' + String( Values.raw_gyro[ 1 ] / 1000.0f, 2 ) + ',' + String( Values.raw_gyro[ 2 ] / 1000.0f, 2 ) + '\n' );
  output.append( "Normalized Gyro Range ( X, Y, Z ): " + String( Values.normalized_gyro[ 0 ] / 1000.0f, 2 ) + ',' + String( Values.normalized_gyro[ 1 ] / 1000.0f, 2 ) + ',' + String( Values.normalized_gyro[ 2 ] / 1000.0f, 2 ) + '\n' );

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

  for ( i = 0; i < 3; i++ ) myFile.print( String( Values.raw_accel[ i ] / 1000.0f ) + ',' );

  for ( i = 0; i < 3; i++ ) myFile.print( String( Values.normalized_accel[ i ] / 1000.0f ) + ',' );

  for ( i = 0; i < 3; i++ ) myFile.print( String( Values.raw_gyro[ i ] / 1000.0f ) + ',' );

  for ( i = 0; i < 3; i++ ) myFile.print( String( Values.normalized_gyro[ i ] / 1000.0f ) + ',' );

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

void Deploy_Parachute( int pin ) { // Arms Parachute

  Para_Armed = 1;

}

void Launch_Parachute( int schute ) { // Launches Parachute

  switch ( schute ) {

    case 0: // Main Schute
      digitalWrite( pinMain, HIGH );
    
    case 1: // Drouge Schute
      digitalWrite( pinDrouge, HIGH );

  }

}

// -----------------------Internal Trigger Functions--------------------------------

int Check_Altitude( int altitude ) { // Checks if altitude is safe

  //TODO: Implement

  return 0; // Safe

}

int Check_Pressure( int pressure ) { // Checks if pressure is safe

  //TODO: Implement

  return 0; // Safe

}

int Check_Tilt( int gyro ) { // Checks if tilt is safe

  //TODO: Implement

  return 0; // Safe

}

int Check_Accel( int accel ) { // Checks if accel is correct

  //TODO: Implement

  return 0; // Safe

}

// -----------------------External Trigger Functions--------------------------------

int Check_Systems( INTData& Values ) { // Checks if systems are safe

  //TODO: Implement
  
  //* Will trigger LED based on error code

  return 0; // Safe

}
