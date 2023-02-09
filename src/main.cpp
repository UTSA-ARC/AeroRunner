/**
 *
 * @file main.cpp
 * @author UTSA ARC Avionics Team 2023
 * @brief The Avionics code for the Spaceport 2023 vehicle
 * @version 1.0.0
 * @date 2022-2023
 * 
 */

#include "functions.h"

void setup() {
  // Find hexadecimal representation of accelerometer range based on decimal global variable AccelRange defined above // 
  // Find decimal representation of LSB Sensitivity based on decimal global variable AccelRange defined above // 

  Serial.begin( 115200 ); //Changed to higher rate 4/21/22

  while ( !Serial );

  // ----------------------------------------------------------------

  // Builtin SD Card Initialization 
  Serial.print("Initializing SD card...");

  while ( !SD.begin( BUILTIN_SDCARD ) ) {

    Serial.println( "Card not readable" );
    delay( 2000 );

  }

  Serial.println( "SD card is ready!" );

  // ----------------------------------------------------------------

  Serial.println( "Initializing BMP3..." );
  while ( !bmp.begin_I2C() ) { // hardware I2C mode, can pass in address & alt Wire

    Serial.println( "Could not find a valid BMP3 sensor, check wiring!\n" );
    delay( 2000 );
    
  }

  Serial.println( "Found and initialized a valid BMP3 I2C sensor!" );

  // ----------------------------------------------------------------

  Serial.println( "Initializing MPU6050..." );

  Wire.beginTransmission( MPU );
  while ( ( Wire.endTransmission() != 0 ) ) {

    Serial.println( "Could not find MPU\n" );
    delay( 2000 );
    
  }

  // ----------------------------------------------------------------

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling( BMP3_OVERSAMPLING_8X );
  bmp.setPressureOversampling( BMP3_OVERSAMPLING_4X );
  bmp.setIIRFilterCoeff( BMP3_IIR_FILTER_COEFF_3 );
  bmp.setOutputDataRate( BMP3_ODR_50_HZ );

  // ----------------------------------------------------------------

  // Pin configuration
  pinMode( pinCS, OUTPUT );
  pinMode( pinMOSI, OUTPUT );

  // ----------------------------------------------------------------

  // Set Ranges
  while ( Set_Accel_Range( AccelRange ) != 0 ) {
    
    Serial.println( "Please Fix Accel Range" );
    delay( 2000 );

    }
  
  while ( Set_Gyro_Range( GyroRange ) != 0 ) {

    Serial.println( "Please Fix Gyro Range" );
    delay( 2000 );

  }

  // ----------------------------------------------------------------

  Init_MPU();            // Initialize MPU
  Configure_MPU( 0x1C ); // Config Register
 
  Configure_Gyro( 0x1B ); // Config Register

  // ----------------------------------------------------------------

  Init_CSV(); // Initialize CSV

  Serial.end(); // End Serial Transmission

  delay( 20 ); // Delay for 20 Milliseconds before starting main loop

}

void loop() {

  Vector<int> values = Get_All_Values_MULT();
  
  // Print & Save All Values
  Record_Data(values);

  delay( 2000 );

}
