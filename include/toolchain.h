#ifdef NATIVE_TC

    #include <ArduinoFake.h>
    #include <WireFake.h>
    #include <EEPROMFake.h>
    #include <SPIFake.h>
    #include <iostream>

    #ifndef A9

        #define PIN_A9 (23)
        static const uint8_t A9 = PIN_A9;

    #endif

#endif

#ifdef ARDUINO_TC

    #include <Wire.h>
    #include <Arduino.h>
    #include <Adafruit_BMP3XX.h>
    #include <TimeLib.h>
    #include <SD.h>

    Adafruit_BMP3XX bmp;
    bool Paras_Armed[2];
    uint16_t ALSB_Sensitivity, GLSB_Sensitivity;


    #ifdef TEENSY41_TC

        #ifndef BUILTIN_SDCARD

            #define BUILTIN_SDCARD 254
            
        #endif

    #endif

#endif

