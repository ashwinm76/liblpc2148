/*
 * timer.h
 *
 *  Created on: Oct 18, 2016
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_TIMER_H_
#define INCLUDE_TIMER_H_

typedef void internal_timer_callback(void*);

struct internal_timer_config
{
  unsigned long pclk_prescale;
  int interrupt_priority;
};

void internal_timer_init(int timer_num, struct internal_timer_config cfg);
void internal_timer_resume(int timer_num);
void internal_timer_pause(int timer_num);
void internal_timer_reset(int timer_num);
unsigned long internal_timer_read(int timer_num);
void internal_timer_start_periodic(int timer_num, unsigned long period, internal_timer_callback* cb, void *cb_data);
void internal_timer_start_oneshot(int timer_num, unsigned long delay, internal_timer_callback* cb, void *cb_data);

#endif /* INCLUDE_TIMER_H_ */
