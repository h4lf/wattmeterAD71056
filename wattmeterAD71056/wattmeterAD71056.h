/*
 * wattmeterAD71056.h
 *
 * Created: 09.05.2016 21:32:59
 *  Author: UB4LAG Vasily Afanasyev
 */ 


#ifndef WATTMETERAD71056_H_
#define WATTMETERAD71056_H_

#define LED		B,5,H

typedef struct
{
	uint32_t	quot;
	uint8_t		rem;
}divmod10_t;

void initial_p(void);
static divmod10_t div_mod_u10(uint32_t);
void dig_to_string(uint32_t, char *, uint8_t, uint8_t);


#endif /* WATTMETERAD71056_H_ */