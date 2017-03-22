/*
 * timer_soft.c
 *
 * Created: 22.03.2017 16:22:49
 *  Author: 1
 */ 

#include "timer_soft.h"

void timer_set(Timer_t *timer, time_t interval)
{
	timer->interval = interval;
	timer->start = get_time();
	timer->flag = started;
}

void timer_reset(Timer_t *timer)
{
	timer->start += timer->interval;
	timer->flag = started;
}

void timer_restart(Timer_t *timer)
{
	timer->start = get_time();
	timer->flag = started;
}

enum Timer_flag timer_expired(Timer_t *timer)
{
	if (timer->flag) return timer->flag;
	if ((time_t)(get_time() - timer->start) >= (time_t)timer->interval)
	{
		timer->flag = stopped;
		return first;
	}
	return started;
}