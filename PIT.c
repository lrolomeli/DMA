/*
 * PIT.c
 *
 *  Created on: 16 Sep 2017
 *      Author: lrolo
 */

#include "MK64F12.h"
#include "PIT.h"
#include "DataTypeDefinitions.h"

PIT_interruptFlags_t PIT_intrStatusFlag;

void PIT0_IRQHandler(void)
{
	//turnOnGPIOIntrStatusFlag();
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF(TRUE);			//cleans the interrupt flag of timer0
	PIT->CHANNEL[0].TCTRL; 								//read control register for clear PIT flag, this is silicon bug
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE(TRUE);		//enables PIT timer interrupt
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN(TRUE);		//enables timer0

}

void PIT_delay(PIT_Timer_t pitTimer,float systemClock ,float period)
{
	volatile uint32_t hexaperiod = (uint32_t)((period*systemClock)/2)-1; 	//EL PERIODO EN EL MAIN ES DE 250 ms POR LO TANTO HAY QUE DIVIDIR (250ms/Periodo del reloj) RESTARLE 1 Y ASIGNARLO A LDVAL.
	PIT->MCR = PIT_MCR;														//ENABLE TIMERS OF THE PIT
	PIT->CHANNEL[pitTimer].LDVAL = PIT_LDVAL_TSV(hexaperiod);				//CHARGES THE HEX VALUE TO LDVAL
	PIT->CHANNEL[pitTimer].TCTRL = PIT_TCTRL_TIE(TRUE);						//ENABLE INTERRUPT OF TIMER 1
	PIT->CHANNEL[pitTimer].TCTRL = PIT_TCTRL_TEN(TRUE);						//ENABLE TIMER 1 OR STARTS TIMER 1

}

void PIT_clockGating(void)
{
	SIM->SCGC6 |= PIT_CLOCK_GATING;		// ENABLE CLOCK GATING TO USE PERIODIC INTERRUPT TIMER
}


uint8 PIT_getIntrStatus(void)
{

	// INDEPENDIENTEMENTE DE SI HUBO UNA INTERRUPCION O NO DEL TIMER ESTA FUNCION TIENE QUE REGRESAR EL ESTADO
	// BANDERA DE INTERRUPCION DE
	return PIT->CHANNEL[0].TFLG;
}

void PIT_clear(void)   //This function clears flags used for PIT module
//
{
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF(TRUE);
}
