/*
TODO:

https://github.com/stm32duino/STM32LowPower
*/

#define READING_INTERVAL 10000L
#define MEMS_FILE_NAME "mems.csv"
#define BME_FILE_NAME "bme.csv"
#define ENS_FILE_NAME "ens.csv"

#include <SPI.h>
#include <SD.h>
#include <Chrono.h>
#include <STM32RTC.h>

STM32RTC& rtc = STM32RTC::getInstance();
void setupRTC() {
  rtc.begin();
}

void printDate(File& file) {
  file.print(rtc.getYear());
  file.print("-");
  file.print(rtc.getMonth());
  file.print("-");
  file.print(rtc.getDay());
  file.print(" ");
  file.print(rtc.getHours());
  file.print(":");
  printDigits(file, rtc.getMinutes());
  file.print(":");
  printDigits(file, rtc.getSeconds());
}

void printDigits(File& file, int digits) {
  if (digits < 10) {
    file.print('0');
  }
  file.print(digits);
}

Chrono readingChrono;

#define WAIT_LED_DELAY 1000

// #include <STM32RTC.h>
// #include <TimeLib.h>

/* Get the rtc object */
// STM32RTC& rtc = STM32RTC::getInstance();

void read() {

  // Check to see if button has been pressed and record.
  loopButton();

  if (readingChrono.hasPassed(READING_INTERVAL)) {

    digitalWrite(LED_BUILTIN, HIGH);
    // Take readings from all sensors.
    readMEMS(getClicks());
    // delay(100);
    // digitalWrite(LED_BUILTIN, LOW);
    // buttonBlink(1);
    // digitalWrite(LED_BUILTIN, HIGH);
    bme688Loop(getClicks());
    // delay(100);
    // digitalWrite(LED_BUILTIN, LOW);
    // buttonBlink(2);
    // digitalWrite(LED_BUILTIN, HIGH);
    //TODO: THIS IS THE PROBLEM. THERE IS AN ISSUE WITH THE ENS CODE.
    // ens160Loop(getClicks());
    // delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    // delay(100);

    // buttonBlink(5);

    // Restart the timer
    readingChrono.restart();
  }
}

void printFileOverSerial(File& file) {
  if (file) {
    Serial.println("test.txt:");

    // Read the file until there's nothing else in it
    while (file.available()) {
      Serial.write(file.read());
    }

    // Close the file
    file.close();
  }
}

void serialCommands() {
  if(!Serial){
    return;
  }

  if (Serial.available()) {
    String command = Serial.readString();
    command.trim();
    char cmd = command.charAt(0);
    Serial.print("You entered: ");

    if (cmd == 'm') {
      File f = SD.open(MEMS_FILE_NAME, FILE_READ);
      Serial.println(command);
      printFileOverSerial(f);
    } else if (cmd == 'b') {
      File f = SD.open(BME_FILE_NAME, FILE_READ);
      Serial.println(command);
      printFileOverSerial(f);
    } else if (cmd == 'e') {
      File f = SD.open(ENS_FILE_NAME, FILE_READ);
      Serial.println(command);
      printFileOverSerial(f);
    }
  }
}


void setup() {
  //Start communication
  setupComms();

  setupRTC();

  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);

  setupButton();

  setupMEMS();

  bme688Setup();

  // ens160Setup();

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
}

void loop() {
  // Run the reading loop
  read();
  serialCommands();
}