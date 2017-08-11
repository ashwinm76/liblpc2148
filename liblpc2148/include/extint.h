/*
 * extint.h
 *
 *  Created on: Aug 10, 2017
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_EXTINT_H_
#define INCLUDE_EXTINT_H_

#include "vic.h"
#include "LPC214x.h"

typedef void extint_handler(void);

enum extint_mode
{
  EXTINT_MODE_LEVEL = 0,
  EXTINT_MODE_EDGE
};

enum extint_polarity
{
  EXTINT_POLARITY_LOW = 0,
  EXTINT_POLARITY_HIGH
};

enum extint_number
{
  EXTINT0 = 0,
  EXTINT1,
  EXTINT2,
  EXTINT3
};

void extint_setup(enum extint_number num, enum extint_mode mode, enum extint_polarity pol, int priority, extint_handler *isr);

static inline void extint_enable(enum extint_number num)
{
  vic_interrupt_enable(1 << (VIC_SRC_EINT0 + num));
}

static inline void extint_disable(enum extint_number num)
{
  vic_interrupt_disable(1 << (VIC_SRC_EINT0 + num));
}

static inline void extint_ack(enum extint_number num)
{
  EXTINT = (EXTINT | (1 << num)) & 0xf;
}

#endif /* INCLUDE_EXTINT_H_ */
