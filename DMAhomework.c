
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
}


void DMA_start(void)
{
	 /*DMA Controller initialization*/
	DMA0->TCD[0].SADDR = (uint32_t)(getData());//defines source data address
	DMA0->TCD[0].SOFF = 0x01;//Source address signed offset
	DMA0->TCD[0].DADDR = (uint32_t)(&destiVar);//defines destination data address
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(1);//CITER=1
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(1);//BITER=1
	DMA0->TCD[0].NBYTES_MLNO = 0x01;//byte number
	DMA0->TCD[0].DOFF = 0x00;//destination address signed offset
	DMA0->TCD[0].ATTR = DMA_ATTR_SSIZE(0) | DMA_ATTR_DSIZE(0);//8 bit transfer size, register default value is undefined
	DMA0->TCD[0].SLAST = -0x01;//restores the source address to the initial value
	DMA0->TCD[0].DLAST_SGA = 0x00;//restores the destination address to the initial value
	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;//The end-of-major loop interrupt is enabled
}

void DMA0_IRQHandler(void)
{
	/**Configuration needed to run DMA*/
	//DMA0->INT =DMA_INT_INT0_MASK;//Interrupt Request Register
	//DMA0->CDNE = DMA_CDNE_CDNE(1);//Clear DONE Status Bit Register
	DMA_start();
	signalValue(&destiVar);
	PIT_delay(PIT_0, SYSTEMCLOCK, .001);
}

int main(void)
{
	PIT_clockGating();

	initDAC();
	DMA_init();
	DMA_start();


	NVIC_enableInterruptAndPriority(DMA_CH0_IRQ,PRIORITY_5);
	EnableInterrupts;

	PIT_delay(PIT_0, SYSTEMCLOCK, .001);

	/* This for loop should be replaced. By default this loop allows a single stepping. */
    for (;;)
    {

    }
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
