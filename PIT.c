/**
	\file
	\brief
		This is the source file for the PIT in Kinetis 64F, it has the functions needed
		to enable any PIT channel, and load values to any PIT channel.
	\author Patricio Gomez García
	\date	13/09/2016
 */

#include "DataTypeDefinitions.h"
#include "PIT.h"
#include "WVGN.h"
#include "PSSWRD.h"
#include "MTRCTRL.h"


void PIT_clockGating(){
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
}

void PIT_enable(){
	PIT_MCR  &= ~(PIT_MCR_FRZ_MASK);
	PIT_MCR  &= ~(PIT_MCR_MDIS_MASK);
}

void PIT_disable(){
	PIT_MCR  |= PIT_MCR_FRZ_MASK;
	PIT_MCR  |= PIT_MCR_MDIS_MASK;
}

void PIT_timerInterruptEnable(PIT_TimerType pitTimer){
		switch(pitTimer){
		case PIT_0:
			PIT_TCTRL0 |= (PIT_TCTRL_TIE_MASK);
			break;
		case PIT_1:
			PIT_TCTRL1 |= (PIT_TCTRL_TIE_MASK);
			break;
		case PIT_2:
			PIT_TCTRL2 |= (PIT_TCTRL_TIE_MASK);
			break;
		case PIT_3:
			PIT_TCTRL3 |= (PIT_TCTRL_TIE_MASK);
			break;
	}
}

void PIT_timerInterruptDisable(PIT_TimerType pitTimer){
		switch(pitTimer){
		case PIT_0:
			PIT_TCTRL0 &= ~(PIT_TCTRL_TIE_MASK);
			break;
		case PIT_1:
			PIT_TCTRL1 &= ~(PIT_TCTRL_TIE_MASK);
			break;
		case PIT_2:
			PIT_TCTRL2 &= ~(PIT_TCTRL_TIE_MASK);
			break;
		case PIT_3:
			PIT_TCTRL3 &= ~(PIT_TCTRL_TIE_MASK);
			break;
	}
}

void PIT_timerEnable(PIT_TimerType pitTimer){
		switch(pitTimer){
		case PIT_0:
			PIT_TCTRL0 |= (PIT_TCTRL_TEN_MASK);
			break;
		case PIT_1:
			PIT_TCTRL1 |= (PIT_TCTRL_TEN_MASK);
			break;
		case PIT_2:
			PIT_TCTRL2 |= (PIT_TCTRL_TEN_MASK);
			break;
		case PIT_3:
			PIT_TCTRL3 |= (PIT_TCTRL_TEN_MASK);
			break;
	}
}

void PIT_timerDisable(PIT_TimerType pitTimer){
		switch(pitTimer){
		case PIT_0:
			PIT_TCTRL0 &= ~(PIT_TCTRL_TEN_MASK);
			break;
		case PIT_1:
			PIT_TCTRL1 &= ~(PIT_TCTRL_TEN_MASK);
			break;
		case PIT_2:
			PIT_TCTRL2 &= ~(PIT_TCTRL_TEN_MASK);
			break;
		case PIT_3:
			PIT_TCTRL3 &= ~(PIT_TCTRL_TEN_MASK);
			break;
	}
}

void PIT_delay(PIT_TimerType pitTimer,float systemClock ,float period){

	/*specified formula to obtain the delay*/
	float delay = (((period*(systemClock))/2)-1);
	switch(pitTimer){
		case PIT_0:
			PIT_LDVAL0 = (uint32)delay;
			break;
		case PIT_1:
			PIT_LDVAL1 = (uint32)delay;
			break;
		case PIT_2:
			PIT_LDVAL2 = (uint32)delay;
			break;
		case PIT_3:
			PIT_LDVAL3 = (uint32)delay;
			break;
	}

}

void PIT0_clearInterrupt(){
	/*Clear interruption flag for PIT channel 0*/
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;
	PIT_TCTRL0;
	/*Enable the timer again*/
	PIT_timerInterruptEnable(PIT_0);
	PIT_timerEnable(PIT_0);
}

void PIT1_clearInterrupt(){
	/*Clear interruption flag for PIT channel 0*/
	PIT_TFLG1 |= PIT_TFLG_TIF_MASK;
	PIT_TCTRL1;
	/*Enable the timer again*/
	PIT_timerInterruptEnable(PIT_1);
	PIT_timerEnable(PIT_1);
}

void PIT3_clearInterrupt(){
	/*Clear interruption flag for PIT channel 3*/
	PIT_TFLG3 |= PIT_TFLG_TIF_MASK;
	PIT_TCTRL3;
	/*Enable the timer again*/
	PIT_timerInterruptEnable(PIT_3);
	PIT_timerEnable(PIT_3);
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function attends the PIT channel 0 interruption, it clears the interruption
 	 	 flags, and has a functionality according to this project
 	 \return void
 */
void PIT0_IRQHandler(){
	PIT0_clearInterrupt();
	/*project functionality added to the PIT channel 0 interruption*/
	WAVEGEN_indexShifting();
 }

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function attends the PIT channel 1 interruption, it clears the interruption
 	 	 flags, and has a functionality according to this project
 	 \return void
 */
void PIT1_IRQHandler(){
	PIT1_clearInterrupt();
	/*project functionality added to the PIT channel 0 interruption*/
	MOTORCONTROL_behaviorChange();
 }

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function attends the PIT channel 3 interruption, it clears the interruption
 	 	 flags, and has a functionality according to this project
 	 \return void
 */
void PIT3_IRQHandler(){
	PIT3_clearInterrupt();
	/*project functionality added to the PIT channel 3 interruption*/
	PASSWORD_ledCorrectPassword();
}

uint32 PIT_readTimerValue(PIT_TimerType pitTimer){
	/*According to the pit Timer, we return the value of the counter in the requested
	 * channel*/
	switch(pitTimer){
	case PIT_0:
		return PIT_CVAL0;
	case PIT_1:
		return PIT_CVAL1;
	case PIT_2:
		return PIT_CVAL2;
	case PIT_3:
		return PIT_CVAL3;
	}
	return FALSE;
}
