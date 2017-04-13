#include <Arduino.h>
#include <Wire.h>
#include "lsm9ds1.h"

#define LSM_ADR (0B1101011)
#define LSM_MAG_ADR (0B0011110)

#define LSM_CTRL_REG4    (0b00011110)
#define LSM_CTRL_REG5_XL (0b00011111)
#define LSM_CTRL_REG6_XL (0b00100000)
#define LSM_CTRL_REG1_G  (0b00010000)
#define LSM_CTRL_REG2_M  (0b00100001)
#define LSM_CTRL_REG3_M  (0b00100010)
#define LSM_WHO_AM_I     (0b00001111)

#define LSM_ACC_START  (0b00101000)
#define LSM_GYRO_START (0B00011000)
#define LSM_MAG_START  (0b00101000)
#define LSM_TS_START   (0b00010101)

#define LSM_GYRO_FACT (0.00875f)
#define LSM_ACC_FACT (0.000732f)
#define LSM_MAG_FACT (0.00029f)

static void writeRegister(uint8_t dev, uint8_t reg, uint8_t value) {
	Wire.beginTransmission(dev);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission();
}

static void readRegister(uint8_t dev, uint8_t startReg, uint8_t len, int8_t *data) {
	Wire.beginTransmission(LSM_ADR);
	Wire.write(startReg);
	Wire.endTransmission();

	// request chuck of data and continue reading after start register.
	Wire.requestFrom(dev, len);

	// wait for data
	while(Wire.available() == 0){};

	for(int i = 0; i < len; i++) {
		data[i] = Wire.read();
	}
}

// Initializes LSM by setting registers to desired values
void initLSM(){
	writeRegister(LSM_ADR, LSM_CTRL_REG4, 0b00111000);       //enable gyro
	writeRegister(LSM_ADR, LSM_CTRL_REG5_XL,0b00111000);     //enable accelerometer
	writeRegister(LSM_ADR, LSM_CTRL_REG6_XL, 0b00001000);    //set accelerometer scale to 16Gs
	writeRegister(LSM_ADR, LSM_CTRL_REG1_G, 0b01000000);     //gyro/accel odr and bw
	writeRegister(LSM_MAG_ADR, LSM_CTRL_REG2_M, 0b00100000); //set mag sensitivity to 8 gauss
	writeRegister(LSM_MAG_ADR, LSM_CTRL_REG3_M, 0b00000000); //enable mag continuous
}


// Writes new data to lsm struct
void readLSM(struct LSMData *out){
	int8_t gyro[6];
	readRegister(LSM_ADR, LSM_GYRO_START, 6, gyro);

	for(int i = 0; i < 3; i++) {
		int16_t gyr = gyro[2*i+1];
		gyr = gyr << 8;
		gyr |= (uint16_t) gyro[2*i];
		out->gyr[i] = (float) gyr * LSM_GYRO_FACT;
	}

	int8_t accel[6];
	readRegister(LSM_ADR, LSM_ACC_START, 6, accel);

	for(int i = 0; i < 3; i++) {
		int16_t acc = accel[2*i+1];
		acc = acc << 8;
		acc |= (uint16_t) accel[2*i];
		out->acc[i] = (float) acc * LSM_ACC_FACT;
	}

	int8_t mag[6];
	readRegister(LSM_MAG_ADR, LSM_MAG_START, 6, mag);

	for(int i = 0; i < 3; i++) {
		int16_t mg = mag[2*i+1];
		mg = mg << 8;
		mg |= (uint16_t) mag[2*i];
		out->mag[i] = (float) mg * LSM_MAG_FACT;
	}

	int8_t temp[2];
	readRegister(LSM_ADR, LSM_TS_START, 2, temp);
	int16_t tmp = temp[1];
	tmp = tmp << 8;
	tmp |= (uint16_t) temp[0];
	out->temp = ((float) tmp/16) + 25;
}

// Returns a string intended for debug logging LSM data;
String lsmToString(struct LSMData *data) {
	String out = "Gyr: X ";
	out += String(data->gyr[0]);
	out += " Y ";
	out += String(data->gyr[1]);
	out += " Z  ";
	out += String(data->gyr[2]);
	out += "\n";
	out += "Acc: X ";
	out += String(data->acc[0]);
	out += " Y ";
	out += String(data->acc[1]);
	out += " Z  ";
	out += String(data->acc[2]);
	out += "\n";
	out += "Mag: X ";
	out += String(data->mag[0]);
	out += " Y ";
	out += String(data->mag[1]);
	out += " Z  ";
	out += String(data->mag[2]);
	out += "\n";
	out += "Temp: ";
	out += String(data->temp);
	out += "\n";
	return out;
}

