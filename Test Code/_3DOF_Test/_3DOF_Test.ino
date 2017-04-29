
#include <Wire.h>

// Address
const byte A3G_R_ADD = 0b1101001;
const byte A3G_W_ADD = 0b1101001;

//CTRL REGS
const byte A3G_CTRL_REG1 = 0x20;
const byte A3G_CTRL_REG2 = 0x21;
const byte A3G_CTRL_REG3 = 0x22;
const byte A3G_CTRL_REG4 = 0x23;
const byte A3G_CTRL_REG5 = 0x24;

// END REgs
const byte A3G_OUT_TEMP = 0x26;
const byte A3G_STATUS_REG = 0x27;
const byte A3G_OUT_START = 0x28;

typedef union Sense
{
  float num[3];
  byte byt[12];
}sense;

sense gyr;
String dataString;


void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(9600);

  Wire.begin();
  writeReg(A3G_CTRL_REG1,0b00001111,A3G_W_ADD); //Enable Gyro, Set Bandwidth
}

void loop() {
  // put your main code here, to run repeatedly:
  
  readData(gyr.num,A3G_R_ADD,A3G_OUT_START);

  for (byte i=0;i<3;i++){
    gyr.num[i] *= .001;
  }

  dataString = (String(gyr.num[0]) + ',' + String(gyr.num[1]) + ',' + String(gyr.num[2]) + ',');
  SerialUSB.println(dataString);
    
}

/*
void readA3G(float data[], byte deviceAddress, byte dataAddress{
  
  for (byte i=0;i<9;i++){
    Wire.beginTransmission(deviceAddress);
    Wire.write(dataAddress);
    Wire.endTransmission(false);
    
    Wire.requestFrom(deviceAddress, 1,true);
    while(Wire.available() == 0){}  

    data[i] = Wire.read()
     
    startRegister += 0b1;
  }
    
}
*/

//This function takes a value and writes it to a given register at a given address
void writeReg(byte reg, byte value, byte device){
  Wire.beginTransmission(device);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void readData(float data[], byte devAdd, byte startReg){
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

