//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//
///////
// ----------------------------------------------------------------------------
// School: University of Victoria, Canada.
// Course: CENG 355 "Microprocessor-Based Systems".
// This is template code for Part 2 of Introductory Lab.
//
// See "system/include/cmsis/stm32f0xx.h" for register/bit definitions.
// See "system/src/cmsis/vectors_stm32f0xx.c" for handler declarations.
// ----------------------------------------------------------------------------

#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"
#include "frequency_period.h"
#include "adc_dac.h"
#include "lcd.h"

// ----------------------------------------------------------------------------
//
// STM32F0 empty sample (trace via $(trace)).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the $(trace) output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#define myTIM3_PRESCALER ((uint16_t)0x0024)
#define myTIM3_PERIOD ((uint16_t)0xFFFF)

void myTIM3_Init()
{
	/* Enable clock for TIM2 peripheral */
	// Relevant register: RCC->APB1ENR
	RCC->APB1ENR |= 0x2;

	/* Configure TIM2: buffer auto-reload, count down, continue counting on overflow,
	 * enable update events, interrupt on overflow only */
	// Relevant register: TIM2->CR1
	TIM3->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR;

	/* Set clock prescaler value */
	TIM3->PSC = myTIM3_PRESCALER;
	/* Set auto-reloaded delay */
	TIM3->ARR = myTIM3_PERIOD;

	/* Update timer registers */
	// Relevant register: TIM2->EGR
	TIM3->EGR |= TIM_EGR_UG;

	/* Assign TIM2 interrupt priority = 0 in NVIC */
	// Relevant register: NVIC->IP[3], or use NVIC_SetPriority
	NVIC_SetPriority(TIM3_IRQn, 128);

	/* Enable TIM2 interrupts in NVIC */
	// Relevant register: NVIC->ISER[0], or use NVIC_EnableIRQ
	NVIC_EnableIRQ(TIM3_IRQn);

	/* Enable update interrupt generation */
	// Relevant register: TIM2->DIER
	TIM3->DIER |= TIM_DIER_UIE;

	// Enable counter
	TIM3->CR1 |= TIM_CR1_CEN;
}

int
main(int argc, char* argv[])
{
	freq_period_init();
	lcd_init();
	adc_dac_init();
	myTIM3_Init();

	trace_printf("This is Part 2 of Introductory Lab...\n");
	trace_printf("System clock: %u Hz\n", SystemCoreClock);

	while (1)
	{
		// Nothing is going on here...
	}

	return 0;
}

/* This handler is declared in system/src/cmsis/vectors_stm32f0xx.c */
void TIM3_IRQHandler()
{
	/* Check if update interrupt flag is indeed set */
	if ((TIM3->SR & TIM_SR_UIF) != 0)
	{
		lcd_display_frequency(get_frequency_mHz() / 1000);
		lcd_display_resistance(get_resistance_ohms());
		/* Clear update interrupt flag */
		// Relevant register: TIM2->SR
		TIM3->SR &= ~TIM_SR_UIF;
	}
	else
	{
		trace_printf("It went off!");
	}
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
