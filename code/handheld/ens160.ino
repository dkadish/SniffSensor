/***************************************************************************
  ENS160 - Digital Air Quality Sensor
  
  This is an example for ENS160 basic reading in standard mode
    
  Updated by Sciosense / 25-Nov-2021


  24L30 - This code is broken. Either update from https://github.com/adafruit/ENS160_driver or (likely better) https://github.com/sparkfun/SparkFun_Indoor_Air_Quality_Sensor-ENS160_Arduino_Library
 ***************************************************************************/
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#include "ScioSense_ENS160.h"  // ENS160 library
//ScioSense_ENS160      ens160(ENS160_I2CADDR_0);
ScioSense_ENS160 ens160(ENS160_I2CADDR_1);
File ensFile;

/*--------------------------------------------------------------------------
  SETUP function
  initiate sensor
 --------------------------------------------------------------------------*/
void ens160Setup() {
  if (Serial) {
    Serial.print("ENS160...");
  }

  ens160.begin();

  if (ens160.available()) {
    // Print ENS160 versions
    if (Serial) {
      Serial.print("\tRev: ");
      Serial.print(ens160.getMajorRev());
      Serial.print(".");
      Serial.print(ens160.getMinorRev());
      Serial.print(".");
      Serial.println(ens160.getBuild());

      Serial.print("\tStandard mode ");
      Serial.println(ens160.setMode(ENS160_OPMODE_STD) ? "done." : "failed!");
    }
  }

  // Open file and write headers
  // ensFile = SD.open(ENS_FILE_NAME, O_APPEND);  // Create or open a file called "data.txt" on the SD card
  // if (ensFile) {
  //   if (ensFile.size() == 0)  // Only create the header if there isn't any data in the file yet
  //   {
  //     ensFile.println("Timestamp\tSample Number\tAQI\tTVOC (ppb)\teCO2 (ppm)\tR HP0 (ohm)\tR HP1 (ohm)\tR HP2 (ohm)\tR HP3 (ohm)");
  //   }
  //   ensFile.close();  // Close the file to properly save the data
  // } else {
  //   // if the file didn't open, print an error:
  //   if (Serial)
  //     Serial.println("error opening ens160.csv");
  // }
}

void ens160Loop(int sampleNumber) {

  if (ens160.available()) {
    ens160.measure(true);
    ens160.measureRaw(true);

    ensFile = SD.open(ENS_FILE_NAME, FILE_WRITE);  // Create or open a file called "data.txt" on the SD card
    if (ensFile) {
      printDate(ensFile);

      //Sample Number
      ensFile.print("\t");
      ensFile.print(sampleNumber);

      ensFile.print("\t");
      ensFile.print(ens160.getAQI());
      ensFile.print("\t");
      ensFile.print(ens160.getTVOC());
      ensFile.print("\t");
      ensFile.print(ens160.geteCO2());
      ensFile.print("\t");
      ensFile.print(ens160.getHP0());
      ensFile.print("\t");
      ensFile.print(ens160.getHP1());
      ensFile.print("\t");
      ensFile.print(ens160.getHP2());
      ensFile.print("\t");
      ensFile.print(ens160.getHP3());
      ensFile.print("\t");
      ensFile.println();

      ensFile.close();  // Close the file to properly save the data
    } else {
      // if the file didn't open, print an error:
      if (Serial)
        Serial.println("error opening ens160.csv");
    }
  }
}
