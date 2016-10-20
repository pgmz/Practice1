/**
	\file
	\brief
		This is the header file for a DC MOTOR process, which has a programmer defined
		behavior, using state machines. It uses different interruptions, and "depends"
		from other processes.
	\author Patricio Gomez García
	\date	26/09/2016
 */

#ifndef SOURCES_MTRCTRL_H_
#define SOURCES_MTRCTRL_H_

#include "DataTypeDefinitions.h"
#define MOTOR_ON 0x1
#define MOTOR_OFF 0x0

/*DEFINE the first behvaior duration, as the direcction of the array containing that
 * information. It points the first position of the array*/
#define FIRST_BEHAVIOR_DURATION &firstSequenceBehaviorAndDuration[0]
/*DEFINE the second behvaior duration, as the direcction of the array containing that
 * information. It points the first position of the array*/
#define SECOND_BEHAVIOR_DURATION &secondSequenceBehaviorAndDuration[0]
/*DEFINE the null behvaior duration, as the direcction of the array containing that
 * information. It points the first position of the array*/
#define NULL_BEHAVIOR_DURATION &nullSequenceBehaviorAndDuration[0]

/*enum 'sequence' that shows the sequence the state machine follows*/
typedef enum {
	FIRST_SEQUENCE,
	SECOND_SEQUENCE,
	NULL_SEQUENCE
}sequence;

/*Struct and Enum state machine*/
typedef struct {
	/*currentBehaviorAndDuration; is a pointer to the direction of the first element in the
	 * array containing that data. Using pointers arithm, and adding a counter, we will
	 * shift in the array. As the array contains two informations: Behavior and duration,
	 * each time we move (with the counter) it must be from 2 in 2. So using the pointer
	 * arith, we will get the Behavior, and by adding 1 to the direction, we will get
	 * the duration*/
	const uint8* currentBehaviorAndDuration;
	/*Number of behaviors each state has, in order to move within the array, without leaving
	 * the array*/
	uint8 numberOfBehaviors :3;
	/*Next state of the state machine*/
	uint8 nextState:2;
	/*function pointer, to the function that will 'update' the LEDs*/
	void(*fptrLedOutput)();
	/*function pointer, to the function that will change the state*/
	void(*fptrMotorOutput)();
	/*LED1_state; state of LED1 (ON or OFF)*/
	uint8 LED1_state :1;
	/*LED1_state; state of LED1 (ON or OFF)*/
	uint8 LED2_state :1;
}motorControlState;

void MOTORCONTROL_init();
void MOTORCONTROL_enable();
void MOTORCONTROL_disable();
void MOTORCONTROL_changeSequence();
void MOTORCONTROL_ledSequence();
void MOTORCONTROL_behaviorChange();

#endif /* SOURCES_MTRCTRL_H_ */
