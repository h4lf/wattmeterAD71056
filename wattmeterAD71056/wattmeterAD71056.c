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