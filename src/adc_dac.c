/*
 * adc_dac.c
 *
 *  Created on: Oct 23, 2017
 *      Author: lymacasm
 */
#include "stm32f0xx_adc.h"
#include "adc_dac.h"


extern void adc_dac_init()
{
	/* ------- ADC -------- */

	GPIOC_MODER0 =

	ADC->ADEN |=

	//Initialize the ADC and DAC for use

	//ADC will be on Pin PC0
	//DAC will be on Pin PA4
}

extern int get_resistance_ohms()
{
	//This function will take the readings from the ADC and DAC and return the resistance
}
