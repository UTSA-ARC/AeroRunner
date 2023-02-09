
#include "setup.h"

// -------------------------Vector Functions-----------------------

Vector<int> Get_Raw_Accel() { // Returns an int vector of the raw acceleration Values from the MPU

  Wire.beginTransmission( MPU );
  Wire.write( 0x3B );                   // Start with register 0x3B ( ACCEL_XOUT_H )
  Wire.endTransmission( false );
  Wire.requestFrom( MPU, 6, true );     // Read 6 registers total, each axis value is stored in 2 registers

  Vector<int> result;

  result.push_back( ( Wire.read() << 8 | Wire.read() ) * 1000 ); // Raw X-axis value
  result.push_back( ( Wire.read() << 8 | Wire.read() ) * 1000 ); // Raw Y-axis value
  result.push_back( ( Wire.read() << 8 | Wire.read() ) * 1000 ); // Raw Z-axis value

  return result;

}

Vector<int> Get_Normalized_Accel( const Vector<int>& raw_accel ) { // Returns the normalized acceleration Values from the MPU
  
  Vector<int> normalized_accel;

  normalized_accel.push_back( raw_accel.at( 0 ) / ALSB_Sensitivity ); 
  normalized_accel.push_back( raw_accel.at( 1 ) / ALSB_Sensitivity );
  normalized_accel.push_back( raw_accel.at( 2 ) / ALSB_Sensitivity );

  return normalized_accel;

}

Vector<int> Get_Raw_Gyro() { // Returns an int vector containing the raw gyrospocic Values from the MPU

  Wire.beginTransmission( MPU );
  Wire.write( 0x43 );                 // Gyro data first register address 0x43
  Wire.endTransmission( false );
  Wire.requestFrom( MPU, 6, true );   // Read 4 registers total, each axis value is stored in 2 registers

  Vector<int> raw_gyro;

  raw_gyro.push_back( ( Wire.read() << 8 | Wire.read() ) * 10000 );
  raw_gyro.push_back( ( Wire.read() << 8 | Wire.read() ) * 10000 );
  raw_gyro.push_back( ( Wire.read() << 8 | Wire.read() ) * 10000 );

  return raw_gyro;

}

Vector<int> Get_Normalized_Gyro( const Vector<int>& raw_gyro ) { // Returns an int vector containing the normalized gyrospocic Values from the MPU

  Vector<int> normalized_gyro;

  normalized_gyro.push_back( raw_gyro.at( 0 ) / GLSB_Sensitivity );
  normalized_gyro.push_back( raw_gyro.at( 1 ) / GLSB_Sensitivity );
  normalized_gyro.push_back( raw_gyro.at( 2 ) / GLSB_Sensitivity );

  return normalized_gyro;

}

Vector<int> Get_All_Values_MULT() {

  // Get Values from Accelorometer
  const Vector<int> raw_accel = raw_accel;
  const Vector<int> normalized_accel = Get_Normalized_Accel( raw_accel );

  // Get Values from Gyroscope
  const Vector<int> raw_gyro = raw_gyro;
  const Vector<int> normalized_gyro = Get_Normalized_Gyro( raw_gyro );

  Vector<int> list;

  list.push_back( millis() );
  for ( int i = 0; i < 3; i++ ) {

    list.push_back( raw_accel.at( i ) );
    list.push_back( normalized_accel.at( i ) );
    list.push_back( raw_gyro.at( i ) );
    list.push_back( normalized_gyro.at( i ) );

  }

  list.push_back( bmp.temperature * 1000 );
  list.push_back( bmp.pressure * 1000 );
  list.push_back( bmp.readAltitude( SEALEVELPRESSURE_HPA ) * 1000 );

  return list;

}

// -----------------------Supplimentary Functions------------------------------------

void Print_All_Values(Vector<int>& Values) { // Print the Values on the serial monitor

  Serial.begin( 115200 ); // Open Serial Port

  String output = ""; // init output string

  output.append( "Time ( S ): " + String( Values.at( 0 ) / 1000.0f ) + "\n" );
  output.append( "Raw Acceleration ( X, Y, Z ): " + String( Values.at( 1 ) / 1000.0f, 2 ) + ',' + String( Values.at( 2 ) / 1000.0f, 2 ) + ',' + String( Values.at( 3 )  / 1000.0f, 2 ) + "\n" );
  output.append( "Normalized Acceleration ( X, Y, Z ): " + String( Values.at( 4 )  / 1000.0f, 2 ) + ',' + String( Values.at( 5 ) / 1000.0f, 2 ) + ',' + String( Values.at( 6 ) / 1000.0f, 2 ) + "\n" );
  output.append( "Raw GyroRange ( X, Y, Z ): " + String( Values.at( 7 ) / 1000.0f, 2 ) + ',' + String( Values.at( 8 ) / 1000.0f, 2 ) + ',' + String( Values.at( 9 ) / 1000.0f, 2 ) + "\n" );
  output.append( "Normalized Gyro Range ( X, Y, Z ): " + String( Values.at( 10 ) / 1000.0f, 2 ) + ',' + String( Values.at( 11 ) / 1000.0f, 2 ) + ',' + String( Values.at( 12 ) / 1000.0f, 2 ) + '\n' );

  output.append( "\n\n Now reading BMP390...\n" );
  output.append( "Tempurature ( C ): " + String( Values.at( 13 ) / 1000.0f ) + "\n" );
  output.append( "Pressure ( kPa ): " + String( Values.at( 14 ) / 1000.0f ) + "\n" );
  output.append( "Altitude ( m ): " + String( Values.at( 15 ) / 1000.0f ) + '\n' );

  Serial.println( output ); // Print output to screen

  Serial.end(); // End Serial Transmission

}

void Write_All_Values_To_SD(Vector<int>& Values) {

  File myFile = SD.open( "Raw_V05.csv", FILE_WRITE );

  for ( uint i = 0; i < Values.size(); i++) {

    myFile.printf( "%.2", Values.at( i ) );

  }

  myFile.println();

  myFile.close();

}

void Record_Data(Vector<int>& Values) {

  Print_All_Values( Values );
  Write_All_Values_To_SD( Values );

}
