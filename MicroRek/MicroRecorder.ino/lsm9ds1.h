#ifndef _LSM9DS1_H_
#define _LSM9DS1_H_

#include <Arduino.h>

struct LSMData {
	//accelerometer data
	float acc[3];
	//gyroscope data
	float gyr[3];
	//magnetometer data
	float mag[3];
	//temperature data
	float temp;
};

void readLSM(struct LSMData *out);
void initLSM();
String lsmToString(struct LSMData *data);
#endif

