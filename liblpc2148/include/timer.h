/*
 * timer.h
 *
 *  Created on: Oct 18, 2016
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_TIMER_H_
#define INCLUDE_TIMER_H_

void internal_timer_init(int timer_num, unsigned long pclk_prescale);
void internal_timer_resume(int timer_num);
void internal_timer_pause(int timer_num);
void internal_timer_reset(int timer_num);
unsigned long internal_timer_read(int timer_num);

#endif /* INCLUDE_TIMER_H_ */
