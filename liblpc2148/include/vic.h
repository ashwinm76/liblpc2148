/*
 * vic.h
 *
 *  Created on: Jul 24, 2017
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_VIC_H_
#define INCLUDE_VIC_H_

#include "LPC214x.h"

enum vic_interrupt_type
{
  VIC_INT_IRQ = 0,
  VIC_INT_FIQ = 1
};

enum vic_interrupt_source
{
  VIC_SRC_WDT = 0,
  VIC_SRC_ARMCORE0 = 2,
  VIC_SRC_ARMCORE1,
  VIC_SRC_TIMER0,
  VIC_SRC_TIMER1,
  VIC_SRC_UART0,
  VIC_SRC_UART1,
  VIC_SRC_PWM0,
  VIC_SRC_I2C0,
  VIC_SRC_SPI0,
  VIC_SRC_SPI1_SSP,
  VIC_SRC_PLL,
  VIC_SRC_RTC,
  VIC_SRC_EINT0,
  VIC_SRC_EINT1,
  VIC_SRC_EINT2,
  VIC_SRC_EINT3,
  VIC_SRC_AD0,
  VIC_SRC_I2C1,
  VIC_SRC_BOD,
  VIC_SRC_AD1,
  VIC_SRC_USB
};

typedef void vic_isr(void);

void vic_setup_isr(enum vic_interrupt_source src, enum vic_interrupt_type type, unsigned int priority, vic_isr *isr);
void vic_set_default_isr(vic_isr *isr);

static inline void vic_interrupt_enable(unsigned long mask)
{
  VICIntEnable |= mask;
}

static inline void vic_interrupt_disable(unsigned long mask)
{
  VICIntEnClr = mask;
}

#endif /* INCLUDE_VIC_H_ */
