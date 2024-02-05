#pragma once

#include "globals.h"
#include "params/function_params.h"

// -------------------------Vector Functions-----------------------

int* Get_Raw_Accel() { // Returns an int vector of the raw acceleration Values from the MPU

    Wire.beginTransmission( MPU_ADDRESS );
    Wire.write( 0x3B );                       // Start with register 0x3B ( ACCEL_XOUT_H )
    Wire.endTransmission( false );
    Wire.requestFrom( MPU_ADDRESS, 6, true );         // Read 6 registers total, each axis value is stored in 2 registers

    static int result[ 3 ];

    for ( int i = 0; i < 3; i++ ) result[ i ] = ( Wire.read() << 8 | Wire.read() ); // Raw values

    return result;

}

float_t* Get_Normalized_Accel( const int* raw_accel ) { // Returns the normalized acceleration Values from the MPU

    static float_t normalized_accel[ 3 ];

    for ( int i = 0; i < 3; i++ ) normalized_accel[ i ] = raw_accel[ i ] / ( ALSB_Sensitivity / 1.0f );

    return normalized_accel;

}

int* Get_Raw_Gyro() { // Returns an int vector containing the raw gyroscopic Values from the MPU

    Wire.beginTransmission( MPU_ADDRESS );
    Wire.write( 0x43 );                     // Gyro data first register address 0x43
    Wire.endTransmission( false );
    Wire.requestFrom( MPU_ADDRESS, 6, true );       // Read 4 registers total, each axis value is stored in 2 registers

    static int raw_gyro[ 3 ];

    for ( int i = 0; i < 3; i++ ) raw_gyro[ i ] = ( Wire.read() << 8 | Wire.read() );

    if ( toUpperCase( HorizontalAxis ) != 'X' ) { // Swap X value

        int temp = raw_gyro[ 0 ];

        if ( toUpperCase( HorizontalAxis ) == 'Y' ) { // Swap X and Y

            raw_gyro[ 0 ] = raw_gyro[ 1 ];
            raw_gyro[ 1 ] = temp;

        }

        else { // Swap X and Z

            raw_gyro[ 0 ] = raw_gyro[ 2 ];
            raw_gyro[ 2 ] = temp;

        }

    }

    if ( toUpperCase( VerticalAxis ) != 'Y' ) { // Swap Y value

        int temp = raw_gyro[ 1 ];

        if ( toUpperCase( VerticalAxis ) == 'X' ) { // Swap Y and X

            raw_gyro[ 1 ] = raw_gyro[ 0 ];
            raw_gyro[ 0 ] = temp;
            
        }

        else { // Swap Y and Z

            raw_gyro[ 1 ] = raw_gyro[ 2 ];
            raw_gyro[ 2 ] = temp;

        }

    }

    if ( toUpperCase( DepthAxis ) != 'Z' ) { // Swap Z value

        int temp = raw_gyro[ 2 ];

        if ( toUpperCase( DepthAxis ) == 'X' ) { // Swap Z and X

            raw_gyro[ 2 ] = raw_gyro[ 0 ];
            raw_gyro[ 0 ] = temp;
            
        }
            
        else { // Swap Z and Y

            raw_gyro[ 2 ] = raw_gyro[ 1 ];
            raw_gyro[ 1 ] = temp;

        }

    }

    return raw_gyro;

}

float_t* Get_Normalized_Gyro( const int* raw_gyro ) { // Returns a float_t vector containing the normalized gyroscopic Values from the MPU

    static float_t normalized_gyro[ 3 ];

    for ( int i = 0; i < 3; i++ ) normalized_gyro[ i ] = raw_gyro[ 0 ] / ( GLSB_Sensitivity / 1.0f );

    if ( FlipHAxis ) normalized_gyro[ 0 ] = -normalized_gyro[ 0 ]; // Flip the X axis
    if ( FlipVAxis ) normalized_gyro[ 1 ] = -normalized_gyro[ 1 ]; // Flip the Y axis
    if ( FlipDAxis ) normalized_gyro[ 2 ] = -normalized_gyro[ 2 ]; // Flip the Z axis


    return normalized_gyro;

}

// -----------------------Struct Functions------------------------------------------

Data Get_All_Values() { // Record all values

    Data data;

    int i = 0;
    // Get Values from Accelerometer
    int *raw_a_ptr = Get_Raw_Accel();
    int *raw_g_ptr = Get_Raw_Gyro();
    float_t *norm_a_ptr = Get_Normalized_Accel( raw_a_ptr );
    float_t *norm_g_ptr = Get_Normalized_Gyro( raw_g_ptr );

    for ( i = 0; i < 3; i++ ) {

        data.raw_accel[ i ] = raw_a_ptr[ i ];
        data.raw_gyro[ i ] = raw_g_ptr[ i ];
        data.normalized_accel[ i ] = norm_a_ptr[ i ];
        data.normalized_gyro[ i ] = norm_g_ptr[ i ];

    }

    time_t time_now = now();
    data.time = (

        String( hour( time_now ) ) + ":" +
        String( minute( time_now ) < 10 ? ( '0' + String( minute( time_now ) ) ) : minute( time_now ) ) + ":" +
        String( second( time_now ) < 10 ? ( '0' + String( second( time_now ) ) ) : second( time_now ) )

        );

    data.temperature = ( bmp.temperature  );
    data.pressure = ( bmp.pressure  );
    data.altitude = ( bmp.readAltitude( SEALEVELPRESSURE_HPA )  );

    return data;

}

// -----------------------Data Handling Functions-----------------------------------

void Print_All_Values( SampleData* Values ) { // Print the Values on the serial monitor

    Serial.begin( 115200 ); // Open Serial Port

    String output = ""; // init output string

    output.append( "Time ( S ): " + Values->time + '\n' );
    output.append( "Time End ( S ) " + Values->timeEnd + '\n');

    output.append(

        "Raw Acceleration ( X, Y, Z ): " +
        String( Values->raw_accel[ 0 ] / 1.0f, 2 ) + ',' +
        String( Values->raw_accel[ 1 ] / 1.0f, 2 ) + ',' +
        String( Values->raw_accel[ 2 ] / 1.0f, 2 ) + '\n'

        );

    output.append(

        "Normalized Acceleration ( X, Y, Z ): " +
        String( Values->normalized_accel[ 0 ] / 1.0f, 2 ) + ',' +
        String( Values->normalized_accel[ 1 ] / 1.0f, 2 ) + ',' +
        String( Values->normalized_accel[ 2 ] / 1.0f, 2 ) + '\n'

        );

    output.append(

        "Raw GyroRange ( X, Y, Z ): " +
        String( Values->raw_gyro[ 0 ] / 1.0f, 2 ) + ',' +
        String( Values->raw_gyro[ 1 ] / 1.0f, 2 ) + ',' +
        String( Values->raw_gyro[ 2 ] / 1.0f, 2 ) + '\n'

        );

    output.append(

        "Normalized Gyro Range ( X, Y, Z ): " +
        String( Values->normalized_gyro[ 0 ] / 1.0f, 2 ) + ',' +
        String( Values->normalized_gyro[ 1 ] / 1.0f, 2 ) + ',' +
        String( Values->normalized_gyro[ 2 ] / 1.0f, 2 ) + '\n'

        );

    output.append( "\n\nNow reading BMP390...\n" );
    output.append( "Temperature ( C ): " + String( Values->temperature / 1.0f ) + '\n' );
    output.append( "Pressure ( kPa ): " + String( Values->pressure / 1.0f ) + '\n' );
    output.append( "Altitude ( m ): " + String( Values->altitude / 1.0f ) + '\n' );

    Serial.println( output ); // Print output to screen

    Serial.end(); // End Serial Transmission

}

void Write_All_Values_To_SD( SampleData* Values, String file_name ) { // Records values to SD card

    String file_string = file_name + ".csv";
    File myFile = SD.open( file_string.c_str(), FILE_WRITE );

    //* Time ( RTC ),
    //* TimeEnd ( RTC ),
    //* Raw Ax ( g ),Raw Ay ( g ),Raw Az ( g ),
    //* Ax ( g ),Ay ( g ),Az ( g ),
    //* Raw Gx ( deg/s ),Raw Gy ( deg/s ),Raw Gz ( deg/s ),
    //* Gx ( deg/s ),Gy ( deg/s ),Gz ( deg/s ),
    //* Temperature ( *C ),Pressure ( kpA ),Altitude ( m ),
    //* Message

    String output = "";

    output += ( Values->time + ',' );

    output += ( Values->timeEnd );

    for ( int i = 0; i < 3; i++ ) output += ( String( Values->raw_accel[ i ] / 1.0f ) + ',' );

    for ( int i = 0; i < 3; i++ ) output += ( String( Values->normalized_accel[ i ] / 1.0f ) + ',' );

    for ( int i = 0; i < 3; i++ ) output += ( String( Values->raw_gyro[ i ] / 1.0f ) + ',' );

    for ( int i = 0; i < 3; i++ ) output += ( String( Values->normalized_gyro[ i ] / 1.0f ) + ',' );

    output += ( String( Values->temperature / 1.0f ) + ',' );

    output += ( String( Values->pressure / 1.0f ) + ',' );

    output += ( String( Values->altitude / 1.0f ) + ',' );

    output += ( Values->message );

    myFile.println( output );

    myFile.close();

}

void Record_Data( SampleData *Values, String file_name ) { // Prints data to screen and saves it to file

    Print_All_Values( Values );
    Write_All_Values_To_SD( Values, file_name );

}

// -----------------------Parachute Functions---------------------------------------

void Arm_Parachute( const int shute ) { // Arms Parachute

   Paras_Armed[ shute ] = 1;

}

Result Launch_Parachute( const int shute ) { // Launches Parachute

    switch ( shute ) {

        case 0: // Main shute

            if ( Paras_Armed[ shute ] ) {

                digitalWrite( PinMain, HIGH );
                Paras_Armed[ shute ] = 0;
                return { 0, "!!MAIN shute LAUNCHED!!" };

            }

            else return { 1, "MAIN shute Not Deployed!" };

        case 1: // Drogue shute

            if ( Paras_Armed[ shute ] ) {

                digitalWrite( PinDrogue, HIGH );
                Paras_Armed[ shute ] = 0;
                return { 0, "!!DROGUE shute LAUNCHED" };

            }

            else return { 2, "DROGUE shute Not Deployed!" };

        default:
            return { -1, "Not a Valid shute!" };

    }

}

// -----------------------Internal Trigger Functions--------------------------------

Result Check_Altitude( const float_t altitude, const float_t prev_altitude = 0, const float_t apogee = 0, const float_t safe_altitude = SafeAltitude, const float_t main_para_alt = MainParaAlt ) { // Checks if altitude is safe/at apogee //* Not being used for immediate compatibility

    if ( apogee > 0 && altitude <= main_para_alt ) return { 1, "!!MAIN PARACHUTE ALTITUDE REACHED!!" };

    if ( altitude > safe_altitude ) {

        return { 0, "Safe Altitude" }; // Safe

    }

    return { -1, "unsafe altitude" }; // Unsafe

}

