#include <Chrono.h>
#include <SparkFun_Qwiic_Button.h>

// Button and LED
QwiicButton button;
int nClicks = 0;

bool doBlink = false;
Chrono ledChrono;
bool ledOn = false;

void setupButton() {
  if (button.begin() == false) {
    Serial.println("Qwiic Button did not acknowledge! Freezing.");
    // while (1)
    //   ;
  }

  buttonBlink(3);
  
  ledChrono.restart();
}

void loopButton() {
  if (button.hasBeenClicked()) {
    button.clearEventBits();

    // Register Click
    nClicks += 1;
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