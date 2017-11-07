/*
 * DAC.c
 *
 *  Created on: 6 Nov 2017
 *      Author: lrolo
 */

#include "MK64F12.h"
#include "DAC.h"
#include "PIT.h"

static const uint8 sineData[SIGNAL_DATA] = {128,136,144,152,160,167,175,182,189,196,203,209,
											215,221,226,231,236,240,243,247,249,251,253,254,
											255,255,255,254,252,250,248,245,242,238,234,229,
											224,218,213,206,200,193,186,179,171,163,156,148,
											140,132,123,115,107,99,92,84,76,69,62,55,49,42,
											37,31,26,21,17,13,10,7,5,3,1,0,0,0,1,2,4,6,8,
											12,15,19,24,29,34,40,46,52,59,66,73,80,88,95,
											103,111,119,127};

const uint8* getData(void){

	return sineData;

}

void initDAC(void)
{
	/**Enable clock gating 2 for DAC0*/
	SIM->SCGC2 |= SIM_SCGC2_DAC0(TRUE);

	/**configures DAC Control Register*/
	/**DAC enable, ready to receive data*/
	/**VDDA selected as a reference voltage*/
	DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
}


void signalValue(byte* value){

	DAC0->DAT[0].DATL = *value;

}
