
/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MK64F12.h"
#include "PIT.h"
#include "NVIC.h"
#include "DAC.h"

static const uint8 sineData[SIGNAL_DATA] = {128,136,144,152,160,167,175,182,189,196,203,209,
											215,221,226,231,236,240,243,247,249,251,253,254,
											255,255,255,254,252,250,248,245,242,238,234,229,
											224,218,213,206,200,193,186,179,171,163,156,148,
											140,132,123,115,107,99,92,84,76,69,62,55,49,42,
											37,31,26,21,17,13,10,7,5,3,1,0,0,0,1,2,4,6,8,
											12,15,19,24,29,34,40,46,52,59,66,73,80,88,95,
											103,111,119,127};
uint8 destiVar=0; //defines destination data space

void DMA_init(void)
{
	//enables DMA MUX clock gate
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	//enables DMA clock gate,default is enable
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	//enables DMA MUX channel
	DMAMUX->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_TRIG_MASK | DMAMUX_CHCFG_SOURCE(60);
	//enables DMA0 request
	DMA0->ERQ = DMA_ERQ_ERQ0_MASK;
	NVIC_enableInterruptAndPriority(DMA_CH0_IRQ,PRIORITY_5);
	EnableInterrupts;
}


void DMA_start(void)
{
	 /*DMA Controller initialization*/
	DMA0->TCD[0].SADDR = (uint32_t)(&sineData[0]);//defines source data address
	DMA0->TCD[0].SOFF = 1;//Source address signed offset
	DMA0->TCD[0].DADDR = (uint32_t)(&destiVar);//defines destination data address
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(100);//CITER=1
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(1);//BITER=1
	DMA0->TCD[0].NBYTES_MLNO = 1;//byte number
	DMA0->TCD[0].DOFF = 0x00;//destination address signed offset
	DMA0->TCD[0].ATTR = DMA_ATTR_SSIZE(0) | DMA_ATTR_DSIZE(0);//8 bit transfer size, register default value is undefined
	DMA0->TCD[0].SLAST = -0x64;//restores the source address to the initial value
	DMA0->TCD[0].DLAST_SGA = 0;//restores the destination address to the initial value
	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;//The end-of-major loop interrupt is enabled
}

void DMA0_IRQHandler(void)
{
	/**Configuration needed to run DMA*/
	DMA0->INT =DMA_INT_INT0_MASK;//Interrupt Request Register
	DMA0->CDNE = DMA_CDNE_CDNE(1);//Clear DONE Status Bit Register
	DMA_start();
	PIT_delay(PIT_0, SYSTEMCLOCK, 0.00002);
}

int main(void)
{

	DMA_init();
	DMA_start();
	PIT_clockGating();

	initDAC();
	PIT_delay(PIT_0, SYSTEMCLOCK, 0.00002);
	/* This for loop should be replaced. By default this loop allows a single stepping. */
    for (;;)
    {
    	signalValue(destiVar);
    }
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
