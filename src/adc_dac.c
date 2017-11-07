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

	GPIOC-> MODER |= (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER0_0); //SETS Pin C0 Bits to 11 which is Analog Mode (ADC_IN10)

	ADC1->CR |= ADC_CR_ADEN; //Sets ADEN bit to 1 which enables the ADC

	// We are either using ADC1->____ (Line 1118 of stm32f0xx.h)

  	//  __IO uint32_t ISR;          !< ADC Interrupt and Status register,                          Address offset:0x00
	//	__IO uint32_t IER;          !< ADC Interrupt Enable register,                              Address offset:0x04
	//	__IO uint32_t CR;           !< ADC Control register,                                       Address offset:0x08
	//	__IO uint32_t CFGR1;        !< ADC Configuration register 1,                               Address offset:0x0C
	//	__IO uint32_t CFGR2;        !< ADC Configuration register 2,                               Address offset:0x10
	//	__IO uint32_t SMPR;         !< ADC Sampling time register,                                 Address offset:0x14
	//	__IO uint32_t TR;           !< ADC watchdog threshold register,                            Address offset:0x20                                                               0x24
	//  __IO uint32_t CHSELR;       !< ADC channel selection register,                             Address offset:0x28                                                            0x2C
	//	__IO uint32_t DR;           !< ADC data register,                                          Address offset:0x40

		//The above are things that you can put after the ->




	/* ------- DAC -------- */
	GPIOA-> MODER |= (GPIO_MODER_MODER4_1 | GPIO_MODER_MODER4_0); //Sets Pin A4 Bits to 11 which is Analog Mode

	DAC-> CR |= DAC_CR_EN1; //Enables DAC by setting Bit 0 = 1, Which is EN1



	//We are using DAC->_____  (Line 1109 of stm32f0xx.h)

	//  __IO uint32_t CR;       /*!< DAC control register,                                    Address offset: 0x00 */
	//  __IO uint32_t SWTRIGR;  /*!< DAC software trigger register,                           Address offset: 0x04 */
	//  __IO uint32_t DHR12R1;  /*!< DAC channel1 12-bit right-aligned data holding register, Address offset: 0x08 */
	//  __IO uint32_t DHR12L1;  /*!< DAC channel1 12-bit left aligned data holding register,  Address offset: 0x0C */
	//  __IO uint32_t DHR8R1;   /*!< DAC channel1 8-bit right aligned data holding register,  Address offset: 0x10 */
	//  __IO uint32_t DHR12R2;  /*!< DAC channel2 12-bit right aligned data holding register, Address offset: 0x14 */
	//  __IO uint32_t DHR12L2;  /*!< DAC channel2 12-bit left aligned data holding register,  Address offset: 0x18 */
	//  __IO uint32_t DHR8R2;   /*!< DAC channel2 8-bit right-aligned data holding register,  Address offset: 0x1C */
	//  __IO uint32_t DHR12RD;  /*!< Dual DAC 12-bit right-aligned data holding register,     Address offset: 0x20 */
	//  __IO uint32_t DHR12LD;  /*!< DUAL DAC 12-bit left aligned data holding register,      Address offset: 0x24 */
	//  __IO uint32_t DHR8RD;   /*!< DUAL DAC 8-bit right aligned data holding register,      Address offset: 0x28 */
	//  __IO uint32_t DOR1;     /*!< DAC channel1 data output register,                       Address offset: 0x2C */
	//  __IO uint32_t DOR2;     /*!< DAC channel2 data output register,                       Address offset: 0x30 */
	//  __IO uint32_t SR;       /*!< DAC status register,                                     Address offset: 0x34 */





	//Initialize the ADC and DAC for use

	//ADC will be on Pin PC0
	//DAC will be on Pin PA4
}

extern int get_resistance_ohms()
{
	//This function will take the readings from the ADC and DAC and return the resistance
}
