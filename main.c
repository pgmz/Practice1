/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include "GPIO.h"
#include "NVIC.h"
#include "PIT.h"
#include "WVGN.h"
#include "PSSWRD.h"
#include "MTRCTRL.h"

//static int i = 0;

int main(void)
{
    /* Write your code here */

	/*Configurations for LED RGB*/
	GPIO_clockGating(GPIOB);
	GPIO_clockGating(GPIOE);
	GPIO_pinControlRegisterType pinControlRegisterPORTB = GPIO_MUX1;
	GPIO_pinControlRegisterType pinControlRegisterPORTE = GPIO_MUX1;
	GPIO_pinControlRegister(GPIOB,BIT21,&pinControlRegisterPORTB);
	GPIO_pinControlRegister(GPIOB,BIT22,&pinControlRegisterPORTB);
	GPIO_pinControlRegister(GPIOE,BIT26,&pinControlRegisterPORTE);
	GPIO_setPIN(GPIOB,BIT21); //LED RGB AZUL
	GPIO_setPIN(GPIOB,BIT22); //LED RGB ROJO
	GPIO_setPIN(GPIOE,BIT26); //LED RGB VERDE
 	GPIO_dataDirectionPIN(GPIOB,GPIO_OUTPUT,BIT21);
	GPIO_dataDirectionPIN(GPIOB,GPIO_OUTPUT,BIT22);
	GPIO_dataDirectionPIN(GPIOE,GPIO_OUTPUT,BIT26);

	/*initialize the three processes*/
	WAVEGEN_init();
	PASSWORD_init();
	MOTORCONTROL_init();

	/*Sets the threshold*/
	NVIC_setBASEPRI_threshold(PRIORITY_15);

	/*Enables the interruptions*/
	EnableInterrupts;

    /* This for loop should be replaced. By default this loop allows a single stepping. */
    for (;;) {

    }
    /* Never leave main */
    return 0;


}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
