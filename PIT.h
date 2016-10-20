/**
	\file
	\brief
		This is the header file for the PIT in Kinetis 64F, it has the functions needed
		to enable any PIT channel, and load values to any PIT channel.
	\author Patricio Gomez García
	\date	13/09/2016
 */

#ifndef PIT_H_
#define PIT_H_


#include "DataTypeDefinitions.h"
#include "MK64F12.h"

/*! This enumerated constant are used to select the PIT to be used*/
typedef enum {PIT_0,PIT_1,PIT_2,PIT_3} PIT_TimerType;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function enables the PIT clock gating
 	 \return void
 */
void PIT_clockGating();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function enables the PIT, in a basic way: The PIT is enabled during debugging
 		mode, and enables the PIT module
 	 \return void
 */
void PIT_enable();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function disables the PIT, by freezing the PIT during debugging mode, and
 	 	 disabling the PIT module
 	 \return void
 */
void PIT_disable();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function receives a PIT channel number, and enables the interruptions in that
 	 	 channel
 	 \param[in] pitTimer PIT channel, that will have interruptions enabled
 	 \return void
 */
void PIT_timerInterruptEnable(PIT_TimerType pitTimer);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function receives a PIT channel number, and disables the interruptions in that
 	 	 channel
 	 \param[in] pitTimer PIT channel, that will have interruptions disabled
 	 \return void
 */
void PIT_timerInterruptDisable(PIT_TimerType pitTimer);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function receives a PIT channel number, and enables that channel
 	 \param[in] pitTimer PIT channel, that will be enabled
 	 \return void
 */
void PIT_timerEnable(PIT_TimerType pitTimer);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function receives a PIT channel number, and disables that channel
 	 \param[in] pitTimer PIT channel, that will be disabled
 	 \return void
 */
void PIT_timerDisable(PIT_TimerType pitTimer);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function receives a PIT channel number, systemClock and period and loads the
 	 	 delay value in the PIT channel
 	 \param[in] pitTimer PIT channel
 	 \param[in] systemClock
 	 \param[in] period
 	 \return void
 */
void PIT_delay(PIT_TimerType pitTimer,float systemClock ,float perior);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function clears the corresponding flags when a PIT channel 0 interruption
 	 	 occurs
 	 \return void
 */
void PIT0_clearInterrupt();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function clears the corresponding flags when a PIT channel 3 interruption
 	 	 occurs
 	 \return void
 */
void PIT3_clearInterrupt();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief This function receives a PIT channel number, and returns the current counter value
 	 	 of the channel
 	 \param[in] pitTimer PIT channel
 	 \return counter value in the PIT channel
 */
uint32 PIT_readTimerValue(PIT_TimerType pitTimer);
#endif /* PIT_H_ */
