#include "params/sample_params.h"
#include "globals.h"
#include "functions.h"

class Sample { // An Average of Measurements

    private:

        SampleData avg_data; // Avg of measurements

        SampleData filtered_data; // Exponential smoothing of measurements
        int measurementCount; // Number of measurements taken in this sample
        
        void updateAverages( const Data* measurement ) {
     
            avg_data.raw_accel[0] = ( avg_data.raw_accel[0]*(measurementCount- 1) + measurement->raw_accel[0] ) / measurementCount;
            avg_data.raw_accel[1] = ( avg_data.raw_accel[1]*(measurementCount- 1) + measurement->raw_accel[1] ) / measurementCount;
            avg_data.raw_accel[2] = ( avg_data.raw_accel[2]*(measurementCount- 1) + measurement->raw_accel[2] ) / measurementCount;

            avg_data.raw_gyro[0] = (  avg_data.raw_gyro[0]*(measurementCount- 1) + measurement->raw_gyro[0] ) / measurementCount;
            avg_data.raw_gyro[1] = (  avg_data.raw_gyro[1]*(measurementCount- 1) + measurement->raw_gyro[1] ) / measurementCount;
            avg_data.raw_gyro[2] = (  avg_data.raw_gyro[2]*(measurementCount- 1) + measurement->raw_gyro[2] ) / measurementCount;

            avg_data.normalized_accel[0] = (  avg_data.normalized_accel[0]*(measurementCount- 1) + measurement->normalized_accel[0] ) / measurementCount;
            avg_data.normalized_accel[1] = (  avg_data.normalized_accel[1]*(measurementCount- 1) + measurement->normalized_accel[1] ) / measurementCount;
            avg_data.normalized_accel[2] = (  avg_data.normalized_accel[2]*(measurementCount- 1) + measurement->normalized_accel[2] ) / measurementCount;

            avg_data.normalized_gyro[0] = (  avg_data.normalized_gyro[0]*(measurementCount- 1) + measurement->normalized_gyro[0] ) / measurementCount;
            avg_data.normalized_gyro[1] = (  avg_data.normalized_gyro[1]*(measurementCount- 1) + measurement->normalized_gyro[1] ) / measurementCount;
            avg_data.normalized_gyro[2] = (  avg_data.normalized_gyro[2]*(measurementCount- 1) + measurement->normalized_gyro[2] ) / measurementCount;

            avg_data.altitude = ( avg_data.altitude*(measurementCount- 1) + measurement->altitude ) / measurementCount;

            avg_data.pressure = ( avg_data.pressure*(measurementCount- 1) + measurement->pressure ) / measurementCount;

            avg_data.temperature = ( avg_data.temperature*(measurementCount- 1) + measurement->temperature ) / measurementCount;

            avg_data.message += measurement->message;
        }

        void updateFilteredData( const Data* measurement, const float_t filter_weight = Filter_Weight) { // updates data through an exponential smoothing filter // TODO: UNIT TEST

            filtered_data.raw_accel[0] = filter_weight * measurement->raw_accel[0] + (1 - filter_weight) * filtered_data.raw_accel[0];
            filtered_data.raw_accel[1] = filter_weight * measurement->raw_accel[1] + (1 - filter_weight) * filtered_data.raw_accel[1];
            filtered_data.raw_accel[2] = filter_weight * measurement->raw_accel[2] + (1 - filter_weight) * filtered_data.raw_accel[2];

            filtered_data.raw_gyro[0] = filter_weight * measurement->raw_gyro[0] + (1 - filter_weight) * filtered_data.raw_gyro[0];
            filtered_data.raw_gyro[1] = filter_weight * measurement->raw_gyro[1] + (1 - filter_weight) * filtered_data.raw_gyro[1];
            filtered_data.raw_gyro[2] = filter_weight * measurement->raw_gyro[2] + (1 - filter_weight) * filtered_data.raw_gyro[2];

            filtered_data.normalized_accel[0] = filter_weight * measurement->normalized_accel[0] + (1 - filter_weight) * filtered_data.normalized_accel[0];
            filtered_data.normalized_accel[1] = filter_weight * measurement->normalized_accel[1] + (1 - filter_weight) * filtered_data.normalized_accel[1];
            filtered_data.normalized_accel[2] = filter_weight * measurement->normalized_accel[2] + (1 - filter_weight) * filtered_data.normalized_accel[2];

            filtered_data.normalized_gyro[0] = filter_weight * measurement->normalized_gyro[0] + (1 - filter_weight) * filtered_data.normalized_gyro[0];
            filtered_data.normalized_gyro[1] = filter_weight * measurement->normalized_gyro[1] + (1 - filter_weight) * filtered_data.normalized_gyro[1];
            filtered_data.normalized_gyro[2] = filter_weight * measurement->normalized_gyro[2] + (1 - filter_weight) * filtered_data.normalized_gyro[2];

            filtered_data.altitude = filter_weight * measurement->altitude + (1 - filter_weight) * filtered_data.altitude;

            filtered_data.pressure = filter_weight * measurement->pressure + (1 - filter_weight) * filtered_data.pressure;

            filtered_data.temperature = filter_weight * measurement->temperature + (1 - filter_weight) * filtered_data.temperature;

            filtered_data.message += measurement->message;

        }

    public:

        Sample() { // Initialize Sample object
            
            elapsedMillis sampleTimer; // Start sample timer

            measurementCount = 0;
            elapsedMillis measurementTimer; // Instanciate measurement time, keeps track of time passed since declaration (in milliseconds)
           
            while ( sampleTimer < SampleTimeSpan ) {

                measurementTimer = 0; // Reset measurement timer
                
                Data measurement = Get_All_Values(); // Get measurements
                measurementCount++; // Update number of measurements collected
                updateAverages( &measurement ); // Update averages

                while ( !MaxSampleRate && measurementTimer < SampleDelay ); // Loop until the time period allocated for one measurement has passed
            }
        }

        Sample( const SampleData &avg_data, const uint8_t measurement_amount ) {

            this->avg_data = avg_data;
            this->measurementCount = measurement_amount;

        }

        
        SampleData Get_Avg_Data() { return avg_data; } // Get Avg_data

        SampleData Get_Filtered_Data() { return filtered_data; } // Get filtered data

        void Append_Message( const String message ) { avg_data.message += message;  } // Append to message string of Sample object

};


class SampleCollection { // A collection of sample objects
    

    private:
        Sample* Samples; // Array of Samples
        int size; // Size of array
        typedef struct ResultPDiff : Result { 
            float_t pDiff; 
        } ResultPDiff;
        

        ResultPDiff Compare_Altitude( const float_t altitude_a, const float_t altitude_b ) { // Compare Altitudes of 2 Samples

            ResultPDiff result;
            
            if ( altitude_a == 0.0 ){
                result.error = -2;
                result.message = "Altitude A is Zero";
                result.pDiff = 0; 
            }

            float_t diff = ( altitude_b - altitude_a )/altitude_a;
            result.pDiff = diff;

            if ( diff < 0 && fabs(diff) > SampleAltTolerance ){
                result.error = -1;
                result.message = "Altitude A Is Greater Than Altitude B";
            } else if ( diff > 0 && fabs(diff) > SampleAltTolerance ){
                result.error = 1;
                result.message = "Altitude A Is Less Than Altitude B";
            } else {
                result.error = 0;
                result.message = "Altitude A Is Equal to Altitude B";
            }    

            return result;

        }

        ResultPDiff Compare_Pressure( const float_t pressure_a, const float_t pressure_b ) { // Compare Pressures of 2 Samples

            ResultPDiff result;
            
            if ( pressure_a == 0.0 ) {
                result.error = -2;
                result.message = "Pressure A is Zero";
                result.pDiff = 0; 
            }

            float_t diff = (pressure_b - pressure_a)/pressure_a;
            result.pDiff = diff;

            if ( diff < 0 && fabs(diff) > SamplePressureTolerance ){
                result.error = -1;
                result.message = "Pressure A Is Greater Than Pressure B";
            } else if ( diff > 0 && fabs(diff) > SamplePressureTolerance ){
                result.error = 1;
                result.message = "Pressure A Is Less Than Pressure B";
            } else {
                result.error = 0;
                result.message = "Pressure A Is Equal to Pressure B";
            }    

            return result;

        }

        ResultPDiff Compare_Temperature( const float_t temperature_a, const float_t temperature_b ) { // Compare Temperatures of 2 Samples

            ResultPDiff result;
            
            if ( temperature_a == 0.0 ) {
                result.error = -2;
                result.message = "Temperature A is Zero";
                result.pDiff = 0; 
            }

            float_t diff = ( temperature_b - temperature_a )/temperature_a;
            result.pDiff = diff;

            if ( diff < 0 && fabs(diff) > SampleTemperatureTolerance ){
                result.error = -1;
                result.message = "Temperature A Is Greater Than Temperature B";
            } else if ( diff > 0 && fabs(diff) > SampleTemperatureTolerance ){
                result.error = 1;
                result.message = "Temperature A Is Less Than Temperature B";
            } else {
                result.error = 0;
                result.message = "Temperature A Is Equal to Temperature B";
            }    

            return result;

        }

        ResultPDiff Compare_Raw_Accel( const int16_t* raw_accel_a, const int16_t* raw_accel_b ) { // Compare the Raw Acceleration of 2 Samples
            
            ResultPDiff result;
            result.error = 0;
            result.message = "";

            float_t diff[3];
            for ( int i = 0; i < 3; i++ ){
                if ( raw_accel_a[i] == 0.0 ){
                    diff[i] = 0; // Do something
                    break;
                }
                diff[i] = ( raw_accel_b[i] - raw_accel_a[i] )/raw_accel_a[i];
            }
            result.pDiff = diff[1]; // The percent change of the y axis
         
            char axis[3] = {'X', 'Y', 'Z'};

            for ( int i = 0; i < 3; i++ ){
                if ( diff[i] < 0 && fabs(diff[i]) > SampleAccelTolerance ){
                    if ( axis[i] == 'Y' ){ result.error = -1; }
                    result.message = axis[i] + " Accel is Decreasing, ";
                } else if ( diff[i] > 0 && fabs(diff[i]) > SampleAccelTolerance ){
                    if ( axis[i] == 'Y' ){ result.error = 1; }
                    result.message = axis[i] + " Accel is Increasing, ";
                } else {
                    if ( axis[i] == 'Y' ){ result.error = 0; }
                    result.message = axis[i] + " Accel is Equal, ";
                }  
            }
    
            return result;
           

        }

        ResultPDiff Compare_Normalized_Accel( const float_t* accel_a, const float_t* accel_b ) { // Compare the Normalized Acceleration of 2 Samples

            ResultPDiff result;
            result.error = 0;
            result.message = "";

            float_t diff[3];
            for ( int i = 0; i < 3; i++ ){
                if ( accel_a[i] == 0.0 ){
                    diff[i] = 0; // Do something
                    break;
                }
                diff[i] = ( accel_b[i] - accel_a[i] )/accel_a[i];
            }
            result.pDiff = diff[1]; // The percent change of the y axis
            
            char axis[3] = {'X', 'Y', 'Z'};

            for ( int i = 0; i < 3; i++ ){
                if ( diff[i] < 0 && fabs(diff[i]) > SampleAccelTolerance ){
                    if ( axis[i] == 'Y' ){ result.error = -1; }
                    result.message = axis[i] + " Accel is Decreasing, ";
                } else if ( diff[i] > 0 && fabs(diff[i]) > SampleAccelTolerance ){
                    if ( axis[i] == 'Y' ){ result.error = 1; }
                    result.message = axis[i] + " Accel is Increasing, ";
                } else {
                    if ( axis[i] == 'Y' ){ result.error = 0; }
                    result.message = axis[i] + " Accel is Equal, ";
                }  
            }
    
            return result;

        }

        ResultPDiff Compare_Raw_Tilt( const int16_t* raw_gyro_a, const int16_t* raw_gyro_b ) { // Compare the Raw Tilt of 2 Samples

            ResultPDiff result;
            result.error = 0;
            result.message = "";

            float_t diff[3];
            for ( int i = 0; i < 3; i++ ){
                if ( raw_gyro_a[i] == 0.0 ){
                    diff[i] = 0; // Do something
                    break;
                }
                diff[i] = ( raw_gyro_b[i] - raw_gyro_a[i] )/raw_gyro_a[i];
            }
            result.pDiff = diff[1]; // The percent change of the y axis
    
            char axis[3] = {'X', 'Y', 'Z'};

            for ( int i = 0; i < 3; i++ ){
                if ( diff[i] < 0 && fabs(diff[i]) > SampleAccelTolerance ){
                    if ( axis[i] == 'Y' ){ result.error = -1; }
                    result.message = axis[i] + " Accel is Decreasing, ";
                } else if ( diff[i] > 0 && fabs(diff[i]) > SampleAccelTolerance ){
                    if ( axis[i] == 'Y' ){ result.error = 1; }
                    result.message = axis[i] + " Accel is Increasing, ";
                } else {
                    if ( axis[i] == 'Y' ){ result.error = 0; }
                    result.message = axis[i] + " Accel is Equal, ";
                }  
            }

            return result;

        }

        ResultPDiff Compare_Normalized_Tilt( const float_t* gyro_a, const float_t* gyro_b ) { // Compare the Normalized Tilt of 2 Samples

            ResultPDiff result;
            result.error = 0;
            result.message = "";

            float_t diff[3];
            for ( int i = 0; i < 3; i++ ){
                if ( gyro_a[i] == 0.0 ){
                    diff[i] = 0; // Do something
                    break;
                }
                diff[i] = ( gyro_b[i] - gyro_a[i] )/gyro_a[i];
            }
            result.pDiff = diff[1]; // The percent change of the y axis
    
            char axis[3] = {'X', 'Y', 'Z'};

            for ( int i = 0; i < 3; i++ ){
                if ( diff[i] < 0 && fabs(diff[i]) > SampleAccelTolerance ){
                    if ( axis[i] == 'Y' ){ result.error = -1; }
                    result.message = axis[i] + " Accel is Decreasing, ";
                } else if ( diff[i] > 0 && fabs(diff[i]) > SampleAccelTolerance ){
                    if ( axis[i] == 'Y' ){ result.error = 1; }
                    result.message = axis[i] + " Accel is Increasing, ";
                } else {
                    if ( axis[i] == 'Y' ){ result.error = 0; }
                    result.message = axis[i] + " Accel is Equal, ";
                }  
            }

            return result;

        }

    public:

        SampleCollection() { // Initialize the Collection object

            Samples = new Sample[ SampleAmount ];
            size = SampleAmount;

        }

        SampleCollection( const int sample_amount ) { // Initialize the Collection object with a given amount

            Samples = new Sample[ sample_amount ];
            size = sample_amount;

        }

        SampleCollection( const SampleCollection &sc ) { // Copy Constructor

            Samples = new Sample[ SampleAmount ];
            *Samples = *sc.Samples;
            size = sc.size;

        }

        Sample* Get_Sample_Array() { return Samples; } // Get Sample Array

        int Size() { return size; } // Get the size of the Sample Array

        Sample Get_Sample( const int index ) { return Samples[ index ]; } // Get a specified Sample

        Result Record_Samples() { // Initialize Samples

            for ( int i = 0; i < size; i++ ) Samples[i] = Sample();

            return { 0, "Recorded Samples" };

        }

        Result Compare_Sample( const int sample_a_index, const int sample_b_index ) { // Compare 2 samples //* Return 0 if equal, 1 if greater, -1 if lesser // TODO: UNIT TEST

            int eq = 0;

            ResultPDiff C_Alt = Compare_Altitude( Samples[sample_a_index].Get_Avg_Data().altitude, Samples[sample_b_index].Get_Avg_Data().altitude );

            ResultPDiff C_Pres = Compare_Pressure( Samples[sample_a_index].Get_Avg_Data().pressure, Samples[sample_b_index].Get_Avg_Data().pressure );

            ResultPDiff C_Temp = Compare_Temperature( Samples[sample_a_index].Get_Avg_Data().temperature, Samples[sample_b_index].Get_Avg_Data().temperature );

            ResultPDiff C_R_Accel = Compare_Raw_Accel( Samples[sample_a_index].Get_Avg_Data().raw_accel, Samples[sample_b_index].Get_Avg_Data().raw_accel );

            ResultPDiff C_N_Accel = Compare_Normalized_Accel( Samples[sample_a_index].Get_Avg_Data().normalized_accel, Samples[sample_b_index].Get_Avg_Data().normalized_accel );

            ResultPDiff C_R_Tilt = Compare_Raw_Tilt( Samples[sample_a_index].Get_Avg_Data().raw_gyro, Samples[sample_b_index].Get_Avg_Data().raw_gyro );

            ResultPDiff C_N_Tilt = Compare_Normalized_Tilt( Samples[sample_a_index].Get_Avg_Data().normalized_gyro, Samples[sample_b_index].Get_Avg_Data().normalized_gyro );

            String msg = C_Alt.message + C_Pres.message + C_Temp.message + C_R_Accel.message + C_N_Accel.message + C_R_Tilt.message + C_N_Tilt.message;

            eq += C_Alt.pDiff * AltEqualityWeight;
            eq += C_Pres.pDiff * PressureEqualityWeight;
            eq += C_Temp.pDiff * TemperatureEqualityWeight;
            eq += C_R_Accel.pDiff * RawAccelEqualityWeight;
            eq += C_N_Accel.pDiff * NormalizedAccelEqualityWeight;
            eq += C_R_Tilt.pDiff * RawGyroEqualityWeight;
            eq += C_N_Tilt.pDiff * NormalizedGyroEqualityWeight;

            if ( eq > EqualitySumMax ) return { 1, msg };
            if ( eq < EqualitySumMin ) return { -1, msg };

            return { 0, msg };

        }

};
