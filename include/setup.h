#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_BMP3XX.h>
#include <TimeLib.h>
#include <Arduino.h>

#define BMP_SCL A5
#define BMP_SDA A4 
#define SEALEVELPRESSURE_HPA ( 1013.25f )

#define pinCS 10
#define pinMOSI 13

#define pinDrouge 39
#define pinMain 33

#define MPU 0x68  // MPU6050 I2C address
#define AccelRange 16
#define GyroRange 2000

#define safeAltitude 16 // In meters
#define safePressure 16 // In kPa

#define safeTiltX 10
#define safeTilty 10
#define safeTiltZ 10

bool Para_Armed = 0;
byte AFS_SEL, GFS_SEL;
unsigned int ALSB_Sensitivity, GLSB_Sensitivity;

Adafruit_BMP3XX bmp;

// Structs
typedef struct INTData { //* All data in integer form 0.0X accuracy (Divide by 1000.0 for dec)

  String time;

  int raw_accel[3];
  int normalized_accel[3];

  int raw_gyro[3];
  int normalized_gyro[3];

  int temperature;
  int pressure;
  int altitude;

  String message = "-";

} INTData;

typedef struct Result {

  int error;
  String msg;

} Result;

int Set_Accel_Range( byte range ) { // Range and sensitivity of accelerometer

  switch ( AccelRange ) {

    case 2:                         // 2Gs of range 
      AFS_SEL = 0x00;
      ALSB_Sensitivity = 16384;
    break;
    
    case 4:                         // 4Gs of range
      AFS_SEL = 0x08;
      ALSB_Sensitivity = 8192;
    break;
    
    case 8:                         // 8Gs of range
      AFS_SEL = 0x10;
      ALSB_Sensitivity = 4096;  
    break;

    case 16:                        // 16Gs of range
      AFS_SEL = 0x18;
      ALSB_Sensitivity = 2048;
    break;

    default:
      Serial.println( "Must input 2, 4, 8, or 16Gs for MPU6050 accelerometer range! Exiting..." );
      return 1;
  }

  return 0;

}

int Set_Gyro_Range( unsigned int range ) { //  Range and sensitivity of gyroscope

  switch ( GyroRange ) {

  case 250:                        // 250 deg/s of range 
      GFS_SEL = 0x00;
      GLSB_Sensitivity = 131;
      break;
  
    case 500:                     // 500 deg/s of range
      GFS_SEL = 0x08;
      GLSB_Sensitivity = 65.5;
      break;
    
    case 1000:                    // 1000 deg/s of range 
      GFS_SEL = 0x10;
      GLSB_Sensitivity = 32.8;  
      break;

    case 2000:                   // 2000 deg/s of range
      GFS_SEL = 0x18;
      GLSB_Sensitivity = 16.4;
      break;

    default:
      Serial.println( "Must input 250, 500, 1000, or 2000 deg/s for MPU6050 gyroscope range! Exiting..." );
      return 1;
  }

  return 0;

}

void Init_Paras() {

  pinMode( pinMain, OUTPUT );
  pinMode( pinDrouge, OUTPUT );

}

void Init_MPU() { // Initialize MPU

  Wire.begin();                        // Initialize comunication
  Wire.beginTransmission( MPU );       // Start communication with MPU6050 // MPU=0x68
  Wire.write( 0x6B );                  // Talk to the register 6B
  Wire.write( 0x00 );                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission( true );        // End the transmission

}
 
void Configure_MPU( int ACCEL_CONFIG ) { // Configure Accelerometer Sensitivity - Full Scale Range ( default +/- 2g )

  Wire.beginTransmission( MPU );
  Wire.write( ACCEL_CONFIG );          //Talk to the ACCEL_CONFIG register ( 1C hex )
  Wire.write( AFS_SEL );               //Set the register bits as 00010000 ( +/- 8g full scale range )
  Wire.endTransmission( true );

}

void Configure_Gyro( int GYRO_CONFIG ) { // Configure Gyro Sensitivity - Full Scale Range ( default +/- 250deg/s )
  
  Wire.beginTransmission( MPU );
  Wire.write( GYRO_CONFIG );           // Talk to the GYRO_CONFIG register ( 1B hex )
  Wire.write( GFS_SEL );               // Set the register bits as 00010000 ( 1000deg/s full scale )
  Wire.endTransmission( true );

}

void Init_CSV() {

  String file_string = String( month() + '-' + day() + '-' + year() ) + ".csv";

  File myFile = SD.open( file_string.c_str(), FILE_WRITE | O_TRUNC );
  myFile.println(

    "Time ( seconds ),Raw Ax ( g ),Raw Ay ( g ),Raw Az ( g ),Ax ( g ),Ay ( g ),Az ( g ),Raw Gx ( deg/s ),Raw Gy ( deg/s ),Raw Gz ( deg/s ),Gx ( deg/s ),Gy ( deg/s ),Gz ( deg/s ),Temperature ( *C ),Pressure ( kpA ),Altitude ( m ),Message" 
    
    );

  myFile.close();

}
