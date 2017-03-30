//shiftOut_SD_Test
/* A proof of concept for writing to an SD card by imitating a data block of 512 bytes
/  using loops and bytes of zeros.
*/

//Includes
#include <tinySPI/tinySPI.h>


//Variables
int16_t fakeDataSize;

int chipSelect = 10;
int MOSI = 11;
int MISO = 12;
int CLK = 13;

String data;

void setup() {
  // put your setup code here, to run once:

pinMode(chipSelect,OUTPUT);
digitalWrite(chipSelect,HIGH);

pinMode(MOSI,OUTPUT);
digitalWrite(MOSI,LOW);

pinMode(MISO,INPUT);

pinMode(CLK,OUTPUT);
digitalWrite(CLK,LOW);

//initialize SD card
delay(5);
  for i < 10{
  shiftOut(MOSI,CLK,LSBFIRST,0xFF);
  i++
  }
  
digitalWrite(chipSelect,LOW);



}

void loop() {

  //Get data
  data = String(millis());
  fakeDataSize = 512 - (sizeof(data));
  

    for i < 512 {
    
    
    
    
    i++
    }
  


}
