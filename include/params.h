#define INT_LOW -2147483647
#define INT_HIGH 2147483647

#define MULT 1000

#define BMP_SCL A5
#define BMP_SDA A4 
#define SEALEVELPRESSURE_HPA ( 1013.25f )

#define PinCS 10
#define PinMOSI 13

#define PinDrouge 39
#define PinMain 33

#define MPU 0x68 // MPU6050 I2C address
#define AccelRange 16
#define GyroRange 2000

#define SafeAltitude 16 // In meters

#define PTolerance 0.05f // % decimal form

#define SafeTiltX 10
#define SafeTiltY 10
#define SafeTiltZ 10
#define TTolerance 0.05f // % decimal form

#define SurfaceAccelX 0
#define SurfaceAccelY -9.8f
#define SurfaceAccelZ 0
#define SafeAccelX 3
#define SafeAccelY 3
#define SafeAccelZ 3
#define ATolerance 0.05f // % decimal form