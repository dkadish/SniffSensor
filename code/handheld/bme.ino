/***************************************************************************
  This is a library for the BME680 gas, humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME680 Breakout
  ----> http://www.adafruit.com/products/3660

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme;  // I2C
//Adafruit_BME680 bme(BME_CS); // hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

#if USE_SD
File bmeFile;
#endif
bool bmeIsOpen = false;
// unsigned long endTime = 0;

//TODO: MAKE A BOOL TO CHECK IF BME IS LOADED.

void bme688Setup() {
  if (Serial) {
    Serial.println(F("BME688 async test"));
  }

  bmeIsOpen = bme.begin(0x76);
  if (!bmeIsOpen) {
    if (Serial) {
      Serial.println(F("Could not find a valid BME688 sensor, check wiring!"));
    }
    // while (1);

    buttonBlink(10);
    return;
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);  // 320*C for 150 ms

  // Open file and write headers
  // bmeFile = SD.open(BME_FILE_NAME, O_APPEND);  // Create or open a file called "data.txt" on the SD card
  // if(bmeFile)
  // {
  //   if(bmeFile.size() == 0)  // Only create the header if there isn't any data in the file yet
  //   {
  //     bmeFile.println("Timestamp\tSample Number\tTemperature (*C)\tPressure (hPa)\tHumidity (%)\tGas (kOhms)\tAltitude (m)");
  //   }
  //   bmeFile.close(); // Close the file to properly save the data
  // }
  // else {
  //   // if the file didn't open, print an error:
  //   if(Serial)
  //     Serial.println("error opening bme688.txt");
  // }

  buttonBlink(4);
}

void bme688Loop(int sampleNumber) {
  if (!bmeIsOpen) {
    return;
  }

  /************* ASYNC READINGS *************
  // Tell BME680 to begin measurement.
  if (endTime == 0) {
    endTime = bme.beginReading();
    if (endTime == 0) {
      if (Serial){
        Serial.println("Failed to begin reading :(");
      }
      // delay(10);
      return;
    }
  } else if (millis() >= endTime) {

    // if(Serial){
    //   Serial.print(F("Reading started at "));
    //   Serial.print(millis());
    //   Serial.print(F(" and will finish at "));
    //   Serial.println(endTime);

    //   Serial.println(F("You can do other work during BME688 measurement."));
    //   delay(1); // This represents parallel work.
    // }

    // There's no need to delay() until millis() >= endTime: bme.endReading()
    // takes care of that. It's okay for parallel work to take longer than
    // BME680's measurement time.

    // Obtain measurement results from BME680. Note that this operation isn't
    // instantaneous even if milli() >= endTime due to I2C/SPI latency.
    if (!bme.endReading()) {
      if (Serial){
        Serial.println("Failed to complete reading :(");
      }
      return;
    }

    // Reset the timer
    endTime = 0;
    */

  if (bme.performReading()) {

#if USE_SD
    bmeFile = SD.open(BME_FILE_NAME, FILE_WRITE);  // Create or open a file called "data.txt" on the SD card
    if (bmeFile) {
      printDate(bmeFile);

      //Sample Number
      bmeFile.print("\t");
      bmeFile.print(sampleNumber);

      bmeFile.print("\t");
      bmeFile.print(bme.temperature);
      bmeFile.print("\t");
      bmeFile.print(bme.pressure / 100.0);
      bmeFile.print("\t");
      bmeFile.print(bme.humidity);
      bmeFile.print("\t");
      bmeFile.print(bme.gas_resistance / 1000.0);
      bmeFile.print("\t");
      bmeFile.print(bme.readAltitude(SEALEVELPRESSURE_HPA));

      bmeFile.println();

      bmeFile.close();  // Close the file to properly save the data
    } else {
      // if the file didn't open, print an error:
      if (Serial)
        Serial.println("error opening bme688.csv");
    }
#endif

#if PRINT_SERIAL_MESSAGES
    if (Serial) {
      Serial.print(F("Reading completed at "));
      Serial.println(millis());

      // Serial.print(F("Temperature = "));
      // Serial.print(bme.temperature);
      // Serial.println(F(" *C"));

      // Serial.print(F("Pressure = "));
      // Serial.print(bme.pressure / 100.0);
      // Serial.println(F(" hPa"));

      // Serial.print(F("Humidity = "));
      // Serial.print(bme.humidity);
      // Serial.println(F(" %"));

      // Serial.print(F("Gas = "));
      // Serial.print(bme.gas_resistance / 1000.0);
      // Serial.println(F(" KOhms"));

      // Serial.print(F("Approx. Altitude = "));
      // Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
      // Serial.println(F(" m"));

      Serial.println();
    }
#endif
  }
}