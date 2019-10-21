/*
 * gpio.c
 *
 *  Created on: Oct 2, 2016
 *      Author: Ashwin Menon
 */

#include "LPC214x.h"

#define DEF_GPIO_FNS(PORT) \
  void gpio ## PORT ## _enable_fast_mode() \
  { SCS |= (1 << PORT); }\
  void gpio ## PORT ## _disable_fast_mode() \
  { SCS &= ~(1 << PORT); }\
  void gpio ## PORT ## _write(unsigned long value) \
  { IOPIN ## PORT = value; }\
  unsigned long  gpio ## PORT ## _read() \
  { return IOPIN ## PORT; }\
  void gpio ## PORT ## _set(unsigned long value) \
  { IOSET ## PORT = value; }\
  void gpio ## PORT ## _clr(unsigned long value) \
  { IOCLR ## PORT = value; }\
  void gpio ## PORT ## _set_dir(unsigned long value) \
  { IODIR ## PORT = value; }\
  unsigned long gpio ## PORT ## _get_dir() \
  { return IODIR ## PORT; }\
  void gpio ## PORT ## _make_output(int pin_num) \
  { IODIR ## PORT |= (1 << pin_num); }\
  void gpio ## PORT ## _make_input(int pin_num) \
  { IODIR ## PORT &= ~(1 << pin_num); }\


DEF_GPIO_FNS(0)
DEF_GPIO_FNS(1)
