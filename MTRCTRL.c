/**
	\file
	\brief
		This is the source file for a DC MOTOR process, which has a programmer defined
		behavior, using state machines. It uses different interruptions, and "depends"
		from other processes.
	\author Patricio Gomez García
	\date	26/09/2016
 */

#include "MTRCTRL.h"
#include "GPIO.h"
#include "NVIC.h"
#include "PIT.h"
#include "GlobalFunctions.h"
#include "MK64F12.h"

/*System clock 21MHz*/
#define SYSTEM_CLOCK 21000000

/*Constant array that contains the MOTOR behavior and the duration of that behavior, when the state is 1*/
static const uint8 firstSequenceBehaviorAndDuration[8] = {MOTOR_OFF, 1, MOTOR_ON, 1, MOTOR_OFF, 3, MOTOR_ON, 1};
/*Constant array that contains the MOTOR behavior and the duration of that behavior, when the state is 2*/
static const uint8 secondSequenceBehaviorAndDuration[4] = {MOTOR_OFF, 4, MOTOR_ON, 4};
/*Constant array that contains the MOTOR behavior and the duration of that behavior, when the state is 0*/
static const uint8 nullSequenceBehaviorAndDuration[2] = {MOTOR_OFF, 0};

/*
 * Struct and enum state machine, containing the pointer to the MOTOR behavior and duration array,
 * the next sequence, two function pointers (ledSequence,changeSequence), and LEDS states
 */
static motorControlState motorConState[3] = {
		{FIRST_BEHAVIOR_DURATION,4,SECOND_SEQUENCE,MOTORCONTROL_ledSequence,MOTORCONTROL_changeSequence,BIT_OFF, BIT_ON},
		{SECOND_BEHAVIOR_DURATION,2,NULL_SEQUENCE,MOTORCONTROL_ledSequence,MOTORCONTROL_changeSequence,BIT_ON,BIT_OFF},
		{NULL_BEHAVIOR_DURATION,1,FIRST_SEQUENCE,MOTORCONTROL_ledSequence,MOTORCONTROL_changeSequence,BIT_ON,BIT_ON}
};

/*current state will indicate which is the state or sequence, the motor is in*/
static uint8 currentState = NULL_SEQUENCE;
/*index for shifting in the arrays containing the behavior and duration*/
static uint8 behaviorIndex = 0;

void MOTORCONTROL_init(){

	/*Set the configuration needed to use SW2*/
	GPIO_pinControlRegisterType pinControlRegisterPORTC = GPIO_MUX1|GPIO_PS|GPIO_PE|INTR_FALLING_EDGE;
	/*Sets PORT C pin 6, with the functionality of MUX_1, as a pull up resistor, pull enabled
	 * and the interruption is activated with a falling edge*/
	GPIO_pinControlRegister(GPIOC,BIT6,&pinControlRegisterPORTC);
	/*Sets PORT C pin 6 as an input (SW2)*/
	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT,BIT6);

	/*Sets the configuration for PORT C pins 16 and 17, and PORT B pin 19*/
	GPIO_pinControlRegisterType pinControlRegisterPORTBC = GPIO_MUX1;
	/*Sets PORT C pins 16 and 17, and PORT B pin 19 with the functionality MUX_1*/
	GPIO_pinControlRegister(GPIOC,BIT16,&pinControlRegisterPORTBC);
	GPIO_pinControlRegister(GPIOC,BIT17,&pinControlRegisterPORTBC);
	GPIO_pinControlRegister(GPIOB,BIT9,&pinControlRegisterPORTBC);
	/*Puts a "safe" value for PORT C pins 16 and 17, and PORT B pin 19*/
	GPIO_clearPIN(GPIOC,BIT16);
	GPIO_clearPIN(GPIOC,BIT17);
	GPIO_clearPIN(GPIOB,BIT9);
	/*Sets PORT C pins 16 and 17, and PORT B pin 19 as outputs*/
	GPIO_dataDirectionPIN(GPIOC,GPIO_OUTPUT,BIT16);
	GPIO_dataDirectionPIN(GPIOC,GPIO_OUTPUT,BIT17);
	GPIO_dataDirectionPIN(GPIOB,GPIO_OUTPUT,BIT9);

	/*Sets the PIT channel 1 interruption, a priority of 8, but doesn't enable the interruption*/
	NVIC_SetPriority(PIT_CH1_IRQ, PRIORITY_10);
	/*Sets the PORT C interruption, a priority of 7, but doesn't enable the interruption*/
	NVIC_SetPriority(PORTC_IRQ, PRIORITY_9);
	/*Enables the PIT clock Gating*/
	PIT_clockGating();
}

void MOTORCONTROL_enable(){
	/*Enables the PIT*/
	PIT_enable();
	/*Enable the timer PIT channel 1*/
	PIT_timerEnable(PIT_1);
	/*Enables the interruption in PORT C; Before this, the SW2 wasn't take on account*/
	NVIC_EnableIRQ(PORTC_IRQ);
	/*Sets as current State, the NULL_sequence, so when the SW2 is pressed, and it actually starts to
	 * produce the motor output, currentState is FIRST_SEQUENCE*/
	currentState = NULL_SEQUENCE;
	/*Enables the PIT timer interrupt for channel 1*/
	PIT_timerInterruptEnable(PIT_1);
	/*RGB red led, is on*/
	GPIO_clearPIN(GPIOB,BIT22); //LED RGB ROJO
}

void MOTORCONTROL_disable(){
	/*Disable MOTOR*/
	GPIO_clearPIN(GPIOB,BIT9);
	/*Disable the PIT channel 1 interruption*/
	NVIC_DisableIRQ(PIT_CH1_IRQ);
	/*Disable the PORT C interruption*/
	NVIC_DisableIRQ(PORTC_IRQ);
	/*RGB red led, is off*/
	GPIO_setPIN(GPIOB,BIT22); //LED RGB ROJO
	/*Verifies that the LEDs corresponding to this process, are off*/
	GPIO_clearPIN(GPIOC,BIT16); //LED1
	GPIO_clearPIN(GPIOC,BIT17); //LED2

}

void MOTORCONTROL_changeSequence(){
	/*currentState, is now changed to nextState*/
	currentState = motorConState[currentState].nextState;
	/*invokes the function that "updates" the LEDS state*/
	motorConState[currentState].fptrLedOutput();
	/*Enables PIT channel 1 interruptions*/
	NVIC_EnableIRQ(PIT_CH1_IRQ);
	/*index, begins at 0*/
	behaviorIndex = 0;
	/*begins to change the behavior of motor control*/
	MOTORCONTROL_behaviorChange();
}

void MOTORCONTROL_ledSequence(){
	/*As there are 3 fixed states for LEDs, those 3 states are covered in the following
	 * sentences*/
	/*If both LEDs are supposed to be on, they are set on */
	if( ( motorConState[currentState].LED1_state == BIT_ON ) && ( motorConState[currentState].LED2_state == BIT_ON ) ){
		GPIO_setPIN(GPIOC,BIT16); //LED1
		GPIO_setPIN(GPIOC,BIT17); //LED2
	/*If LED1 is supposed to be on, and LED2 is supposed to be off, we configure that way*/
	} else if ( ( motorConState[currentState].LED1_state == BIT_ON ) && ( motorConState[currentState].LED2_state == BIT_OFF) ){
		GPIO_setPIN(GPIOC,BIT16); //LED1
		GPIO_clearPIN(GPIOC,BIT17); //LED2
	/*Finally, the last case we can have, is when LED1 is off, and LED2 is on*/
	} else {
		GPIO_clearPIN(GPIOC,BIT16); //LED1
		GPIO_setPIN(GPIOC,BIT17); //LED2
	}
}

void MOTORCONTROL_behaviorChange(){
	/*If the currentState is NULL_SEQUENCE, the motor is Off*/
	if(currentState == NULL_SEQUENCE){
		GPIO_clearPIN(GPIOB,BIT9);
		return;
	}

	/*If the currentState is FIRST_SEQUENCE or SECOND_SEQUENCE, the PIT delay is taken from
	 * the array and the index behavior*/
	//uint8 temp_debug_currentBehaviorDuration = *(motorConState[currentState].currentBehaviorAndDuration + 1 + behaviorIndex*2);
	//uint8 temp_debug_currentBehavior = *(motorConState[currentState].currentBehaviorAndDuration + behaviorIndex*2);
	PIT_delay(PIT_1,SYSTEM_CLOCK,2*(*(motorConState[currentState].currentBehaviorAndDuration + 2*behaviorIndex + 1)));

	/*If the current behavior is ON, the motor is ON, otherwise, is OFF*/
	if(*(motorConState[currentState].currentBehaviorAndDuration + 2*behaviorIndex) == MOTOR_ON){
		GPIO_setPIN(GPIOB,BIT9);
	} else {
		GPIO_clearPIN(GPIOB,BIT9);
	}

	/*Ensures that the behavior index is shifted, but never beyond the number of behaviors
	 * of the current state*/
	if(behaviorIndex == (motorConState[currentState].numberOfBehaviors - 1)){
		behaviorIndex = 0;
	} else {
		behaviorIndex++;
	}

}

void PORTC_IRQHandler(){
	/*When the SW2 is pressed, the motor sequence is changed*/
	motorConState[currentState].fptrMotorOutput();
	/*Clears interrupt flags for PORT C*/
	GPIO_clearInterrupt(GPIOC);
	/*digital delay*/
	delay(30000);

}
