#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <STM32RTC.h>
#include <Adafruit_ADS1X15.h>

// ADC for MEMS sensors
Adafruit_ADS1015 ads1, ads2;  // Construct an ads101

#if USE_SD
File memsFile;
#endif

void setupMEMS() {
#if USE_SERIAL
  Serial.println("Starting MEMS Sensors");
#endif

  ads1.begin();
  ads2.begin(0x49);
}

void readMEMS(int sampleNumber) {
  // Variable Setup
  int16_t hcho, ch4, voc, odor, etoh, h2s, no2, nh3;
  float hchoVolt, ch4Volt, vocVolt, odorVolt, etohVolt, h2sVolt, no2Volt, nh3Volt;

  // Raw read
  hcho = ads1.readADC_SingleEnded(0);
  ch4 = ads1.readADC_SingleEnded(1);
  voc = ads1.readADC_SingleEnded(2);
  odor = ads1.readADC_SingleEnded(3);
  etoh = ads2.readADC_SingleEnded(0);
  h2s = ads2.readADC_SingleEnded(1);
  no2 = ads2.readADC_SingleEnded(2);
  nh3 = ads2.readADC_SingleEnded(3);

  // Volatage calculation
  hchoVolt = ads1.computeVolts(hcho);
  ch4Volt = ads1.computeVolts(ch4);
  vocVolt = ads1.computeVolts(voc);
  odorVolt = ads1.computeVolts(odor);
  etohVolt = ads2.computeVolts(etoh);
  h2sVolt = ads2.computeVolts(h2s);
  no2Volt = ads2.computeVolts(no2);
  nh3Volt = ads2.computeVolts(nh3);

#if USE_SD
  memsFile = SD.open(MEMS_FILE_NAME, FILE_WRITE);  // Create or open a file called "data.txt" on the SD card
  if (memsFile) {
    printDate(memsFile);

    //Sample Nuber
    memsFile.print("\t");
    memsFile.print(sampleNumber);

    memsFile.print("\t");
    memsFile.print(hchoVolt);
    memsFile.print("\t");
    memsFile.print(ch4Volt);
    memsFile.print("\t");
    memsFile.print(vocVolt);
    memsFile.print("\t");
    memsFile.print(odorVolt);
    memsFile.print("\t");
    memsFile.print(etohVolt);
    memsFile.print("\t");
    memsFile.print(h2sVolt);
    memsFile.print("\t");
    memsFile.print(no2Volt);
    memsFile.print("\t");
    memsFile.print(nh3Volt);

    memsFile.println();

    memsFile.close();  // Close the file to properly save the data
  } else {
  }
#endif

#if PRINT_SERIAL_MESSAGES
  if (Serial) {
    //Sample Nuber
    Serial.print(sampleNumber);

    Serial.print(",");
    Serial.print(hchoVolt);
    Serial.print(",");
    Serial.print(ch4Volt);
    Serial.print(",");
    Serial.print(vocVolt);
    Serial.print(",");
    Serial.print(odorVolt);
    Serial.print(",");
    Serial.print(etohVolt);
    Serial.print(",");
    Serial.print(h2sVolt);
    Serial.print(",");
    Serial.print(no2Volt);
    Serial.print(",");
    Serial.print(nh3Volt);

    Serial.println();
  }
#endif
}