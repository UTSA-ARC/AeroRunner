#include "params/sample_params.h"
#include "globals.h"
#include "functions.h"

class Sample {

    private:

        typedef struct SampleData : Data { String timeEnd; } SampleData;

        Data measurements[MeasurementAmount];
        SampleData avg_data;

        void Set_Measurements( Data* new_measurements ) { *measurements = *new_measurements; }

        void Find_Avg() {

            avg_data.message = "";
            avg_data.time = measurements[0].time;
            avg_data.timeEnd = measurements[MeasurementAmount -  1].time;

            for ( int i = 0; i < MeasurementAmount; i++ ) {

                avg_data.raw_accel[0] += ( measurements[i].raw_accel[0] * ( 1.0f / MeasurementAmount ) );
                avg_data.raw_accel[1] += ( measurements[i].raw_accel[1] * ( 1.0f / MeasurementAmount ) );
                avg_data.raw_accel[2] += ( measurements[i].raw_accel[2] * ( 1.0f / MeasurementAmount ) );

                avg_data.raw_gyro[0] += ( measurements[i].raw_gyro[0] * ( 1.0f / MeasurementAmount ) );
                avg_data.raw_gyro[1] += ( measurements[i].raw_gyro[1] * ( 1.0f / MeasurementAmount ) );
                avg_data.raw_gyro[2] += ( measurements[i].raw_gyro[2] * ( 1.0f / MeasurementAmount ) );

                avg_data.normalized_accel[0] += ( measurements[i].normalized_accel[0] * ( 1.0f / MeasurementAmount ) );
                avg_data.normalized_accel[1] += ( measurements[i].normalized_accel[1] * ( 1.0f / MeasurementAmount ) );
                avg_data.normalized_accel[2] += ( measurements[i].normalized_accel[2] * ( 1.0f / MeasurementAmount ) );

                avg_data.normalized_gyro[0] += ( measurements[i].normalized_gyro[0] * ( 1.0f / MeasurementAmount ) );
                avg_data.normalized_gyro[1] += ( measurements[i].normalized_gyro[1] * ( 1.0f / MeasurementAmount ) );
                avg_data.normalized_gyro[2] += ( measurements[i].normalized_gyro[2] * ( 1.0f / MeasurementAmount ) );

                avg_data.altitude += ( measurements[i].altitude * ( 1.0f / MeasurementAmount ) );

                avg_data.pressure += ( measurements[i].pressure * ( 1.0f / MeasurementAmount ) );

                avg_data.temperature += ( measurements[i].temperature * ( 1.0f / MeasurementAmount ) );

                avg_data.message += measurements[i].message;

            }

        }

    public:

        Sample() {

            for ( int i = 0; i < MeasurementAmount; i++ ) {

                measurements[i] = Get_All_Values();
                delay( ( SampleTimeSpan * 1000 ) / MeasurementAmount );

            }

            Find_Avg();

        }

        Sample( Data* new_measurements ) {

            Set_Measurements( new_measurements );

            Find_Avg();

        }

        Data* Get_Measurements() { return measurements; }

        SampleData Get_Avg_Data() { return avg_data; }

};

class SampleCollection {

    private:
        Sample* Samples;
        int size;

        Result Compare_Altitude( float altitude_a, float altitude_b ) {

            float H = altitude_a * ( 1 + SampleAltTolerance );
            float L = altitude_a * ( 1 - SampleAltTolerance );

            if ( altitude_b > H ) return { 1, "Altitude A Is Less Than Altitude B" };

            if ( altitude_b < L ) return { 2, "Altitude A Is Greater Than Altitude B" };

            return { 0, "Altitude A Is Equal to Altitude B" };

        }

        Result Compare_Pressure( float pressure_a, float pressure_b ) {

            float H = pressure_a * ( 1 + SamplePressureTolerance );
            float L = pressure_a * ( 1 - SamplePressureTolerance );

            if ( pressure_b > H ) return { -1, "Pressure A is Less than Pressure B" };

            if ( pressure_b < L ) return { 1, "Pressure A is Greater than Pressure B" };

            return { 0, "Pressure A is Equal to Pressure B" };

        }

        Result Compare_Temperature( float temperature_a, float temperature_b ) {

            float H = temperature_a * ( 1 + SampleTemperatureTolerance );
            float L = temperature_b * ( 1 - SampleTemperatureTolerance );

            if ( temperature_b > H ) return { -1, "Temperature A is Less than Temperature B" };

            if ( temperature_b < L ) return { 1, "Temperature A is Greater than Temperature B" };

            return { 0, "Temperature A is Equal to Temperature B" };

        }

        Result Compare_Raw_Accel( float* raw_accel_a, float* raw_accel_b ) {

            float H[ 3 ] = {

                abs( raw_accel_a[0] * ( 1 + SampleAccelTolerance ) ),
                abs( raw_accel_a[1] * ( 1 + SampleAccelTolerance ) ),
                abs( raw_accel_a[2] * ( 1 + SampleAccelTolerance ) )

            };

            float L[ 3 ] = {

                abs( raw_accel_a[0] * ( 1 - SampleAccelTolerance ) ),
                abs( raw_accel_a[1] * ( 1 - SampleAccelTolerance ) ),
                abs( raw_accel_a[2] * ( 1 - SampleAccelTolerance ) )

            };

            bool X, Y, Z;

            if ( abs( raw_accel_b[0] ) > L[0] && abs( raw_accel_b[0] ) < H[0] ) X = true;

            if ( abs( raw_accel_b[1] ) > L[1] && abs( raw_accel_b[1] ) < H[1] ) Y = true;

            if ( abs( raw_accel_b[2] ) > L[2] && abs( raw_accel_b[2] ) < H[2] ) Z = true;

            if ( !X && Y && Z ) return { 1, "Y and Z raw Accel Axis are Equal" };
            if ( X && !Y && Z ) return { 2, "X and Z raw Accel Axis are Equal" };
            if ( X && Y && !Z ) return { 3, "X and Y raw Accel Axis are Equal" };
            if ( X && !Y && !Z ) return { 4, "Only X raw Accel Axis is Equal" };
            if ( !X && Y && !Z ) return { 5, "Only Y raw Accel Axis is Equal" };
            if ( !X && !Y && Z ) return { 6, "Only Z raw Accel Axis is Equal" };
            if ( !X && !Y && !Z ) return { -1, "No raw Accel Axis are Equal" };
            
            return { 0, "All raw Accel Axis are Equal " };

        }

        Result Compare_Normalized_Accel( float* accel_a, float* accel_b ) {

            float H[ 3 ] = {

                abs( accel_a[0] * ( 1 + SampleAccelTolerance ) ),
                abs( accel_a[1] * ( 1 + SampleAccelTolerance ) ),
                abs( accel_a[2] * ( 1 + SampleAccelTolerance ) )

            };

            float L[ 3 ] = {

                abs( accel_a[0] * ( 1 - SampleAccelTolerance ) ),
                abs( accel_a[1] * ( 1 - SampleAccelTolerance ) ),
                abs( accel_a[2] * ( 1 - SampleAccelTolerance ) )

            };

            bool X, Y, Z;

            if ( abs( accel_b[0] ) > L[0] && abs( accel_b[0] ) < H[0] ) X = true;

            if ( abs( accel_b[1] ) > L[1] && abs( accel_b[1] ) < H[1] ) Y = true;

            if ( abs( accel_b[2] ) > L[2] && abs( accel_b[2] ) < H[2] ) Z = true;

            if ( !X && Y && Z ) return { 1, "Y and Z Accel Axis are Equal" };
            if ( X && !Y && Z ) return { 2, "X and Z Accel Axis are Equal" };
            if ( X && Y && !Z ) return { 3, "X and Y Accel Axis are Equal" };
            if ( X && !Y && !Z ) return { 4, "Only X Accel Axis is Equal" };
            if ( !X && Y && !Z ) return { 5, "Only Y Accel Axis is Equal" };
            if ( !X && !Y && Z ) return { 6, "Only Z Accel Axis is Equal" };
            if ( !X && !Y && !Z ) return { -1, "No Accel Axis are Equal" };
            
            return { 0, "All Accel Axis are Equal " };

        }

        Result Compare_Raw_Tilt( float* raw_gyro_a, float* raw_gyro_b ) {

            float H[ 3 ] = {

                ( raw_gyro_a[0] * ( 1 + SampleTiltTolerance ) ),
                ( raw_gyro_a[1] * ( 1 + SampleTiltTolerance ) ),
                ( raw_gyro_a[2] * ( 1 + SampleTiltTolerance ) )

            };

            float L[ 3 ] = {

                ( raw_gyro_a[0] * ( 1 - SampleTiltTolerance ) ),
                ( raw_gyro_a[1] * ( 1 - SampleTiltTolerance ) ),
                ( raw_gyro_a[2] * ( 1 - SampleTiltTolerance ) )

            };

            bool X, Y, Z;

            if ( raw_gyro_b[0] > L[0] && raw_gyro_b[0] < H[0] ) X = true;

            if ( raw_gyro_b[1] > L[1] && raw_gyro_b[1] < H[1] ) Y = true;

            if ( raw_gyro_b[2] > L[2] && raw_gyro_b[2] < H[2] ) Z = true;

            if ( !X && Y && Z ) return { 1, "Y and Z Raw Gyro Axis are Equal" };
            if ( X && !Y && Z ) return { 2, "X and Z Raw Gyro Axis are Equal" };
            if ( X && Y && !Z ) return { 3, "X and Y Raw Gyro Axis are Equal" };
            if ( X && !Y && !Z ) return { 4, "Only X Raw Gyro Axis is Equal" };
            if ( !X && Y && !Z ) return { 5, "Only Y Raw Gyro Axis is Equal" };
            if ( !X && !Y && Z ) return { 6, "Only Z Raw Gyro Axis is Equal" };
            if ( !X && !Y && !Z ) return { -1, "No Raw Gyro Axis are Equal" };
            
            return { 0, "All Raw Gyro Axis are Equal " };

        }

        Result Compare_Normalized_Tilt( float* gyro_a, float* gyro_b ) {

            float H[ 3 ] = {

                ( gyro_a[0] * ( 1 + SampleTiltTolerance ) ),
                ( gyro_a[1] * ( 1 + SampleTiltTolerance ) ),
                ( gyro_a[2] * ( 1 + SampleTiltTolerance ) )

            };

            float L[ 3 ] = {

                ( gyro_a[0] * ( 1 - SampleTiltTolerance ) ),
                ( gyro_a[1] * ( 1 - SampleTiltTolerance ) ),
                ( gyro_a[2] * ( 1 - SampleTiltTolerance ) )

            };

            bool X, Y, Z;

            if ( gyro_b[0] > L[0] && gyro_b[0] < H[0] ) X = true;

            if ( gyro_b[1] > L[1] && gyro_b[1] < H[1] ) Y = true;

            if ( gyro_b[2] > L[2] && gyro_b[2] < H[2] ) Z = true;

            if ( !X && Y && Z ) return { 1, "Y and Z Gyro Axis are Equal" };
            if ( X && !Y && Z ) return { 2, "X and Z Gyro Axis are Equal" };
            if ( X && Y && !Z ) return { 3, "X and Y Gyro Axis are Equal" };
            if ( X && !Y && !Z ) return { 4, "Only X Gyro Axis is Equal" };
            if ( !X && Y && !Z ) return { 5, "Only Y Gyro Axis is Equal" };
            if ( !X && !Y && Z ) return { 6, "Only Z Gyro Axis is Equal" };
            if ( !X && !Y && !Z ) return { -1, "No Gyro Axis are Equal" };
            
            return { 0, "All Gyro Axis are Equal " };

        }

    public:

        SampleCollection() {

            Samples = new Sample[ SampleAmount ];
            size = SampleAmount;

        }

        SampleCollection( int sample_amount ) {

            Samples = new Sample[ sample_amount ];
            size = sample_amount;

        }

        Sample* Get_Sample_Array() { return Samples; }

        Sample Get_Sample( int index ) { return Samples[ index ]; }

        Sample* Record_Samples() { 

            static Sample samples[ SampleAmount ];

            for ( int i = 0; i < SampleAmount; i++ ) samples[i] = Sample();

            return samples;

        }

        Result Compare_Sample( Sample* sample_a, Sample* sample_b ) { // Return 0 if equal, 1 if greater, -1 if lesser



        }

};

