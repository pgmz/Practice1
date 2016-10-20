/**
	\file
	\brief
		This is the header file for the DAC in the Kinetis 64F. Includes the needed
		functions to use the DAC0 (initialize, enable, disable, loadValues). The
		output of the DAC, is DAC0_OUT
	\author Patricio Gomez García
	\date	22/09/2016
 */

#ifndef SOURCES_DAC_H_
#define SOURCES_DAC_H_

#include "DataTypeDefinitions.h"

/*Constant that enables de DAC*/
#define DAC_ENABLE 0x00000080
/*Constant that toogles the reference select*/
#define DAC_REFERENCE_SELECT 0x00000040
/*Constant that enables de clock gating for the DAC0*/
#define DAC0_CLOCK_GATING 0x00001000

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function configure the DAC clock gating, enable and reference select
 	 \return void

 */
void DAC_init();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function enables the DAC0, by putting a 1 in the DAC Enable bit
 	 \return void

 */
void DAC_enable();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function disables the DAC0 by putting a 0 in the DAC Enable bit
 	 \return void

 */
void DAC_disable();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 This function loads the output voltage value, by putting the 12 bits value in the high and low part
 	 	 of the DAC0 data
 	 \param[in] signal_value, output voltage value (12 bits)
 	 \return void

 */
void DAC_loadValues(uint16 signal_value);

#endif /* SOURCES_DAC_H_ */
