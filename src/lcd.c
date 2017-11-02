/*
 * lcd.c
 *
 *  Created on: Oct 23, 2017
 *      Author: lymacasm
 */

#define ASCII_0 (48)

#include "cmsis/cmsis_device.h"
#include "diag/Trace.h"

/* LUT used to compute 10^index */
uint32_t power_base10_lookup[10] = {
		1,
		10,
		100,
		1000,
		10000,
		100000,
		1000000,
		10000000,
		100000000,
		1000000000
};

static void lcd_SPI_init()
{
	/* Initialize the clock to GPIOB */
	RCC_AHBPeriphResetCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* Initialize the PB3 and PB5 pins for alternate function mode */
	GPIOB->MODER |= GPIO_MODER_MODER3_1 | GPIO_MODER_MODER5_1;

	/* Decrease GPIO speed to lowest */
	GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR3_0 | GPIO_OSPEEDR_OSPEEDR5_0);

	/* Set AF0 for both pins */
	GPIOB->AFR[0] &= ~ (GPIO_AFRL_AFR3 | GPIO_AFRL_AFR5);
}

/* Converts a number to an ASCII string
 * parameters:
 *   - num: Number to convert to string
 *   - precision: The number of significant digits to use in result
 *   - string: The placeholder to output move the output of the conversion
 *   - string_len: Length of the string. It is up to the user to allocate enough space for the string.
 *   	The length must be greater than (precision + 6) to allow for the 10^y term, and the null terminating character.
 */
static void num_to_string(const uint32_t num, uint8_t precision, char * string, uint8_t string_len)
{
	/* 2^32 can have up to 10 decimal digits (0-based) */
	int8_t digit_position = 9;
	uint8_t str_idx = 0;

	if(!string)
	{
		return;
	}

	if(precision > 10) precision = 10;

	/* length of string needs to be at least 6 + precision characters (x*10^y\0) */
	if((precision + 6) > string_len)
	{
		string = 0;
		return;
	}

	/* Find precision most significant digits */
	uint32_t num_holder = num;
	for(; digit_position >= 0; digit_position--)
	{
		/* Divide num_holder by 10^digit_position */
		uint32_t tmp = num_holder;
		uint32_t digit = power_base10_lookup[digit_position];
		tmp /= digit;

		/* Check if we are in the right range */
		if(tmp > 0)
		{
			/* Add digit to string */
			string[str_idx++] = tmp + ASCII_0;
			num_holder -= tmp*digit;
			if(str_idx == precision) break;
		}
	}

	/* Add *10^digit_position to string if needed */
	if(digit_position > 0)
	{
		string[str_idx++] = '*';
		string[str_idx++] = '1';
		string[str_idx++] = '0';
		string[str_idx++] = '^';
		string[str_idx++] = digit_position + ASCII_0;
	}

	/* Append NULL terminating character */
	string[str_idx] = '\0';

	trace_printf("Number is %s with precision %d. Original: %d\n", string, precision, num);
}

extern void lcd_init()
{
	const uint8_t str_size = 20;
	char text[str_size];
	lcd_SPI_init();
	num_to_string(9567364, 6, text, str_size);
}
