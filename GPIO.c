/**
	\file
	\brief
		This is the source file for the GPIO device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	7/09/2014
 */
#include "MK64F12.h"
#include "GPIO.h"
#include "DataTypeDefinitions.h"


void GPIO_clearInterrupt(GPIO_portNameType portName){
	switch(portName){
		case GPIOA:
			PORTA_ISFR=0xFFFFFFFF;
			break;
		case GPIOB:
			PORTB_ISFR=0xFFFFFFFF;
			break;
		case GPIOC:
			PORTC_ISFR=0xFFFFFFFF;
			break;
		case GPIOD:
			PORTD_ISFR=0xFFFFFFFF;
			break;
		case GPIOE:
			PORTE_ISFR=0xFFFFFFFF;
			break;
		default:
			break;
	}
}

uint8 GPIO_clockGating(GPIO_portNameType portName){
	switch(portName){
		case GPIOA:
			SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTA;
			break;
		case GPIOB:
			SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTB;
			break;
		case GPIOC:
			SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTC;
			break;
		case GPIOD:
			SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTD;
			break;
		case GPIOE:
			SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTE;
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

uint8 GPIO_pinControlRegister(GPIO_portNameType portName,uint8 pin,GPIO_pinControlRegisterType* pinControlRegister){
	switch(portName){
		case GPIOA:
			PORTA_PCR(pin) = *pinControlRegister;
			break;
		case GPIOB:
			PORTB_PCR(pin) = *pinControlRegister;
			break;
		case GPIOC:
			PORTC_PCR(pin) = *pinControlRegister;
			break;
		case GPIOD:
			PORTD_PCR(pin) = *pinControlRegister;
			break;
		case GPIOE:
			PORTE_PCR(pin) = *pinControlRegister;
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

void GPIO_dataDirectionPORT(GPIO_portNameType portName, uint32 direction){
	switch(portName){
		case GPIOA:
			GPIOA_PDDR = direction;
			break;
		case GPIOB:
			GPIOB_PDDR = direction;
			break;
		case GPIOC:
			GPIOC_PDDR = direction;
			break;
		case GPIOD:
			GPIOD_PDDR = direction;
			break;
		case GPIOE:
			GPIOE_PDDR = direction;
			break;
	}
}

void GPIO_dataDirectionPIN(GPIO_portNameType portName, uint8 state, uint8 pin){

	switch(portName){
		case(GPIOA):
				if(state == GPIO_OUTPUT){
					GPIOA_PDDR |= (BIT_ON << pin);
				}
				else{
					GPIOA_PDDR &= ~(BIT_ON << pin);
				}
				break;
		case(GPIOB):
				if(state == GPIO_OUTPUT){
					GPIOB_PDDR |= (BIT_ON << pin);
				}
				else{
					GPIOB_PDDR &= ~(BIT_ON << pin);
				}
				break;
		case(GPIOC):
				if(state == GPIO_OUTPUT){
					GPIOC_PDDR |= (BIT_ON << pin);
				}
				else{
					GPIOC_PDDR &= ~(BIT_ON << pin);
				}
				break;
		case(GPIOD):
				if(state == GPIO_OUTPUT){
					GPIOD_PDDR |= (BIT_ON << pin);
				}
				else{
					GPIOD_PDDR &= ~(BIT_ON << pin);
				}
				break;
		case(GPIOE):
				if(state == GPIO_OUTPUT){
					GPIOE_PDDR |= (BIT_ON << pin);
				}
				else{
					GPIOE_PDDR &= ~(BIT_ON << pin);
				}
				break;
	}
}

uint32 GPIO_readPORT(GPIO_portNameType portName){

	switch(portName){
		case(GPIOA):
				return GPIOA_PDIR;
				break;
		case(GPIOB):
				return GPIOB_PDIR;
				break;
		case(GPIOC):
				return GPIOC_PDIR;
				break;
		case(GPIOD):
				return GPIOD_PDIR;
				break;
		case(GPIOE):
				return GPIOE_PDIR;
				break;
		default:
				return FALSE;
	}

}

uint8 GPIO_readPIN(GPIO_portNameType portName, uint8 pin){
	switch(portName){
		case(GPIOA):
				return (GPIOA_PDIR & (1 << pin))?1:0;
				break;
		case(GPIOB):
				return (GPIOB_PDIR & (1 << pin))?1:0;
				break;
		case(GPIOC):
				return (GPIOC_PDIR & (1 << pin))?1:0;
				break;
		case(GPIOD):
				return (GPIOD_PDIR & (1 << pin))?1:0;
				break;
		case(GPIOE):
				return (GPIOE_PDIR & (1 << pin))?1:0;
				break;
		default:
			return FALSE;
	}
}

void GPIO_writePORT(GPIO_portNameType portName, uint32 data){

	switch(portName){
		case GPIOA:
			GPIOA_PDOR = data;
			break;
		case GPIOB:
			GPIOB_PDOR = data;
			break;
		case GPIOC:
			GPIOC_PDOR = data;
			break;
		case GPIOD:
			GPIOD_PDOR = data;
			break;
		case GPIOE:
			GPIOE_PDOR = data;
			break;
	}
}

void GPIO_setPIN(GPIO_portNameType portName, uint8 pin){

switch(portName){
	case(GPIOA):
			GPIOA_PSOR = (BIT_ON << pin);
			break;
	case(GPIOB):
			GPIOB_PSOR = (BIT_ON << pin);
			break;
	case(GPIOC):
			GPIOC_PSOR = (BIT_ON << pin);
			break;
	case(GPIOD):
			GPIOD_PSOR = (BIT_ON << pin);
			break;
	case(GPIOE):
			GPIOE_PSOR = (BIT_ON << pin);
			break;
	}
}

void GPIO_clearPIN(GPIO_portNameType portName, uint8 pin){
	switch(portName){
		case GPIOA:
			GPIOA_PCOR = (BIT_ON << pin);
			break;
		case GPIOB:
			GPIOB_PCOR = (BIT_ON << pin);
			break;
		case GPIOC:
			GPIOC_PCOR = (BIT_ON << pin);
			break;
		case GPIOD:
			GPIOD_PCOR = (BIT_ON << pin);
			break;
		case GPIOE:
			GPIOE_PCOR = (BIT_ON << pin);
			break;
	}
}

void GPIO_tooglePIN(GPIO_portNameType portName, uint8 pin){
	switch(portName){
		case GPIOA:
			GPIOA_PTOR = (BIT_ON << pin);
			break;
		case GPIOB:
			GPIOB_PTOR = (BIT_ON << pin);
			break;
		case GPIOC:
			GPIOC_PTOR = (BIT_ON << pin);
			break;
		case GPIOD:
			GPIOD_PTOR = (BIT_ON << pin);
			break;
		case GPIOE:
			GPIOE_PTOR = (BIT_ON << pin);
			break;
	}
}



