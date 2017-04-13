/*
  Patrick Chizek
  MicroRecorder
  2017
*/

//Standard Libraries
#include <Wire.h>
#include <SD.h>
#include <string.h>

// Custom Libraries
#include "lsm9ds1.h"
#include "barometer.h"

#define BUZZER 5
#define LED 13
#define DEBUG 1

// Is debug mode?
bool debug = true;

// Structures for data
struct MS5611data mData;
struct LSMData lData;

// Set CS pin to pin 
const int chipSelect = 9;   // CS Pin 9

// Other variables
char fileName[11];
int lastBaroRead; // Loop time
char logNum[2];
void setup(){
//unsigned long timeStamp;
  
  pinMode(BUZZER,OUTPUT);
  pinMode(LED,OUTPUT);
  
  
    //SerialUSB.begin(9600);
    //while (!SerialUSB)  {
      //;
    //}

  delay(200);
    
  // Gyro/Accel/Mag Setup
  
    //SerialUSB.println("Initializing LSM9DS1TR...");
  
  Wire.begin(); // Start I2C
  
  initLSM();

    //SerialUSB.println("LSM9DS1TR Initialized");
    //SerialUSB.println("Initializing MS5611...");

  beep(440,400);
 
  initMS5611();

    //SerialUSB.println("MS5611 Initialized");

  beep(550,400);
   
  // SD Setup
    //SerialUSB.println("Initializing SD card...");

  // Check for SD 
  if (!SD.begin(chipSelect)) {
      //SerialUSB.println("Card Error");
      errorScream();
  }

    //SerialUSB.println("SD initialized.");

  beep(660,400);
  
  // Make File
  for (int i = 1; i < 100; i++){ 
    fileName[0] = '\0'; // clear array
    logNum[0] = '\0';
    if (i < 10) {
      strcpy(fileName,"data");
      //Serial.println(String(i));
      strcat(fileName,"0");
      itoa(i,logNum,10);
      strcat(fileName,logNum);
      strcat(fileName,".txt");
    } 
    else {
      //Serial.println(String(i));
      if (i > 99) {
       //SerialUSB.println("Card full or file error.");
       errorScream();
      }
      strcpy(fileName,"data");
      //Serial.println(String(i));
      itoa(i,logNum,10);
      strcat(fileName,logNum);
      strcat(fileName,".txt");
      //Serial.println(fileName);
    }
  
    if (!SD.exists(fileName)){// If file does not exist
      break; // This will be the fileName
    } 

  }

  //SerialUSB.print("File Name: ");
  //SerialUSB.println(fileName);

  // Prime barometer for first read
  primePressureMS5611();
  lastBaroRead = millis();
  delay(10);
    
}


void loop() {
    static struct LSMData lData;
    static struct MS5611data mData;
    char dataString[127] = "";

    if ((millis() - lastBaroRead) > 10) {
      readPressureMS5611(&mData); // Take a baro reading
      //readTempMS5611(&mData);
      mData.temperature = lData.temp; // Update temperature
      
      calcAltitudeMS5611(&mData);

      primePressureMS5611(); // Prime baro for next reading
      //primeTempMS5611();
      
      lastBaroRead = millis();
    }

    readLSM(&lData); // Get 9 axis data

    
    
    createDataString(dataString, &lData, &mData);
    fileName[10] = '\0';
    //SerialUSB.print(String(dataString));
  // Write to SD
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
    File datalog = SD.open(fileName,FILE_WRITE);
      
    // if the file is available, write to it:
    if (datalog) {

      digitalWrite(LED,HIGH);
    
      datalog.println(dataString);
      datalog.close();

      digitalWrite(LED,LOW);
      // print to the serial port too:

      //SerialUSB.println(" -- W");
    
    }
  // if the file isn't open, pop up an error:
    else {
      //#if debug
      //SerialUSB.print("Error opening ");
      //SerialUSB.println(fileName);
      errorScream();
      
      //#endif
    }

}

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

  dataString[0] = '\0';
    
  char tmp[32] = ""; // temporary variable to hold characters
  // timestamp
  long timeMs = millis();
  String timeStamp = String(timeMs);
  tmp[0] = '\0';
  timeStamp.toCharArray(tmp,32);
  
  //itoa(timeStamp, tmp, 10);
  strcat(dataString,tmp);
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
