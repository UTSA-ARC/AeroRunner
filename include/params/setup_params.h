#pragma once

#define CSV_FILE_NAME NULL // By Default the file will be set to "MM-DD-YY(-num).csv" 

#define PinInputVoltage A9
#define MINIMUM_INPUT_VOLTAGE 3.4f
#define MAXIMUM_INPUT_VOLTAGE 5.0f

#define PinVBAT 49

#define BMP_SCL A5
#define BMP_SDA A4
#define SEALEVELPRESSURE_HPA ( 1013.25f )

#define MPU_ADDRESS 0x68 // MPU6050 I2C address
#define ACCEL_CONFIG 0x1C
#define GYRO_CONFIG 0x1B
#define AccelRange 16
#define GyroRange 2000

#define HorizontalAxis 'X' // Define the horizontal axis ( Will be set as 'X' Axis in program )
#define FlipHAxis false // Define if the horizontal axis should be flipped
#define VerticalAxis 'Y' // Define the vertical axis ( Will be set as 'Y' Axis in program )
#define FlipVAxis false // Define if the vertical axis should be flipped
#define DepthAxis 'Z' // Define the depth axis ( Will be set as 'Z' Axis in program )
#define FlipDAxis false  // Define if the depth axis should be flipped

#define PinDrogue 39
#define PinMain 33

#define InitValueDelay 2 // In seconds

#define ConsoleDelay 1 // In seconds

#define ExitSetup 20 // In seconds

#define SurfaceAltBias 10 //In meters

#define PinSrc 1 // FIX, continuity check pin, input
#define PinGnd 1 // FIX, continuity check pin, output

#define MICROS_TO_SECONDS 0.000001f // 1 us = 10^(-6) s
#define G_TO_SI_UNITS 9.80665f // 1 g-unit = 9.80665 m/s^2

#define SUBSONIC_SPEED 205.8f // Units: m/s (343 m/s = Mach 1)

#define PMTolerance 0.05f // Pressure Tolerance, % decimal form
#define SurfacePressure 40 // Surface Pressure
#define SurfPTolerance 0.05f // % in decimal form

#define SafeSurfaceAccelX 0 // Safe Surface X Acceleration
#define SafeSurfaceAccelY -9.8f // Safe Surface Y Acceleration
#define SafeSurfaceAccelZ 0 // Safe Surface Z Acceleration
#define AccTolerance 0.05f // Accel tolerance, % decimal form

#define SafeSurfaceTiltX 0 // Safe Surface X tilt
#define SafeSurfaceTiltY 0 // Safe Surface Y tilt
#define SafeSurfaceTiltZ 0 // Safe Surface Z tilt
#define SurfaceTTolerance 0.05f // % in decimal form

#define PinSystemsGood 2 // Pin for "Systems Good"
#define PinSystemsBad 12 // Pin for "Systems Bad"
