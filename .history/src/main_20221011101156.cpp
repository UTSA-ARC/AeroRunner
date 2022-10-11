/*
   Arduino and MPU6050 Accelerometer and Gyroscope Sensor Tutorial
   by Dejan, https://howtomechatronics.com
*/
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_BMP3XX.h>
#include <vector>
#include "fixed.h"

//#define BMP_SCK 13
//#define BMP_MISO 12
//#define BMP_MOSI 11
//#define BMP_CS 10
#define BMP_SCL A5
#define BMP_SDA A4 
#define SEALEVELPRESSURE_HPA (1013.25)

#define pinCS 10
#define pinMOSI 13

#define MPU 0x68  // MPU6050 I2C address
#define AccelRange 16
#define GyroRange 2000

typedef numeric::Fixed(4, 2);

std::vector<fixed, fixed, fixed> accel()

Adafruit_BMP3XX bmp;

// int RawAccX, RawAccY, RawAccZ; // Want to read the raw accelerometer values from MPU6050
// int AccX, AccY, AccZ; // Want to calculate the true accelerometer values from MPU6050
// int RawGyroX, RawGyroY, RawGyroZ; // Want to read the raw gyroscope values from MPU6050
// int GyroX, GyroY, GyroZ; // Want to calculate the true gyroscope values from MPU6050
short AFS_SEL, GFS_SEL, ALSB_Sensitivity, GLSB_Sensitivity;


void setup() {
  // Find hexadecimal representation of accelerometer range based on decimal global variable AccelRange defined above // 
  // Find decimal representation of LSB Sensitivity based on decimal global variable AccelRange defined above // 

  //Serial.begin(115200); //Changed to higher rate 4/21/22

  while (!Serial)
    Serial.println("Adafruit BMP388 / BMP390 test");

  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  pinMode(pinCS, OUTPUT);
  pinMode(pinMOSI, OUTPUT);

  // SD Card Initialization
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    //
    return;
  }

  switch (AccelRange) {

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
    Serial.println("Must input 2, 4, 8, or 16g for MPU6050 accelerometer range! Exiting...");
    return;
  }

  // Find hexadecimal representation of gyroscope range based on decimal global variable GyroRange defined above // 
  // Find decimal representation of LSB Sensitivity based on decimal global variable GyroRange defined above // 

  switch (GyroRange) {

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
      Serial.println("Must input 250, 500, 1000, or 2000 deg/s for MPU6050 gyroscope range! Exiting...");
      return;
  }



  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(AFS_SEL);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  
  // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(GFS_SEL);                   // Set the register bits as 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);

  Serial.println("Time (seconds),Raw Ax (g),Ax (g),Raw Ay (g),Ay (g),Raw Az (g),Az (g),Raw Gx (deg/s),Gx (deg/s),Raw Gy (deg/s),Gy (deg/s),Raw Gz (deg/s),Gz (deg/s), Temp (*C), P (kPa), Alt (m)");
  Serial.println();
 
  File myFile = SD.open("Raw_V05.csv", FILE_WRITE);  
  myFile.print("Time (seconds),Raw Ax (g),Ax (g),Raw Ay (g),Ay (g),Raw Az (g),Az (g),Raw Gx (deg/s),Gx (deg/s),Raw Gy (deg/s),Gy (deg/s),Raw Gz (deg/s),Gz (deg/s), Temperature (*C), Pressure (kpA), Altitude (m)");
  myFile.print("Time,RAx,Ax,RAy,Ay,RAz,Az,RGx,Gx,RGy,Gy,RGz,Gz, Temp (*C), P (kPa), Alt (m)");
 
  myFile.println();
  myFile.close();

  delay(20);

}

void loop() {

  Serial.begin(115200);
  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  float RawAccX = (Wire.read() << 8 | Wire.read()); // Raw X-axis value
  float RawAccY = (Wire.read() << 8 | Wire.read()); // Raw Y-axis value
  float RawAccZ = (Wire.read() << 8 | Wire.read()); // Raw Z-axis value
  
  float AccX = RawAccX / ALSB_Sensitivity; // X-axis value
  float AccY = RawAccY / ALSB_Sensitivity; // Y-axis value
  float AccZ = RawAccZ / ALSB_Sensitivity; // Z-axis value

  // === Read gyroscope data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  float RawGyroX = (Wire.read() << 8 | Wire.read());
  float RawGyroY = (Wire.read() << 8 | Wire.read());
  float RawGyroZ = (Wire.read() << 8 | Wire.read());
  
  float GyroX = RawGyroX / GLSB_Sensitivity;
  float GyroY = RawGyroY / GLSB_Sensitivity;
  float GyroZ = RawGyroZ / GLSB_Sensitivity;
 
  // Print the values on the serial monitor

  char c = ',';

  Serial.println("Now reading MPU6050...");
  Serial.print("Time (S): ");
  Serial.println(millis()/1000.0);

  Serial.print("Raw Acceleration (X, Y, Z): ");
  Serial.println(RawAccX + c + RawAccY + c + RawAccZ); // Arduino is really dumb
  
  Serial.print("Calc Acceleration (X, Y, Z): ");
  Serial.println(AccX + c + AccY + c + AccZ); // Arduino is really dumb again
  
  Serial.print("Raw GyroRange (X, Y, Z): ");
  Serial.println(RawGyroX + c + RawGyroY + c + RawGyroZ); // Arduino is really dumb again,...again

  Serial.print("Gyro Range (X, Y, Z): ");
  Serial.println(GyroX + c + GyroY + c + GyroZ); // Arduino is really dumb again,...again, and again

  Serial.println();

  Serial.println("Now reading BMP390...");
  
  Serial.print("Tempurature (C): ");
  Serial.println(bmp.temperature);
  
  Serial.print("Pressure (kPa): ");
  Serial.println(bmp.pressure / 1000.0);
  
  Serial.print("Altitude (m): ");
  Serial.println(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println();
  Serial.println();
  Serial.end();

  // myFile = SD.open("Raw_V05.csv", FILE_WRITE);
  // myFile.print(bmp.temperature);
  // myFile.print(",");
  // myFile.print(bmp.pressure / 1000.0);
  // myFile.print(",");
  // myFile.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  // myFile.println();
  // myFile.close();
}
