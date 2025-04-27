#if USE_STM
#define CHIP_SELECT PA4
#elif USE_ESP
#define CHIP_SELECT 5
#endif

void blink(bool onOff, int delayMs) {
  digitalWrite(LED_BUILTIN, onOff);
  delay(delayMs);
}

void setupComms() {
  pinMode(LED_BUILTIN, OUTPUT);

Wire.setClock(10000);

#if USE_SERIAL
  for (int i = 0; i < 10; i++) {
    if (Serial) break;
    Serial.begin(115200);
    blink(true, 250);
    blink(false, 250);
  }

  delay(500);

  for (int i = 0; i < 5; i++) {
    blink(true, 100);
    blink(false, 100);
  }

  if (Serial) {
    Serial.println("Serial port started.");
  }
#endif

#if USE_SD
  bool sd = SD.begin(CHIP_SELECT);
  #if USE_SERIAL
  if (!sd) {
    if (Serial) Serial.println("Card failed, or not present");
    // don't do anything more:
  } else {
    if (Serial) Serial.println("card initialized.");
  }
  #endif
#endif

  Wire.begin();
  delay(100);
}