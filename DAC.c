/**
	\file
	\brief
		This is the source file for the DAC in the Kinetis 64F. Includes the needed
		functions to use the DAC0 (initialize, enable, disable, loadValues). The
		output of the DAC, is DAC0_OUT
	\author Patricio Gomez García
	\date	22/09/2016
 */

#include "MK64F12.h"
#include "DAC.h"
#include "DataTypeDefinitions.h"


void DAC_init(){

	/*Enable the clock gating in System Clock Gating 2 for DAC0*/
	SIM_SCGC2 |= DAC0_CLOCK_GATING;

	/*Enable the DAC0_C0, and set DACREF2 as the reference voltage*/
	DAC0_C0 = DAC_ENABLE | DAC_REFERENCE_SELECT;

	/*Load an initial value of 0, in the DAC output*/
	DAC_loadValues(FALSE);
}

void DAC_enable(){
	/*Enable the DAC0_C0*/
	DAC0_C0 |= DAC_ENABLE;
}


void DAC_disable(){
	/*Disable the DAC0_C0*/
	DAC0_C0 &= ~(DAC_ENABLE);
}


void DAC_loadValues(uint16 signal_value){

	/*Loads the low part of the output voltage value; By doing a mask, of 'and' with
	 * 0x00FF, we obtain the lower part (8 bits)	*/
	DAC0_DAT0L = signal_value & (0x00FF);

	/*Loads the high part of the output voltage value; By doing a mask, of 'and' with
	 * 0x0F00, we obtain the higher part (4 bits)*/
	DAC0_DAT0H = (signal_value & (0x0F00))>>8;
}

