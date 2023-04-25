/**
 *
 * @file main.cpp
 * @author UTSA ARC Avionics Team 2023
 * @brief The Avionics code for the Spaceport 2023 vehicle
 * @version 1.0.0
 * @date 2022-2023
 *
 */

#include "setup.h"
#include "functions.h"
#include "samples.h"

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

    // Serial.println( "Initializing MPU6050..." );

    // Wire.beginTransmission( MPU );
    // while ( ( Wire.endTransmission() != 0 ) ) {

    //     Serial.println( "Could not find MPU\n" );
    //     delay( 2000 );

    // }

    // Serial.println( "Initialized MPU6050! ");

    // ----------------------------------------------------------------

    // Set up oversampling and filter initialization
    bmp.setTemperatureOversampling( BMP3_OVERSAMPLING_8X );
    bmp.setPressureOversampling( BMP3_OVERSAMPLING_4X );
    bmp.setIIRFilterCoeff( BMP3_IIR_FILTER_COEFF_3 );
    bmp.setOutputDataRate( BMP3_ODR_50_HZ );

    // ----------------------------------------------------------------

    // Set Ranges
    // while ( Set_Accel_Range( AccelRange ) != 0 ) {

    //     Serial.println( "Please Fix Accel Range" );
    //     delay( 2000 );

    //     }

    // while ( Set_Gyro_Range( GyroRange ) != 0 ) {

    //     Serial.println( "Please Fix Gyro Range" );
    //     delay( 2000 );

    // }

    // ----------------------------------------------------------------

    // Init_MPU();            // Initialize MPU

    // Configure_MPU( 0x1C ); // Config Register

    // Configure_Gyro( 0x1B ); // Config Register

    // ----------------------------------------------------------------

    Init_CSV(); // Initialize CSV

    // ----------------------------------------------------------------
    
    Data values = prev_values = Get_All_Values();
    Result Check_Systems_result = Check_Systems( values, prev_values );
    while ( Check_Systems_result.error != 0 ) {

        Serial.println( Check_Systems_result.message );

        prev_values = values;
        values = Get_All_Values();

        Check_Systems_result = Check_Systems( values, prev_values );

    }

    // ----------------------------------------------------------------

    Serial.end(); // End Serial Transmission

    delay( 20 ); // Delay for 20 Milliseconds before starting main loop

}

void loop() {

    SampleCollection Samples; // Get all data values

    Sample* sample_arr = Samples.Get_Sample_Array();
    const int sample_size = Samples.Size();

    String output;

    Result alt_results[sample_size], pres_results[sample_size];

    for ( int i = sample_size - 1; i > 0; i--) {

        alt_results[i - 1] = Check_Altitude(sample_arr[i].Get_Avg_Data().altitude, sample_arr[i - 1].Get_Avg_Data().altitude, apogee);
        pres_results[i - 1] = Check_Pressure_Delta(sample_arr[i].Get_Avg_Data().pressure, sample_arr[i - 1].Get_Avg_Data().pressure);
       
    }

    for ( int i = 0; i < sample_size - 1; i++ ) {

        output+= "ALT_RESULTS:" + alt_results[i].message + "; ";
        output += "PRES_RESULTS:" + pres_results[i].message + "; ";
    
    }

   int sample_movement;

    for (int i = 1; i < sample_size; i++ ) { sample_movement += Samples.Compare_Sample( ( i - 1 ), i ).error; }

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

    prev_values = values;

    // Print & Save All Values
    Record_Data( values );
    delay(1000); //! FOR JUST EASY READING

}

