/*
 * timer.h
 *
 *  Created on: Oct 18, 2016
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_TIMER_H_
#define INCLUDE_TIMER_H_

typedef void internal_timer_callback();

struct internal_timer_config
{
  unsigned long pclk_prescale;
  int interrupt_priority;
};

enum internal_timer_match_number
{
  MATCH0 = 0,
  MATCH1,
  MATCH2,
  MATCH3
};

enum internal_timer_capture_number
{
  CAPTURE0 = 0,
  CAPTURE1,
  CAPTURE2,
  CAPTURE3
};

enum internal_timer_match_action
{
  MAT_NOTHING = 0,
  MAT_RESET_TIMER,
  MAT_STOP_TIMER,
  MAT_RESET_AND_STOP_TIMER
};

enum internal_timer_ext_match_action
{
  EXT_MAT_NOTHING = 0,
  EXT_MAT_CLEAR_MATCH_OUTPUT,
  EXT_MAT_SET_MATCH_OUTPUT,
  EXT_MAT_TOGGLE_MATCH_OUTPUT
};

enum internal_timer_capture_edge
{
  EDGE_RISING=0,
  EDGE_FALLING=1,
  EDGE_BOTH=2
};

void internal_timer_init(int timer_num, struct internal_timer_config cfg);
void internal_timer_resume(int timer_num);
void internal_timer_pause(int timer_num);
void internal_timer_reset(int timer_num);
unsigned long internal_timer_read(int timer_num);
void internal_timer_setup_match(int timer_num,
                                   unsigned long period,
                                   enum internal_timer_match_number match_num,
                                   enum internal_timer_match_action action,
                                   enum internal_timer_ext_match_action ext_action,
                                   int oneshot,
                                   internal_timer_callback* cb);
void internal_timer_setup_capture(int timer_num,
                                  enum internal_timer_capture_number cap_num,
                                  enum internal_timer_capture_edge edge,
                                  internal_timer_callback* cb);

#endif /* INCLUDE_TIMER_H_ */
