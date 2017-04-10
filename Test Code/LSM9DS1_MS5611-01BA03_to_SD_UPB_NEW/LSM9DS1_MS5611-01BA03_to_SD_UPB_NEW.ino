/*




*/

//Standard Libraries
#include <Wire.h>
#include <SD.h>
#include <string.h>

// Custom Libraries
#include "lsm9ds1.h"
#include "barometer.h"

#define BUZZER 5

// Is debug mode?
//bool debug = true;

// Structures for data
struct MS5611data mData;
struct LSMData lData;

// Set CS pin to pin 
const int chipSelect = 9;   // CS Pin 10

// Other variables
char fileName[13];
int lastBaroRead; // Loop time
char logNum[3];
void setup(){
  pinMode(5,OUTPUT);
  
  //#if debug
    SerialUSB.begin(9600);
    //while (!SerialUSB)  {
      //;
    //}
  //#endif
  delay(200);
    
  // Gyro/Accel/Mag Setup
  //#if debug
    SerialUSB.println("Initializing LSM9DS1TR...");
  //#endif
  
  Wire.begin(); // Start I2C
  
  initLSM();

  //#if debug
    SerialUSB.println("LSM9DS1TR Initialized");
    SerialUSB.println("Initializing MS5611...");
  //#endif

  beep(440,400);
 
  initMS5611();

  //#if debug
    SerialUSB.println("MS5611 Initialized");
  //#endif

  beep(550,400);
 /*
  WriteReg(CTRL_REG4,0B00111000,AG_Add);     //enable gyro
  WriteReg(CTRL_REG5_XL,0B00111000,AG_Add);  //enable accelerometer
  WriteReg(CTRL_REG6_XL,0B00001000,AG_Add);  //set accelerometer scale to 16Gs
  WriteReg(CTRL_REG1_G,0B01000000,AG_Add);   //gyro/accel odr and bw
  WriteReg(CTRL_REG2_M,0B00100000,M_Add);     //set mag sensitivity to 8 gauss
  WriteReg(CTRL_REG3_M,0B00000000,M_Add);     //enable mag continuous
             
  
  // Barometer Setup
  SerialUSB.println("Initializing MS5611...");
  
  while(!ms5611.begin())
  {
    SerialUSB.println("Could not find a valid MS5611 sensor, check wiring!");
    delay(500);
  }

  // Get reference pressure for relative altitude
  referencePressure = ms5611.readPressure();

  // Check settings
  checkSettings();
  */
  //#if debug
    SerialUSB.println("MS5611 Initialized");
  //#endif
  
  // SD Setup
  //#if debug
    SerialUSB.println("Initializing SD card...");
  //#endif
  
  // Check for SD 
  if (!SD.begin(chipSelect)) {
    //#if debug
      SerialUSB.println("Card Error");
    //#endif
      errorScream();
  }
  
  //#if debug
    SerialUSB.println("SD initialized.");
  //#endif

  beep(660,400);
  
  // Make File
 //char tmp[10] =""
 for (int i = 1; i < 100; i++){ 
  fileName[0] = '\0'; // clear array
  if (i < 10) {
    strcpy(fileName,"DATALOG0");
    strncat(fileName,itoa(i,logNum,1),1);
    strncat(fileName,".txt",4);
  }
  else {
    if (i = 100) {
       errorScream();
    }
    strcpy(fileName,"DATALOG");
    strncat(fileName,itoa(i,logNum,2),2);
    strncat(fileName,".txt",4);
  }
  
  if (!SD.exists(fileName)){// If file does not exist
    break; // This will be the fileName
  } 
  //#if debug
    SerialUSB.print("File Name: ");
    SerialUSB.print(fileName);
  //#endif
  
  
  primePressureMS5611();
  delay(10);
    
  }
}

void loop() {
    static struct LSMData lData;
    static struct MS5611data mData;
    char dataString[127] = "";
    
    if ((millis() - lastBaroRead) > 10) {
      readPressureMS5611(&mData); // Take a baro reading
      calcAltitudeMS5611(&mData);

      primePressureMS5611(); // Prime baro for next reading

      lastBaroRead = millis();
    }

    readLSM(&lData); // Get 9 axis data

    createDataString(dataString, &lData, &mData);
    
    SerialUSB.print(dataString);
  
  // Write to SD
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File datalog = SD.open(fileName,FILE_WRITE);
  
  // if the file is available, write to it:
  if (datalog) {
    
    datalog.print(dataString);

    datalog.close();
    // print to the serial port too:

    //#if debug
      SerialUSB.println(" -- W");
    //#endif
    
  }
  // if the file isn't open, pop up an error:
  else {
    //#if debug
     SerialUSB.print("Error opening ");
     SerialUSB.print(fileName);
    //#endif
  }

}
/*
void checkSettings()
{
  SerialUSB.print("Oversampling: ");
  SerialUSB.println(ms5611.getOversampling());
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
*/

void beep(float hz, float dur) { //beep frequency in Hz for duration in ms (not us)
  int halfT = 500000 / hz;
  for (int i = 0; i < (500 * dur / halfT) ; i++) {
    digitalWrite(5, HIGH);
    delayMicroseconds(halfT);
    digitalWrite(5, LOW);
    delayMicroseconds(halfT);
  }
}


void errorScream(){

  while(1){

    for (int j = 440; j < 660; j++){
    
      beep(j,8);
   
    }

  }
}

void createDataString(char* dataString, struct LSMData *lData, struct MS5611data *mdata){

  char tmp[32] = ""; // temporary variable to hold characters
  // timestamp
  dataString = itoa(millis(),tmp,10);
  strcat(dataString,",");
  
  // 9 AXIS SENSOR

    // x acc
    tmp[0] = '\0';
    fToA(tmp, lData->acc[0]);
    strcat(dataString, tmp);
    strcat(dataString, ",");
    // y acc
    tmp[0] = '\0';
    fToA(tmp, lData->acc[1]);
    strcat(dataString, tmp);
    strcat(dataString, ",");
    // z acc
    tmp[0] = '\0';
    fToA(tmp, lData->acc[2]);
    strcat(dataString, tmp);
    strcat(dataString, ",");
  
    // x rot
    tmp[0] = '\0';
    fToA(tmp, lData->gyr[0]);
    strcat(dataString, tmp);
    strcat(dataString, ",");
    // y rot
    tmp[0] = '\0';
    fToA(tmp, lData->gyr[1]);
    strcat(dataString, tmp);
    strcat(dataString, ",");
    // z rot
    tmp[0] = '\0';
    fToA(tmp, lData->gyr[2]);
    strcat(dataString, tmp);
    strcat(dataString, ",");
    
    // x mag
    tmp[0] = '\0';
    fToA(tmp, lData->mag[0]);
    strcat(dataString, tmp);
    strcat(dataString, ",");
    // y mag
    tmp[0] = '\0';
    fToA(tmp, lData->mag[1]);
    strcat(dataString, tmp);
    strcat(dataString, ",");
    // z mag
    tmp[0] = '\0';
    fToA(tmp, lData->mag[2]);
    strcat(dataString, tmp);
    strcat(dataString, ",");
  
  // BAROMETER

    // pressure
    tmp[0] = '\0';
    fToA(tmp, mdata->pressure);
    strcat(dataString, tmp);
    strcat(dataString, ",");
    // temperature
    tmp[0] = '\0';
    fToA(tmp, mdata->temperature);
    strcat(dataString, tmp);
    strcat(dataString, ",");
    // altitude
    tmp[0] = '\0';
    fToA(tmp, mdata->altitude);
    strcat(dataString, tmp);
}

void fToA(char* out, float in) {
  char temp[32]; // TODO: figure out right length
  int32_t whole = (int32_t) in;    //truncate whole numbers
  int16_t frac =  (int16_t) ((float)(in - (float) whole)*10000); //remove whole part of flt and shift 5 places over
  if(frac < 0) {
    frac *= -1;
  }

  itoa(whole, temp, 10);
  strcat(out, temp);
  strcat(out, ".");
  itoa(frac, temp, 10);
  strcat(out, temp);
}
