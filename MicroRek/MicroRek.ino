


//Standard Libraries
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

// Barometer library
#include <MS5611.h>
MS5611 ms5611;

// Gyroscope Definitions
typedef union Sense
{
  float num[3];
  byte byt[12];
}sense;

//arrays for measurement storage
sense gyr;
sense mag;
sense acc;

//conversion factors for sensors
float A_fact = 0.000732;
float G_fact = 0.00875;
float M_fact = 0.00029;

//device address where SDO is tied to Vdd
byte AG_Add = 0B1101011;
byte M_Add = 0B0011110;

//first register of measurements for each device (start register)
byte A_St = 0B00101000;
byte G_St = 0B00011000;
byte M_St = 0B00101000;

//other necessary registers
byte CTRL_REG4    = 0B00011110;
byte CTRL_REG5_XL = 0B00011111;
byte CTRL_REG6_XL = 0B00100000;
byte CTRL_REG1_G  = 0B00010000;
byte CTRL_REG2_M  = 0B00100001;
byte CTRL_REG3_M  = 0B00100010;

// Set CS pin to pin 10
const int chipSelect = 10;   // CS Pin 10

// Other variables
double referencePressure;
String timeStamp;
String accData;
String gyrData;
String magData;
String presData;
String altData;
String tempData;
String fileName = ("DATALOG.txt");

void setup(){
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // Gyro/Accel/Mag Setup
  Serial.println("Initializing LSM9DS1TR...");
  
  Wire.begin();
  WriteReg(CTRL_REG4,0B00111000,AG_Add);     //enable gyro
  WriteReg(CTRL_REG5_XL,0B00111000,AG_Add);  //enable accelerometer
  WriteReg(CTRL_REG6_XL,0B00001000,AG_Add);  //set accelerometer scale to 16Gs
  WriteReg(CTRL_REG1_G,0B01000000,AG_Add);   //gyro/accel odr and bw
  WriteReg(CTRL_REG2_M,0B00100000,M_Add);     //set mag sensitivity to 8 gauss
  WriteReg(CTRL_REG3_M,0B00000000,M_Add);     //enable mag continuous
            
  Serial.println("LSM9DS1TR Initialized");

  // Barometer Setup
  Serial.println("Initializing MS5611...");
  
  while(!ms5611.begin())
  {
    Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    delay(500);
  }

  // Get reference pressure for relative altitude
  referencePressure = ms5611.readPressure();

  // Check settings
  checkSettings();

  Serial.println("MS5611 Initialized");
  
  // SD Setup
  Serial.println("Initializing SD card...");

  // Check for SD 
  if (!SD.begin(chipSelect)) {
    Serial.println("Card Error");
    // don't do anything more:
    while(1){}
  }
  Serial.println("SD initialized.");

  // Make File
 /*for (int i = 1; i < 99; i++){ 
  if (i < 10) {
    fileName = ("DATALOG0" + String(i) + ".txt"); 
  }
  else {
    fileName = ("DATALOG" + String(i) + ".txt"); 
  }

  
  File dataFile = SD.open("DATALOG");
  if (dataFile == false)
    break; 
  } */

  File dataFile = SD.open("DATALOG");
  
  Serial.println("File Name: " + fileName);
  
}

void loop(){
    // Timestamp (ms)
    timeStamp = (String(millis()) + "|");
    Serial.print(timeStamp) ;  //debug
  
    //Gyro/Accel/Mag Readings
    //get current measurements
    ReadData(mag.num, M_Add, M_St);
    ReadData(acc.num, AG_Add, A_St);
    ReadData(gyr.num, AG_Add, G_St);
    
    //convert measurements to useful units
    for (byte i=0;i<3;i++){
      acc.num[i] *= A_fact;  //multiply by conversion factor for Gs
      gyr.num[i] *= G_fact;  //multiply by conversion factor for deg/s
      mag.num[i] *= M_fact;  //multiply by conversion factor for Gauss
    }

    accData = (String(acc.num[0]) + ' ' + String(acc.num[1]) + ' ' + String(acc.num[2]) + '|');
    Serial.print(accData);
    gyrData = (String(gyr.num[0]) + ' ' + String(gyr.num[1]) + ' ' + String(gyr.num[2]) + '|');
    Serial.print(gyrData);
    magData = (String(mag.num[0]) + ' ' + String(mag.num[1]) + ' ' + String(mag.num[2]) + '|');
    Serial.print(magData);

    //Baro/Therm Readings
    // Read raw values
    uint32_t rawTemp = ms5611.readRawTemperature();
    uint32_t rawPressure = ms5611.readRawPressure();

    // Read true temperature & Pressure
    double realTemperature = ms5611.readTemperature();
    long realPressure = ms5611.readPressure();

    // Calculate altitude
    float absoluteAltitude = ms5611.getAltitude(realPressure);
    float relativeAltitude = ms5611.getAltitude(realPressure, referencePressure);

    presData = (String(realPressure) + '|');
    Serial.print(presData);
    altData = (String(absoluteAltitude) + ' ' +String(relativeAltitude) + '|');
    Serial.print(altData);
    tempData = (String(realPressure));
    Serial.println(tempData);
  
  // Write to SD
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(fileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    
    dataFile.print(timeStamp);
    dataFile.print(accData);
    dataFile.print(gyrData);
    dataFile.print(magData);
    dataFile.print(presData);
    dataFile.print(altData);
    dataFile.println(tempData);

    dataFile.close();
    // print to the serial port too:
    Serial.println(" -- W");
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("Error opening " + fileName);
  }

  delay(10);                   // Short delay between reads
  
}

void checkSettings()
{
  Serial.print("Oversampling: ");
  Serial.println(ms5611.getOversampling());
}

//This function takes a value and writes it to a given register at a given address
void WriteReg(byte reg, byte value, byte device){
  Wire.beginTransmission(device);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void ReadData(float data[], byte devAdd, byte startReg){
  Wire.beginTransmission(devAdd);
  Wire.write(startReg);
  Wire.endTransmission(false);
  Wire.requestFrom(devAdd,6,true);
  while (Wire.available() == 0){
  }
  for (byte i=0;i<3;i++){
    data[i] = (Wire.read())|(Wire.read()<<8);
  }
  Wire.endTransmission();
}

