/*
 * adc_dac.c
 *
 *  Created on: Oct 23, 2017
 *      Author: lymacasm
 */
#include "stm32f0xx_adc.h"
#include "stm32f0xx_dac.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx.h"
#include "adc_dac.h"

static void ADC_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//1. Calibrate ADC
	ADC1->CR &= ~(ADC_CR_ADEN); //Sets ADEN bit to 0 which disables the ADC
	ADC1->CR |= (ADC_CR_ADCAL); //Starts Calibration by setting bit to 1

	while((ADC1->CR & ADC_CR_ADCAL) != 0);

	//2. Set up ADC Clock

	//Option 1 - PCLK
		/*
		 * Set CKMODE [1:0] to 01 (div 2) or 10 (div 4)
		 * Set ADC_CFGR2 to 11
		 *
		 * Div 2 -> Latency: 2.75  ADC Clock Cycles
		 * Div 4 -> Latency: 2.625 ADC Clock Cycles
		 */

	ADC1->CFGR2 |= ADC_CFGR2_CKMODE_0; //Set Div 2

	//Option 2 - 14MHz Asynchronous Clock
		/*
		 *  Set CKMODE[1:0] to 00
		 *  Set ADC_CFGR2 to 00
		 */

	//3. Select channel
	ADC1->CHSELR |= ADC_CHSELR_CHSEL10; // Sets the channel to the corresponding channel 10

	//4. Configure Sampling Time
	ADC1->SMPR &= ~(ADC_SMPR_SMP_2 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_0); //000 - 1.5 ADC Clock Cycles
		/* Set bits [2:0] in order to set sample time
		 * 000:   1.5  ADC clock cycles
		 * 001:   7.5  ADC clock cycles
		 * 010:  13.5  ADC clock cycles
		 * 011:  28.5  ADC clock cycles
		 * 100:  41.5  ADC clock cycles
		 * 101:  55.5  ADC clock cycles
		 * 110:  71.5  ADC clock cycles
		 * 111: 239.5  ADC clock cycles
		 *
		 * tconv = sampling time + 12.5*ADC clock cycles
		 */

	//5.  Setting Conversion Mode to continous
	ADC1->CFGR1 |= ADC_CFGR1_CONT | ADC_CFGR1_OVRMOD;

	//6.  Start ADC
	ADC1->ISR |= ADC_ISR_ADRDY;
	ADC1->CR |= ADC_CR_ADEN; //Enables the ADC
	while((ADC1->ISR & ADC_ISR_ADRDY) != 1); //Waits until ready
	ADC1->CR |= ADC_CR_ADEN;
	ADC1->CR |= ADC_CR_ADSTART; //Sets ADSTART to 1
}

static void DAC_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;

	//2. Output Buffer Enable
	DAC->CR &= ~DAC_CR_BOFF1;

	//5. DAC Enable
	DAC->CR |= DAC_CR_EN1; //Enables DAC by setting Bit 0 = 1, Which is EN1

}

static void GPIO_init()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN;
	GPIOC->MODER |= (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER0_0); //SETS Pin C0 Bits to 11 which is Analog Mode (ADC_IN10)
	GPIOA->MODER |= (GPIO_MODER_MODER4_1 | GPIO_MODER_MODER4_0); //Sets Pin A4 Bits to 11 which is Analog Mode (DAC_CH1)
}

extern void adc_dac_init()
{
	GPIO_init();
	ADC_init();
	DAC_init();
}

/*--------------GET RESISTANCE FUNCTION------------*/
/*
 * Input:  Voltage measurements
 * Output: Resistance value
 *
 * Called When: ADC polling needs to calculate resistance
 *
 */
extern int get_resistance_ohms()
{
	//This function will take the readings from the ADC
	//Convert readings from ADC to ohms
	uint32_t adc_val = get_adc();

	return (adc_val * 5000) / 0xFFF;
}

extern uint32_t get_adc()
{
	// Grab conversion from data register
	int32_t adc_val = ADC1->DR;

	// Output to DAC
	//DAC->DHR12R1 = (uint32_t)( -( (adc_val) * (adc_val - 8190) ) / (4095) ) & 0xFFF;
	DAC->DHR12R1 = (uint32_t)( (-( (adc_val) * (adc_val - 8190) ) / (5733)) + 1170) & 0xFFF;

	return ADC1->DR;
}
