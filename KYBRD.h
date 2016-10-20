/**
	\file
	\brief
		This is the header file for a KEYBOARD using a MM74C922, with the Kinetis
		64F. Includes the functions to initialize the KEYBOARD, and PORTB_IRQHandler.
	\author Patricio Gomez García
	\date	23/09/2016
 */

#ifndef SOURCES_KYBRD_H_
#define SOURCES_KYBRD_H_

/*Buttons interpretation: If data is 0x4, button 0 was pressed*/
#define BUTTON_0 0x4
/*Buttons interpretation: If data is 0xf, button 1 was pressed*/
#define BUTTON_1 0xf
/*Buttons interpretation: If data is 0x7, button 2 was pressed*/
#define BUTTON_2 0x7
/*Buttons interpretation: If data is 0xb, button 3 was pressed*/
#define BUTTON_3 0xb
/*Buttons interpretation: If data is 0xd, button 4 was pressed*/
#define BUTTON_4 0xd
/*Buttons interpretation: If data is 0x5, button 5 was pressed*/
#define BUTTON_5 0x5
/*Buttons interpretation: If data is 0x9, button 6 was pressed*/
#define BUTTON_6 0x9
/*Buttons interpretation: If data is 0xe, button 7 was pressed*/
#define BUTTON_7 0xe
/*Buttons interpretation: If data is 0x6, button 8 was pressed*/
#define BUTTON_8 0x6
/*Buttons interpretation: If data is 0xa, button 9 was pressed*/
#define BUTTON_9 0xa
/*Buttons interpretation: If data is 0x3, button A was pressed*/
#define BUTTON_A 0x3
/*Buttons interpretation: If data is 0x1, button B was pressed*/
#define BUTTON_B 0x1
/*Buttons interpretation: If data is 0x2, button C was pressed*/
#define BUTTON_C 0x2
/*Buttons interpretation: If data is 0x0, button D was pressed*/
#define BUTTON_D 0x0
/*Buttons interpretation: If data is 0xc, button * was pressed*/
#define BUTTON_STARKEY 0xc
/*Buttons interpretation: If data is 0x8, button # was pressed*/
#define BUTTON_NUMBER 0x8

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function initializes everything related to the Keyboard usage, such as input
 	 	 	 pins.
 	 \return void
 */
void KEYBOARD_init();


#endif /* SOURCES_KYBRD_H_ */
