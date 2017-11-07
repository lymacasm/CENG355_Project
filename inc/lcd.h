/*
 * lcd.h
 *
 *  Created on: Oct 23, 2017
 *      Author: lymacasm
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

#define DISPLAY_ROW_1 0x80
#define DISPLAY_ROW_2 0xC0


extern void lcd_init();
extern void lcd_send_command(uint8_t command, uint8_t rs);
extern void lcd_display_string(const char * display, uint8_t display_row);
extern void lcd_display_frequency(uint32_t frequency);
extern void lcd_display_resistance(uint32_t resistance);


#endif /* LCD_H_ */
