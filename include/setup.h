#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_BMP3XX.h>
#include <Vector.h>

#define BMP_SCL A5
#define BMP_SDA A4 
#define SEALEVELPRESSURE_HPA ( 1013.25 )

#define pinCS 10
#define pinMOSI 13

#define MPU 0x68  // MPU6050 I2C address
#define AccelRange 16
#define GyroRange 2000

byte AFS_SEL, GFS_SEL;
short ALSB_Sensitivity, GLSB_Sensitivity;

File myFile = SD.open( "Raw_V05.csv", FILE_WRITE );

Adafruit_BMP3XX bmp;

int Set_Accel_Range( byte range ) {

  switch ( AccelRange ) {

    case 2:
      AFS_SEL = 0x00;
      ALSB_Sensitivity = 16384;
    break;
    
    case 4:
      AFS_SEL = 0x08;
      ALSB_Sensitivity = 8192;
    break;
    
    case 8:
      AFS_SEL = 0x10;
      ALSB_Sensitivity = 4096;  
    break;

    case 16:
      AFS_SEL = 0x18;
      ALSB_Sensitivity = 2048;
    break;

    default:
      Serial.println( "Must input 2, 4, 8, or 16g for MPU6050 accelerometer range! Exiting..." );
      return 1;
  }

  return 0;

}

int Set_Gyro_Range( short range ) {

  switch ( GyroRange ) {

    case 250:
      GFS_SEL = 0x00;
      GLSB_Sensitivity = 131;
      break;
  
    case 500:
      GFS_SEL = 0x08;
      GLSB_Sensitivity = 65.5;
      break;
    
    case 1000:
      GFS_SEL = 0x10;
      GLSB_Sensitivity = 32.8;  
      break;

    case 2000:
      GFS_SEL = 0x18;
      GLSB_Sensitivity = 16.4;
      break;

    default:
      Serial.println( "Must input 250, 500, 1000, or 2000 deg/s for MPU6050 gyroscope range! Exiting..." );
      return 0;
  }

  return 1;

}

void initMPU() { // Initialize MPU

  Wire.begin();                        // Initialize comunication
  Wire.beginTransmission( MPU );       // Start communication with MPU6050 // MPU=0x68
  Wire.write( 0x6B );                  // Talk to the register 6B
  Wire.write( 0x00 );                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission( true );        //end the transmission

}
 
void configureMPU( int ACCEL_CONFIG ) { // Configure Accelerometer Sensitivity - Full Scale Range ( default +/- 2g )

  Wire.beginTransmission( MPU );
  Wire.write( ACCEL_CONFIG );                  //Talk to the ACCEL_CONFIG register ( 1C hex )
  Wire.write( AFS_SEL );               //Set the register bits as 00010000 ( +/- 8g full scale range )
  Wire.endTransmission( true );

}

void configureGyro( int GYRO_CONFIG ) { // Configure Gyro Sensitivity - Full Scale Range ( default +/- 250deg/s )
  
  Wire.beginTransmission( MPU );
  Wire.write( GYRO_CONFIG );                   // Talk to the GYRO_CONFIG register ( 1B hex )
  Wire.write( GFS_SEL );                // Set the register bits as 00010000 ( 1000deg/s full scale )
  Wire.endTransmission( true );

}

Vector<int> Get_Raw_Accel() { // Returns an int vector of the raw acceleration values from the MPU

  Wire.beginTransmission( MPU );
  Wire.write( 0x3B ); // Start with register 0x3B ( ACCEL_XOUT_H )
  Wire.endTransmission( false );
  Wire.requestFrom( MPU, 6, true ); // Read 6 registers total, each axis value is stored in 2 registers

  Vector<int> result;

  result.push_back( ( Wire.read() << 8 | Wire.read() ) * 1000 ); // Raw X-axis value
  result.push_back( ( Wire.read() << 8 | Wire.read() ) * 1000 ); // Raw Y-axis value
  result.push_back( ( Wire.read() << 8 | Wire.read() ) * 1000 ); // Raw Z-axis value

  return result;

}

Vector<int> Normalized_Accel( const Vector<int>& raw_accel ) {
  
  Vector<int> normalized_accel;

  normalized_accel.push_back( raw_accel.at( 0 ) / ALSB_Sensitivity ); 
  normalized_accel.push_back( raw_accel.at( 1 ) / ALSB_Sensitivity );
  normalized_accel.push_back( raw_accel.at( 2 ) / ALSB_Sensitivity );

  return normalized_accel;

}

Vector<int> Get_Raw_Gyro() { // Returns an int vector containing the raw gyrospocic values from the MPU

  Wire.beginTransmission( MPU );
  Wire.write( 0x43 ); // Gyro data first register address 0x43
  Wire.endTransmission( false );
  Wire.requestFrom( MPU, 6, true ); // Read 4 registers total, each axis value is stored in 2 registers

  Vector<int> raw_gyro;

  raw_gyro.push_back( ( Wire.read() << 8 | Wire.read() ) * 10000 );
  raw_gyro.push_back( ( Wire.read() << 8 | Wire.read() ) * 10000 );
  raw_gyro.push_back( ( Wire.read() << 8 | Wire.read() ) * 10000 );

  return raw_gyro;

}

Vector<int> Normalized_Gyro( const Vector<int>& raw_gyro ) { // Returns an int vector containing the raw gyrospocic values from the MPU

  Vector<int> normalized_gyro;

  normalized_gyro.push_back( raw_gyro.at( 0 ) / GLSB_Sensitivity );
  normalized_gyro.push_back( raw_gyro.at( 1 ) / GLSB_Sensitivity );
  normalized_gyro.push_back( raw_gyro.at( 2 ) / GLSB_Sensitivity );

  return normalized_gyro;

}
