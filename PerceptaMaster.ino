//VL53L0X and DRV2605 with Switch Button
//Last Update:   sensor.setMeasurementTimingBudget(15000); Added this and decreased time from 33000 also commented out print statements to increase responsiveness
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

#include <Wire.h>
#include "Adafruit_DRV2605.h"
Adafruit_DRV2605 drv;

#include <Bounce2.h> 
Bounce2::Button button = Bounce2::Button(); // INSTANTIATE A Bounce2::Button OBJECT

float resultInch;
int effect;

int butPin = A0;
int mode = 1;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  sensor.setMeasurementTimingBudget(15000);
  sensor.startContinuous();

  if (! drv.begin()) {
    Serial.println("Could not find DRV2605");
    while (1) delay(10);
  }
  drv.selectLibrary(1);
  drv.setMode(DRV2605_MODE_INTTRIG); 

  button.attach(A0, INPUT_PULLUP);
  button.interval(20);
  button.setPressedState(LOW); 
}

void loop() {
  button.update();
  resultInch = sensor.readRangeContinuousMillimeters() * 0.0393701;
  
  if (button.pressed()) {
    mode++;
    if (mode > 3) {
      mode = 1;
    }
  }

  switch (mode) {
    case 1:
      off();
      //Serial.println("-----");
      //Serial.println("Mode 1: OFF");
      //Serial.println("-----");
      break;
    case 2:
      proximityMode();
      //Serial.println("-----");
      //Serial.println("Mode 2: Proximity");
      //Serial.println("-----");
      break;
    case 3:
      horizonMode();
      //Serial.println("-----");
      //Serial.println("Mode 3: Horizon");
      //Serial.println("-----");
      break;
  }

  if (sensor.timeoutOccurred()) { 
    Serial.print(" TIMEOUT"); 
  }

  //printEffectInfo(); 

  drv.setWaveform(0, effect);
  drv.setWaveform(1, 0);       
  drv.go();
  //delay(500); 
}

void off() {
  effect=0;
  drv.setWaveform(0, 0);  
}
void proximityMode() {
  if (resultInch > 0 && resultInch <= 7) {
    effect = 7;
  } else if (resultInch > 7 && resultInch <= 14) {
    effect = 9;
  } else if (resultInch > 14 && resultInch <=21) {
    effect = 81;
  } else if (resultInch > 21) {
    effect = 76;
  } 
  // Serial.print(resultInch);
  // Serial.println(" INCHES -----");
}

void horizonMode() {
  if (resultInch > 0 && resultInch <= 15) {
    effect = 7;
  } else if (resultInch > 15 && resultInch <= 30) {
    effect = 9;
  } else if (resultInch > 30 && resultInch <= 45) {
    effect = 81;
  } else if (resultInch > 45) {
    effect = 76;
  } 
  //Serial.print(resultInch);
  //Serial.println(" INCHES -----");
}

void printEffectInfo() {
  switch (effect) {
    case 7:
      Serial.println(F("7 − Soft Bump - 100%"));
      break;
    case 9: 
      Serial.println(F("9 − Soft Bump - 30%"));
      break;
    case 13:
      Serial.println(F("13 − Soft Fuzz - 60%"));
      break;
    case 14:
      Serial.println(F("14 − Strong Buzz - 100%"));
      break;
    case 16: 
      Serial.println(F("16 − 1000 ms Alert 100%"));
      break;
    case 27: 
      Serial.println(F("27 − Short Double Click Strong 1 – 100%"));
      break;
    case 37:
      Serial.println(F("37 - Long Double Sharp Click Strong 1 - 100%"));
      break;
    case 47:
      Serial.println(F("47 − Buzz 1 – 100%"));
      break;
    case 52:
      Serial.println(F("52 − Pulsing Strong 1 – 100%"));
      break;
    case 55:
      Serial.println(F("55 − Pulsing Medium 2 – 60%"));
      break;
    case 76:
      Serial.println(F("76 − Transition Ramp Down Long Sharp 1 – 100 to 0%"));
      break;
    case 81:
      Serial.println(F("81 − Transition Ramp Down Short Sharp 2 – 100 to 0%"));
      break;
    case 118: 
     Serial.println(F("118 − Long buzz for programmatic stopping – 100%"));
     break;
  }
}
