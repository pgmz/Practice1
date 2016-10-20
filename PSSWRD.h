/**
	\file
	\brief
		This is the header file for a password recognition process, which has been
		programmer designed to enable other processes, and recognize from a keyboard
		digits, and obtain passwords.
	\author Patricio Gomez García
	\date	23/09/2016
 */

#ifndef SOURCES_PSSWRD_H_
#define SOURCES_PSSWRD_H_

/*Struct that contains data that will be needed during the Password process*/
typedef struct{
	/*buttonPressedCounter will indicate how many times a button has been pressed, in order
	 * to obtain a full password (4 digits)*/
	uint8 buttonPressedCounter :3;
	/*currentCode will store the password; stores each digit, a button is pressed, however
	 * the state machine won't be affected, until currentCode has a full password (4 digits)*/
	uint16 currentCode;
	/*ableToReceivePassword, will indicate the button interruption and state machine, if during
	 * the process, we are able to receive a password, or if we are expected button A or button B, to
	 * start a process; When we are receiving a password, we won't take on account button A or button B, until
	 * we have a right or wrong full password. When we aren't receiving a password, we won't take on account
	 * any full password, or digit, until we know with process is to be activated*/
	uint8 ableToReceivePassword :1;
	/*currentProcess, indicates to the state machine, which process we are trying to activate*/
	uint8 currentProcess :4;
	/*processMotorStart, indicates to the state machine if this process is activated or not, in order to
	 * enable or disable this process, when requested from user*/
	uint8 processMotorStart :1;
	/*processWaveGenStart, indicates to the state machine if this process is activated or not, in order to
	 * enable or disable this process, when requested from user*/
	uint8 processWaveGenStart :1;
	/*pitCounter, when a right or wrong full password is obtained, the pitCounter will indicate to the PIT,
	 * if its needed to start the counter again, in order to blink a LED*/
	uint8 pitCounter :2;
	/*ledCorrectAnswerFlag, indicates to the ledCorrectPassword() function, which LED should blink*/
	uint8 ledCorrectAnswerFlag:1;
}Password_FlagsData;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function initializes everything related to the Password process, such as LEDS, PIT,
 	 	 interruptions, Keyboard, etc.
 	 \return void
 */
void PASSWORD_init();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function is the state machine for the password process, which is a 'switch case'
 	 	 state machine. When called, it checks different parameters in the struct that has the
 	 	 flags to manage the current state. According to the currentProcess, currentCode, if a
 	 	 Process is enabled or disabled, the state machine will change. However, this state
 	 	 machine function, isn't invoked until a button pressed change is detected.
 	 \return void
 */
void PASSWORD_stateMachine();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function receives the data received from keyboard, when a button is pressed, and checks
 	 	 that the button is a "enable/disable process" indication (BUTTON A or BUTTON B), else, if the
 	 	 button pressed was a different button, it collects 4 digits, to form a "full password", and then
 	 	 make the state machine, if a change is to be done.
 	 \param[in] keyBoardData Data received from the Keyboard, when a button is pressed (See KYBRD.c)
 	 \return void
 */
void PASSWORD_getNewData(uint8 keyBoardData);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function set to FALSE 4 flags in the password_flagsData struct
 	 \return void
 */
void PASSWORD_restartFlags();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function is in charge of making the correct/incorrect LED blink. It checks which LED
 	 	 should blink, and manages the PIT, in order to toogle the LED as it should.
 	 \return void
 */
void PASSWORD_ledCorrectPassword();


#endif /* SOURCES_PSSWRD_H_ */
