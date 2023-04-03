#define SampleAmount 3
#define MeasurementAmount 20
#define SampleTimeSpan 1 // In Seconds

#define SampleAltTolerance 0.05f // % in decimal form
#define SamplePressureTolerance 0.05f // % in decimal form
#define SampleAccelTolerance 0.05f // % in decimal form
#define SampleTiltTolerance 0.05f // % in decimal form
#define SampleTemperatureTolerance 0.05f // % in decimal form

#define AltEqualityWeight 5
#define PressureEqualityWeight 2
#define TemperatureEqualityWeight 0
#define RawAccelEqualityWeight 0
#define NormalizedAccelEqualityWeight 1
#define RawGyroEqualityWeight 0
#define NormalizedGyroEqualityWeight 1

#define EqualitySumMin -2 // What the weights should sum to, to be considered equal
#define EqualitySumMax 2 // What the weights should sum under, to be considered equal
