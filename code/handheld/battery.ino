#include "Adafruit_LC709203F.h"

Adafruit_LC709203F lc;

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 32     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  /// Large: 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);  // 128x32 pixel canvas

bool blinkLine = false;

void setupBattery() {
  if (!lc.begin()) {
#if USE_SERIAL
    Serial.println(F("Couldnt find Adafruit LC709203F?\nMake sure a battery is plugged in!"));
#endif
    // while (1) delay(10);
  }
#if USE_SERIAL
  Serial.println(F("Found LC709203F"));
  Serial.print("Version: 0x");
  Serial.println(lc.getICversion(), HEX);
#endif
  lc.setThermistorB(3950);

  lc.setPackSize(LC709203F_APA_3000MAH);

  lc.setAlarmVoltage(3.8);

#if USE_DISPLAY
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
#if USE_SERIAL
    Serial.println(F("SSD1306 allocation failed"));
#endif
    for (;;)
      ;  // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1);

  // Clear the buffer
  display.clearDisplay();
#endif
}

#if USE_DISPLAY
void drawPower(float cellVoltage, float cellPercent, int sampleNumber) {
  
  // Clear the buffer
  display.clearDisplay();

  canvas.fillScreen(0);
  if(cellPercent < 20.0){
    canvas.setCursor(0, 0);
    canvas.setTextSize(2);
    canvas.print("CHARGE NOW");
    canvas.setTextSize(1);
  } else {
    canvas.setCursor(0, 0);
    canvas.setTextSize(2);
    canvas.print("Sample ");
    canvas.print(sampleNumber);
    canvas.setTextSize(1);
  }
  canvas.setCursor(0, 24);
  canvas.print("Batt: ");
  canvas.print(int(cellPercent));
  canvas.print(" % (");
  canvas.print(cellVoltage);
  canvas.println(" V)");
  
  if(blinkLine){
    // canvas.drawLine(0, 24, 128, 24, 1);
    canvas.drawCircle(125, 29, 2, SSD1306_WHITE);
  }
  blinkLine = !blinkLine;

  display.drawBitmap(0, 0, canvas.getBuffer(), 128, 32, 1); // Copy to screen

  
  display.display();
}
#endif

void loopBattery(int sampleNumber) {
#if USE_SERIAL
  Serial.print("Batt_Voltage:");
  Serial.print(lc.cellVoltage(), 3);
  Serial.print("\t");
  Serial.print("Batt_Percent:");
  Serial.print(lc.cellPercent(), 1);
  Serial.print("\t");
  Serial.print("Batt_Temp:");
  Serial.println(lc.getCellTemperature(), 1);
#endif

#if USE_DISPLAY
  drawPower(lc.cellVoltage(), lc.cellPercent(), sampleNumber);
#endif
}