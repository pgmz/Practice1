/**
	\file
	\brief
		This is the header file for a wave or signal, generation process, which uses
		different interruptions, and depends on other processes.
	\author Patricio Gomez García
	\date	20/09/2016
 */


#ifndef SOURCES_WVGN_H_
#define SOURCES_WVGN_H_

#include "DataTypeDefinitions.h"

/*Define SQUARE_SIGNAL, as the direction of the first state in the state machine
 * this is used in the linked state machine*/
#define SQUARE_SIGNAL &waveGenState[0]
/*Define SINE_SIGNAL, as the direction of the second state in the state machine
 * this is used in the linked state machine*/
#define SINE_SIGNAL &waveGenState[1]
/*Define TRIANGLE_SIGNAL, as the direction of the third state in the state machine
 * this is used in the linked state machine*/
#define TRIANGLE_SIGNAL &waveGenState[2]

/*Define SQUARE_SIGNAL_INDEX, as the direction of the first index in the array of
 * values in the square signal values*/
#define SQUARE_SIGNAL_INDEX &squareSignalValues[0]
/*Define SINE_SIGNAL_INDEX, as the direction of the first index in the array of
 * values in the sine signal values*/
#define	SINE_SIGNAL_INDEX &sineSignalValues[0]
/*Define TRIANGLE_SIGNAL_INDEX, as the direction of the first index in the array of
 * values in the triangle signal values*/
#define TRIANGLE_SIGNAL_INDEX &triangleSignalValues[0]

/*State machine definition; Linked state machine*/
typedef struct state{
	/*Pointer to a state machine 'next'; This pointer, has the direction of the next
	 * state in the state machine, meaning that will point to the next index in the array
	 * of state machine*/
	struct state* next;
	/*Function pointer to the function that 'updates' the LEDs*/
	void(*fptrLedOutput)();
	/*Function pointer to the function that changes the output signal*/
	void(*fptrWaveOutput)();
	/*current_index is pointer to the beginning of the array that contains the signal values
	 * for this state. It's constant, as it will always point to the same direction. Using
	 * another counter, and by using pointers artihmetic, we can shift the index in the array*/
	const uint16* current_index;
	/*LED1_state; state of LED1 (ON or OFF)*/
	uint8 LED1_state :1;
	/*LED2_state; state of LED2 (ON or OFF)*/
	uint8 LED2_state :1;
}waveGeneratorState;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function configure everything related to the initialization of the Wave Generator process, this includes
 	 	 activating the clock gating for all the ports to be used, the DAC and the PIT, enables de interruptions, and
 	 	 configures the pins to be used.
 	 \return void

 */
void WAVEGEN_init();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function turns on/off the 2 LEDS corresponding to this process, according to the
 	 	 current state, of the state machine
 	 \return void

 */
static void WAVEGEN_ledSequence();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function enables the Wave Generator Process, but prevents the process from actually
 	 	 starting and producing a wave output.
 	 \return void

 */
void WAVEGEN_enable();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function disables the Wave Generator Process; It prevents the process from producing
 	 	  a wave output.
 	 \return void

 */
void WAVEGEN_disable();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function is invoked/called, when the SW2 is pressed. It changes the current State
 	 	 to the next state, according to the state machine. 'Updates' the LEDs status.
 	 \return void

 */
void WAVEGEN_changeSequence();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function manages the index shifting, in order to send to the DAC (load in the
 	 	 DAC registers) the proper value from the array of values.
 	 \return void

 */
void WAVEGEN_sendToDac();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function is manages the index shifting, and PIT delay loading. Invokes
 	 	 WAVEGEN_sendToDac() function, and sets the delay in PIT.
 	 \return void

 */
void WAVEGEN_indexShifting();

#endif /* SOURCES_WVGN_H_ */
