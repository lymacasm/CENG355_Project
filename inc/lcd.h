/*
 * lcd.h
 *
 *  Created on: Oct 23, 2017
 *      Author: lymacasm
 */

#ifndef LCD_H_
#define LCD_H_


extern void lcd_init();
extern void lcd_send_command(uint8_t command);
extern void lcd_display_frequency(int frequency);
extern void lcd_display_resistance(int resistance);


#endif /* LCD_H_ */
