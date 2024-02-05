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

float_t apogee = 0;
float_t SurfaceAlt = 0;
bool landed = false;

String csv_file_name = CSV_FILE_NAME;

// Used to keep track of main loop time
TimeData loopTime;
elapsedMicros loopTimer = 0;

void setup() {
    // Find hexadecimal representation of accelerometer range based on decimal global variable AccelRange defined above //
    // Find decimal representation of LSB Sensitivity based on decimal global variable AccelRange defined above //

    Serial.begin( 115200 ); //Changed to higher rate 4/21/22

    while ( !Serial ); //! DELETE WHEN PUTTING IN ROCKET

    // ----------------------------------------------------------------

    // Builtin SD Card Initialization
    Serial.print( "Initializing SD card..." );

    while ( !SD.begin( BUILTIN_SDCARD ) ) {

        Serial.println( "Card not readable" );
        delay( 2000 );

    }

    Serial.println( "SD card is ready!" );

    // ----------------------------------------------------------------

    Serial.println( "Initializing BMP390..." );
    while ( !bmp.begin_I2C() ) { // hardware I2C mode, can pass in address & alt Wire

        Serial.println( "Could not find a valid BMP390 sensor, check wiring!\n" );
        delay( 2000 );

    }

    Serial.println( "Found and initialized a valid BMP390 I2C sensor!" );

    // ----------------------------------------------------------------

     Serial.println( "Initializing MPU6050..." );

     Wire.beginTransmission( MPU_ADDRESS );
     while ( ( Wire.endTransmission() != 0 ) ) {

        Serial.println( "Could not find MPU\n" );
        delay( 2000 );

    }

    Serial.println( "Initialized MPU6050! ");

    // ----------------------------------------------------------------

    // Set up oversampling and filter initialization
    bmp.setTemperatureOversampling( BMP3_OVERSAMPLING_8X );
    bmp.setPressureOversampling( BMP3_OVERSAMPLING_4X );
    bmp.setIIRFilterCoeff( BMP3_IIR_FILTER_COEFF_3 );
    bmp.setOutputDataRate( BMP3_ODR_50_HZ );

    // ----------------------------------------------------------------

    //Set Ranges
    while ( Set_Accel_Range( AccelRange ) != 0 ) {

        Serial.println( "Please Fix Accel Range" );
        delay( 2000 );

        }

    while ( Set_Gyro_Range( GyroRange ) != 0 ) {

        Serial.println( "Please Fix Gyro Range" );
        delay( 2000 );

    }

    // ----------------------------------------------------------------

    Init_MPU( MPU_ADDRESS );            // Initialize MPU

    Configure_MPU( ACCEL_CONFIG ); // Config Register

    Configure_Gyro( GYRO_CONFIG ); // Config Register

    // ----------------------------------------------------------------

    csv_file_name = Init_CSV( csv_file_name ); // Initialize CSV

    // ----------------------------------------------------------------

    uint8_t src_pins[] = { PinSrcDrogue, PinSrcMain }; // Set source pins for continuity check
    uint8_t gnd_pins[] = { PinGnd, PinGnd };           // Set ground pins for continuity check

    Data init_values = Get_All_Values(); // Set Initial Values
    delay( InitValueDelay * 1000 );      // Delay to compare data
    SurfaceAlt = init_values.altitude;   // Get surface altitude (Assuming Setup() will be called on surface ONLY)

    Data values = Get_All_Values(); // Get Current Values
    Result Check_Systems_Result = Check_Systems( &values, &init_values, src_pins, gnd_pins, 2 ); // Check Health of Systems
    while ( Check_Systems_Result.error != 0 ) { // While Systems Bad

        Serial.println( Check_Systems_Result.message ); // Print result

        init_values = values;      // Re-initialize values
        values = Get_All_Values(); // ' '

        Check_Systems_Result = Check_Systems( &values, &init_values, src_pins, gnd_pins, 2 ); // Re-Check result

    }

    // ----------------------------------------------------------------

    Serial.end(); // End Serial Transmission

    delay( ExitSetup * 1000 ); // Delay for N seconds before starting main loop

}

void loop() {

    if ( landed ) return; // Do nothing if landed

    loopTime.dt = loopTimer; // get loop time interval
    loopTimer = 0; // reset timer
    SampleCollection Samples; // Get all data values

    Sample* sample_arr = Samples.Get_Sample_Array(); // Get Sample Array
    const int sample_size = Samples.Size(); // Get Sample Array Size

    // -------------------------------------------------
    
    float_t mpuVelY = 0.0f; // Vertical velocity based on accelerometer data
    float_t avg_y_accel = 0.0f; // Accelerometer data based on most recent Samples
    
    for ( int i = 0; i < SampleAmount; i++ ) { // Calculate average MPU y-axis acceleration over all Samples

        avg_y_accel += sample_arr[ i ].Get_Avg_Data().normalized_accel[ 1 ] / SampleAmount;
        
    }

    // Calculate MPU-based y-axis velocity
    mpuVelY = ( avg_y_accel * G_TO_SI_UNITS ) * ( loopTime.dt * MICROS_TO_SECONDS );

    //-------------------------------------------------

    int sample_movement[ sample_size - 1 ]; // Init Comparison array

    for ( int i = 1; i < sample_size; i++ ) { sample_movement[ i - 1 ] = Samples.Compare_Sample( ( i - 1 ), i ).error; } // Find movement of samples

    if ( !Paras_Armed[ 0 ] ) { // If Drogue is not armed

        for ( int i = 0; i < sample_size; i++ ) { // Iterate through all samples

            Result alt_result = Check_Altitude( sample_arr[ i ].Get_Avg_Data().altitude ); // Check if at Safe Arming Altitude
            sample_arr[ i ].Append_Message( ( alt_result.message + ',' ) ); // Record Result

            if ( alt_result.error == 0 ) { Arm_Parachute( 0 ); Arm_Parachute( 1 ); break; } // If safe, arm Paras

        }

    }

    if ( Paras_Armed[ 1 ] && apogee == 0 ) { // If Drogue Para is armed and apogee is not set

        int i = 0;
        while ( sample_movement[ i ] > 0 && i < ( sample_size - 2 ) ) i++; // Iterate through Comparison array until reading 0 or -1

        // mpuVelY < SUBSONIC_SPEED adds immunity to Mach-related disturbances
        if ( sample_movement[ i + 1 ] < 0 && mpuVelY < SUBSONIC_SPEED) { // If next index is -1

            Result launch_result = Launch_Parachute( 1 ); // Launch Drogue
            sample_arr[ i + 1 ].Append_Message( ( launch_result.message + ',' ) ); // Record result

        }

        apogee = sample_arr[ i + 1 ].Get_Avg_Data().altitude; // Set apogee

    }

    if ( apogee > 0 ) { // If apogee is set

        if ( Paras_Armed[ 0 ] ) { // If Main Para is armed

            for ( int i = 0; i < sample_size; i++ ) { // Iterate through Samples

                Result alt_result = Check_Altitude( sample_arr[ i ].Get_Avg_Data().altitude, apogee ); // Check if at Main Para Altitude
                if ( alt_result.error == 1 ) { // If at Main Para Altitude

                    Launch_Parachute( 0 ); // Launch Main Para
                    sample_arr[ i ].Append_Message( ( alt_result.message + ',' ) ); // Record Result
                    break;

                }

            }

        }

        else { // If Main Para is not armed

            for ( int i = 0; i < sample_size; i++ ) { // Iterate through all Samples
                
                if ( sample_arr[ i ].Get_Avg_Data().altitude <= ( SurfaceAlt + SurfaceAltBias ) ) { // Check if sample is landed
                    
                    landed = true; // Set as true
                    sample_arr[ i ].Append_Message( "!!LANDED VEHICLE WOOOOOO!!" ); // Record Landing
                    break;

                }

            }

        }

    }

    for ( int i = 0; i < sample_size; i++ ) Record_Data( &( sample_arr->Get_Avg_Data() ), csv_file_name ); // Print & Save All Values

    delay( ConsoleDelay * 1000 ); //! FOR JUST EASY READING

}

