#define CHIP_SELECT PA4

void blink(bool onOff, int delayMs){
    digitalWrite(LED_BUILTIN, onOff);
    delay(delayMs);
}

void setupComms() {
  pinMode(LED_BUILTIN, OUTPUT);

  for (int i = 0; i < 10; i++) {
    if (Serial) break;
    Serial.begin(9600);
    blink(true, 250);
    blink(false, 250);
  }

  for (int i = 0; i < 5; i++) {
    blink(true, 100);
    blink(false, 100);
  }

  if(Serial){
    Serial.println("Serial port started.");
  }

  if (!SD.begin(CHIP_SELECT)) {
    if(Serial) Serial.println("Card failed, or not present");
    // don't do anything more:
  } else {
    if(Serial) Serial.println("card initialized.");
  }

  Wire.begin();
  delay(100);
}