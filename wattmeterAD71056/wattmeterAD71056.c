/*
 * wattmeterAD71056.c
 *
 * Created: 08.05.2016 1:49:53
 *  Author: UB4LAG Vasily Afanasyev
 
 The MIT License (MIT)

 Copyright (c) 2016 Vasily Afanasyev

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "wattmeterAD71056.h"
#include "pin_macros.h"
#include "nokia5110.h"

void initial_p(void)
{
	//------Init CLK
	CLKPR = 1 << CLKPCE; // CLK Change Enable
	CLKPR = (1 << CLKPS0) | (1 << CLKPS1); // CLK/8
	
	//------Init GPIO
	DRIVER(LED, OUT);
}

static divmod10_t div_mod_u10(uint32_t Num)
{
	divmod10_t Res;
	uint32_t Qq;
	
	// умножаем на 0.8
	Res.quot = Num >> 1;
	Res.quot += Res.quot >> 1;
	Res.quot += Res.quot >> 4;
	Res.quot += Res.quot >> 8;
	Res.quot += Res.quot >> 16;
	Qq = Res.quot;
	// делим на 8
	Res.quot >>= 3;
	// вычисляем остаток
	Res.rem = (uint8_t)(Num - ((Res.quot << 1) + (Qq & ~7ul)));
	// корректируем остаток и частное
	if (Res.rem > 9)
	{
		Res.rem -= 10;
		Res.quot++;
	}
	return Res;
}

void dig_to_string(uint32_t Dig, char * Str, uint8_t Len, uint8_t Pos)
{
	uint8_t i;
	divmod10_t Res;
	
	Str += Len;
	for (i=0; i<Len; i++)
	{
		Str--;
		if ((Pos!=0)&&(i==Pos))
		{
			*Str = '.';
			if (Dig==0)
			{
				*--Str = '0';
				i++;
			}
		}
		else
		{
			if ((i>Pos)&&(Dig==0))
			{
				*Str = ' ';
			}
			else
			{
				Res = div_mod_u10(Dig);
				*Str = Res.rem + '0';
				Dig = Res.quot;
			}
		}
	}
}

int main(void)
{
	initial_p();
	lcd_init();
	
	lcd_pgm_print("NOKIA 5110 LCD");
	lcd_scursor_xy(0, 1);
	lcd_pgm_print("1234567890-W*h");
    while(1)
    {
		ON(LED);
		_delay_ms(1000);
		OFF(LED);
		_delay_ms(2000);
    }
}