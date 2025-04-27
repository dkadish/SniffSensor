#if USE_STM
#define CHIP_SELECT PA4
#elif USE_ESP
#endif

void blink(bool onOff, int delayMs) {
  digitalWrite(LED_BUILTIN, onOff);
  delay(delayMs);
}

void printCardInfo(){
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  }

void setupComms() {
  pinMode(LED_BUILTIN, OUTPUT);

// Wire.setClock(100000);

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
  #if USE_STM
  bool sd = SD.begin(CHIP_SELECT);
  #elif USE_ESP
  bool sd = SD.begin();
  #endif
  #if USE_SERIAL
  if (!sd) {
    if (Serial) Serial.println("Card failed, or not present");
    // don't do anything more:
  } else {
    if (Serial) Serial.println("card initialized.");
  }

    uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  #endif
#endif

  Wire.begin();
  delay(100);
}