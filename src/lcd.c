/*
 * lcd.c
 *
 *  Created on: Oct 23, 2017
 *      Author: lymacasm
 */

#define ASCII_0 (48)

#include "lcd.h"

#include "cmsis/cmsis_device.h"
#include "stm32f0xx_spi.h"
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

static void lcd_GPIO_init()
{
	/* Initialize the clock to GPIOB */
	RCC_AHBPeriphResetCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* Initialize the PB3 and PB5 pins for alternate function mode (SPI), and PB7 to output (Latch) */
	GPIOB->MODER |= GPIO_MODER_MODER3_1 | GPIO_MODER_MODER5_1 | GPIO_MODER_MODER7_0;

	/* Decrease GPIO speed to lowest */
	GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR3_0 | GPIO_OSPEEDR_OSPEEDR5_0 | GPIO_OSPEEDR_OSPEEDR7_0);

	/* Set AF0 for both pins */
	GPIOB->AFR[0] &= ~ (GPIO_AFRL_AFR3 | GPIO_AFRL_AFR5);
}

static void lcd_SPI_init()
{
	/* Enable clock to SPI1 */
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	/* Set MSBFIRST, slowest baudrate, master mode */
	SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 | SPI_CR1_MSTR;

	/* Set SPI data length to 8 bits */
	SPI1->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;

	/* Enable peripheral */
	SPI1->CR1 |= SPI_CR1_SPE;
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
	num_to_string(9567364, 6, text, str_size);

	lcd_GPIO_init();
	lcd_SPI_init();
}

static void lcd_send_data(uint8_t data)
{
	/* Force LCK signal to 0 */
	GPIOB->ODR &= ~GPIO_ODR_7;

	/* Wait until SPI is not busy */
	while(SPI1->SR & SPI_SR_BSY);

	/* Send command */
	SPI_SendData8(SPI1, data);

	/* Wait until SPI is not busy */
	while(SPI1->SR & SPI_SR_BSY);

	/* Force LCK to 1 */
	GPIOB->ODR |= GPIO_ODR_7;

	/* TODO: Add a delay here */
}

extern void lcd_send_command(uint8_t command)
{
	uint8_t high_nybble = (command & 0xF0) >> 4;
	uint8_t low_nybble = command & 0xF;
	const uint8_t ends = 0x00;
	const uint8_t middle = 0x80;

	/* Send high side first */
	lcd_send_data(ends | high_nybble);
	lcd_send_data(middle | high_nybble);
	lcd_send_data(ends | high_nybble);

	/* Send low side second */
	lcd_send_data(ends | low_nybble);
	lcd_send_data(middle | low_nybble);
	lcd_send_data(ends | low_nybble);
}

extern void lcd_display_frequency(uint32_t frequency)
{

}

extern void lcd_display_resistance(uint32_t resistance)
{

}
