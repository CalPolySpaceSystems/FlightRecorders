// coats.cpp

#include "coats.h"

coats::coats(uint16_t ending, bool counterEnable)
{
	endString = ending;
	timerEnable = counterEnable;
}

/*
 *	UART Interface
 */

void coats::serialInit(HardwareSerial& serialInst, long baud)
{
	//create Serialport SerialCOATS
	SerialCOATS = &serialInst;
	SerialCOATS->begin(baud);
	
}

void coats::serialWriteTlm(uint8_t id){

	if (timerEnable) {
    //rtc = real ttime counter
		uint16_t rtc = (uint16_t)(millis());
		SerialCOATS->write(id);
		SerialCOATS->write(rtc);
		SerialCOATS->write(rtc >> 8);
		SerialCOATS->write((byte *)packetPointers[id], packetSizes[id]);
		SerialCOATS->write(endString >> 8);
		SerialCOATS->write(endString); 
	}

  //this is for if timer is not enabled, this is not being used, dont use this
	else{
		SerialCOATS->write(id);
		SerialCOATS->write((byte *)packetPointers[id], packetSizes[id]);
		SerialCOATS->write(endString >> 8);
		SerialCOATS->write(endString); 
	}
	
}

/*
 *	SPI Interface
 */
 
/*
 *	I2C Interface
 */
		 
/*
 *	Other Functions
 */ 

void coats::addTlm(uint8_t id, uint32_t * data, size_t dataSize){
	
	packetSizes[id] = dataSize;
	packetPointers[id] = data;

}


//buildtelemetry: use if you want to output as string rather than send through serial port
void coats::buildTlm(uint8_t id, String packet)
{
	size_t dataSize = packetSizes[id];
	
	if (timerEnable) {
		uint16_t rtc = (uint16_t)(millis());
		packet[0] = id;
		packet[1] = rtc;
		packet[2] = (rtc >> 8);
		for (int i=0;i<dataSize;i++)
		{
			packet[i+3] = *(packetPointers[id] + 8*i);
		}

		packet[dataSize+3] = (endString >> 8);
		packet[dataSize+4] = (endString);
	}
	
	else{
		packet[0] = id;
		for (int i=0;i<dataSize;i++)
		{
			packet[i+1] = *(packetPointers[id] + 8*i);
		}

		packet[dataSize+1] = (endString >> 8);
		packet[dataSize+2] = (endString); 
	}

}
