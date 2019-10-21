/*
 * extint.c
 *
 *  Created on: Aug 10, 2017
 *      Author: Ashwin Menon
 */

#include "extint.h"
#include "vic.h"
#include "LPC214x.h"

void extint_setup(enum extint_number num, enum extint_mode mode, enum extint_polarity pol, int priority, extint_handler *isr)
{
  vic_interrupt_disable(1 << (VIC_SRC_EINT0 + num));

  EXTMODE = (EXTMODE & ~(1 << num)) & 0xf;
  EXTMODE = (EXTMODE | (mode << num)) & 0xf;

  EXTPOLAR = (EXTPOLAR & ~(1 << num)) & 0xf;
  EXTPOLAR = (EXTPOLAR | (pol << num)) & 0xf;

  vic_setup_isr(VIC_SRC_EINT0 + num, VIC_INT_IRQ, priority, isr);
}

