#include <Arduino.h>
#include <Wire.h>
#include <array>

#include "MAX30105.h"
#include "heartRate.h"
#include "utils/utils.h"
#include <DFRobot_MLX90614.h>

MAX30105 particleSensor;
DFRobot_MLX90614_I2C sensor;

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

  // initialize the sensor
  while( NO_ERR != sensor.begin() ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   * adjust sensor sleep mode
   * mode select to enter or exit sleep mode, it's enter sleep mode by default
   *      true is to enter sleep mode
   *      false is to exit sleep mode (automatically exit sleep mode after power down and restart)
   */
  sensor.enterSleepMode();
  delay(50);
  sensor.enterSleepMode(false);
  delay(200);
}

void loop() {
  int32_t data = particleSensor.getIR();
  bool beat = checkForBeat(data);
  int16_t value = getCurrentACVal();
  double filteredValue = filter(value);
  

  Serial.printf("%d,%d\n", filteredValue, value);


  
   /**
   * get ambient temperature, unit is Celsius
   * return value range： -40.01 °C ~ 85 °C
   */
  float ambientTemp = sensor.getAmbientTempCelsius();

  /**
   * get temperature of object 1, unit is Celsius
   * return value range： 
   * @n  -70.01 °C ~ 270 °C(MLX90614ESF-DCI)
   * @n  -70.01 °C ~ 380 °C(MLX90614ESF-DCC)
   */
  float objectTemp = sensor.getObjectTempCelsius();

  // print measured data in Celsius
  Serial.print("Ambient celsius : "); Serial.print(ambientTemp); Serial.println(" °C");
  Serial.print("Object celsius : ");  Serial.print(objectTemp);  Serial.println(" °C");

  // print measured data in Fahrenheit
  Serial.print("Ambient fahrenheit : "); Serial.print(ambientTemp*9/5 + 32); Serial.println(" F");
  Serial.print("Object fahrenheit : ");  Serial.print(objectTemp*9/5 + 32);  Serial.println(" F");

  Serial.println();
  delay(500);
}