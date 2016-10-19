/*
 * timer.c
 *
 *  Created on: Oct 18, 2016
 *      Author: Ashwin Menon
 */

#include "LPC214x.h"

#define TIMER_IR   (0x00/sizeof(unsigned long))
#define TIMER_TCR  (0x04/sizeof(unsigned long))
#define TIMER_TC   (0x08/sizeof(unsigned long))
#define TIMER_PR   (0x0C/sizeof(unsigned long))
#define TIMER_PC   (0x10/sizeof(unsigned long))
#define TIMER_MCR  (0x14/sizeof(unsigned long))
#define TIMER_MR0  (0x18/sizeof(unsigned long))
#define TIMER_MR1  (0x1C/sizeof(unsigned long))
#define TIMER_MR2  (0x20/sizeof(unsigned long))
#define TIMER_MR3  (0x24/sizeof(unsigned long))
#define TIMER_CCR  (0x28/sizeof(unsigned long))
#define TIMER_CR0  (0x2C/sizeof(unsigned long))
#define TIMER_CR1  (0x30/sizeof(unsigned long))
#define TIMER_CR2  (0x34/sizeof(unsigned long))
#define TIMER_CR3  (0x38/sizeof(unsigned long))
#define TIMER_EMR  (0x3C/sizeof(unsigned long))
#define TIMER_CTCR (0x70/sizeof(unsigned long))

void internal_timer_init(int timer_num, unsigned long pclk_prescale)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  // Disable and reset the timer counter.
  *(timer_base + TIMER_TCR) = 2;

  // Clear any interrupts.
  *(timer_base + TIMER_IR) = 0xff;

  // Select timer mode on every rising PCLK edge.
  *(timer_base + TIMER_CTCR) = 0;

  // Set the prescale.
  *(timer_base + TIMER_PR) = pclk_prescale;

  // Disable matches.
  *(timer_base + TIMER_MCR) = 0;

  // Disable captures.
  *(timer_base + TIMER_CCR) = 0;

  // Disable external matches.
  *(timer_base + TIMER_EMR) = 0;
}

void internal_timer_resume(int timer_num)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  *(timer_base + TIMER_TCR) = 1;
}

void internal_timer_pause(int timer_num)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  *(timer_base + TIMER_TCR) = 0;
}

void internal_timer_reset(int timer_num)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  *(timer_base + TIMER_TCR) = 2;
}

unsigned long internal_timer_read(int timer_num)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  return *(timer_base + TIMER_TC);
}
