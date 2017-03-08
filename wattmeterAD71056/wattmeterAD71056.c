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

volatile struct isrflagglob
{
	unsigned itmr0	: 1;
	unsigned itmr1	: 1;
} IsrFlag;

ISR(TIMER0_OVF_vect) /* 256 imp = 4500 W*s = 1.25 W*h */
{
	IsrFlag.itmr0 = 1;
}

ISR(TIMER1_COMPA_vect)
{
	IsrFlag.itmr1 = 1;
}

void initial_p(void)
{
	//------Init CLK
	CLKPR = 1 << CLKPCE; // CLK Change Enable
	//CLKPR = (1 << CLKPS0) | (1 << CLKPS1); // CLK/8
	CLKPR = (1 << CLKPS2); // CLK/16
	
	//------Disable comparator
	ACSR = 0x80;
	
	//------Init GPIO
	DRIVER(LED, OUT);
	DRIVER(T0IN, IN);
	DRIVER(T0IN, PULLUP);
	
	//------Init TIM0 (Mode 0 Normal)
	TCCR0A = 0;
	TCNT0 = 0;
	TIMSK0 = (1 << TOIE0);
	TCCR0B = (1 << CS01) | (1 << CS02); // T0 clock source, falling edge
	
	//------Init TIM1 (Mode 4 CTC)
	TCCR1A = 0;
	TCCR1C = 0;
	TCNT1 = 0;
	OCR1A = TOP_TIMER1;
	TIMSK1 = (1 << OCIE1A);
	TCCR1B = (1 << CS11) | (1 << WGM12); // CTC mode; clk/8;
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
	uint8_t CountSec = 0;
	
	initial_p();
	lcd_init();
	sei();
	
	lcd_pgm_print("NOKIA 5110 LCD");
	lcd_scursor_xy(0, 1);
	lcd_pgm_print("1234567890-W*h");
    while(1)
    {
		
    }
}