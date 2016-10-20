/**
	\file
	\brief
		This is the source file for a wave or signal, generation process, which uses
		different interruptions, and depends on other processes.
	\author Patricio Gomez García
	\date	20/09/2016
 */

#include "WVGN.h"
#include "GPIO.h"
#include "NVIC.h"
#include "PIT.h"
#include "DAC.h"
#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#include "GlobalFunctions.h"

/*System clock 21MHz*/
#define SYSTEM_CLOCK 21000000
/*Needed delay to show the samples of a sine/square/triangle signal of 5Hz*/
#define DELAY 0.004878*2

/*Constant array containing the values of a period of a square signal of 5Hz, this values will be loaded in the
 * DAC*/
static const uint16 squareSignalValues[41] = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4095};
/*Constant array containing the values of a period of a sine signal of 5Hz, this values will be loaded in the
 * DAC*/
static const uint16 sineSignalValues[41] = {2048, 2368, 2680, 2977, 3251, 3495, 3704, 3872, 3995, 4070, 4095, 4070, 3995, 3872, 3704, 3495, 3251, 2977, 2680, 2368, 2048, 1727, 1415, 1118, 844, 600, 391, 223, 100, 25, 0, 25, 100, 223, 391, 600, 844, 1118, 1415, 1727, 2047};
/*Constant array containing the values of a period of a triangle signal of 5Hz, this values will be loaded in the
 * DAC*/
static const uint16 triangleSignalValues[41] = {0, 205, 409, 614, 819, 1024, 1228, 1433, 1638, 1843, 2048, 2252, 2457, 2662, 2867, 3071, 3276, 3481, 3685, 3890, 4095, 3890, 3685, 3481, 3276, 3071, 2866, 2662, 2457, 2252, 2047, 1843, 1638, 1433, 1229, 1024, 819, 614, 409, 205, 0};

/*
 * Linked State machine, with three states (SQUARE, SINE, TRIANGLE), each state contains the next state direction
 * the function WAVEGEN_ledSequence, the direction of the array containing the values to be loaded to the DAC, and the
 * LED1 and LED2 status
 */
static waveGeneratorState waveGenState[3] = {
		{SINE_SIGNAL,WAVEGEN_ledSequence,WAVEGEN_changeSequence,SQUARE_SIGNAL_INDEX,BIT_OFF,BIT_ON},
		{TRIANGLE_SIGNAL,WAVEGEN_ledSequence,WAVEGEN_changeSequence,SINE_SIGNAL_INDEX,BIT_ON,BIT_OFF},
		{SQUARE_SIGNAL,WAVEGEN_ledSequence,WAVEGEN_changeSequence,TRIANGLE_SIGNAL_INDEX,BIT_ON,BIT_ON}
};

/*currentState, will indicate to this file, what values to take to the DAC, which LED configuration to take, etc.*/
static waveGeneratorState* currentState;
/*index_shift, will shift the index in the arrays containing the values to be loaded in the DAC, to generate the
 * desired signal*/
uint8 index_shift = 0;


void WAVEGEN_init(){

	/*Enables the clock gating for PORT A, in order to use the SW3*/
	GPIO_clockGating(GPIOA);
	/*Sets the configuration needed to use the SW3*/
	GPIO_pinControlRegisterType pinControlRegisterPORTA = GPIO_MUX1|GPIO_PS|GPIO_PE|INTR_FALLING_EDGE;
	/*Sets PORT A pin 4, with the functionality of MUX_1, as a pull up resistor, pull enable, and the interruption
	 * actives with the falling edge*/
	GPIO_pinControlRegister(GPIOA,BIT4,&pinControlRegisterPORTA);
	/*Sets PORT A pin 4 as an input (SW3)*/
	GPIO_dataDirectionPIN(GPIOA,GPIO_INPUT,BIT4);

	/*Enables the clock gating for PORT C, in order to use pin 10 and 11 as outputs for LEDS 1 and 2*/
	GPIO_clockGating(GPIOC);
	/*Sets the configuration needed to use the PORTC pins as MUX1*/
	GPIO_pinControlRegisterType pinControlRegisterPORTC = GPIO_MUX1;
	/*Sets PORTC pin 10, with the funcionality of MUX_1*/
	GPIO_pinControlRegister(GPIOC,BIT10,&pinControlRegisterPORTC);
	/*Sets PORTC pin 11, with the funcionality of MUX_1*/
	GPIO_pinControlRegister(GPIOC,BIT11,&pinControlRegisterPORTC);
	/*Sets LED1 as turned off*/
	GPIO_clearPIN(GPIOC,BIT10); //LED1
	/*Sets LED2 as turned off*/
	GPIO_clearPIN(GPIOC,BIT11); //LED2
	/*Sets PORT C pin 10 as an output*/
	GPIO_dataDirectionPIN(GPIOC,GPIO_OUTPUT,BIT10);
	/*Sets PORT C pin 11 as an output*/
	GPIO_dataDirectionPIN(GPIOC,GPIO_OUTPUT,BIT11);

	/*Sets the PIT channel 0 interruption, a priority of 9, but doesn't enable the interruption*/
	NVIC_SetPriority(PIT_CH0_IRQ, PRIORITY_9);
	/*Sets the PORT A interruption, a priority of 9, but doesn't enable the interruption*/
	NVIC_SetPriority(PORTA_IRQ, PRIORITY_10);

	/*Initializes the DAC*/
	DAC_init();
	/*Disables de DAC; The DAC is initialized, but disabled*/
	DAC_disable();
	/*Enables the PIT clock Gating*/
	PIT_clockGating();


}

void WAVEGEN_enable(){
	/*Enables the DAC*/
	DAC_enable();
	/*Enables the PIT*/
	PIT_enable();
	/*Set the delay for PIT*/
	PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY);
	/*Enable the timer PIT channel 0*/
	PIT_timerEnable(PIT_0);
	/*Enables the interruption in PORT A; Before this, the SW3 wasn't take on account*/
	NVIC_EnableIRQ(PORTA_IRQ);
	/*Sets as current State, a triangle signal, so when the SW3 is pressed, and it actually starts to
	 * produce the wave output, currentState is square signal*/
	currentState = TRIANGLE_SIGNAL;
	/*Enables the PIT timer interrupt for channel 0*/
	PIT_timerInterruptEnable(PIT_0);
	/*RGB green led, is on*/
	GPIO_clearPIN(GPIOE,BIT26); //LED RGB VERDE

}

void WAVEGEN_disable(){
	/*Disable the PIT channel 0 interruption*/
	NVIC_DisableIRQ(PIT_CH0_IRQ);
	/*Disable the PORT A interruption*/
	NVIC_DisableIRQ(PORTA_IRQ);
	/*Loads to the DAC, an output value of 0*/
	DAC_loadValues(0);
	/*Disables the DAC*/
	DAC_disable();
	/*RGB green led, is off*/
	GPIO_setPIN(GPIOE,BIT26); //LED RGB VERDE
	/*Verifies that the LEDs corresponding to this process, are off*/
	GPIO_clearPIN(GPIOC,BIT10); //LED1
	GPIO_clearPIN(GPIOC,BIT11); //LED2

}

void WAVEGEN_ledSequence(){
	/*As there are 3 fixed states for LEDs, those 3 states are covered in the following
	 * sentences*/
	/*If both LEDs are supposed to be on, they are set on */
	if( ( currentState->LED1_state == BIT_ON ) && ( currentState->LED2_state == BIT_ON ) ){
		GPIO_setPIN(GPIOC,BIT10); //LED1
		GPIO_setPIN(GPIOC,BIT11); //LED2
	/*If LED1 is supposed to be on, and LED2 is supposed to be off, we configure that way*/
	} else if ( ( currentState->LED1_state == BIT_ON ) && ( currentState->LED2_state == BIT_OFF) ){
		GPIO_setPIN(GPIOC,BIT10); //LED1
		GPIO_clearPIN(GPIOC,BIT11); //LED2
	/*Finally, the last case we can have, is when LED1 is off, and LED2 is on*/
	} else {
		GPIO_clearPIN(GPIOC,BIT10); //LED1
		GPIO_setPIN(GPIOC,BIT11); //LED2
	}
}



void WAVEGEN_changeSequence(){
	/*Always make sure the interruptions for PIT channel 0, are enabled*/
	NVIC_EnableIRQ(PIT_CH0_IRQ);
	/*Always make sure DAC, is enabled*/
	DAC_enable();
	/*currentState, is now the next state*/
	currentState = currentState->next;
	/*LEDs state are changed, according to the fixed sequence*/
	currentState->fptrLedOutput();
}

 void WAVEGEN_sendToDac(){
	 /*index_shift, makes sure that the index is in the range of the elements of
	  * the array*/
	if(index_shift == 40){
		index_shift = 0;
	} else {
		index_shift = index_shift + 1;
	}

	/*Load to the DAC, the value of the pointer plus the index shift*/
	DAC_loadValues(*(currentState->current_index + index_shift));
}

void WAVEGEN_indexShifting(){
	 /*send to DAC the next value, according to the pointer and the index shift*/
	 WAVEGEN_sendToDac();
	 /*Set the delay again on the PIT*/
	 PIT_delay(PIT_0,SYSTEM_CLOCK,DELAY);
}

void PORTA_IRQHandler()
{
	/*Invoke the function for Wave Output Sequence*/
	currentState->fptrWaveOutput();
	/*Clear interruption flags for PORT A*/
	GPIO_clearInterrupt(GPIOA);
	/*Digital debouncer*/
	delay(30000);

}
