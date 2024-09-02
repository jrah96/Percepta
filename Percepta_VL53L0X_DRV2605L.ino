#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

#include <Wire.h>
#include "Adafruit_DRV2605.h"
Adafruit_DRV2605 drv;

float resultInch;
int effect;

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
  sensor.startContinuous();
  
  Serial.println("Adafruit DRV2605 Basic test");
  if (! drv.begin()) {
    Serial.println("Could not find DRV2605");
    while (1) delay(10);
  }
  drv.selectLibrary(1);
  drv.setMode(DRV2605_MODE_INTTRIG); 
}

void loop()
{
  resultInch = sensor.readRangeContinuousMillimeters() * 0.0393701;
  if (resultInch > 0 && resultInch <= 6) {
    effect = 14;
  } else if (resultInch > 7 && resultInch <= 12) {
    effect = 13;
  } else if (resultInch > 12) {
    effect = 55;
  } 
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.print(resultInch);
  Serial.print(" ... ");

  if (effect == 13) {
    Serial.println(F("13 − Soft Fuzz - 60%"));
  } else if (effect == 14) {
    Serial.println(F("14 − Strong Buzz - 100%"));
  } else if (effect == 118) {
    Serial.println(F("118 − Long buzz for programmatic stopping – 100%"));
  } else if (effect == 52) {
    Serial.println(F("52 − Pulsing Strong 1 – 100%"));
  } else if (effect == 55) {
    Serial.println(F("55 − Pulsing Medium 2 – 60%"));
  }
  drv.setWaveform(0, effect);
  drv.setWaveform(1, 0);       
  drv.go();
  delay(250); 
}
