
#include <Wire.h>

// Address
#define A3G_DEVICE_ADD (0b1101001)

//CTRL REGS
const byte A3G_CTRL_REG1 = 0x20;
const byte A3G_CTRL_REG2 = 0x21;
const byte A3G_CTRL_REG3 = 0x22;
const byte A3G_CTRL_REG4 = 0x23;
const byte A3G_CTRL_REG5 = 0x24;

// END REgs
#define A3G_OUT_TEMP   (0x26)
#define A3G_STATUS_REG (0x27)
#define A3G_OUT_X (0x28)
#define A3G_OUT_Y (0x2A)
#define A3G_OUT_Z (0x2C)
#define STAT_ZYXOR (0x80)

double A3GData[3] = {0};

String dataString;


void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(9600);

  Wire.begin();
  writeReg(A3G_CTRL_REG1,0b00111111,A3G_DEVICE_ADD); //Enable Gyro, Set Bandwidth
  writeReg(A3G_CTRL_REG2,0b00000011,A3G_DEVICE_ADD);
}

void loop() {
  // put your main code here, to run repeatedly:

  
  A3GData[0] = readA3G('x');
  A3GData[1] = readA3G('y');
  A3GData[2] = readA3G('z');
 
  dataString = (String(A3GData[0]) + ", " + String(A3GData[1]) + ", " + String(A3GData[2]) + ';');
  SerialUSB.println(dataString);

  if (checkRegister(A3G_STATUS_REG,STAT_ZYXOR,A3G_DEVICE_ADD)){
    SerialUSB.println("An overwrite occured!");
  }
  
}

//This function takes a value and writes it to a given register at a given address
void writeReg(byte reg, byte value, byte device){
  Wire.beginTransmission(device);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

double readA3G(char axis){
  int16_t rawData;
  for (byte i=0;i<2;i++){
   
    Wire.beginTransmission(A3G_DEVICE_ADD);   
    switch(axis){
      case 'x':  
        Wire.write(A3G_OUT_X + (1-i));
      case 'y': 
        Wire.write(A3G_OUT_Y + (1-i));
      case 'z':
        Wire.write(A3G_OUT_Z + (1-i));
      default:
        break;
    }
    Wire.endTransmission(false);
    
    Wire.requestFrom(A3G_DEVICE_ADD,1,true);
    while (Wire.available() == 0);
    
    rawData |= (Wire.read() << 8*(1-i));
    
  }

  //(rawData * 0.00875f); 
  //out *= 0.00875;
  return ((double)rawData * 0.007477);
}

bool checkRegister(byte targetRegister, byte expectedValue, byte deviceAddress){
  Wire.beginTransmission(deviceAddress);
  Wire.write(targetRegister);
  Wire.endTransmission(false);

  Wire.requestFrom(deviceAddress,1,true);
  while(Wire.available() == 0);
  
  byte regValue = Wire.read();

  if ((regValue & expectedValue) > 0) {
  return true;  
  }

  else{
  return false;
  }
  
}

