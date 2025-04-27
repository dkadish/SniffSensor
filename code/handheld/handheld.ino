/*
TODO:

https://github.com/stm32duino/STM32LowPower
*/



#define READING_INTERVAL 1000L
#define WAIT_LED_DELAY 1000

#define USE_SERIAL 0
#define PRINT_SERIAL_MESSAGES 0
#define ACCEPT_COMMANDS 0
#define USE_MEMS 1
#define USE_BME 0

#define USE_BUTTON 1
#define USE_BATTERYMONITOR 1
#define USE_DISPLAY 1
#define USE_SD 1
#define USE_DATE 0

#define USE_STM 0
#define USE_ESP 1

#if USE_STM
  #define MEMS_FILE_NAME "mems.csv"
  #define BME_FILE_NAME "bme.csv"
  #define ENS_FILE_NAME "ens.csv"
  #define WRITE_MODE FILE_WRTIE
#else
  #define MEMS_FILE_NAME "/mems.csv"
  #define BME_FILE_NAME "/bme.csv"
  #define ENS_FILE_NAME "/ens.csv"
  #define WRITE_MODE FILE_APPEND
#endif
 
#include <SPI.h>
#include <SD.h>
#include <Chrono.h>

#if USE_STM
#include <STM32RTC.h>
#endif

// /*********************************************************/

// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #define SCREEN_WIDTH 128  // OLED display width, in pixels
// #define SCREEN_HEIGHT 64  // OLED display height, in pixels

// class OLEDDisplay {
// private:
//   Adafruit_SSD1306 m_display;  //(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//   int m_width;
//   int m_height;
//   int m_addr;
//   int m_batteryLevel = 0;

// public:
//   OLEDDisplay(int width, int height, int addr)
//     : m_display(width, height, &Wire, -1), m_width(width), m_height(height), m_addr(addr) {}

//   void setup();

//   void setBattery(int level);

//   void update();
// };

// void OLEDDisplay::setup() {
//   if (!m_display.begin(SSD1306_SWITCHCAPVCC, m_addr)) {
//     Serial.println(F("SSD1306 allocation failed"));
//   }
// }

// void OLEDDisplay::setBattery(int level) {
//   m_batteryLevel = level;
// }

// void OLEDDisplay::update() {
//   Serial.println("Updating display");
//   m_display.clearDisplay();

//   m_display.setTextSize(2);  // Draw 2X-scale text
//   m_display.setTextColor(SSD1306_WHITE);
//   m_display.setCursor(10, 0);
//   m_display.println(F("HI!"));
//   m_display.setCursor(20, 0);
//   m_display.println(m_batteryLevel);
//   m_display.display();  // Show initial text
// }

// /*********************************************************/

#if USE_STM
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
#endif

void printDigits(File& file, int digits) {
  if (digits < 10) {
    file.print('0');
  }
  file.print(digits);
}

Chrono readingChrono;

#if USE_DISPLAY1
OLEDDisplay display(128, 64, 0x3D);
#endif

// #include <STM32RTC.h>
// #include <TimeLib.h>

/* Get the rtc object */
// STM32RTC& rtc = STM32RTC::getInstance();

void read() {

// Check to see if button has been pressed and record.
#if USE_BUTTON
  loopButton();
#endif

  if (readingChrono.hasPassed(READING_INTERVAL)) {

    digitalWrite(LED_BUILTIN, HIGH);
// Take readings from all sensors.
#if USE_MEMS
    readMEMS(getClicks());
#endif

#if USE_BME
    bme688Loop(getClicks());
#endif

#if USE_BATTERYMONITOR
    loopBattery(getClicks());
#endif
    // Restart the timer
    readingChrono.restart();
    digitalWrite(LED_BUILTIN, HIGH);
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
  if (!Serial) {
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

#if USE_STM
  setupRTC();
#endif

  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);

#if USE_BUTTON
  setupButton();
#endif

#if USE_MEMS
  setupMEMS();
#endif

#if USE_BME
  bme688Setup();
#endif

  // ens160Setup();


#if USE_BATTERYMONITOR
    setupBattery();
#endif

#if USE_DISPLAY1
  display.setup();
#endif

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
#if USE_SERIAL
  Serial.println("Finished Setup.");
#endif
}

void loop() {
  // Run the reading loop
  read();

#if USE_DISPLAY1
  display.update();
#endif

#if ACCEPT_COMMANDS
  serialCommands();
#endif
}