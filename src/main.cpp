/*
   Arduino and MPU6050 Accelerometer and Gyroscope Sensor Tutorial
   by Dejan, https://howtomechatronics.com
*/

#include "setup.h"

void setup() {
  // Find hexadecimal representation of accelerometer range based on decimal global variable AccelRange defined above // 
  // Find decimal representation of LSB Sensitivity based on decimal global variable AccelRange defined above // 

  Serial.begin( 115200 ); //Changed to higher rate 4/21/22

  while ( !Serial );
  
  // Builtin SD Card Initialization 
  Serial.print("Initializing SD card...");

  while ( !SD.begin( BUILTIN_SDCARD ) )
    Serial.println("Card not readable");

  Serial.println("Card initialized");

  while ( !bmp.begin_I2C() ) { // hardware I2C mode, can pass in address & alt Wire

    Serial.println( "Could not find a valid BMP3 sensor, check wiring!\n" );
    delay( 2000 );
    
  }

  Wire.beginTransmission( MPU );
  while ( ( Wire.endTransmission() != 0 ) ) {

    Serial.println( "Could not find MPU\n" );
    delay( 2000 );
    
  }

  // Set up oversampling and filter initialization

  bmp.setTemperatureOversampling( BMP3_OVERSAMPLING_8X );
  bmp.setPressureOversampling( BMP3_OVERSAMPLING_4X );
  bmp.setIIRFilterCoeff( BMP3_IIR_FILTER_COEFF_3 );
  bmp.setOutputDataRate( BMP3_ODR_50_HZ );

  pinMode( pinCS, OUTPUT );
  pinMode( pinMOSI, OUTPUT );

  while ( Set_Accel_Range( AccelRange ) != 0 ) {
    
    Serial.println( "Please Fix Accel Range" );
    delay( 2000 );

    }
  
  while ( Set_Gyro_Range( GyroRange ) != 0 ) {

    Serial.println( "Please Fix Gyro Range" );
    delay( 2000 );

  }

  // Find hexadecimal representation of gyroscope range based on decimal global variable GyroRange defined above
  // Find decimal representation of LSB Sensitivity based on decimal global variable GyroRange defined above

  Init_MPU();            // initialize MPU
  Configure_MPU( 0x1C ); // Config Register
 
  Configure_Gyro( 0x1B ); // Config Register

  myFile.print( "Time ( seconds ),Raw Ax ( g ),Ax ( g ),Raw Ay ( g ),Ay ( g ),Raw Az ( g ),Az ( g ),Raw Gx ( deg/s ),Gx ( deg/s ),Raw Gy ( deg/s ),Gy ( deg/s ),Raw Gz ( deg/s ),Gz ( deg/s ), Temperature ( *C ), Pressure ( kpA ), Altitude ( m )" );
  myFile.print( "Time,RAx,Ax,RAy,Ay,RAz,Az,RGx,Gx,RGy,Gy,RGz,Gz, Temp ( *C ), P ( kPa ), Alt ( m )" );
 
  myFile.println();
  myFile.close();

  Serial.end();

  delay( 20 );

}

void loop() {

  Serial.begin( 115200 );
  while ( !Serial );
  
  // === Read acceleromter data === //
  
  Vector<int> raw_accel = Get_Raw_Accel();
  Vector<int> accel = Normalized_Accel( raw_accel );

  // === Read gyroscope data === //

  const Vector<int> raw_gyro = Get_Raw_Gyro();
  const Vector<int> gyro = Normalized_Gyro( raw_gyro );
 
  // Print the values on the serial monitor

  char c = ',';

  Serial.println( "Time ( S ): " + String( millis() / 1000.0 ) );

  Serial.println( "Raw Acceleration ( X, Y, Z ): " + String( raw_accel.at( 0 ) / 1000.0, 2 ) + c + String( raw_accel.at( 1 ) / 1000.0, 2 ) + c + String( raw_accel.at( 0 )  / 1000.0, 2 ) ); // Arduino is really dumb
  
  Serial.println( "Normalized Acceleration ( X, Y, Z ): " + String( accel.at( 0 )  / 1000.0, 2 ) + c + String( accel.at( 1 ) / 1000.0, 2 ) + c + String( accel.at( 2 ) / 1000.0, 2 ) ); // Arduino is really dumb again
  
  Serial.println( "Raw GyroRange ( X, Y, Z ): " + String( raw_gyro.at( 0 ) / 1000.0, 2 ) + c + String( raw_gyro.at( 1 ) / 1000.0, 2 ) + c + String( raw_gyro.at( 2 ) / 1000.0, 2 ) ); // Arduino is really dumb again,...again

  Serial.println( "Normalized Gyro Range ( X, Y, Z ): " + String( gyro.at( 0 ) / 1000.0, 2 ) + c + String( gyro.at( 1 ) / 1000.0, 2 ) + c + String( gyro.at( 2 ) / 1000.0, 2 ) + '\n' ); // Arduino is really dumb again,...again, and again

  Serial.println( "Now reading BMP390..." );
  
  Serial.println( "Tempurature ( C ): " + String( bmp.temperature ) );
  
  Serial.println( "Pressure ( kPa ): " + String( bmp.pressure / 1000.0 ) );

  Serial.println( "Altitude ( m ): " + String( bmp.readAltitude( SEALEVELPRESSURE_HPA ) ) + '\n' );

  Serial.end();

  // myFile = SD.open( "Raw_V05.csv", FILE_WRITE );
  // myFile.println( String( bmp.temperature ) + c + String( bmp.pressure / 1000.0 ) + c + String( bmp.readAltitude( SEALEVELPRESSURE_HPA ) ) + '\n' );
  // myFile.close();

  delay( 2000 );
}
