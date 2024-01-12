#pragma once

#define PinInputVoltage A9
#define MINIMUM_INPUT_VOLTAGE 3.4f
#define MAXIMUM_INPUT_VOLTAGE 5.0f

#define PinVBAT 49

#define BMP_SCL A5
#define BMP_SDA A4
#define SEALEVELPRESSURE_HPA ( 1013.25f )

#define MPU 0x68 // MPU6050 I2C address
#define AccelRange 16
#define GyroRange 2000

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

#define Y_ACCEL_INDEX 1 // Index of y-axis acceleration in acceleration vector
#define SUBSONIC_SPEED 205.8f // Units: m/s (343 m/s = Mach 1)