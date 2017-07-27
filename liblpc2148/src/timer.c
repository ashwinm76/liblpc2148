/*
 * timer.c
 *
 *  Created on: Oct 18, 2016
 *      Author: Ashwin Menon
 */

#include "timer.h"
#include "vic.h"
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

internal_timer_callback *callbacks[2];
void *callback_data[2];
static void internal_timer_isr();

void internal_timer_init(int timer_num, struct internal_timer_config cfg)
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
  *(timer_base + TIMER_PR) = cfg.pclk_prescale;

  // Disable matches.
  *(timer_base + TIMER_MCR) = 0;

  // Disable captures.
  *(timer_base + TIMER_CCR) = 0;

  // Disable external matches.
  *(timer_base + TIMER_EMR) = 0;

  // Set up the TIMER ISR and enable it
  vic_setup_isr(timer_num == 0 ? VIC_SRC_TIMER0 : VIC_SRC_TIMER1, VIC_INT_IRQ, cfg.interrupt_priority, internal_timer_isr);
  vic_interrupt_enable(1 << (timer_num == 0 ? VIC_SRC_TIMER0 : VIC_SRC_TIMER1));
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

void internal_timer_start_periodic(int timer_num, unsigned long period, internal_timer_callback* cb, void *cb_data)
{
  volatile unsigned long *timer_base;
    timer_base = (
        timer_num == 0 ?
            (volatile unsigned long *) TMR0_BASE_ADDR :
            (volatile unsigned long *) TMR1_BASE_ADDR);

    callbacks[timer_num] = cb;
    callback_data[timer_num] = cb_data;

    // Use Match 0
    *(timer_base + TIMER_MR0) = period;

    // Interrupt and reset the timer on match
    *(timer_base + TIMER_MCR) = 3;

    // Reset and start the timer
    *(timer_base + TIMER_TCR) = 2;
    *(timer_base + TIMER_TCR) = 1;
}

void internal_timer_start_oneshot(int timer_num, unsigned long delay, internal_timer_callback* cb, void *cb_data)
{

}

static void internal_timer_isr()
{
  volatile unsigned long *timer_base = 0;
  unsigned long status;
  int timer_num;

  if (VICIRQStatus & (1 << VIC_SRC_TIMER0))
  {
    timer_num = 0;
    timer_base = (unsigned long*)TMR0_BASE_ADDR;
  }
  if (VICIRQStatus & (1 << VIC_SRC_TIMER1))
  {
    timer_num = 1;
    timer_base = (unsigned long*)TMR1_BASE_ADDR;
  }
  if (timer_base == 0)
  {
    return;
  }

  // Read and clear the interrupt bit
  status = *(timer_base + TIMER_IR);
  *(timer_base + TIMER_IR) = status;

  // Call the callback if MR0 matches
  if (status & 1)
  {
    callbacks[timer_num](callback_data[timer_num]);
  }
}
