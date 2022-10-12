/*
   Arduino and MPU6050 Accelerometer and Gyroscope Sensor Tutorial
   by Dejan, https://howtomechatronics.com
*/

#include "header.h"

void setup() {
  // Find hexadecimal representation of accelerometer range based on decimal global variable AccelRange defined above // 
  // Find decimal representation of LSB Sensitivity based on decimal global variable AccelRange defined above // 

  Serial.begin(115200); //Changed to higher rate 4/21/22

  while (!Serial);

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

  // myFile.print("Time (seconds),Raw Ax (g),Ax (g),Raw Ay (g),Ay (g),Raw Az (g),Az (g),Raw Gx (deg/s),Gx (deg/s),Raw Gy (deg/s),Gy (deg/s),Raw Gz (deg/s),Gz (deg/s), Temperature (*C), Pressure (kpA), Altitude (m)");
  // myFile.print("Time,RAx,Ax,RAy,Ay,RAz,Az,RGx,Gx,RGy,Gy,RGz,Gz, Temp (*C), P (kPa), Alt (m)");
 
  // myFile.println();
  // myFile.close();

  Serial.end();

  delay(20);

}

void loop() {

  Serial.begin(9600);
  while (!Serial);
  
  // === Read acceleromter data === //
  
  Vector<int> raw_accel = Get_Raw_Accel();
  Vector<int> accel = Normalized_Accel(raw_accel);

  // === Read gyroscope data === //

  const Vector<int> raw_gyro = Get_Raw_Gyro();
  const Vector<int> gyro = Normalized_Gyro(raw_gyro);
 
  // Print the values on the serial monitor

  char c = ',';

  //Serial.println("Now reading MPU6050...");
  Serial.println("Time (S): " + String(millis() / 1000.0));

  //Serial.print("Raw Acceleration (X, Y, Z): ");
  Serial.println("Raw Acceleration (X, Y, Z): " + String(raw_accel.at(0) / 1000.0, 2) + c + String(raw_accel.at(1) / 1000.0, 2) + c + String(raw_accel.at(0)  / 1000.0, 2)); // Arduino is really dumb
  
  //Serial.print("Calc Acceleration (X, Y, Z): ");
  Serial.println("Normalized Acceleration (X, Y, Z): " + String(accel.at(0)  / 1000.0, 2) + c + String(accel.at(1) / 1000.0, 2) + c + String(accel.at(2) / 1000.0, 2)); // Arduino is really dumb again
  
  //Serial.print("Raw GyroRange (X, Y, Z): ");
  Serial.println("Raw GyroRange (X, Y, Z): " + String(raw_gyro.at(0) / 1000.0, 2) + c + String(raw_gyro.at(1) / 1000.0, 2) + c + String(raw_gyro.at(2) / 1000.0, 2)); // Arduino is really dumb again,...again

  //Serial.print("Gyro Range (X, Y, Z): ");
  Serial.println("Normalized Gyro Range (X, Y, Z): " + String(gyro.at(0) / 1000.0, 2) + c + String(gyro.at(1) / 1000.0, 2) + c + String(gyro.at(2) / 1000.0, 2)); // Arduino is really dumb again,...again, and again

  Serial.println();

  Serial.println("Now reading BMP390...");
  
  //Serial.print("Tempurature (C): ");
  Serial.println("Tempurature (C): " + String(bmp.temperature));
  
  //Serial.print("Pressure (kPa): ");
  Serial.println("Pressure (kPa): " + String(bmp.pressure / 1000.0));
  
  Serial.print("Altitude (m): ");
  Serial.println("Altitude (m): " + String(bmp.readAltitude(SEALEVELPRESSURE_HPA)));
  Serial.println("\n");

  Serial.end();

  // myFile = SD.open("Raw_V05.csv", FILE_WRITE);
  // myFile.print(bmp.temperature);
  // myFile.print(",");
  // myFile.print(bmp.pressure / 1000.0);
  // myFile.print(",");
  // myFile.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  // myFile.println();
  // myFile.close();

  delay(200);
}
