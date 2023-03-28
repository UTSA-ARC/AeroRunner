/**
 *
 * @file main.cpp
 * @author UTSA ARC Avionics Team 2023
 * @brief The Avionics code for the Spaceport 2023 vehicle
 * @version 1.0.0
 * @date 2022-2023
 *
 */

#include "functions.h"

INTData prev_data;
int apogee;

void setup() {
    // Find hexadecimal representation of accelerometer range based on decimal global variable AccelRange defined above //
    // Find decimal representation of LSB Sensitivity based on decimal global variable AccelRange defined above //

    Serial.begin( 115200 ); //Changed to higher rate 4/21/22

    while ( !Serial ); //! DELETE WHEN PUTTING IN ROCKET

    // ----------------------------------------------------------------

    // Builtin SD Card Initialization
    Serial.print("Initializing SD card...");

    while ( !SD.begin( BUILTIN_SDCARD ) ) {

        Serial.println( "Card not readable" );
        delay( 2000 );

    }

    Serial.println( "SD card is ready!" );

    // ----------------------------------------------------------------

    Serial.println( "Initializing BMP3..." );
    while ( !bmp.begin_I2C() ) { // hardware I2C mode, can pass in address & alt Wire

        Serial.println( "Could not find a valid BMP3 sensor, check wiring!\n" );
        delay( 2000 );

    }

    Serial.println( "Found and initialized a valid BMP3 I2C sensor!" );

    // ----------------------------------------------------------------

    Serial.println( "Initializing MPU6050..." );

    Wire.beginTransmission( MPU );
    while ( ( Wire.endTransmission() != 0 ) ) {

        Serial.println( "Could not find MPU\n" );
        delay( 2000 );

    }

    Serial.println( "Initialized MPU6050! ");

    // ----------------------------------------------------------------

    // Check if connected to sufficient voltage
    int input_voltage = analogRead( PinInputVoltage );
    Result input_safe = Check_Input_Voltage( input_voltage );
    while ( input_safe.error != 0 ) {

        Serial.println( input_safe.message );

    }

    // ----------------------------------------------------------------

    // Set up oversampling and filter initialization
    bmp.setTemperatureOversampling( BMP3_OVERSAMPLING_8X );
    bmp.setPressureOversampling( BMP3_OVERSAMPLING_4X );
    bmp.setIIRFilterCoeff( BMP3_IIR_FILTER_COEFF_3 );
    bmp.setOutputDataRate( BMP3_ODR_50_HZ );

    // ----------------------------------------------------------------

    // Pin configuration
    pinMode( PinCS, OUTPUT );
    pinMode( PinMOSI, OUTPUT );

    // ----------------------------------------------------------------

    // Set Ranges
    while ( Set_Accel_Range( AccelRange ) != 0 ) {

        Serial.println( "Please Fix Accel Range" );
        delay( 2000 );

        }

    while ( Set_Gyro_Range( GyroRange ) != 0 ) {

        Serial.println( "Please Fix Gyro Range" );
        delay( 2000 );

    }

    // ----------------------------------------------------------------

    Init_MPU();            // Initialize MPU

    Configure_MPU( 0x1C ); // Config Register

    Configure_Gyro( 0x1B ); // Config Register

    // ----------------------------------------------------------------

    Init_CSV(); // Initialize CSV

    Serial.end(); // End Serial Transmission

    delay( 20 ); // Delay for 20 Milliseconds before starting main loop

}

void loop() {

    INTData values = Get_All_Values_INT(); // Get all data values

    Result alt_result = Check_Altitude( values.altitude, prev_data.altitude, apogee );
    Result pres_result = Check_Pressure_Delta( values.pressure, prev_data.pressure );
    Result tilt_result = Check_Tilt( values.normalized_gyro, prev_data.normalized_gyro );
    Result accel_result = Check_Accel( values.normalized_accel, prev_data.normalized_accel, alt_result.error );

    values.message = alt_result.message + ',' +
                      pres_result.message + ',' +
                      tilt_result.message + ',' +
                      accel_result.message + ',';

    switch ( alt_result.error ) {

        case 0: // Reached Safe Altitude

            Arm_Parachute( 0 ); // Deploy Main
            Arm_Parachute( 1 ); // Deploy Drouge
            break;

        case 1: // Reached Apogee

            Launch_Parachute( 1 ); // Launch Drouge
            apogee = values.altitude;
            break;

        case 2: // Reached Main Parachute Altitude

            Launch_Parachute( 0 ); // Launch Main
            break;

        default: // If no special case
            break;

    }

    prev_data = values;

    // Print & Save All Values
    Record_Data( values );
    delay(1000); //! FOR JUST EASY READING

}

