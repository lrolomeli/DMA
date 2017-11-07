/*
 * PIT.h
 *
 *  Created on: 16/08/2017
 *      Author: jlpe
 */

#ifndef PIT_H_
#define PIT_H_

#include "DataTypeDefinitions.h"



typedef struct
{
	uint8 flagPitT0 : 1;
	uint8 flagPitT1 : 1;
	uint8 flagPitT2 : 1;
	uint8 flagPitT3 : 1;

} PIT_interruptFlags_t;

#define SYSTEMCLOCK 21000000
#define DELAY .2F
/** Constant that represent the clock enable for PIT */
#define PIT_CLOCK_GATING 0x800000

/** Constant that configures the timers of PIT */
#define PIT_MCR    0x00

/*! This enumerated constant are used to select the PIT to be used*/
typedef enum {PIT_0,PIT_1,PIT_2,PIT_3} PIT_Timer_t;



void PIT0_IRQHandler(void);
void PIT1_IRQHandler(void);
void PIT2_IRQHandler(void);
void PIT3_IRQHandler(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function configure the PIT to generate a delay base on the system clock.
 	 Internally it configures the clock gating and enables the PIT module.
 	 It is important to note that this strictly is not device driver since everything is
 	 contained in a single function, but in general you have to avoid this practices, this only
 	 for the propose of the homework

 	 \param[in]  portName Port to be configured.
 	 \return void
 */
void PIT_delay(PIT_Timer_t pitTimer,float systemClock ,float period);

void PIT_clockGating(void);

uint8 PIT_interruptFlagStatus(void);

uint8 PIT_getIntrStatus(void);

void PIT_clear(void);


#endif /* PIT_H_ */
