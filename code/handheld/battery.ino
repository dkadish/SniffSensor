#include "Adafruit_LC709203F.h"

Adafruit_LC709203F lc;

void setupBattery() {
  if (!lc.begin()) {
    Serial.println(F("Couldnt find Adafruit LC709203F?\nMake sure a battery is plugged in!"));
    while (1) delay(10);
  }
  Serial.println(F("Found LC709203F"));
  Serial.print("Version: 0x");
  Serial.println(lc.getICversion(), HEX);

  lc.setTemperatureMode(LC709203F_TEMPERATURE_I2C);

  lc.setPackSize(LC709203F_APA_3000MAH);

  lc.setAlarmVoltage(3.8);
}

void loopBattery() {

#if USE_DISPLAY
  display.setBattery(lc.cellPercent());
#endif
}