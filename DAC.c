/*
 * DAC.c
 *
 *  Created on: 6 Nov 2017
 *      Author: lrolo
 */

#include "MK64F12.h"
#include "DAC.h"
#include "PIT.h"



void initDAC(void)
{
	/**Enable clock gating 2 for DAC0*/
	SIM->SCGC2 |= SIM_SCGC2_DAC0(TRUE);

	/**configures DAC Control Register*/
	/**DAC enable, ready to receive data*/
	/**VDDA selected as a reference voltage*/
	DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
}


void signalValue(byte value){

	DAC0->DAT[0].DATL = value;

}
