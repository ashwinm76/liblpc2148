/*
 * gpio_new.h
 *
 *  Created on: Oct 2, 2016
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_GPIO_H_
#define INCLUDE_GPIO_H_

#include "LPC214x.h"

// GPIO 0 fast access registers.
#define FIO0PIN0 (*(volatile unsigned char*)  (&FIO0PIN))
#define FIO0PIN1 (*((volatile unsigned char*) (&FIO0PIN) + 1))
#define FIO0PIN2 (*((volatile unsigned char*) (&FIO0PIN) + 2))
#define FIO0PIN3 (*((volatile unsigned char*) (&FIO0PIN) + 3))
#define FIO0PINL (*(volatile unsigned short*) (&FIO0PIN))
#define FIO0PINU (*((volatile unsigned short*)(&FIO0PIN) + 1))

#define FIO0SET0 (*(volatile unsigned char*)  (&FIO0SET))
#define FIO0SET1 (*((volatile unsigned char*) (&FIO0SET) + 1))
#define FIO0SET2 (*((volatile unsigned char*) (&FIO0SET) + 2))
#define FIO0SET3 (*((volatile unsigned char*) (&FIO0SET) + 3))
#define FIO0SETL (*(volatile unsigned short*) (&FIO0SET))
#define FIO0SETU (*((volatile unsigned short*)(&FIO0SET) + 1))

#define FIO0CLR0 (*(volatile unsigned char*)  (&FIO0CLR))
#define FIO0CLR1 (*((volatile unsigned char*) (&FIO0CLR) + 1))
#define FIO0CLR2 (*((volatile unsigned char*) (&FIO0CLR) + 2))
#define FIO0CLR3 (*((volatile unsigned char*) (&FIO0CLR) + 3))
#define FIO0CLRL (*(volatile unsigned short*) (&FIO0CLR))
#define FIO0CLRU (*((volatile unsigned short*)(&FIO0CLR) + 1))

#define FIO0DIR0 (*(volatile unsigned char*)  (&FIO0DIR))
#define FIO0DIR1 (*((volatile unsigned char*) (&FIO0DIR) + 1))
#define FIO0DIR2 (*((volatile unsigned char*) (&FIO0DIR) + 2))
#define FIO0DIR3 (*((volatile unsigned char*) (&FIO0DIR) + 3))
#define FIO0DIRL (*(volatile unsigned short*) (&FIO0DIR))
#define FIO0DIRU (*((volatile unsigned short*)(&FIO0DIR) + 1))

#define FIO0MASK0 (*(volatile unsigned char*)  (&FIO0MASK))
#define FIO0MASK1 (*((volatile unsigned char*) (&FIO0MASK) + 1))
#define FIO0MASK2 (*((volatile unsigned char*) (&FIO0MASK) + 2))
#define FIO0MASK3 (*((volatile unsigned char*) (&FIO0MASK) + 3))
#define FIO0MASKL (*(volatile unsigned short*) (&FIO0MASK))
#define FIO0MASKU (*((volatile unsigned short*)(&FIO0MASK) + 1))

// GPIO 1 fast access registers.
#define FIO1PIN0 (*(volatile unsigned char*)  (&FIO1PIN))
#define FIO1PIN1 (*((volatile unsigned char*) (&FIO1PIN) + 1))
#define FIO1PIN2 (*((volatile unsigned char*) (&FIO1PIN) + 2))
#define FIO1PIN3 (*((volatile unsigned char*) (&FIO1PIN) + 3))
#define FIO1PINL (*(volatile unsigned short*) (&FIO1PIN))
#define FIO1PINU (*((volatile unsigned short*)(&FIO1PIN) + 1))

#define FIO1SET0 (*(volatile unsigned char*)  (&FIO1SET))
#define FIO1SET1 (*((volatile unsigned char*) (&FIO1SET) + 1))
#define FIO1SET2 (*((volatile unsigned char*) (&FIO1SET) + 2))
#define FIO1SET3 (*((volatile unsigned char*) (&FIO1SET) + 3))
#define FIO1SETL (*(volatile unsigned short*) (&FIO1SET))
#define FIO1SETU (*((volatile unsigned short*)(&FIO1SET) + 1))

#define FIO1CLR0 (*(volatile unsigned char*)  (&FIO1CLR))
#define FIO1CLR1 (*((volatile unsigned char*) (&FIO1CLR) + 1))
#define FIO1CLR2 (*((volatile unsigned char*) (&FIO1CLR) + 2))
#define FIO1CLR3 (*((volatile unsigned char*) (&FIO1CLR) + 3))
#define FIO1CLRL (*(volatile unsigned short*) (&FIO1CLR))
#define FIO1CLRU (*((volatile unsigned short*)(&FIO1CLR) + 1))

#define FIO1DIR0 (*(volatile unsigned char*)  (&FIO1DIR))
#define FIO1DIR1 (*((volatile unsigned char*) (&FIO1DIR) + 1))
#define FIO1DIR2 (*((volatile unsigned char*) (&FIO1DIR) + 2))
#define FIO1DIR3 (*((volatile unsigned char*) (&FIO1DIR) + 3))
#define FIO1DIRL (*(volatile unsigned short*) (&FIO1DIR))
#define FIO1DIRU (*((volatile unsigned short*)(&FIO1DIR) + 1))

#define FIO1MASK0 (*(volatile unsigned char*)  (&FIO1MASK))
#define FIO1MASK1 (*((volatile unsigned char*) (&FIO1MASK) + 1))
#define FIO1MASK2 (*((volatile unsigned char*) (&FIO1MASK) + 2))
#define FIO1MASK3 (*((volatile unsigned char*) (&FIO1MASK) + 3))
#define FIO1MASKL (*(volatile unsigned short*) (&FIO1MASK))
#define FIO1MASKU (*((volatile unsigned short*)(&FIO1MASK) + 1))

#define DEF_GPIO_FAST_FNS(PORT, FNSUFF, REGSUFF, PARAMTYPE) \
  static inline void gpio ## PORT ## _write_fast ## FNSUFF (unsigned PARAMTYPE value)\
  { FIO ## PORT ## PIN ## REGSUFF = value; }\
  static inline unsigned PARAMTYPE gpio ## PORT ## _read_fast ## FNSUFF ()\
  { return FIO ## PORT ## PIN ## REGSUFF; }\
  static inline void gpio ## PORT ## _set_fast ## FNSUFF (unsigned PARAMTYPE value)\
  { FIO ## PORT ## SET ## REGSUFF = value; }\
  static inline void gpio ## PORT ## _clr_fast ## FNSUFF (unsigned PARAMTYPE value)\
  { FIO ## PORT ## CLR ## REGSUFF = value; }\
  static inline void gpio ## PORT ## _set_mask_fast ## FNSUFF (unsigned PARAMTYPE value)\
  { FIO ## PORT ## MASK ## REGSUFF = value; }\
  static inline void gpio ## PORT ## _set_dir_fast ## FNSUFF (unsigned PARAMTYPE value)\
  { FIO ## PORT ## DIR ## REGSUFF = value; }\
  static inline void gpio ## PORT ## _make_output_fast ## FNSUFF (int pin_num)\
  { FIO ## PORT ## DIR ## REGSUFF |= (1 << pin_num); }\
  static inline void gpio ## PORT ## _make_input_fast ## FNSUFF (int pin_num)\
  { FIO ## PORT ## DIR ## REGSUFF &= ~(1 << pin_num); }\
  static inline unsigned PARAMTYPE gpio ## PORT ## _get_mask_fast ## FNSUFF ()\
  { return FIO ## PORT ## MASK ## REGSUFF; }\
  static inline unsigned PARAMTYPE gpio ## PORT ## _get_dir_fast ## FNSUFF ()\
  { return FIO ## PORT ## DIR ## REGSUFF; }\


DEF_GPIO_FAST_FNS(0, 32, , long)
DEF_GPIO_FAST_FNS(0, 16_u, U, short)
DEF_GPIO_FAST_FNS(0, 16_l, L, short)
DEF_GPIO_FAST_FNS(0, 8_0, 0, char)
DEF_GPIO_FAST_FNS(0, 8_1, 1, char)
DEF_GPIO_FAST_FNS(0, 8_2, 2, char)
DEF_GPIO_FAST_FNS(0, 8_3, 3, char)

DEF_GPIO_FAST_FNS(1, 32, , long)
DEF_GPIO_FAST_FNS(1, 16_u, U, short)
DEF_GPIO_FAST_FNS(1, 16_l, L, short)
DEF_GPIO_FAST_FNS(1, 8_0, 0, char)
DEF_GPIO_FAST_FNS(1, 8_1, 1, char)
DEF_GPIO_FAST_FNS(1, 8_2, 2, char)
DEF_GPIO_FAST_FNS(1, 8_3, 3, char)

#undef DEF_GPIO_FAST_FNS

#define DEF_GPIO_FN_PROTOS(PORT) \
  void gpio ## PORT ## _enable_fast_mode(); \
  void gpio ## PORT ## _disable_fast_mode(); \
  void gpio ## PORT ## _write(unsigned long value); \
  unsigned long  gpio ## PORT ## _read(); \
  void gpio ## PORT ## _set(unsigned long value); \
  void gpio ## PORT ## _clr(unsigned long value); \
  void gpio ## PORT ## _set_dir(unsigned long value); \
  unsigned long gpio ## PORT ## _get_dir(); \
  void gpio ## PORT ## _make_output(int pin_num); \
  void gpio ## PORT ## _make_input(int pin_num); \

DEF_GPIO_FN_PROTOS(0)
DEF_GPIO_FN_PROTOS(1)

#undef GPIO_FN_PROTOS

// Generated fast write functions:
// static inline void gpio0_write_fast32(unsigned long)
// static inline void gpio0_write_fast16_u(unsigned short)
// static inline void gpio0_write_fast16_l(unsigned short)
// static inline void gpio0_write_fast8_0(unsigned char)
// static inline void gpio0_write_fast8_1(unsigned char)
// static inline void gpio0_write_fast8_2(unsigned char)
// static inline void gpio0_write_fast8_3(unsigned char)
// Similarly instead of write, also set, clr, set_mask, set_dir, make_output, make_input
//
// Generated fast read functions:
// static inline unsigned long gpio0_read_fast32()
// static inline unsigned short gpio0_read_fast16_u()
// static inline unsigned short gpio0_read_fast16_l()
// static inline unsigned char gpio0_read_fast8_0()
// static inline unsigned char gpio0_read_fast8_1()
// static inline unsigned char gpio0_read_fast8_2()
// static inline unsigned char gpio0_read_fast8_3()
// Similarly instead of read, also get_mask, get_dir
//
// Generated slow functions:
// void gpio0_enable_fast_mode()
// void gpio0_disable_fast_mode()
// void gpio0_write(unsigned long)
// unsigned long gpio0_read()
// void gpio0_set(unsigned long)
// void gpio0_clr(unsigned long)
// void gpio0_set_dir(unsigned long)
// unsigned long gpio0_get_dir()
// void gpio0_make_output(int)
// void gpio0_make_input(int)
//
// And all of the above for gpio1 as well.

static inline void gpio_make_pin_input_fast(int port, unsigned long mask)
{
  switch(port)
  {
    case 0 :
      gpio0_set_dir_fast32(gpio0_get_dir_fast32() & ~mask);
      break;
    case 1 :
      gpio1_set_dir_fast32(gpio1_get_dir_fast32() & ~mask);
      break;
  }
}

static inline void gpio_make_pin_output_fast(int port, unsigned long mask)
{
  switch(port)
  {
    case 0 :
      gpio0_set_dir_fast32(gpio0_get_dir_fast32() | mask);
      break;
    case 1 :
      gpio1_set_dir_fast32(gpio1_get_dir_fast32() | mask);
      break;
  }
}

static inline void gpio_pin_write_fast(int port, unsigned long mask, unsigned long value)
{
  switch (port)
  {
    case 0 :
      gpio0_set_mask_fast32(0xffffffff & ~mask);
      gpio0_write_fast32(value);
      break;
    case 1 :
      gpio1_set_mask_fast32(0xffffffff & ~mask);
      gpio1_write_fast32(value);
      break;
  }
}

static inline unsigned long gpio_pin_read_fast(int port, unsigned long mask)
{
  switch(port)
  {
    case 0 :
      gpio0_set_mask_fast32(0xffffffff & ~mask);
      return gpio0_read_fast32();
      break;
    case 1 :
      gpio1_set_mask_fast32(0xffffffff & ~mask);
      return gpio1_read_fast32();
      break;
  }
  return 0;
}

#endif /* INCLUDE_GPIO_H_ */
