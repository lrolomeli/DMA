/*
 * DAC.h
 *
 *  Created on: 6 Nov 2017
 *      Author: lrolo
 */

#ifndef DAC_H_
#define DAC_H_

#include "DataTypeDefinitions.h"


#define SIGNAL_DATA 100

const uint8* getData(void);
void initDAC(void);
void signalValue(byte value);

#endif /* DAC_H_ */
