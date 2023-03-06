
#include "setup.h"

// -------------------------Vector Functions-----------------------

int* Get_Raw_Accel() { // Returns an int vector of the raw acceleration Values from the MPU

    Wire.beginTransmission( MPU );
    Wire.write( 0x3B );                       // Start with register 0x3B ( ACCEL_XOUT_H )
    Wire.endTransmission( false );
    Wire.requestFrom( MPU, 6, true );         // Read 6 registers total, each axis value is stored in 2 registers

    static int result[ 3 ];

    for ( int i = 0; i < 3; i++ ) result[ i ] = ( ( Wire.read() << 8 | Wire.read() ) * MULT ); // Raw values

    return result;

}

int* Get_Normalized_Accel( int* raw_accel ) { // Returns the normalized acceleration Values from the MPU

    static int normalized_accel[ 3 ];

    for ( int i = 0; i < 3; i++ ) normalized_accel[ i ] = raw_accel[ i ] / ALSB_Sensitivity ;

    return normalized_accel;

}

int* Get_Raw_Gyro() { // Returns an int vector containing the raw gyrospocic Values from the MPU

    Wire.beginTransmission( MPU );
    Wire.write( 0x43 );                     // Gyro data first register address 0x43
    Wire.endTransmission( false );
    Wire.requestFrom( MPU, 6, true );       // Read 4 registers total, each axis value is stored in 2 registers

    static int raw_gyro[ 3 ];

    for ( int i = 0; i < 3; i++ ) raw_gyro[ i ] = ( Wire.read() << 8 | Wire.read() ) * MULT;

    return raw_gyro;

}

int* Get_Normalized_Gyro( int* raw_gyro ) { // Returns an int vector containing the normalized gyrospocic Values from the MPU

    static int normalized_gyro[ 3 ];

    for ( int i = 0; i < 3; i++ ) normalized_gyro[ i ] = raw_gyro[ 0 ] / GLSB_Sensitivity;

    return normalized_gyro;

}

// -----------------------Struct Functions------------------------------------------

INTData Get_All_Values_INT() {

    INTData data;

    int i = 0;
    // Get Values from Accelorometer
    int *raw_a_ptr = Get_Raw_Accel();
    int *raw_g_ptr = Get_Raw_Gyro();
    int *norm_a_ptr = Get_Normalized_Accel( raw_a_ptr );
    int *norm_g_ptr = Get_Normalized_Gyro( raw_g_ptr );

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

    data.temperature = static_cast<int>( bmp.temperature * MULT );
    data.pressure = static_cast<int>( bmp.pressure * MULT );
    data.altitude = static_cast<int>( bmp.readAltitude( SEALEVELPRESSURE_HPA ) * MULT );

    return data;

}

// -----------------------Data Handling Functions-----------------------------------

void Print_All_Values( INTData& Values ) { // Print the Values on the serial monitor

    Serial.begin( 115200 ); // Open Serial Port

    String output = ""; // init output string

    output.append( "Time ( S ): " + Values.time + '\n' );

    output.append(

        "Raw Acceleration ( X, Y, Z ): " +
        String( Values.raw_accel[ 0 ] / ( MULT * 1.0f ), 2 ) + ',' +
        String( Values.raw_accel[ 1 ] / ( MULT * 1.0f ), 2 ) + ',' +
        String( Values.raw_accel[ 2 ]    / ( MULT * 1.0f ), 2 ) + '\n'

        );

    output.append(

        "Normalized Acceleration ( X, Y, Z ): " +
        String( Values.normalized_accel[ 0 ] / ( MULT * 1.0f ), 2 ) + ',' +
        String( Values.normalized_accel[ 1 ] / ( MULT * 1.0f ), 2 ) + ',' +
        String( Values.normalized_accel[ 2 ] / ( MULT * 1.0f ), 2 ) + '\n'

        );

    output.append(

        "Raw GyroRange ( X, Y, Z ): " +
        String( Values.raw_gyro[ 0 ] / ( MULT * 1.0f ), 2 ) + ',' +
        String( Values.raw_gyro[ 1 ] / ( MULT * 1.0f ), 2 ) + ',' +
        String( Values.raw_gyro[ 2 ] / ( MULT * 1.0f ), 2 ) + '\n'

        );

    output.append(

        "Normalized Gyro Range ( X, Y, Z ): " +
        String( Values.normalized_gyro[ 0 ] / ( MULT * 1.0f ), 2 ) + ',' +
        String( Values.normalized_gyro[ 1 ] / ( MULT * 1.0f ), 2 ) + ',' +
        String( Values.normalized_gyro[ 2 ] / ( MULT * 1.0f ), 2 ) + '\n'

        );

    output.append( "\n\nNow reading BMP390...\n" );
    output.append( "Tempurature ( C ): " + String( Values.temperature / ( MULT * 1.0f ) ) + '\n' );
    output.append( "Pressure ( kPa ): " + String( Values.pressure / ( MULT * 1.0f ) ) + '\n' );
    output.append( "Altitude ( m ): " + String( Values.altitude / ( MULT * 1.0f ) ) + '\n' );

    Serial.println( output ); // Print output to screen

    Serial.end(); // End Serial Transmission

}

void Write_All_Values_To_SD( INTData& Values ) { // Records values to Sd card

    String file_string = String( month() + '-' + day() + '-' + year() ) + ".csv";
    File myFile = SD.open( file_string.c_str(), FILE_WRITE );

    //* Time ( seconds ),
    //* Raw Ax ( g ),Raw Ay ( g ),Raw Az ( g ),
    //* Ax ( g ),Ay ( g ),Az ( g ),
    //* Raw Gx ( deg/s ),Raw Gy ( deg/s ),Raw Gz ( deg/s ),
    //* Gx ( deg/s ),Gy ( deg/s ),Gz ( deg/s ),
    //* Temperature ( *C ),Pressure ( kpA ),Altitude ( m ),
    //* Message

    String output = "";

    output += ( Values.time + ',' );

    int i;

    for ( i = 0; i < 3; i++ ) output += ( String( Values.raw_accel[ i ] / ( MULT * 1.0f ) ) + ',' );

    for ( i = 0; i < 3; i++ ) output += ( String( Values.normalized_accel[ i ] / ( MULT * 1.0f ) ) + ',' );

    for ( i = 0; i < 3; i++ ) output += ( String( Values.raw_gyro[ i ] / ( MULT * 1.0f ) ) + ',' );

    for ( i = 0; i < 3; i++ ) output += ( String( Values.normalized_gyro[ i ] / ( MULT * 1.0f ) ) + ',' );

    output += ( String( Values.temperature / ( MULT * 1.0f ) ) + ',' );

    output += ( String( Values.pressure / ( MULT * 1.0f ) ) + ',' );

    output += ( String( Values.altitude / ( MULT * 1.0f ) ) + ',' );

    output += ( Values.message );

    myFile.println( output );

    myFile.close();

}

void Record_Data( INTData& Values ) { // Prints data to screen and saves it to file

    Print_All_Values( Values );
    Write_All_Values_To_SD( Values );

}

// -----------------------Parachute Functions---------------------------------------

void Deploy_Parachute( int schute ) { // Arms Parachute

   Paras_Armed[ schute ] = 1;

}

Result Launch_Parachute( int schute ) { // Launches Parachute

    switch ( schute ) {

        case 0: // Main Schute

            if ( Paras_Armed[ schute ] ) {

                digitalWrite( PinMain, HIGH );
                return { 0, "!!MAIN SCHUTE LAUNCHED!!" };

                }

            else return { 1, "MAIN Schute Not Deployed!" };

        case 1: // Drouge Schute

            if ( Paras_Armed[ schute ] ) {

                digitalWrite( PinDrouge, HIGH );
                return { 0, "!!DROUGE SCHUTE LAUNCHED" };

            }

            else return { 2, "DROUGE Schute Not Deployed!" };

        default:
            return { -1, "Not a Valid Schute!" };

    }

}

// -----------------------Internal Trigger Functions--------------------------------

Result Check_Altitude( int altitude, int prev_altitude = -1 ) { // Checks if altitude is safe/at apogee

    if ( altitude > SafeAltitude * MULT )

        if ( altitude <= prev_altitude ) return { 2, "!!AT APOGEE!!" }; // Around Apogee

        else return { 0, "Safe Altitude" }; // Safe

    return { -1, "unsafe altitude" }; // Unsafe

}

Result Check_Pressure_Delta( int pressure, int prev_pressure ) { // Checks pressure delta

    int H = prev_pressure * ( 1 + PTolerance ); // Upperbound
    int L = prev_pressure * ( 1 - PTolerance ); // Lowerbound

    if ( pressure <= H && pressure >= L ) return { 0, "Constant Pressure" };

    if ( pressure < L ) return { 1, "Pressure Decreasing" };

    if ( pressure > H ) return { 2, "Pressure Increasing" };

}

Result Check_Tilt( int* gyro, int* prev_gyro ) { // Checks if tilt is safe
    
    int H[ 3 ] = { // Upperbounds (X,Y,Z)

        prev_gyro[ 0 ] * ( 1 + TTolerance ),
        prev_gyro[ 1 ] * ( 1 + TTolerance ),
        prev_gyro[ 2 ] * ( 1 + TTolerance )

        };

    int L[ 3 ] = { // Lowerbounds (X,Y,Z)

        prev_gyro[ 0 ] * ( 1 - TTolerance ),
        prev_gyro[ 1 ] * ( 1 - TTolerance ),
        prev_gyro[ 2 ] * ( 1 - TTolerance )

        };

    if ( gyro[ 0 ] > SafeTiltX * MULT ) return { -1, "!!DANGEROUS X-AXIS TILT!!" };

    if ( gyro[ 1 ] > SafeTiltY * MULT ) return { -2, "!!DANGEROUS Y-AXIS TILT!!" };

    if ( gyro[ 2 ] > SafeTiltZ * MULT ) return { -3, "!!DANGEROUS Z-AXIS TILT!!" };

    if ( gyro[ 0 ] < L[ 0 ] || gyro[ 0 ] > H[ 0 ] ) return { 1, "!!X-AXIS TILT DETECTED!!" };

    if ( gyro[ 1 ] < L[ 1 ] || gyro[ 1 ] > H[ 1 ] ) return { 2, "!!Y-AXIS TILT DETECTED!!" };

    if ( gyro[ 2 ] < L[ 2 ] || gyro[ 2 ] > H[ 2 ] ) return { 3, "!!Z-AXIS TILT DETECTED!!" };

    return { 0, "safe tilt"};

}

Result Check_Accel( int* accel, int* prev_accel, bool surface ) { // Checks if accel is correct
     
    if ( surface ) {

        int Hs[ 3 ] = { // Surface Upperbounds (X,Y,Z)

            ( SurfaceAccelX * ( 1 + ATolerance ) ) == 0 ? ATolerance : ( SurfaceAccelX * ( 1 + ATolerance ) ),
            ( SurfaceAccelY * ( 1 + ATolerance ) ) == 0 ? ATolerance : ( SurfaceAccelY * ( 1 + ATolerance ) ),
            ( SurfaceAccelZ * ( 1 + ATolerance ) ) == 0 ? ATolerance : ( SurfaceAccelZ * ( 1 + ATolerance ) )

        };

        int Ls[ 3 ] = {

            ( SurfaceAccelX * ( 1 - ATolerance ) ) == 0 ? -1 * ATolerance : ( SurfaceAccelX * ( 1 - ATolerance ) ),
            ( SurfaceAccelY * ( 1 - ATolerance ) ) == 0 ? -1 * ATolerance : ( SurfaceAccelY * ( 1 - ATolerance ) ),
            ( SurfaceAccelZ * ( 1 - ATolerance ) ) == 0 ? -1 * ATolerance : ( SurfaceAccelZ * ( 1 - ATolerance ) )

        };

        if ( accel[ 0 ] < Ls[ 0 ] || accel[ 0 ] > Hs[ 0 ] ) return { -10, "!!DANGEROUS X-AXIS SURFACE ACCELERATION!!" };

        if ( accel[ 1 ] < Ls[ 1 ] || accel[ 1 ] > Hs[ 1 ] ) return { -20, "!!DANGEROUS Y-AXIS SURFACE ACCELERATION" };

        if ( accel[ 2 ] < Ls[ 2 ] || accel[ 2 ] > Hs[ 2 ] ) return { -30, "!!DANGEROUS Z-AXIS SURFACE ACCELERATION" };

    }

    else {
        
        int H[ 3 ] = { // Upperbounds (X,Y,Z)

            prev_accel[ 0 ] * ( 1 + ATolerance ),
            prev_accel[ 1 ] * ( 1 + ATolerance ),
            prev_accel[ 2 ] * ( 1 + ATolerance )

        };

        int L[ 3 ] = { // Lowerbounds (X,Y,Z)

            prev_accel[ 0 ] * ( 1 - ATolerance ),
            prev_accel[ 1 ] * ( 1 - ATolerance ),
            prev_accel[ 2 ] * ( 1 - ATolerance )

        };

        if ( accel[ 0 ] > SafeAccelX * MULT ) return { -1, "!!DANGEROUS X-AXIS ACCELERATION!!" };

        if ( accel[ 1 ] > SafeAccelY * MULT ) return { -2, "!!DANGEROUS Y-AXIS ACCEL!!" };

        if ( accel[ 2 ] > SafeAccelX * MULT ) return { -3, "!!DANGEROUS Z-AXIS ACCEL!!" };

        if ( accel[ 0 ] < L[ 0 ] || accel[ 0 ] > H[ 0 ] ) return { 1, "EXTRA X-AXIS ACCEL DETECTED" };

        if ( accel[ 1 ] < L[ 1 ] || accel[ 1 ] > H[ 1 ] ) return { 2, "EXTRA Y-AXIS ACCEL DETECTED" };

        if ( accel[ 2 ] < L[ 2 ] || accel[ 2 ] > H[ 2 ] ) return { 2, "EXTRA Z-AXIS ACCEL DETECTED" };

    }

    return { 0, "safe accel" }; // Safe

}

// -----------------------External Trigger Functions--------------------------------

int Check_Systems( INTData& Values ) { // Checks if systems are safe

    //TODO: Implement

    //* Will trigger LED based on error code

    return 0; // Safe

}
