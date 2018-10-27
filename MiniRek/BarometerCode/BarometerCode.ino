

/*
  BAROMETER SOURCE INFO
  MS5611 Barometric Pressure & Temperature Sensor. Simple Example
  Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/czujnik-cisnienia-i-temperatury-ms5611.html
  GIT: https://github.com/jarzebski/Arduino-MS5611
  Web: http://www.jarzebski.pl
  (c) 2014 by Korneliusz Jarzebski
*/
#include "coats.h"
#include <Wire.h>
#include "MS5611.h"

#define ENDING 0x4350
#define ALT_ID 134

#define ptr_t uint32_t

float ALTDATA[3];

coats downlink = coats(ENDING, COUNTER_EN);

MS5611 ms5611;

double referencePressure;

void setup() 
{
  downlink.addTlm(ALT_ID,(ptr_t*)ALTDATA,sizeof(ALTDATA));
  Wire.begin();
  SerialUSB.begin(9600);
  while(!ms5611.begin())
  {
    SerialUSB.println("Could not find a valid MS5611 sensor, check wiring!");
    delay(500);
  }

  // Get reference pressure for relative altitude
  referencePressure = ms5611.readPressure();
  // Check settings
  checkSettings();

  downlink.serialInit(Serial,9600);
}

void checkSettings()
{
  Serial.print("Oversampling: ");
  Serial.println(ms5611.getOversampling());
}

struct baroVals {
  uint32_t rawTemp;
  uint32_t rawPressure;
  float realTemperature;
  long realPressure;
  float absoluteAltitude;
  float relativeAltitude; 
};

struct baroVals ms5611_read_vals() {
  // value[0] = rawTemp
  // value[1] = realTemperature
  struct baroVals values;

  // Read raw values
  values.rawTemp = ms5611.readRawTemperature();
  values.rawPressure = ms5611.readRawPressure();

  // Read true temperature & Pressure
  values.realTemperature = ms5611.readTemperature();
  values.realPressure = ms5611.readPressure();

  // Calculate altitude
  values.absoluteAltitude = ms5611.getAltitude(values.realPressure);
  values.relativeAltitude = ms5611.getAltitude(values.realPressure, referencePressure);

  return values;
}

void loop()
{

  struct baroVals values;
  values = ms5611_read_vals();


  ALTDATA[0] = values.realTemperature;
  ALTDATA[1] = 1.0 * values.realPressure;
  ALTDATA[2] = values.absoluteAltitude;

  SerialUSB.println(ALTDATA[0]);
  SerialUSB.println(ALTDATA[1]);
  SerialUSB.println(ALTDATA[2]);

/*
  SerialUSB.print(" rawTemp = ");
  SerialUSB.print(values.rawTemp);
  SerialUSB.print(", realTemp = ");
  SerialUSB.print(values.realTemperature);
  SerialUSB.println(" *C");

  SerialUSB.print(" rawPressure = ");
  SerialUSB.print(values.rawPressure);
  SerialUSB.print(", realPressure = ");
  SerialUSB.print(values.realPressure);
  SerialUSB.println(" Pa");

  SerialUSB.print(" absoluteAltitude = ");
  SerialUSB.print(values.absoluteAltitude);
  SerialUSB.print(" m, relativeAltitude = ");
  SerialUSB.print(values.relativeAltitude);    
  SerialUSB.println(" m");
*/

  downlink.serialWriteTlm(ALT_ID);

  delay(50);
}
