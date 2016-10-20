/**
	\file
	\brief
		This is the source file for a KEYBOARD using a MM74C922, with the Kinetis
		64F. Includes the functions to initialize the KEYBOARD, and PORTB_IRQHandler.
	\author Patricio Gomez García
	\date	23/09/2016
 */


#ifndef SOURCES_KEYB_C_
#define SOURCES_KEYB_C_

#include "NVIC.h"
#include "GPIO.h"
#include "GlobalFunctions.h"
#include "PSSWRD.h"
#include "MK64F12.h"
#include "KYBRD.h"

/*local variable for the data received in the keyboard*/
static uint8 keyBoardData = FALSE;


void KEYBOARD_init(){
	/*Sets the configuration needed to receive an interruption when there is data available in the keyboard*/
	GPIO_pinControlRegisterType pinControlRegisterPORTB = GPIO_MUX1|INTR_RISING_EDGE;
	/*Sets PORT B pin 20, with the functionality of MUX_1  and the interruption actives with the rising edge*/
	GPIO_pinControlRegister(GPIOB,BIT20,&pinControlRegisterPORTB);
	/*Sets PORT B pin 20 as an input (Data available from the Keyboard interruption)*/
	GPIO_dataDirectionPIN(GPIOB,GPIO_INPUT,BIT20);

	/*Sets the configuration needed to get the data available from ports B2, B3, B10 and B11*/
	GPIO_pinControlRegisterType pinControlRegisterPORTB2_11 = GPIO_MUX1;
	/*Sets PORT B pins 2, 3, 10 and 11, with the functionality of MUX_1 */
	GPIO_pinControlRegister(GPIOB,BIT2,&pinControlRegisterPORTB2_11);
	GPIO_pinControlRegister(GPIOB,BIT3,&pinControlRegisterPORTB2_11);
	GPIO_pinControlRegister(GPIOB,BIT10,&pinControlRegisterPORTB2_11);
	GPIO_pinControlRegister(GPIOB,BIT11,&pinControlRegisterPORTB2_11);
	/*Sets PORT B pins 2, 3, 10 and 11 as inputs (Data from the keyboard)*/
	GPIO_dataDirectionPIN(GPIOB,GPIO_INPUT,BIT2);
	GPIO_dataDirectionPIN(GPIOB,GPIO_INPUT,BIT3);
	GPIO_dataDirectionPIN(GPIOB,GPIO_INPUT,BIT10);
	GPIO_dataDirectionPIN(GPIOB,GPIO_INPUT,BIT11);

	/*Enable the PORTB interruptions with PRIORITY 10*/
	NVIC_enableInterruptAndPriority(PORTB_IRQ, PRIORITY_6);


}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This is actived when an interruption happens in the PORT B, also recieves the data
 	 	 	 from the keyboard.
 	 \return void
 */
void PORTB_IRQHandler(){
	/*Double check of the interruption*/
	if(GPIO_readPIN(GPIOB,BIT20)){
		/*Gather from the pins 2, 3, 10, 11 in PORT B, the keyboard data*/
		keyBoardData = ( ( GPIO_readPIN(GPIOB,BIT2) << 3 ) )
				| ( ( GPIO_readPIN(GPIOB,BIT3) << 2 ) ) | ( ( GPIO_readPIN(GPIOB,BIT10) << 1 ) )
				| ( ( GPIO_readPIN(GPIOB,BIT11) ) );
		/*Call the function that will attend the data change in PASSWORD process and state machine*/
		PASSWORD_getNewData(keyBoardData);
		/*Do a software Debouncer, with a delay*/
	}
	/*Clear the interruption flags*/
	GPIO_clearInterrupt(GPIOB);
	/*Digital Debouncer*/
	delay(25000);
}


#endif /* SOURCES_KEYB_C_ */
