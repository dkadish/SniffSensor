#include <Chrono.h>
#include <SparkFun_Qwiic_Button.h>
#include <Preferences.h>

// Button and LED
QwiicButton button;
int nClicks = 0;

bool doBlink = false;
Chrono ledChrono;
bool ledOn = false;

Preferences preferences;

void setupButton() {
  if (button.begin() == false) {
    Serial.println("Qwiic Button did not acknowledge! Freezing.");
    // while (1)
    //   ;
  }

  preferences.begin("smell", false);

  nClicks = preferences.getInt("clicks", 0);

  buttonBlink(3);
  
  ledChrono.restart();
}

void loopButton() {
  if (button.hasBeenClicked()) {
    button.clearEventBits();

    // Register Click
    nClicks += 1;
    preferences.putInt("clicks", nClicks);
  }

  if(doBlink && ledChrono.hasPassed(WAIT_LED_DELAY)){
    ledOn = !ledOn;
    button.LEDon( ledOn ? 255 : 0);

    ledChrono.restart();
  }
}

void buttonBlink(int n){
  for(int i=0; i< n; i++){
    button.LEDon();
    delay(100);
    button.LEDoff();
    delay(100);
  }
}

int getClicks(){
  return nClicks;
}