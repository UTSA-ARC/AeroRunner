#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_BMP3XX.h>
#include <Vector.h>

#define BMP_SCL A5
#define BMP_SDA A4 
#define SEALEVELPRESSURE_HPA (1013.25)

#define pinCS 10
#define pinMOSI 13

#define MPU 0x68  // MPU6050 I2C address
#define AccelRange 16
#define GyroRange 2000

short AFS_SEL, GFS_SEL, ALSB_Sensitivity, GLSB_Sensitivity;
File myFile = SD.open("Raw_V05.csv", FILE_WRITE);

Adafruit_BMP3XX bmp;

Vector<int> Get_Raw_Accel() {

  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers

  Vector<int> result;

  result.push_back((Wire.read() << 8 | Wire.read()) * 1000); // Raw X-axis value
  result.push_back((Wire.read() << 8 | Wire.read()) * 1000); // Raw Y-axis value
  result.push_back((Wire.read() << 8 | Wire.read()) * 1000); // Raw Z-axis value

  return result;

}

Vector<int> Normalized_Accel(const Vector<int>& raw_accel) {
  
  Vector<int> normalized_accel;

  normalized_accel.push_back(raw_accel.at(0) / ALSB_Sensitivity); 
  normalized_accel.push_back(raw_accel.at(1) / ALSB_Sensitivity);
  normalized_accel.push_back(raw_accel.at(2) / ALSB_Sensitivity);

  return normalized_accel;

}

Vector<int> Get_Raw_Gyro() {

  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers

  Vector<int> raw_gyro;

  raw_gyro.push_back((Wire.read() << 8 | Wire.read()) * 10000);
  raw_gyro.push_back((Wire.read() << 8 | Wire.read()) * 10000);
  raw_gyro.push_back((Wire.read() << 8 | Wire.read()) * 10000);

  return raw_gyro;

}

Vector<int> Normalized_Gyro(const Vector<int>& raw_gyro) {

  Vector<int> normalized_gyro;

  normalized_gyro.push_back(raw_gyro.at(0) / GLSB_Sensitivity);
  normalized_gyro.push_back(raw_gyro.at(1) / GLSB_Sensitivity);
  normalized_gyro.push_back(raw_gyro.at(2) / GLSB_Sensitivity);

  return normalized_gyro;

}
