#include <Arduino.h>
#include <Wire.h>
#include <array>

#include "MAX30105.h"
#include "heartRate.h"
#include "utils/utils.h"

MAX30105 particleSensor;


void setup() {
 
  
  Serial.begin(9600);
  
  Wire.begin(21, 22); // ESP32 SDA, SCL

  Serial.println("Initializing...");

  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30102 was not found. Check wiring/power.");
    while (1);
  }

  Serial.println("Place your finger on the sensor.");

  particleSensor.setup();
  particleSensor.setPulseAmplitudeIR(0x0A);
  particleSensor.setSampleRate(50);
  
}

void loop() {
  int32_t data = particleSensor.getIR();
  bool beat = checkForBeat(data);
  int16_t value = getCurrentACVal();
  double filteredValue = filter(value);
  

  Serial.printf("%d,%d\n", filteredValue, value);
}