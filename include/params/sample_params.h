#define SampleAmount 2 // Amount of samples to analyze at a time
#define MeasurementAmount 20 // Amount of measurements in 1 sample
#define SampleTimeSpan 1 // In Seconds
#define SampleDelay ( SampleTimeSpan * 1000 ) / MeasurementAmount // In Milliseconds
#define MaxSampleRate false

#define SampleAltTolerance 0.05f // % in decimal form
#define SamplePressureTolerance 0.05f // % in decimal form
#define SampleAccelTolerance 0.05f // % in decimal form
#define SampleTiltTolerance 0.05f // % in decimal form
#define SampleTemperatureTolerance 0.05f // % in decimal form

#define AltEqualityWeight 2
#define PressureEqualityWeight 1
#define TemperatureEqualityWeight 1
#define RawAccelEqualityWeight 0
#define NormalizedAccelEqualityWeight 0
#define RawGyroEqualityWeight 0
#define NormalizedGyroEqualityWeight 0

#define EqualitySumMax AltEqualityWeight*SampleAltTolerance + PressureEqualityWeight*SamplePressureTolerance + TemperatureEqualityWeight*SampleTemperatureTolerance + RawAccelEqualityWeight*SampleAccelTolerance + NormalizedAccelEqualityWeight*SampleAccelTolerance + RawGyroEqualityWeight*SampleTiltTolerance + NormalizedGyroEqualityWeight*SampleTiltTolerance// What the weights should sum to, to be considered equal
#define EqualitySumMin -1*EqualitySumMax // What the weights should sum under, to be considered equal

#define Filter_Weight 0.2f // Smoothing factor of filter, lower weight means higher filtering, but decreased responsiveness