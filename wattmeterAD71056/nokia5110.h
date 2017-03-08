/*
 * nokia5110.h
 *
 * Created: 17.05.2016 19:29:32
 *  Author: UB4LAG Vasily Afanasyev
 */ 


#ifndef NOKIA5110_H_
#define NOKIA5110_H_

#define LCD_SCK D,3,H
#define LCD_SDI D,5,H
#define LCD_D_C D,6,H
#define LCD_SCE D,7,H
#define LCD_RES B,0,H

#define lcd_scursor_xy(x,y) lcd_cursor_xy(x*6,y)

void lcd_init_gpio(void);							// Initializes the LCD pins
void lcd_init(void);								// Initializes the LCD.
void lcd_write_command(uint8_t);					// Writes a command
void lcd_write_data(uint8_t);						// Writes a data
void lcd_clock_data(uint8_t);						// 
void lcd_clear_ram(void);							// Erase the DDRAM contents.
void lcd_clear_str(uint8_t);					// очистка строки на N пикселей
void lcd_cursor_xy(uint8_t,uint8_t);	// Position cursor to x,y.
void lcd_put_char(char);							// Write one character.
void lcd_put_str_pgm(const char *);					// Writes a string of characters (in flash).
void lcd_put_str_ram(char *);						// Writes a string of characters (in RAM).
void lcd_put_hex_byte(uint8_t);						// вывод hex кода байта

#define lcd_pgm_print(x) do {\
	static const char PROGMEM Str[] = x;\
	lcd_put_str_pgm( Str );\
} while(0)

#endif /* NOKIA5110_H_ */