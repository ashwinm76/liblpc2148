/*
 * vic.c
 *
 *  Created on: Jul 24, 2017
 *      Author: Ashwin Menon
 */

#include "vic.h"
#include "LPC214x.h"

void vic_setup_isr(enum vic_interrupt_source src, enum vic_interrupt_type type, unsigned int priority, vic_isr *isr)
{
  if (priority > 15)
  {
    return;
  }

  unsigned long sel = VICIntSelect;
  sel &= ~(1 << src);
  sel |= ((unsigned long)type << src);
  VICIntSelect = sel;

  *(&VICVectCntl0 + priority) = (src & 0x1f) | (1 << 5);
  *(&VICVectAddr0 + priority) = (unsigned long)isr;
}

void vic_set_default_isr(vic_isr *isr)
{
  VICDefVectAddr = (unsigned long)isr;
}

void vic_irq_handler()
{
  ((vic_isr*)VICVectAddr)();
  VICVectAddr = 0xff;
}
