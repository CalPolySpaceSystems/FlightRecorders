#ifndef _BARO_H
#define _BARO_H

#include <Arduino.h>

struct MS5611data {
	float temperature;
	float pressure;
	float altitude;
};

// initialize MS5611 into operating state and collect calibration data
void initMS5611(void);

/*--- Begin Temperature Section ---*/

// Primes MS5611 to organize temperature data for reading
void primeTempMS5611(void);

/* Reads MS5611 temperature data into MS5611data struct
 * Assumptions:
 *     MS5611 primed for temperature
 *     10 ms delay after priming
 */
void readTempMS5611(struct MS5611data *data);

/*--- End Temperature Section ---*/


/*--- Begin Pressure Section ---*/

// Primes MS5611 to orgainze pressure data for reading
void primePressureMS5611(void);

/* Reads MS5611 pressure data into MS5611data struct
 * Assumptions:
 *    MS5611 primed for pressure
 *    10 ms delay after priming
*/
void readPressureMS5611(struct MS5611data *data);

/*--- End Pressure Section ---*/


// Calculate altitude using data from MS5611data struct
void calcAltitudeMS5611(struct MS5611data *data);

// Convert MS5611 struct data to string for output
String ms5611ToString(struct MS5611data *data);

/* Reads the raw values based on the previous priming method (Temp or Pressure)
 * ONLY CALL THIS IF YOU MUST!
 * IT IS ALREADY IMPLEMENTED IN readTempMS6511 and readPressureMS5611
*/
uint32_t readRawMS5611();


#endif

