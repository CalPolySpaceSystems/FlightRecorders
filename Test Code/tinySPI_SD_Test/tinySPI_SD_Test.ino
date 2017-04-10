//shiftOut_SD_Test
/* A proof of concept for writing to an SD card by imitating a data block of 512 bytes
/  using loops and bytes of zeros.
*/

//Includes
#include <tinySPI/tinySPI.h>


//Variables
int16_t fakeDataSize;
byte R_1,R_2,R_3,R_4

bool highCapacity = true;
int SDversion = 3;


int SDSelect = 3; //In the end I'll use an inverter to make this happen maybe.
int baroSelect = 4;

String data;

void setup() {
  // put your setup code here, to run once:

  pinMode(SDSelect,OUTPUT);
  digitalWrite(SDSelect,HIGH);

  // Begin SPI
  SPI.begin();
  setDataMode(SPI_MODE1);

  //Initialize Barometer

  //Initialize SD card

    //digitalWrite(SDSelect,HIGH); //is already high
  for i < 10{

    SPI.transfer(0xFF); //>74 Cycles of Dummy clock with DI(MOSI) High

  i++}

  R1 = SPI.transfer(0x40); //Response from sending CMD0, resets SD card
    
    if R1 <> 0X01{ // i can do this better
      //errorScream
    }
  
  SPI.transfer(0x48); //send CMD 8
  
  SPI.transfer(0x00); //Argument
  SPI.transfer(0x00);
  SPI.transfer(0x01);
  R_1 = SPI.transfer(0xAA); // Starts recieving
  R_2 = SPI.transfer(0x00);
  R_3 = SPI.transfer(0x00);
  R_4 = SPI.transfer(0x00);

  if R_1 = 0x05{ // initialize for SDCv1 or MMCv3

  
    
    
  }
  
  elseif R_1 <> 0x05 {
    
    //errorScream
  
  }

  
  if (R_3 & R_4) <> 0xAB{

    //errorScream
    
  }

 
  // Must be ok/SDC V2 rn
  
  R_1 = 0x01
  while R_1 = 0x01{
  
    SPI.transfer(0x81); // Send ACMD41

    SPI.transfer(0x40); //Argument
    SPI.transfer(0x00);
    SPI.transfer(0x01);
    R_1 = SPI.transfer(0xAA); // Starts recieving

    if (R_1 | 0x01) <> 0x00{

      //errorScream
    
    }
  }

  //accepted command

  SPI.transfer(0x98); // Send CMD58

  SPI.transfer(0x00); //Argument
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  R_1 = SPI.transfer(0x00); // Starts recieving
  R_2 = SPI.transfer(0x00);
  R_3 = SPI.transfer(0x00);
  R_4 = SPI.transfer(0x00);

  if R_1 = 0x40
  
     //Send SPI initialize command CMD8

    /*
     * 
     * IDK how to do this
     * 
     */
  if{// 0x1AA match


    
    }
  
  elseif {//no response
    
  }

  else{ // error

    //errorScream
    
  #endif
   // Send either ACMD41 or CMD1
  
}

void loop() {

  //Get data
  data = String(millis());
  fakeDataSize = 512 - (sizeof(data));
  
  //Open SD Card


    for i < 512 {
    
    
    
    
    i++
    }
  


}

void errorScream(){
  
  
  
  }
