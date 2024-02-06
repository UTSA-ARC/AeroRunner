#ifdef NATIVE_TC

    #include <ArduinoFake.h>
    #include <WireFake.h>
    #include <EEPROMFake.h>
    #include <SPIFake.h>

#endif

#ifdef ARDUINO_TC

    #include <Wire.h>
    #include <Arduino.h>
    #include <Adafruit_BMP3XX.h>
    #include <TimeLib.h>
    #include <SD.h>

    Adafruit_BMP3XX bmp;


    #ifdef TEENSY41_TC

        #ifndef BUILTIN_SDCARD

            #define BUILTIN_SDCARD 254
            
        #endif

    #endif

#endif

