/**
	\file
	\brief
		This is the source file for a password recognition process, which has been
		programmer designed to enable other processes, and recognize from a keyboard
		digits, and obtain passwords.
	\author Patricio Gomez García
	\date	23/09/2016
 */

#include "DataTypeDefinitions.h"
#include "PSSWRD.h"
#include "KYBRD.h"
#include "WVGN.h"
#include "PIT.h"
#include "GPIO.h"
#include "NVIC.h"
#include "MTRCTRL.h"

#define MASTER_PROCESS 0x00000001
#define MOTOR_CONTROL_PROCESS 0x00000002
#define WAVE_GENERATOR_PROCESS 0x00000003
/*Master code representation in hexadecimal (1234)*/
#define MASTER_CODE 0x0000F7BD
/*Motor control code representation in hexadecimal (4567)*/
#define MOTOR_CONTROL_CODE 0x0000D59E
/*Wave generator code representation in hexadecimal (7890)*/
#define WAVE_GENERATOR_CODE 0x0000E6A4
/*System clock to be used in this process*/
#define SYSTEM_CLOCK 21000000
/*Delay to be used in this process, in order to turn on or off for 1 second periods, the LEDs
 * that indicate if the password was right or wrong*/
#define DELAY 2
/*struct that specifies currentData in the Password process (detailed in PSSWRD.h)*/
static Password_FlagsData password_flagsData = {
		/*Button pressed counter, starts in 0*/
		0,
		/*Current code, starts in 0*/
		0,
		/*At the beginning, the process is able to receive passwords*/
		TRUE,
		/*Current Process activation*/
		MASTER_PROCESS,
		/*Motor Process, begins in off*/
		BIT_OFF,
		/*Wave generator Process, begins in off*/
		BIT_OFF,
		/*PIT counter, starts in 0*/
		0,
		/*The LED that will be taken on account, will be the correct password LED*/
		BIT_ON
};

void PASSWORD_init(){
	/*Initializes Keyboard peripheral*/
	KEYBOARD_init();

	/*Sets the configuration needed to use the PORTB pins as MUX1*/
	GPIO_pinControlRegisterType pinControlRegisterPORTB = GPIO_MUX1;
	/*Sets PORTB pin 18, with the funcionality of MUX_1*/
	GPIO_pinControlRegister(GPIOB,BIT18,&pinControlRegisterPORTB);
	/*Sets PORTB pin 19, with the funcionality of MUX_1*/
	GPIO_pinControlRegister(GPIOB,BIT19,&pinControlRegisterPORTB);
	/*Sets LEDincorrect as turned off*/
	GPIO_clearPIN(GPIOB,BIT18); //LEDincorrect
	/*Sets LEDcorrect as turned off*/
	GPIO_clearPIN(GPIOB,BIT19); //LEDcorrect
	/*Sets PORT B pin 18 as an output*/
	GPIO_dataDirectionPIN(GPIOB,GPIO_OUTPUT,BIT18); //LEDincorrect
	/*Sets PORT B pin 19 as an output*/
	GPIO_dataDirectionPIN(GPIOB,GPIO_OUTPUT,BIT19); //LEDcorrect

	/*Enables PIT*/
 	PIT_enable();

 	/*Enables the interruption for PIT channel 3, with priority 10
 	 * This PIT channel is in charge of blinking the LEDs for 1 second
 	 * */
	NVIC_enableInterruptAndPriority(PIT_CH3_IRQ, PRIORITY_10);

	/*Set a delay to PIT channel 3*/
	PIT_delay(PIT_3,SYSTEM_CLOCK,DELAY);
}

void PASSWORD_stateMachine(){
	/*Switch case, the first parameter taken in account, is the current Process*/
	switch(password_flagsData.currentProcess){

	/*At the beginning of the Password process, MASTER_PROCESS, is the current process, this, to
	 * indicate, that the system is expecting a Password, in order to change the current process*/
	case MASTER_PROCESS:

		/*During the MASTER_PROCESS, we check that the currentCode, is equal to the MASTER_CODE*/
		if(password_flagsData.currentCode == MASTER_CODE){

			/*If the currentCode equals the MASTER_CODE, the RGB leds turns blue*/
			GPIO_clearPIN(GPIOB,BIT21); //LED RGB AZUL

			/*If the currentCode equals the MASTER_CODE, we are no longer able to receive a password,
			 * now we are expecting a BUTTON A or BUTTON B, to indicate a change of process*/
			password_flagsData.ableToReceivePassword = FALSE;

			/*If the currentCode equals the MASTER_CODE, we invoke the PASSWORD_restartFlags() function*/
			PASSWORD_restartFlags();

			/*If the currentCode equals the MASTER_CODE, we indicate that the led that will blink, is the
			 * correct password LED, and invoke the function PASSWORD_ledCorrectPassword() that will make
			 * the LED blink*/
			password_flagsData.ledCorrectAnswerFlag = TRUE;
			PASSWORD_ledCorrectPassword();

		} else {

			/*If the currentCode isn´t equal to the MASTER_CODE, we indicate that we are still able to
			 * receive a password, as we aren´t able yet to start a process (Motor Control or Wave Generator)*/
			password_flagsData.ableToReceivePassword = TRUE;

			/*If the currentCode isn´t equal to the MASTER_CODE, currentCode is FALSE (0)*/
			password_flagsData.currentCode = FALSE;

			/*If the currentCode isn´t equal to the MASTER_CODE, button counter restarts to 0*/
			password_flagsData.buttonPressedCounter = FALSE;

			/*If the currentCode isn´t equal to the MASTER_CODE, we indicate that hte leed that will blink,
			 * is the incorrect password LED, and invoke the function PASSWORD_ledCorrectPassword() that
			 * will make the LED blink*/
			password_flagsData.ledCorrectAnswerFlag = FALSE;
			PASSWORD_ledCorrectPassword();
		}

		break;

	/*If the current case is MOTOR_CONTROL_PROCESS, this means, that BUTTON A has been pressed, and now
	 * we are expecting a full password (4 digits), that matches or not the right password */
	case MOTOR_CONTROL_PROCESS:

		/*The first time we enter in this case, we "make the system" able to receive password, and we
		 * leave this function, as no button has been pressed.*/
		password_flagsData.ableToReceivePassword = TRUE;
		if(password_flagsData.buttonPressedCounter == FALSE){
			break;
		}

		/*The second time we enter in this case, we evaluate the currentCode to equal the MOTOR_CONTROL_CODE	*/
		if(password_flagsData.currentCode == MOTOR_CONTROL_CODE){
			/*If the currentCode equals the WAVE_GENERATOR_CODE, we indicate that the led that will blink, is the
			 * correct password LED, and invoke the function PASSWORD_ledCorrectPassword() that will make
			 * the LED blink*/
			password_flagsData.ledCorrectAnswerFlag = TRUE;
			PASSWORD_ledCorrectPassword();

			/*If the currentCode equals the WAVE_GENERATOR_CODE, we invoke the PASSWORD_restartFlags() function*/
			PASSWORD_restartFlags();

			/*If the currentCode equals the MOTOR_CONTROL_CODE, and if ..*/
			password_flagsData.processMotorStart = (password_flagsData.processMotorStart)?(BIT_OFF):(BIT_ON);
			if(password_flagsData.processMotorStart){
				MOTORCONTROL_enable();
			} else {
				MOTORCONTROL_disable();
			}
		} else {

			/*If the currentCode isn´t equal to the MOTOR_CONTROL_CODE, we invoke the PASSWORD_restartFlags()
			 * function**/
			PASSWORD_restartFlags();

			/*If the currentCode isn´t equal to the MOTOR_CONTROL_CODE, we indicate that the led that will blink,
			 * is the incorrect password LED, and invoke the function PASSWORD_ledCorrectPassword() that
			 * will make the LED blink*/
			password_flagsData.ledCorrectAnswerFlag = FALSE;
			PASSWORD_ledCorrectPassword();
		}
		break;

	/*If the current case is WAVE_GENERATOR_PROCESS, this means, that BUTTON B has been pressed, and now
	 * we are expecting a full password (4 digits), that matches or not the right password */
	case WAVE_GENERATOR_PROCESS:

		/*The first time we enter in this case, we "make the system" able to receive password, and we
		 * leave this function, as no button has been pressed.*/
		password_flagsData.ableToReceivePassword = TRUE;
		if(password_flagsData.buttonPressedCounter == FALSE){
			break;
		}

		/*The second time we enter in this case, we evaluate the currentCode to equal the WAVE_GENERATOR_CODE	*/
		if(password_flagsData.currentCode == WAVE_GENERATOR_CODE){

			/*If the currentCode equals the WAVE_GENERATOR_CODE, we indicate that the led that will blink, is the
			 * correct password LED, and invoke the function PASSWORD_ledCorrectPassword() that will make
			 * the LED blink*/
			password_flagsData.ledCorrectAnswerFlag = TRUE;
			PASSWORD_ledCorrectPassword();

			/*If the currentCode equals the WAVE_GENERATOR_CODE, we invoke the PASSWORD_restartFlags() function*/
			PASSWORD_restartFlags();

			/*If the currentCode equals the WAVE_GENERATOR_CODE, the Wave generator process enable, will be
			 * "toogled", if the process was enabled, now will be disabled, and vice versa*/
			password_flagsData.processWaveGenStart = (password_flagsData.processWaveGenStart)?(BIT_OFF):(BIT_ON);
			if(password_flagsData.processWaveGenStart){
				WAVEGEN_enable();
			} else {
				WAVEGEN_disable();
			}

		} else {

			/*If the currentCode isn´t equal to the WAVE_GENERATOR_CODE, we invoke the PASSWORD_restartFlags()
			 * function**/
			PASSWORD_restartFlags();

			/*If the currentCode isn´t equal to the WAVE_GENERATOR_CODE, we indicate that the led that will blink,
			 * is the incorrect password LED, and invoke the function PASSWORD_ledCorrectPassword() that
			 * will make the LED blink*/
			password_flagsData.ledCorrectAnswerFlag = FALSE;
			PASSWORD_ledCorrectPassword();
		}

		break;

	default:
		break;
	}
}

void PASSWORD_getNewData(uint8 keyBoardData){

	/*Checks if the button pressed is BUTTON A or BUTTON B*/
	if( ( keyBoardData != BUTTON_A ) && ( keyBoardData != BUTTON_B ) ){

		/*If the button pressed isn't BUTTON A or BUTTON B, then it checks if "the system" is able to receive
		 * a password, and that the button pressed counter is less or equal to 3*/
		if( password_flagsData.buttonPressedCounter <= 3 && password_flagsData.ableToReceivePassword) {

			/*The keyboard data, is shifted according to the button pressed counter, and "added" to the currentCode,
			 * this, in order to collect 4 digits, and make the "full password"*/
			password_flagsData.currentCode |= (keyBoardData << (4*(3 - password_flagsData.buttonPressedCounter)));

			/*The button pressed counter is increased*/
			password_flagsData.buttonPressedCounter++;

			/*If the button pressed counter is 4, this means that we now have a password, and the state machine
			 * can ensure if a change is needed*/
			if( password_flagsData.buttonPressedCounter == 4 ){
				PASSWORD_stateMachine();
			}
		}

	/*If the button pressed was BUTTON A or BUTTON B, we verify that we aren't receiving a password, and instead,
	 * we are able to receive a "enable/disable process indication"*/
	} else if (!password_flagsData.ableToReceivePassword){

		/*If we are able to "enable/disable a process", we store in currentProcess with process we are trying to
		 * enable or disable, according to which BUTTON was pressed, and the state machine can ensure if a change
		 * is needed*/
		password_flagsData.currentProcess = (keyBoardData == BUTTON_A)?(MOTOR_CONTROL_PROCESS)
				:(WAVE_GENERATOR_PROCESS);
		PASSWORD_stateMachine();
	}

 }

void PASSWORD_ledCorrectPassword(){

	/*Verifies which LED should blink, from the ledCorrectAnswerFlag*/
	if(password_flagsData.ledCorrectAnswerFlag){

		/*toogles led correct (green)*/
		GPIO_tooglePIN(GPIOB,BIT19); //LEDcorrect
	} else {
		/*toogles led incorrect (red)*/
		GPIO_tooglePIN(GPIOB,BIT18); //LEDincorrect
	}

	/*Depending on the pitCounter, the PIT is loaded again with a delay value*/
	if(password_flagsData.pitCounter == 3){
		PIT_timerInterruptDisable(PIT_3);
		password_flagsData.pitCounter = 0;
	} else {
		PIT_delay(PIT_3,SYSTEM_CLOCK,DELAY);
		/*Enable the timer interruption for PIT channel 3*/
		PIT_timerInterruptEnable(PIT_3);
		/*Enables the timer for PIT channel 3*/
		PIT_timerEnable(PIT_3);
		password_flagsData.pitCounter++;
	}
}

void PASSWORD_restartFlags(){
	/*Sets able to receive a password to FALSE, instead we are expecting a "enable/disable process indication"*/
	password_flagsData.ableToReceivePassword = FALSE;
	/*Sets currentCode to FALSE*/
	password_flagsData.currentCode = FALSE;
	/*Sets buttonPressedCounter to 0*/
	password_flagsData.buttonPressedCounter = FALSE;
	/*Sets currentProcess to none (0)*/
	password_flagsData.currentProcess = FALSE;
}
