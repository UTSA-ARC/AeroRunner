#define BMP_SCL A5
#define BMP_SDA A4 
#define SEALEVELPRESSURE_HPA ( 1013.25f )

#define pinCS 10
#define pinMOSI 13

#define pinDrouge 39
#define pinMain 33

#define MPU 0x68    // MPU6050 I2C address
#define AccelRange 16
#define GyroRange 2000

#define safeAltitude 16 // In meters
#define safePressure 16 // In kPa

#define safeTiltX 10
#define safeTilty 10
#define safeTiltZ 10