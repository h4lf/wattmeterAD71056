/*
 * timer_soft.h
 *
 * Created: 22.03.2017 15:13:18
 *  Author: 1
 */ 


#ifndef TIMER_SOFT_H_
#define TIMER_SOFT_H_

#include <stdint.h>

typedef uint8_t time_t;

enum Timer_flag { started = 0, stopped, first };

typedef struct
{
	time_t start;
	time_t interval;
	enum Timer_flag flag;
}Timer_t;

void timer_set(Timer_t *timer, time_t interval);
void timer_reset(Timer_t *timer);
void timer_restart(Timer_t *timer);
enum Timer_flag timer_expired(Timer_t *timer);

#endif /* TIMER_SOFT_H_ */