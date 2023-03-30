#define PinInputVoltage A9
#define MINIMUM_INPUT_VOLTAGE 3.4f
#define MAXIMUM_INPUT_VOLTAGE 5.0f

#define PinVBAT 49

#define SurfaceAccelX 0
#define SurfaceAccelY -9.8f
#define SurfaceAccelZ 0
#define SurfaceTiltX 0
#define SurfaceTiltY 0
#define SurfaceTiltZ 0
#define SurfacePressure 40
#define SurfPTolerance 0.05f // % in decimal form
#define SurfaceTilt 5
#define PinSystemsGood 2
#define PinSystemsBad 12

#define BMP_SCL A5
#define BMP_SDA A4
#define SEALEVELPRESSURE_HPA ( 1013.25f )

#define PinCS 10 //! remove
#define PinMOSI 13 //! remove

#define PinDrouge 39
#define PinMain 33

#define MPU 0x68 // MPU6050 I2C address
#define AccelRange 16
#define GyroRange 2000

#define SafeAltitude 16 // In meters
#define MainParaADelta 100 // In meters //* Change as ratio from apogee
#define ATolerance 0.05f // % in decimal form

#define PTolerance 0.05f // % decimal form

#define SafeTiltX 10
#define SafeTiltY 10
#define SafeTiltZ 10
#define TTolerance 0.05f // % decimal form

#define SafeAccelX 3
#define SafeAccelY 3
#define SafeAccelZ 3
#define ATolerance 0.05f // % decimal form