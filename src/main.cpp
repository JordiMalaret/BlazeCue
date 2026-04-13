#include <Arduino.h>
#include <Wire.h>
#include <array>
#include "MAX30105.h"
#include "heartRate.h"
#include "utils/utils.h"
#include <Adafruit_MLX90614.h>

MAX30105 particleSensor;
Adafruit_MLX90614 sensor = Adafruit_MLX90614();

void setup() {
 
  pinMode(0, OUTPUT);
  
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
  if (!sensor.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };
  Serial.println("Begin ok!");

  /**
   * adjust sensor sleep mode
   * mode select to enter or exit sleep mode, it's enter sleep mode by default
   *      true is to enter sleep mode
   *      false is to exit sleep mode (automatically exit sleep mode after power down and restart)
   */

}

void loop() {
  int32_t data = particleSensor.getIR();
  bool beat = checkForBeat(data);
  if(beat){
    digitalWrite(0,1);
    Serial.println(1);
  }
  else{
    digitalWrite(0,0);
  }
  int16_t value = getCurrentACVal();
  double filteredValue = filter(value);
  

  // Serial.printf("%f,%d\n", filteredValue, value);
  Serial.printf("%d,%d\n", data, value);
  Serial.println(value);


  
   /**
   * get ambient temperature, unit is Celsius
   * return value range： -40.01 °C ~ 85 °C
   */
  float ambientTemp = sensor.readAmbientTempC();

  /**
   * get temperature of object 1, unit is Celsius
   * return value range： 
   * @n  -70.01 °C ~ 270 °C(MLX90614ESF-DCI)
   * @n  -70.01 °C ~ 380 °C(MLX90614ESF-DCC)
   */
  float objectTemp = sensor.readObjectTempC();

  // Serial.printf("%f,%f\n", ambientTemp*9/5 + 32, objectTemp*9/5 + 32);
  // print measured data in Celsius
  // Serial.print("Ambient celsius : "); Serial.print(ambientTemp); Serial.println(" °C");
  // Serial.print("Object celsius : ");  Serial.print(objectTemp);  Serial.println(" °C");

  // // print measured data in Fahrenheit
  // Serial.print("Ambient fahrenheit : "); Serial.print(ambientTemp*9/5 + 32); Serial.println(" F");
  // Serial.print("Object fahrenheit : ");  Serial.print(objectTemp*9/5 + 32);  Serial.println(" F");

  
}